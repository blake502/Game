#include "game.h"

#include <core/logging.h>

b8 game_initialize(game* game_inst)
{
    S_TRACE("Test trace message");
    S_DEBUG("Test debug message");
    S_INFO("Test info message");
    S_WARN("Test warn message");
    S_ERROR("Test error message");
    S_FATAL("Test fatal message");
    return true;
}

b8 game_update(game* game_inst, f32 delta_time)
{
    return true;
}

b8 game_render(game* game_inst, f32 delta_time)
{
    return true;
}

void game_on_resize(game* game_inst, u32 width, u32 height)
{
    
}