
#include <iostream>
#include <windows.h>
#include <CommCtrl.h>
#include <vector>
#include "hooks.h"
#include "Macro.h"
#include <fstream>
#include "gui.h"
#include "WindowProcs.h"

#pragma warning(disable : 4996)

HWND main_window_handle;
HWND overlay_handle;

int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    SetProcessDPIAware();
    

#pragma region create window

    WNDCLASSEX mm_wc = { sizeof(WNDCLASSEX) };
    mm_wc.cbSize = sizeof(WNDCLASSEX);
    mm_wc.style = CS_VREDRAW | CS_HREDRAW;
    mm_wc.lpfnWndProc = &main_window_proc;
    mm_wc.cbClsExtra = 0;
    mm_wc.cbWndExtra = 0;
    mm_wc.hInstance = GetModuleHandle(0);
    mm_wc.hIcon = NULL;
    mm_wc.hCursor = LoadCursor(0, IDC_ARROW);
    mm_wc.hbrBackground = NULL;
    mm_wc.lpszMenuName = NULL;
    mm_wc.lpszClassName = L"mmwcclass";
    mm_wc.hIconSm = NULL;
    if (!RegisterClassEx(&mm_wc)) { return 0; }
    

    HWND window_handle = CreateWindowEx(
        
        WS_EX_LEFT| WS_EX_LTRREADING| WS_EX_RIGHTSCROLLBAR,
        mm_wc.lpszClassName,
        TEXT("Macro Manager"),
        WS_OVERLAPPEDWINDOW|WS_VISIBLE| WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        GetModuleHandle(NULL),
        0);
    
#pragma endregion create window

    main_window_handle = window_handle;

    gui_handles_struct gui_handles = create_gui(window_handle);




    Macro::load_saved_macros();
    
    MSG message;
    
    while (GetMessage(&message, NULL, NULL, NULL)) {

        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    


    return 0;
}