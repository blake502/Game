#include <defines.h>
#include <core\assertion.h>
#include <core\application.h>
#include <core\logging.h>

int main()
{
    S_INFO("Starting...");
    
    application_config app_config;
    app_config.name = "Game Application";
    app_config.start_pos_x = 50;
    app_config.start_pos_y = 80;
    app_config.start_width = 1280;
    app_config.start_height = 720;

    application_create(&app_config);
    application_run();

    return 0;
}