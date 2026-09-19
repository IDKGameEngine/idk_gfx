#pragma once

#define VK_NO_PROTOTYPES
#include <volk/volk.h>
 
namespace idk::gfx
{
    class GraphicsDevice
    {
    public:
        GraphicsDevice();
        ~GraphicsDevice();

    private:
        VkInstance       mInstance;
        VkSurfaceKHR     mSurface;
        VkPhysicalDevice mPhysicalDevice;
        VkDevice         mDevice;
        VkQueue          mGraphicsQueue;
        VkQueue          mPresentQueue;
    };
}

