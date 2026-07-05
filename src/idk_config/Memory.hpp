#pragma once

#include "idk/core/metric.hpp"
#include "idk/core/Allocator.hpp"

namespace idk::config::memory
{
    static constexpr size_t IDK_GFX_ALLOCATOR_SIZE = 256 * idk::KILO;
    using AllocatorType = idk::core::BumpAllocator<IDK_GFX_ALLOCATOR_SIZE>;

    static inline AllocatorType &getAllocator()
    {
        static AllocatorType instance;
        return instance;
    }
}

#include "idk/core/New.hpp"
