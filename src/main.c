#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "platform.h"


int main()
{
    window_info* w_info = (window_info*)malloc(sizeof(window_info));
    w_info->class_name = "smudge_window_class";
    w_info->window_title = "Hello";
    create_window(w_info);

    return 0;
}