#include "GraphicsDevice.hpp"
#include "libidk/Assert.hpp"

#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>


idk::gfx::GraphicsDevice::GraphicsDevice()
:   mInstance(VK_NULL_HANDLE),
    mSurface(VK_NULL_HANDLE),
    mPhysicalDevice(VK_NULL_HANDLE),
    mDevice(VK_NULL_HANDLE),
    mGraphicsQueue(VK_NULL_HANDLE),
    mPresentQueue(VK_NULL_HANDLE)
{
    volkInitialize();

    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "How to Vulkan",
        .apiVersion = VK_API_VERSION_1_4
    };

    uint32_t instanceExtCount = 0;
    const char *const *instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&instanceExtCount);
    VkInstanceCreateInfo instanceCI = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = instanceExtCount,
        .ppEnabledExtensionNames = instanceExtensions,
    };

    IDK_ASSERT(
        VK_SUCCESS == vkCreateInstance(&instanceCI, nullptr, &mInstance),
        "[GraphicsDevice::GraphicsDevice] vkCreateInstance failure"
    );
}



idk::gfx::GraphicsDevice::~GraphicsDevice()
{
    if (mInstance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(mInstance, nullptr);
        mInstance = VK_NULL_HANDLE;
    }
}

