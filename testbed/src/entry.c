#include "game.h"
#include <entry.h>

#include <platform/platform.h>

b8 create_game(game* out_game)
{
    application_config* app_config = &out_game->app_config;
    app_config->start_pos_x = 50;
    app_config->start_pos_y = 80;
    app_config->start_width = 1280;
    app_config->start_height = 720;
    app_config->name = "Game Application";

    out_game->initialize = game_initialize;
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->on_resize = game_on_resize;

    out_game->state = platform_allocate(sizeof(game_state));

    return true;
}