#include "core\application.h"
#include "core\logging.h"
#include "game_types.h"

extern b8 create_game(game* out_game);


//The main entry point of the application
int main(void)
{
    S_INFO("Starting...");

    game game_inst;
    if(!create_game(&game_inst))
    {
        S_FATAL("Could not create game!");
        return -1;
    }

    if(!game_inst.initialize || !game_inst.update || !game_inst.render || !game_inst.on_resize)
    {
        S_FATAL("Game function pointers are not assigned!");
        return -2;
    }


    if(!application_create(&game_inst))
    {
        S_INFO("Application failed to create!");
        return 1;
    }

    //Begin game loop
    if(!application_run())
    {
        S_INFO("Application did not shut down gracefully!");
        return 2;
    }
    
    return 0;
}