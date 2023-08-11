#include "renderer_frontend.h"

#include "renderer_backend.h"

#include "core/logging.h"
#include "core/s_memory.h"

static renderer_backend* backend = 0;

b8 renderer_initialize(const char* application_name, struct platform_state* plat_state)
{
    backend = s_allocate(sizeof(renderer_backend), MEMORY_TAG_RENDERER);

    //TODO: Confirgurability
    renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend);
    backend->frame_number = 0;

    if(!backend->initialize(backend, application_name, plat_state))
    {
        S_FATAL("Failed to initialize renderer backend!");
        return false;
    }

    return true;
}

void renderer_shutdown()
{
    backend->shutdown(backend);
    s_free(backend, sizeof(renderer_backend), MEMORY_TAG_RENDERER);
}

b8 renderer_begin_frame(f32 delta_time)
{
    return backend->begin_frame(backend, delta_time);
}

b8 renderer_end_frame(f32 delta_time)
{
    b8 result = backend->begin_frame(backend, delta_time);
    backend->frame_number++;
    return result;
}

b8 renderer_draw_frame(render_packet* packet)
{
    if(renderer_begin_frame(packet->delta_time))
    {
        b8 result = renderer_end_frame(packet->delta_time);
        if(!result)
        {
            S_ERROR("Renderer end fram failed!")
            return false;
        }
    }

    return true;
}

void renderer_on_resized(u16 width, u16 height)
{

}