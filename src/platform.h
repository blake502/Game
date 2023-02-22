#pragma once

typedef struct window_info{
    char* window_title;
    char* class_name;
} window_info;

void create_window(window_info* w_info);