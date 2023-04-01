#include "defines.h"
#include "core/assertion.h"
#include "platform/platform.h"


int main()
{
    platform_state state;
    S_ASSERT_MSG(platform_startup(&state, "Test", 50, 50, 1280, 720), "Failed to start application");

    while(true)
        if(!platform_pump_messages(&state))
            break;

    platform_shutdown(&state);
    return 0;
}