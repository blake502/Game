#include "defines.h"

#ifdef WINDOWS
#include <windows.h>
#include <windowsx.h>
#include <timeapi.h>
#include "platform.h"
#include "input.h"
#include "clock.h"

static f64 clock_frequency;
static LARGE_INTEGER start_time;
static UINT min_period;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_SIZE:
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
            //if (MessageBox(hwnd, "Are you sure you want to exit? Any unsaved progress will be lost.", "Game", MB_OKCANCEL) == IDOK)
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        //TODO: Enable
        /*
        case WM_ERASEBKGND:
            //Done to notify the OS that erasing will be handled by the application to prevent flicker.
            return 1;
        */
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            b8 is_key_down = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
            
            keys key = (keys)wParam;

            //TODO: Handle extended scan codes
            /*
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
            i32 x = GET_X_LPARAM(lParam);
            i32 y = GET_Y_LPARAM(lParam);

            // TODO: Handle mouse input
            //onMouseMove(x, y);
        break;
    }
    
    TIMECAPS tc;
    timeGetDevCaps(&tc, sizeof(tc));
    min_period = tc.wPeriodMin;

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void* platform_allocate(u32 size)
{
    return malloc(size);
}

void platform_free(void* block)
{
    free(block);
}



void clock_setup() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);
}

f64 platform_get_absolute_time() {
    if (!clock_frequency) {
        clock_setup();
    }

    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
    clock clock;
    clock_start(&clock);
    timeBeginPeriod(min_period);
    Sleep(ms);
    timeEndPeriod(min_period);

    clock_update(&clock);
    f64 observed = clock.elapsed * 1000.0;
    f64 ms_remaining = ms - observed;

    // spin lock
    clock_start(&clock);
    while(clock.elapsed * 1000.0 < ms_remaining) {
        _mm_pause();
        clock_update(&clock);
    }
}

void create_window(window_info* w_info)
{
    HINSTANCE h_instance = GetModuleHandleA(0);

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = h_instance;
    wc.lpszClassName = w_info->class_name;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                      // Optional window styles.
        w_info->class_name,     // Window class
        w_info->window_title,   // Window text
        WS_OVERLAPPEDWINDOW,    // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,       // Parent window    
        NULL,       // Menu
        h_instance, // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    //TODO: Handle failed to create window
        return;

    ShowWindow(hwnd, 5);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
#endif