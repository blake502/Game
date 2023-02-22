#include "defines.h"

#ifdef WINDOWS
#include <windows.h>
#include "platform.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_SIZE:
                int width = LOWORD(lParam);  // Macro to get the low-order word.
                int height = HIWORD(lParam); // Macro to get the high-order word.

                // Respond to the message:
                //OnSize(hwnd, (UINT)wParam, width, height);
            break;
        case WM_PAINT:
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                // All painting occurs here, between BeginPaint and EndPaint.

                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

                EndPaint(hwnd, &ps);
            break;
        case WM_CLOSE:
            //if (MessageBox(hwnd, "Are you sure you want to exit? Any unsaved progress will be lost.", "Game", MB_OKCANCEL) == IDOK)
                DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void create_window(window_info* w_info)
{
    WNDCLASS wc = { };

    HINSTANCE h_instance = GetModuleHandleA(0);

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = h_instance;
    wc.lpszClassName = w_info->class_name;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
    0,                              // Optional window styles.
    w_info->class_name,                     // Window class
    w_info->window_title,    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    NULL,       // Parent window    
    NULL,       // Menu
    h_instance,  // Instance handle
    NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return;
    }

    ShowWindow(hwnd, 5);

    
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
#endif