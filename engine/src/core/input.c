#include <core/input.h>
#include <core/event.h>
#include <core/s_memory.h>
#include <core/logging.h>

typedef struct keyboard_state {
    b8 keys[KEYS_MAX_KEYS];
} keyboard_state;

typedef struct mouse_state {
    i16 x;
    i16 y;
    u8 buttons[BUTTON_MAX];
} mouse_state;

typedef struct input_state{
    keyboard_state keyboard_current;
    keyboard_state keyboard_previous;
    mouse_state mouse_current;
    mouse_state mouse_previous;
} input_state;

static b8 initialized = false;
static input_state state = {0};

void input_initialize()
{
    s_zero_memory(&state, sizeof(input_state));
    initialized = true;
    S_INFO("Input initialized.");
}
void input_shutdown()
{
    //TODO: Shutdown routines
    initialized = false;
}

void input_update(f64 delta_time)
{
    if (!initialized)
        return;
        
    s_copy_memory(&state.keyboard_previous, &state.keyboard_current, sizeof(keyboard_state));
    s_copy_memory(&state.mouse_previous, &state.mouse_current, sizeof(mouse_state));
}

b8 input_is_key_down(keys key)
{
    if(!initialized)
        return false;

    return state.keyboard_current.keys[key] == true;
}

b8 input_is_key_up(keys key)
{
    if(!initialized)
        return false;

    return state.keyboard_current.keys[key] == false;
}

b8 input_was_key_down(keys key)

{
    if(!initialized)
        return false;

    return state.keyboard_previous.keys[key] == true;
}

b8 input_was_key_up(keys key)
{
    if(!initialized)
        return false;

    return state.keyboard_previous.keys[key] == false;
}

void input_process_key(keys key, b8 pressed)
{
    if(state.keyboard_current.keys[key] != pressed)
    {
        state.keyboard_current.keys[key] = pressed;

        event_context context;
        context.data.u16[0] = key;
        event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    }
}

b8 input_is_button_down(buttons button)
{
    if(!initialized)
        return false;

    return state.mouse_current.buttons[button] == true;
}

b8 input_is_button_up(buttons button)
{
    if(!initialized)
        return false;

    return state.mouse_current.buttons[button] == false;
}

b8 input_was_button_down(buttons button)
{
    if(!initialized)
        return false;

    return state.mouse_previous.buttons[button] == true;
}

b8 input_was_button_up(buttons button)
{
    if(!initialized)
        return false;

    return state.mouse_previous.buttons[button] == false;
}

void input_get_mouse_position(i32* x, i32* y)
{
    if(!initialized)
    {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mouse_current.x;
    *y = state.mouse_current.y;
}

void input_get_previous_mouse_position(i32* x, i32* y)
{
    if(!initialized)
    {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mouse_previous.x;
    *y = state.mouse_previous.y;
}

void input_process_button(buttons button, b8 pressed)
{
    if(state.mouse_current.buttons[button] != pressed)
    {
        state.mouse_current.buttons[button] = pressed;

        event_context context;
        context.data.u16[0] = button;
        event_fire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}

void input_process_mouse_move(i16 x, i16 y)
{
    //S_INFO("Mouse move: (%i, %i)", x, y);
    if(state.mouse_current.x != x || state.mouse_current.y != y)
    {
        state.mouse_current.x = x;
        state.mouse_current.y = y;

        event_context context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        event_fire(EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}

void input_process_mouse_wheel(i8 delta)
{
    event_context context;
    context.data.u8[0] = delta;
    event_fire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}