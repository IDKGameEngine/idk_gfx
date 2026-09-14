#include "idk/gfx/VulkanRenderer.hpp"

#include "libidk/platform-sdl3/SdlPlatform.hpp"

#include <vulkan/vulkan.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace idk::gfx
{
    namespace
    {
        void require(VkResult result, const char *operation)
        {
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error(operation);
            }
        }
    }

    class VulkanRenderer::Impl
    {
    public:
        explicit Impl(platform::Window &window)
        : window_(window), surfaceProvider_(sdl3::vulkanSurface(window))
        {
            try
            {
                if (surfaceProvider_ == nullptr)
                {
                    throw std::invalid_argument("VulkanRenderer requires an SDL3 Vulkan window");
                }

                createInstance();
                if (!surfaceProvider_->createVulkanSurface(instance_, surface_))
                {
                    throw std::runtime_error("SDL_Vulkan_CreateSurface failed");
                }
                selectDevice();
                createDevice();
                createCommandResources();
                createSwapchainResources();
            }
            catch (...)
            {
                destroy();
                throw;
            }
        }

        ~Impl()
        {
            destroy();
        }

    private:
        void destroy()
        {
            if (device_ != VK_NULL_HANDLE)
            {
                vkDeviceWaitIdle(device_);
                destroySwapchainResources();
                if (renderFinished_ != VK_NULL_HANDLE) { vkDestroySemaphore(device_, renderFinished_, nullptr); }
                if (imageAvailable_ != VK_NULL_HANDLE) { vkDestroySemaphore(device_, imageAvailable_, nullptr); }
                if (commandPool_ != VK_NULL_HANDLE) { vkDestroyCommandPool(device_, commandPool_, nullptr); }
                vkDestroyDevice(device_, nullptr);
                device_ = VK_NULL_HANDLE;
            }
            if (surface_ != VK_NULL_HANDLE) { vkDestroySurfaceKHR(instance_, surface_, nullptr); surface_ = VK_NULL_HANDLE; }
            if (instance_ != VK_NULL_HANDLE) { vkDestroyInstance(instance_, nullptr); instance_ = VK_NULL_HANDLE; }
        }

    public:
        void render()
        {
            const platform::Extent windowExtent = window_.extent();
            if (windowExtent.width <= 0 || windowExtent.height <= 0)
            {
                return;
            }
            if (swapchainExtent_.width != static_cast<uint32_t>(windowExtent.width)
                || swapchainExtent_.height != static_cast<uint32_t>(windowExtent.height))
            {
                recreateSwapchain();
            }

            uint32_t imageIndex = 0;
            const VkResult acquire = vkAcquireNextImageKHR(
                device_, swapchain_, UINT64_MAX, imageAvailable_, VK_NULL_HANDLE, &imageIndex);
            if (acquire == VK_ERROR_OUT_OF_DATE_KHR || acquire == VK_SUBOPTIMAL_KHR)
            {
                recreateSwapchain();
                return;
            }
            require(acquire, "vkAcquireNextImageKHR failed");

            require(vkResetCommandBuffer(commandBuffer_, 0), "vkResetCommandBuffer failed");
            VkCommandBufferBeginInfo beginInfo {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            require(vkBeginCommandBuffer(commandBuffer_, &beginInfo), "vkBeginCommandBuffer failed");

            VkClearValue clearValue {};
            std::copy(clearColor_.begin(), clearColor_.end(), clearValue.color.float32);
            VkRenderPassBeginInfo renderPassInfo {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass_;
            renderPassInfo.framebuffer = framebuffers_.at(imageIndex);
            renderPassInfo.renderArea.extent = swapchainExtent_;
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearValue;
            vkCmdBeginRenderPass(commandBuffer_, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdEndRenderPass(commandBuffer_);
            require(vkEndCommandBuffer(commandBuffer_), "vkEndCommandBuffer failed");

            const VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo submitInfo {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &imageAvailable_;
            submitInfo.pWaitDstStageMask = &waitStage;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer_;
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &renderFinished_;
            require(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE), "vkQueueSubmit failed");

            VkPresentInfoKHR presentInfo {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &renderFinished_;
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain_;
            presentInfo.pImageIndices = &imageIndex;
            const VkResult present = vkQueuePresentKHR(graphicsQueue_, &presentInfo);
            require(vkQueueWaitIdle(graphicsQueue_), "vkQueueWaitIdle failed");
            if (present == VK_ERROR_OUT_OF_DATE_KHR || present == VK_SUBOPTIMAL_KHR)
            {
                recreateSwapchain();
                return;
            }
            require(present, "vkQueuePresentKHR failed");
        }

        void setClearColor(float red, float green, float blue, float alpha)
        {
            clearColor_ = { red, green, blue, alpha };
        }

    private:
        void createInstance()
        {
            const auto extensions = surfaceProvider_->requiredVulkanExtensions();
            if (extensions.empty())
            {
                throw std::runtime_error("SDL3 reported no Vulkan instance extensions");
            }

            VkApplicationInfo appInfo {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "IDK";
            appInfo.apiVersion = VK_API_VERSION_1_3;
            VkInstanceCreateInfo createInfo {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();
            require(vkCreateInstance(&createInfo, nullptr, &instance_), "vkCreateInstance failed");
        }

        void selectDevice()
        {
            uint32_t deviceCount = 0;
            require(vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr), "vkEnumeratePhysicalDevices failed");
            std::vector<VkPhysicalDevice> devices(deviceCount);
            require(vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data()), "vkEnumeratePhysicalDevices failed");

            for (const VkPhysicalDevice candidate : devices)
            {
                uint32_t queueCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(candidate, &queueCount, nullptr);
                std::vector<VkQueueFamilyProperties> queues(queueCount);
                vkGetPhysicalDeviceQueueFamilyProperties(candidate, &queueCount, queues.data());
                for (uint32_t index = 0; index < queueCount; ++index)
                {
                    VkBool32 canPresent = VK_FALSE;
                    require(vkGetPhysicalDeviceSurfaceSupportKHR(candidate, index, surface_, &canPresent), "vkGetPhysicalDeviceSurfaceSupportKHR failed");
                    if ((queues[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 && canPresent == VK_TRUE)
                    {
                        physicalDevice_ = candidate;
                        queueFamily_ = index;
                        return;
                    }
                }
            }
            throw std::runtime_error("No Vulkan graphics device can present to the window");
        }

        void createDevice()
        {
            const float priority = 1.0f;
            VkDeviceQueueCreateInfo queueInfo {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueFamilyIndex = queueFamily_;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = &priority;
            const char *extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
            VkDeviceCreateInfo createInfo {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount = 1;
            createInfo.pQueueCreateInfos = &queueInfo;
            createInfo.enabledExtensionCount = 1;
            createInfo.ppEnabledExtensionNames = extensions;
            require(vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_), "vkCreateDevice failed");
            vkGetDeviceQueue(device_, queueFamily_, 0, &graphicsQueue_);
        }

        void createCommandResources()
        {
            VkCommandPoolCreateInfo poolInfo {};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            poolInfo.queueFamilyIndex = queueFamily_;
            require(vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool_), "vkCreateCommandPool failed");
            VkCommandBufferAllocateInfo allocationInfo {};
            allocationInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocationInfo.commandPool = commandPool_;
            allocationInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocationInfo.commandBufferCount = 1;
            require(vkAllocateCommandBuffers(device_, &allocationInfo, &commandBuffer_), "vkAllocateCommandBuffers failed");
            VkSemaphoreCreateInfo semaphoreInfo {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            require(vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &imageAvailable_), "vkCreateSemaphore failed");
            require(vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &renderFinished_), "vkCreateSemaphore failed");
        }

        void createSwapchainResources()
        {
            VkSurfaceCapabilitiesKHR capabilities {};
            require(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice_, surface_, &capabilities), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed");
            uint32_t formatCount = 0;
            require(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_, surface_, &formatCount, nullptr), "vkGetPhysicalDeviceSurfaceFormatsKHR failed");
            std::vector<VkSurfaceFormatKHR> formats(formatCount);
            require(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_, surface_, &formatCount, formats.data()), "vkGetPhysicalDeviceSurfaceFormatsKHR failed");
            const auto format = formats.front();

            const platform::Extent windowExtent = window_.extent();
            swapchainExtent_ = capabilities.currentExtent;
            if (swapchainExtent_.width == UINT32_MAX)
            {
                swapchainExtent_.width = std::clamp(static_cast<uint32_t>(windowExtent.width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                swapchainExtent_.height = std::clamp(static_cast<uint32_t>(windowExtent.height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            }
            uint32_t imageCount = capabilities.minImageCount + 1;
            if (capabilities.maxImageCount != 0) { imageCount = std::min(imageCount, capabilities.maxImageCount); }
            VkSwapchainCreateInfoKHR swapchainInfo {};
            swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainInfo.surface = surface_;
            swapchainInfo.minImageCount = imageCount;
            swapchainInfo.imageFormat = format.format;
            swapchainInfo.imageColorSpace = format.colorSpace;
            swapchainInfo.imageExtent = swapchainExtent_;
            swapchainInfo.imageArrayLayers = 1;
            swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainInfo.preTransform = capabilities.currentTransform;
            swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
            swapchainInfo.clipped = VK_TRUE;
            require(vkCreateSwapchainKHR(device_, &swapchainInfo, nullptr, &swapchain_), "vkCreateSwapchainKHR failed");

            require(vkGetSwapchainImagesKHR(device_, swapchain_, &imageCount, nullptr), "vkGetSwapchainImagesKHR failed");
            images_.resize(imageCount);
            require(vkGetSwapchainImagesKHR(device_, swapchain_, &imageCount, images_.data()), "vkGetSwapchainImagesKHR failed");
            createRenderPass(format.format);
            createFramebuffers(format.format);
        }

        void createRenderPass(VkFormat format)
        {
            VkAttachmentDescription colorAttachment {};
            colorAttachment.format = format;
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            VkAttachmentReference colorReference { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
            VkSubpassDescription subpass {};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorReference;
            VkRenderPassCreateInfo renderPassInfo {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = 1;
            renderPassInfo.pAttachments = &colorAttachment;
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;
            require(vkCreateRenderPass(device_, &renderPassInfo, nullptr, &renderPass_), "vkCreateRenderPass failed");
        }

        void createFramebuffers(VkFormat format)
        {
            for (const VkImage image : images_)
            {
                VkImageViewCreateInfo imageViewInfo {};
                imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                imageViewInfo.image = image;
                imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                imageViewInfo.format = format;
                imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageViewInfo.subresourceRange.levelCount = 1;
                imageViewInfo.subresourceRange.layerCount = 1;
                VkImageView imageView = VK_NULL_HANDLE;
                require(vkCreateImageView(device_, &imageViewInfo, nullptr, &imageView), "vkCreateImageView failed");
                imageViews_.push_back(imageView);
                VkFramebufferCreateInfo framebufferInfo {};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = renderPass_;
                framebufferInfo.attachmentCount = 1;
                framebufferInfo.pAttachments = &imageViews_.back();
                framebufferInfo.width = swapchainExtent_.width;
                framebufferInfo.height = swapchainExtent_.height;
                framebufferInfo.layers = 1;
                VkFramebuffer framebuffer = VK_NULL_HANDLE;
                require(vkCreateFramebuffer(device_, &framebufferInfo, nullptr, &framebuffer), "vkCreateFramebuffer failed");
                framebuffers_.push_back(framebuffer);
            }
        }

        void destroySwapchainResources()
        {
            for (const VkFramebuffer framebuffer : framebuffers_) { vkDestroyFramebuffer(device_, framebuffer, nullptr); }
            for (const VkImageView imageView : imageViews_) { vkDestroyImageView(device_, imageView, nullptr); }
            framebuffers_.clear();
            imageViews_.clear();
            images_.clear();
            if (renderPass_ != VK_NULL_HANDLE) { vkDestroyRenderPass(device_, renderPass_, nullptr); renderPass_ = VK_NULL_HANDLE; }
            if (swapchain_ != VK_NULL_HANDLE) { vkDestroySwapchainKHR(device_, swapchain_, nullptr); swapchain_ = VK_NULL_HANDLE; }
        }

        void recreateSwapchain()
        {
            vkDeviceWaitIdle(device_);
            destroySwapchainResources();
            createSwapchainResources();
        }

        platform::Window &window_;
        sdl3::VulkanSurfaceProvider *surfaceProvider_;
        VkInstance instance_ = VK_NULL_HANDLE;
        VkSurfaceKHR surface_ = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
        VkDevice device_ = VK_NULL_HANDLE;
        VkQueue graphicsQueue_ = VK_NULL_HANDLE;
        uint32_t queueFamily_ = 0;
        VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
        VkRenderPass renderPass_ = VK_NULL_HANDLE;
        VkCommandPool commandPool_ = VK_NULL_HANDLE;
        VkCommandBuffer commandBuffer_ = VK_NULL_HANDLE;
        VkSemaphore imageAvailable_ = VK_NULL_HANDLE;
        VkSemaphore renderFinished_ = VK_NULL_HANDLE;
        VkExtent2D swapchainExtent_ {};
        std::vector<VkImage> images_;
        std::vector<VkImageView> imageViews_;
        std::vector<VkFramebuffer> framebuffers_;
        std::array<float, 4> clearColor_ { 0.1F, 0.2F, 0.3F, 1.0F };
    };

    VulkanRenderer::VulkanRenderer(platform::Window &window)
    : impl_(std::make_unique<Impl>(window))
    {
    }

    VulkanRenderer::~VulkanRenderer() = default;

    void VulkanRenderer::render()
    {
        impl_->render();
    }

    void VulkanRenderer::setClearColor(float red, float green, float blue, float alpha)
    {
        impl_->setClearColor(red, green, blue, alpha);
    }
}
