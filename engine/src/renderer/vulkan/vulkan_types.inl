#pragma once

#include "defines.h"

#include <vulkan/vulkan.h>

#define VK_CHECK(expr)           \
{                                \
    S_ASSERT(expr == VK_SUCCESS);\
}

typedef struct vulkan_context {
    VkInstance instance;
    VkAllocationCallbacks* allocator;
#ifdef DEBUG
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} vulkan_context;

