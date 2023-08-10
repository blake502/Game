
#include "application.h"
#include "game_types.h"
#include "assertion.h"
#include "logging.h"
#include "platform/platform.h"
#include "core/event.h"
#include "core/s_memory.h"
#include "core/input.h"

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

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context);
b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context);

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
    
    input_initialize();

    if(!event_initialize())
    {
        S_ERROR("Event system failed to initialize!");
        return false;
    }

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

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
    
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

            //TODO: Detla time
            input_update(0);
        }
    }

    app_state.is_running = false;

    
    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
    input_shutdown();
    event_shutdown();

    platform_shutdown(&app_state.platform);
    return true;
}

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context)
{
    switch (code)
    {
        case EVENT_CODE_APPLICATION_QUIT: {
            S_INFO("Quit code recieved, shutting down.");
            app_state.is_running = false;
            return true;
        }
    }
    return false;
}

b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context)
{
    if(code == EVENT_CODE_KEY_PRESSED)
    {
        u16 key_code = context.data.u16[0];
        S_INFO("'%c' key pressed", key_code);
        if(key_code == KEY_ESCAPE)
        {
            event_context data = {0};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);

            return true;
        }
    }
    return false;
}