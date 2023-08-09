
#include "application.h"
#include "game_types.h"
#include "assertion.h"
#include "logging.h"
#include "platform/platform.h"
#include "core/s_memory.h"

typedef struct application_state {
    game* game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialized = false;
static application_state app_state;

b8 application_create(game* game_inst)
{
    application_config* app_config = &game_inst->app_config;

    if(initialized)
    {
        S_ERROR("Attempted to initialize application! Application is already initialized!");
        return false;
    }
    
    app_state.game_inst = game_inst;

    app_state.is_running = true;
    app_state.is_suspended = false;

    if(!platform_startup(&app_state.platform,
        app_config->name,
        app_config->start_pos_x,
        app_config->start_pos_y,
        app_config->start_width,
        app_config->start_height))
    {
        S_ERROR("Failed to initiate platform layer.");
        return false;
    }

    if(!app_state.game_inst->initialize(app_state.game_inst))
    {
        S_FATAL("Could not initialize game!");
        return false;
    }

    app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);
    
    initialized = true;
    return true;
}

b8 application_run()
{
    S_INFO(get_memory_usage_str());
    
    while(app_state.is_running)
    {
        if(!platform_pump_messages(&app_state.platform))
        {
            app_state.is_running = false;
            break;
        }
        if(!app_state.is_suspended)
        {
            //TODO: Delta time
            if(!app_state.game_inst->update(app_state.game_inst, (f32)0))
            {
                S_FATAL("Game update failed, shutting down!");
                app_state.is_running = false;
                break;
            }

            if(!app_state.game_inst->render(app_state.game_inst, (f32)0))
            {
                S_FATAL("Game render failed, shutting down!");
                app_state.is_running = false;
                break;
            }
        }
    }

    app_state.is_running = false;
    platform_shutdown(&app_state.platform);
    return true;
}