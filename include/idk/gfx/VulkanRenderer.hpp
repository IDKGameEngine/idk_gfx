#pragma once

#include "libidk/platform/Platform.hpp"

#include <memory>

namespace idk::gfx
{
    // Owns the Vulkan instance, device, swapchain, and presentation loop.
    // A platform supplies the native surface; it never owns rendering state.
    class VulkanRenderer
    {
    public:
        explicit VulkanRenderer(platform::Window &window);
        ~VulkanRenderer();

        VulkanRenderer(const VulkanRenderer &) = delete;
        VulkanRenderer &operator=(const VulkanRenderer &) = delete;

        void render();
        void setClearColor(float red, float green, float blue, float alpha);

    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
    };
}
