#pragma once
#include "defines.h"

typedef struct window_info{
    char* window_title;
    char* class_name;
} window_info;

void create_window(window_info* w_info);

void* platform_allocate(u32 size);
void platform_free(void* block);