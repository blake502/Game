#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "platform.h"
#include "clock.h"


int main()
{
    /*
    window_info* w_info = platform_allocate(sizeof(window_info));
    w_info->class_name = "smudge_window_class";
    w_info->window_title = "Hello";

    create_window(w_info);

    platform_free(w_info);
    */

    clock clock;
    printf("Begin");
    clock_start(&clock);
    platform_sleep(1000/240);
    clock_update(&clock);
    printf("End %f", clock.elapsed*1000);

    return 0;
}