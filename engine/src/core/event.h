#pragma once

#include "defines.h"

typedef struct event_context
{
    //128 byte data
    union
    {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];

        i16 i16[8];
        u16 u16[8];

        i8 i8[16];
        u8 u8[16];

        char c[16];
    } data;
} event_context;


typedef enum system_event_code {
    //Shuts down the application next frame
    EVENT_CODE_APPLICATION_QUIT,

    //Keyboard key pressed
    //Usage:
    //u16 key_code = data.data.u16[0];
    EVENT_CODE_KEY_PRESSED,

    //Keyboard key released
    //Usage:
    //u16 key_code = data.data.u16[0];
    EVENT_CODE_KEY_RELEASED,

    //Mouse button pressed
    //Usage:
    //u16 button = data.data.u16[0];
    EVENT_CODE_BUTTON_PRESSED,

    //Mouse button released
    //Usage:
    //u16 button = data.data.u16[0];
    EVENT_CODE_BUTTON_RELEASED,
    
    //Mouse moved
    //Usage:
    //u16 x = data.data.u16[0];
    //u16 y = data.data.u16[1];
    EVENT_CODE_MOUSE_MOVED,

    //Mouse wheel
    //Usage:
    //u16 delta = data.data.u8[0];
    EVENT_CODE_MOUSE_WHEEL,

    //Resize
    //Usage:
    //u16 width = data.data.u16[0];
    //u16 height = data.data.u16[1];
    EVENT_CODE_RESIZED,

    //The max codes defined by the engine
    EVENT_CODE_MAX = 0XFF
} system_event_code;

typedef b8 (*PFN_on_event)(u16 code, void* sender, void* listener_inst, event_context data);

b8 event_initialize();
void event_shutdown();

S_API b8 event_register(u16 code, void* listener, PFN_on_event on_event);

S_API b8 event_unregister(u16 code, void* listener, PFN_on_event on_event);

S_API b8 event_fire(u16 code, void* sender, event_context data);