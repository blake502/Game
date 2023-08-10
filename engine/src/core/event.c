#include "event.h"

#include "s_memory.h"
#include "core/logging.h"
#include "containers/darray.h"

typedef struct registered_event {
    void* listener;
    PFN_on_event callback;
}registered_event;

typedef struct event_code_entry {
    registered_event* events;
}event_code_entry;

#define MAX_MESSAGE_CODES 4096

typedef struct event_system_state {
    event_code_entry registered[MAX_MESSAGE_CODES];
}event_system_state;

static b8 is_initialized = false;
static event_system_state state;

b8 event_initialize()
{
    if(is_initialized == true)
    {
        return false;
    }

    s_zero_memory(&state, sizeof(state));

    is_initialized = true;
    
    return true;
}

void event_shutdown()
{
    for(u16 i = 0; i < MAX_MESSAGE_CODES; i++)
    {
        if(state.registered[i].events != 0)
        {
            darray_destroy(state.registered[i].events);
            state.registered[i].events = 0;
        }
    }
}

b8 event_register(u16 code, void* listener, PFN_on_event on_event)
{
    if(is_initialized == false)
    {
        S_ERROR("Event register before event system initialized!");
        return false;
    }

    if(state.registered[code].events == 0)
    {
        state.registered[code].events = darray_create(registered_event);
    }

    u64 registered_count = darray_length(state.registered[code].events);
    for(u64 i = 0; i < registered_count; i++)
    {
        if(state.registered[code].events[i].listener == listener)
        {
            //TODO: Warn double listening
            return false;
        }
    }

    registered_event event;
    event.listener = listener;
    event.callback = on_event;
    darray_push(state.registered[code].events, event);

    return true;
}

b8 event_unregister(u16 code, void* listener, PFN_on_event on_event)
{
    if(is_initialized == false)
    {
        S_ERROR("Event unregister before event system initialized!");
        return false;
    }

    if(state.registered[code].events == 0)
    {
        S_WARN("Event to unregister not found");
        return false;
    }

    u64 registered_count = darray_length(state.registered[code].events);
    for(u64 i = 0; i < registered_count; i++)
    {
        registered_event e = state.registered[code].events[i];
        if(e.listener == listener && e.callback == on_event)
        {
            registered_event popped_event;
            darray_pop_at(state.registered[code].events, i, &popped_event);
            return true;
        }
    }

    return false;
}

b8 event_fire(u16 code, void* sender, event_context data)
{
    if(is_initialized == false)
    {
        S_ERROR("Event fired before event system initialized!");
        return false;
    }

    if(state.registered[code].events == 0)
    {
        return false;
    }

    u64 registered_count = darray_length(state.registered[code].events);
    for(u64 i = 0; i < registered_count; i++)
    {
        registered_event e = state.registered[code].events[i];
        if(e.callback(code, sender, e.listener, data))
        {
            return true;
        }
    }

    return false;
}