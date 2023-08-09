#pragma once

#include "defines.h"

typedef struct platform_state {
    void* internal_state;
} platform_state;

b8 platform_startup(
    platform_state *plat_state,
    const char *app_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height
);

void platform_shutdown(platform_state *plat_state);

b8 platform_pump_messages(platform_state *plat_state);

S_API void* platform_allocate(u32 size);
S_API void platform_free(void* block);
void platform_zero(void* block, u64 size);
void platform_copy(void* dest, const void* source, u64 size);
void platform_setmem(void* dest, i32 value, u64 size);

void platform_console_write(const char* message, u8 color);
void platform_console_write_error(const char* message, u8 color);


f64 platform_get_absolute_time();
void platform_sleep(u64 ms);