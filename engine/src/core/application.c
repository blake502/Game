#include "application.h"
#include "assertion.h"
#include "logging.h"
#include "platform/platform.h"

typedef struct application_state {
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialized = false;
static application_state app_state;

S_API b8 application_create(application_config* config)
{
    S_ASSERT_MSG(!initialized, "Attempted to initialize application! Application is already initialized!");
    app_state.is_running = true;
    app_state.is_suspended = false;

    initialize_logging();

    S_ASSERT_MSG(
        platform_startup(&app_state.platform,
            config->name,
            config->start_pos_x,
            config->start_pos_y,
            config->start_width,
            config->start_height),
        "Failed to initiate platform layer.");

    
    initialized = true;
    return true;
}

b8 application_run()
{
    while(app_state.is_running)
        if(!platform_pump_messages(&app_state.platform))
        {
            app_state.is_running = false;
            break;
        }

    app_state.is_running = false;
    platform_shutdown(&app_state.platform);
    return true;
}