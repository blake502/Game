#include "defines.h"
#include "platform.h"

#ifdef WINDOWS
#include "core/logging.h"
#include "core/assertion.h"
#include "core/event.h"
#include "core/input.h"

#include "containers/darray.h"

#include "renderer/vulkan/vulkan_platform.h"

#include <windows.h>
#include <windowsx.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include "renderer/vulkan/vulkan_types.inl"

typedef struct internal_state
{
    HINSTANCE h_instance;
    HWND hwnd;
    VkSurfaceKHR surface;
}internal_state;

static f64 clock_frequency;
static LARGE_INTEGER start_time;

//Prototype
LRESULT CALLBACK windows_proccess_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

b8 platform_startup(
    platform_state *plat_state,
    const char *app_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height){
    
    plat_state->internal_state = malloc(sizeof(internal_state));

    internal_state *state = (internal_state*)plat_state->internal_state;

    state->h_instance = GetModuleHandleA(0);

    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));

    wc.lpfnWndProc = windows_proccess_message;
    wc.hInstance = state->h_instance;
    wc.lpszClassName = "smudge_window_class";

    S_ASSERT_MSG(RegisterClassA(&wc), "Failed to register window class");

    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    window_x += border_rect.left;
    window_y += border_rect.top;

    window_width += border_rect.right - border_rect.left;
    window_width += border_rect.bottom - border_rect.top;

    state->hwnd = CreateWindowExA(
        window_ex_style, wc.lpszClassName, app_name, window_style,
        window_x, window_y, window_width, window_height, 0 , 0 ,
        state->h_instance, 0);

    S_ASSERT_MSG(state->hwnd, "Failed to create window!");

    ShowWindow(state->hwnd, SW_SHOW);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return true;
}

void platform_shutdown(platform_state *plat_state)
{
    internal_state *state = (internal_state*)plat_state->internal_state;

    if(state && state->hwnd)
    {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    }
}

b8 platform_pump_messages(platform_state *plat_state)
{
    MSG message;
    while(PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT)
            return false;
        
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return true;
}

void* platform_allocate(u32 size)
{
    return malloc(size);
}

void platform_free(void* block)
{
    free(block);
}

void* platform_zero(void* block, u64 size)
{
    return memset(block, 0, size);
}

void* platform_copy(void* dest, const void* source, u64 size)
{
    return memcpy(dest, source, size);
}

void* platform_setmem(void* dest, i32 value, u64 size)
{
    return memset(dest, value, size);
}

void platform_console_write(const char* message, u8 color)
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(console_handle, message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char* message, u8 color)
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
}

f64 platform_get_absolute_time()
{
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms)
{
    Sleep(ms);
}

void platform_get_required_extension_names(const char ***names_darray)
{
    darray_push(*names_darray, &"VK_KHR_win32_surface")
}

b8 platform_create_vulkan_surface(platform_state *plat_state, vulkan_context* context)
{
    internal_state *state = (internal_state*)plat_state->internal_state;

    VkWin32SurfaceCreateInfoKHR create_info = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    create_info.hinstance = state->h_instance;
    create_info.hwnd = state->hwnd;

    VkResult result = vkCreateWin32SurfaceKHR(context->instance, &create_info, context->allocator, &state->surface);
    if(result != VK_SUCCESS)
    {
        S_FATAL("Failed to create Vulkan surface");
        return false;
    }

    context->surface = state->surface;
    return true;
}

LRESULT CALLBACK windows_proccess_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        /*case WM_ERASEBKGND:
        {
            S_TRACE("WM_ERASEBKGND");
            return 1; //Prevents flicker?
        }*/
        case WM_SIZE:
        {
            RECT r;
            GetClientRect(hwnd, &r);

            u32 width = r.right - r.left;
            u32 height = r.bottom - r.top;

            event_context context;
            context.data.u16[0] = width;
            context.data.u16[0] = height;
            event_fire(EVENT_CODE_RESIZED, 0, context);
            
        }break;
        case WM_PAINT:
        {
            //TODO: Handle paint case
            /*
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
            */
        } break;
        case WM_CLOSE:
        {
            S_INFO("WM_CLOSE");
            DestroyWindow(hwnd);
        } break;
        case WM_DESTROY: {
            event_context context = {0};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, context);
            S_INFO("WM_DESTROY");
            PostQuitMessage(0);
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            b8 pressed = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
            keys key = (u16)wParam;

            input_process_key(key, pressed);
            
        } break;
        case WM_MOUSEMOVE:
        {
            i32 x = GET_X_LPARAM(lParam);
            i32 y = GET_Y_LPARAM(lParam);

            input_process_mouse_move(x, y);
        } break;
        case WM_MOUSEWHEEL:
        {
            i32 delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (delta != 0)
            {
                delta = (delta < 0) ? -1 : 1;
                input_process_mouse_wheel(delta);
            }
            
        }break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            b8 pressed = uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_LBUTTONDOWN;
            buttons mouse_button = BUTTON_MAX;
            switch (uMsg)
            {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    mouse_button = BUTTON_LMB;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    mouse_button = BUTTON_MMB;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    mouse_button = BUTTON_RMB;
                    break;
            }

            if(mouse_button != BUTTON_MAX)
                input_process_button(mouse_button, pressed);

            
        }break;
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

#endif