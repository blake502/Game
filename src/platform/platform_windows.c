#include "platform.h"

#ifdef WINDOWS
#include "core\logging.h"
#include "core\assertion.h"
#include <windows.h>
#include <windowsx.h>

typedef struct internal_state
{
    HINSTANCE h_instance;
    HWND hwnd;
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

void platform_zero(void* block, u64 size)
{
    memset(block, 0, size);
}

void platform_copy(void* dest, const void* source, u64 size)
{
    memcpy(dest, source, size);
}

void platform_setmem(void* dest, i32 value, u64 size)
{
    memset(dest, value, size);
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

LRESULT CALLBACK windows_proccess_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_ERASEBKGND: S_TRACE("WM_ERASEBKGND"); return 1; //Prevents flicker
        case WM_SIZE:
            S_TRACE("WM_SIZE")
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

            //TODO: Handle resizing
            //OnSize(width, height);
            break;
        case WM_PAINT:
            //TODO: Handle paint case
            /*
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
            */
            break;
        case WM_CLOSE:
            S_INFO("WM_CLOSE");
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            S_INFO("WM_DESTROY");
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:            
            /*
            b8 is_key_down = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);

            //keys key = (keys)wParam;

            //TODO: Handle extended scan codes
            b8 is_extended = (HIWORD(lParam) & KF_EXTENDED) == KF_EXTENDED;
            if (wParam == VK_MENU) {
                key = is_extended ? KEY_RALT : KEY_LALT;
            } else if (wParam == VK_SHIFT) {
                // Annoyingly, KF_EXTENDED is not set for shift keys.
                u32 left_shift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
                u32 scancode = ((lParam & (0xFF << 16)) >> 16);
                key = scancode == left_shift ? KEY_LSHIFT : KEY_RSHIFT;
            } else if (wParam == VK_CONTROL) {
                key = is_extended ? KEY_RCONTROL : KEY_LCONTROL;
            }
            */

            break;
        case WM_MOUSEMOVE:
            //i32 x = GET_X_LPARAM(lParam);
            //i32 y = GET_Y_LPARAM(lParam);

            // TODO: Handle mouse input
            //onMouseMove(x, y);
        break;
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

#endif