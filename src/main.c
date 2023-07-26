#include "defines.h"
#include "core/assertion.h"
#include "core/logging.h"
#include "platform/platform.h"
#include "vulkan/vulkan_renderer.h"


int main()
{
    S_INFO("Starting");
    platform_state state;
    S_ASSERT_MSG(platform_startup(&state, "Test", 50, 50, 1280, 720), "Failed to initiate platform layer.");

    while(true)
        if(!platform_pump_messages(&state))
            break;

    platform_shutdown(&state);
    return 0;
}