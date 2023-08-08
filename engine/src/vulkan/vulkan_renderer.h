#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include <vulkan/vulkan.h>

int initialize_vulkan()
{
    VkApplicationInfo application_info = {};
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.apiVersion = VK_API_VERSION_1_2;

    const char* instance_extensions[2] = {"VK_KHR_surface",
            "VK_KHR_win32_surface"};

    const char* enabled_layers[1] = {"VK_LAYER_KHRONOS_validation"};

    VkInstanceCreateInfo instance_create_info = {};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &application_info;
    instance_create_info.enabledExtensionCount = 2;
    instance_create_info.ppEnabledExtensionNames = instance_extensions;
    instance_create_info.enabledLayerCount = 1;
    instance_create_info.ppEnabledLayerNames = enabled_layers;

    VkInstance instance;
    VkResult instance_create_result = vkCreateInstance(&instance_create_info, 0, &instance);
    //CHECK_VULKAN_RESULT(instance_create_result);

    uint32_t count;
    vkEnumeratePhysicalDevices(instance, &count, 0);
    //assert(count > 0);

    VkPhysicalDevice* physical_devices =
            (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * count);   
    vkEnumeratePhysicalDevices(instance, &count, physical_devices);

    VkPhysicalDevice physical_device = physical_devices[0];

    f32 priority = 1.0f;
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = 0;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &priority;

    const char* enabled_extensions[1] = {"VK_KHR_swapchain"};

    VkDeviceCreateInfo device_create_info = {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.enabledExtensionCount = 1;
    device_create_info.ppEnabledExtensionNames = enabled_extensions;

    VkDevice device;
    VkResult device_create_result = vkCreateDevice(physical_device, &device_create_info, 0, &device);
    //CHECK_VULKAN_RESULT(device_create_result);

    VkImageCreateInfo image_create_info = {};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = VK_FORMAT_R8G8B8A8_UNORM;
    image_create_info.extent.width = 1280;
    image_create_info.extent.height = 720;
    image_create_info.arrayLayers = 1;
    image_create_info.mipLevels = 1;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;

    VkImage image;
    VkResult image_create_result = vkCreateImage(device, &image_create_info, 0, &image);
    //CHECK_VULKAN_RESULT(image_create_result);


    //VkSurfaceKHR surface = {};

    return 0;
}