
#include "application.h"
#include "game_types.h"
#include "assertion.h"
#include "logging.h"

#include "platform/platform.h"

#include "core/event.h"
#include "core/s_memory.h"
#include "core/input.h"
#include "core/clock.h"

#include "renderer/renderer_frontend.h"

typedef struct application_state {
    game* game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    clock clock;
    f64 last_time;
} application_state;

//TODO: Configurable target FPS
const f64 target_frame_seconds = 1.0f / 240;

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

    if (!renderer_initialize(game_inst->app_config.name, &app_state.platform))
    {
        S_FATAL("Failed to initialize renderer!");
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
    clock_start(&app_state.clock);
    clock_update(&app_state.clock);
    app_state.last_time = app_state.clock.elapsed;
    f64 running_time = 0;
    u8 frame_count = 0;

    S_INFO(get_memory_usage_str());

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
    
    //The main game loop
    while(app_state.is_running)
    {
        if(!platform_pump_messages(&app_state.platform))
        {
            S_INFO("Platform initiated shutdown...");
            app_state.is_running = false;
            break;
        }

        if(!app_state.is_suspended)
        {

            #pragma region Clock Update
            clock_update(&app_state.clock);
            f64 current_time = app_state.clock.elapsed;
            f64 delta = (current_time - app_state.last_time);
            f64 fram_start_time = platform_get_absolute_time();
            #pragma endregion

            input_update(delta);

            #pragma region Application
            if(!app_state.game_inst->update(app_state.game_inst, (f32)delta))
            {
                S_FATAL("Game update failed, shutting down!");
                app_state.is_running = false;
                break;
            }
            if(!app_state.game_inst->render(app_state.game_inst, (f32)delta))
            {
                S_FATAL("Game render failed, shutting down!");
                app_state.is_running = false;
                break;
            }
            #pragma endregion

            #pragma region Redering
            //TODO: Render packet
            render_packet packet;
            packet.delta_time = delta;
            renderer_draw_frame(&packet);
            #pragma endregion

            #pragma region Sleep/Timing
            f64 frame_end_time = platform_get_absolute_time();
            f64 frame_elapsed_time = frame_end_time - fram_start_time;
            running_time += frame_elapsed_time;
            f64 remaining_seconds = target_frame_seconds - frame_elapsed_time;
            
            b8 limit_frames = false;
            if(remaining_seconds > 0 && limit_frames)
            {
                u64 remaining_ms = (remaining_seconds * 1000);

                //TODO: Enable sleeping
                if (remaining_ms > 0)
                    platform_sleep(remaining_ms - 1);

                frame_count++;
            }
            #pragma endregion

            app_state.last_time = current_time;
        }
    }

    application_shutdown();

    return true;
}

void application_shutdown()
{
    app_state.is_running = false;

    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    input_shutdown();

    event_shutdown();
    
    renderer_shutdown();

    platform_shutdown(&app_state.platform);
}

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context)
{
    switch (code)
    {
        case EVENT_CODE_APPLICATION_QUIT:
        {
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