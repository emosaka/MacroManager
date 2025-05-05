#include <windows.h>
#include "gui.h"
#include "Macro.h"
#include "WindowProcs.h"
#include "helpers.h"

gui_handles_struct create_gui(HWND window_handle)
{
    create_window_classes();

    RECT wind_dims;
    GetClientRect(window_handle, &wind_dims);

    gui_handles_struct bot_gui_handles = create_bot_gui(window_handle,wind_dims);
    gui_handles_struct top_gui_handles = create_top_gui(window_handle,wind_dims);

    gui_handles_struct gui_handles = {0};

    gui_handles.bot_gui = bot_gui_handles.bot_gui;
    gui_handles.top_gui = top_gui_handles.top_gui;
    gui_handles.play_button = top_gui_handles.play_button;
    gui_handles.rec_button = top_gui_handles.rec_button;
    gui_handles.macro_list = top_gui_handles.macro_list;

    return gui_handles;
}


void create_window_classes()
{

	WNDCLASSEX top_gui_class = { sizeof(WNDCLASSEX) };
	top_gui_class.cbSize = sizeof(WNDCLASSEX);
	top_gui_class.style = CS_VREDRAW | CS_HREDRAW;
	top_gui_class.lpfnWndProc = &top_gui_proc;
	top_gui_class.cbClsExtra = 0;
	top_gui_class.cbWndExtra = 0;
	top_gui_class.hInstance = GetModuleHandle(0);
	top_gui_class.hIcon = NULL;
	top_gui_class.hCursor = LoadCursor(0, IDC_ARROW);
	top_gui_class.hbrBackground =(HBRUSH) COLOR_HIGHLIGHT +1;
	top_gui_class.lpszMenuName = NULL;
	top_gui_class.lpszClassName = L"top gui class";
	top_gui_class.hIconSm = NULL;

	RegisterClassEx(&top_gui_class);


	WNDCLASSEX overlay_class = { 0 };
    overlay_class.cbSize = sizeof(WNDCLASSEX);
    overlay_class.lpfnWndProc = &overlay_proc;
    overlay_class.hInstance = GetModuleHandle(0);
    overlay_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    overlay_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    overlay_class.lpszClassName = L"overlay class";

    RegisterClassEx(&overlay_class);

}


gui_handles_struct create_bot_gui(HWND window_handle,RECT wind_dims)
{

    HWND hwnd_bot_gui = CreateWindowEx(
        0L,
        L"static",
        L"BOTGUI",
        WS_VISIBLE | WS_CHILD | SS_BLACKRECT,    //  styles 
        0,                          //  x position 
        wind_dims.bottom / 2,      //  y position 
        wind_dims.right,          //  width
        wind_dims.bottom / 2,    //  height
        window_handle,          //  parent window
        (HMENU)BOT_GUI,              //  identifier
        GetModuleHandle(0),
        NULL);

    gui_handles_struct bot_gui_handles = { 0 };
    bot_gui_handles.bot_gui = hwnd_bot_gui;
    return bot_gui_handles;
}


gui_handles_struct create_top_gui(HWND window_handle, RECT wind_dims)
{
    HWND hwnd_top_gui = CreateWindowEx(
        0L,
        L"top gui class",
        L"TOPGUI",                    
        WS_VISIBLE | WS_CHILD| WS_CLIPCHILDREN,       //  styles 
        0,                           //  x position 
        0,                          //  y position 
        wind_dims.right,           //  width
        wind_dims.bottom / 2,     //  height
        window_handle,           //  parent window
        (HMENU)TOP_GUI,               //  identifier
        GetModuleHandle(0),
        NULL);

    RECT top_gui_dims;
    GetClientRect(hwnd_top_gui, &top_gui_dims);


    HWND hwnd_rec_button = CreateWindowEx(
        0L,
        L"button",
        L"record",                 
        WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON | BS_TEXT,    //  styles 
        top_gui_dims.right  / 4,    //  x position 
        top_gui_dims.bottom / 4,     //  y position 
        top_gui_dims.right  / 8,     //  width
        top_gui_dims.bottom / 2,     //  height
        hwnd_top_gui,              //  parent window
        (HMENU)REC_BUTTON,           //  identifier
        GetModuleHandle(0),
        NULL);

    HWND hwnd_stopcode_button = CreateWindowEx(
		0L,
		L"static",
		L"None",
		WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_CENTER |SS_CENTERIMAGE,    //  styles 
		top_gui_dims.right * 9/32,   //  x position 
		top_gui_dims.bottom * 25/32,   //  y position 
		top_gui_dims.right * 2/32,       //  width
		top_gui_dims.bottom * 6/32,   //  height
		hwnd_top_gui,            //  parent window
		(HMENU)STOPCODE_BUTTON,     //  identifier
		GetModuleHandle(0),
		NULL);


    HWND hwnd_play_button = CreateWindowEx(
        0L,
        L"button",
        L"play",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_TEXT,    //  styles 
        top_gui_dims.right  / 16,       //  x position 
        top_gui_dims.bottom / 4,        //  y position 
        top_gui_dims.right  / 8,      //  width
        top_gui_dims.bottom / 2,    //  height
        hwnd_top_gui,             //  parent window
        (HMENU)PLAY_BUTTON,       //  identifier
        GetModuleHandle(0),
        NULL);



#pragma region create listview
    HWND hwnd_macro_list = CreateWindowEx(
        0L,
        WC_LISTVIEW,
        L"Macro List",
        WS_BORDER |       //  styles 
        WS_VISIBLE |
        WS_CHILD |
        LVS_REPORT |
        WS_CLIPCHILDREN |
        LVS_EDITLABELS |
        LVS_SINGLESEL |
        LVS_SHOWSELALWAYS,
        top_gui_dims.right * 10/16,     //  x position 
        top_gui_dims.bottom / 4,       //  y position 
        top_gui_dims.right * 5/16,    //  width
        top_gui_dims.bottom / 2,     //  height
        hwnd_top_gui,               //  parent window
        (HMENU)MACRO_LISTBOX,      //  identifier
        GetModuleHandle(0),
        NULL);

	ListView_SetExtendedListViewStyle(
        hwnd_macro_list,
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_DOUBLEBUFFER|
        LVS_EX_AUTOSIZECOLUMNS|
        LVS_EX_CHECKBOXES);
        
	WCHAR macro_name_column_text[] = L"Macro";
    LVCOLUMN macro_name_column = { 0 };
    macro_name_column.mask = LVCF_TEXT | LVCF_ORDER | LVCF_MINWIDTH|LVCF_WIDTH;
    macro_name_column.pszText = macro_name_column_text;
    macro_name_column.cxMin = 256;
    macro_name_column.cx = 256;
    macro_name_column.iOrder = 0;
    ListView_InsertColumn(hwnd_macro_list, 0, &macro_name_column);


    WCHAR macro_hotkey_column_text[] = L"Hotkey";
    LVCOLUMN macro_hotkey_column = { 0 };
    macro_hotkey_column.mask = LVCF_TEXT | LVCF_ORDER | LVCF_MINWIDTH | LVCF_WIDTH;
    macro_hotkey_column.pszText = macro_hotkey_column_text;
    macro_hotkey_column.cxMin = 256;
    macro_hotkey_column.cx = 256;
    macro_hotkey_column.iOrder = 1;
    ListView_InsertColumn(hwnd_macro_list, 1, &macro_hotkey_column);
#pragma endregion create listview



    HWND hwnd_new_macro_button = CreateWindowEx(
        0L,
        L"button",
        L"New Macro",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_TEXT | BS_MULTILINE,    //  styles 
        top_gui_dims.right * 35/64,         //  x position 
        top_gui_dims.bottom * 9/32,        //  y position 
        top_gui_dims.right / 16,           //  width
        top_gui_dims.bottom * 4/32,     //  height
        hwnd_top_gui,                 //  parent window
        (HMENU)NEW_MACRO_BUTTON,     //  identifier
        GetModuleHandle(0),
        NULL);


    HWND hwnd_activate_button = CreateWindowEx(
		0L,
		L"button",
		L"Activate macros",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_TEXT | BS_MULTILINE,    //  styles 
		top_gui_dims.right * 35 / 64,      //  x position 
		top_gui_dims.bottom * 14 / 32,     //  y position 
		top_gui_dims.right / 16,           //  width
		top_gui_dims.bottom * 4 / 32,     //  height
		hwnd_top_gui,                 //  parent window
		(HMENU)ACTIVATE_BUTTON,          //  identifier
		GetModuleHandle(0),
		NULL);


	HWND hwnd_save_button = CreateWindowEx(
		0L,
		L"button",
		L"Save Macro",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_TEXT | BS_MULTILINE,    //  styles 
		top_gui_dims.right * 35 / 64,    //  x position 
		top_gui_dims.bottom * 19 / 32,       //  y position 
		top_gui_dims.right / 16,           //  width
		top_gui_dims.bottom * 4 / 32,     //  height
		hwnd_top_gui,            //  parent window
		(HMENU)SAVE_BUTTON,         //  identifier
		GetModuleHandle(0),
		NULL);

	 overlay_handle = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
		L"overlay class",
		L"Recording Overlay",
		WS_POPUP,
		0,
        0,
		200,
        100,
		main_window_handle,
        NULL,
        GetModuleHandle(0),
        NULL);

	//  window transparency
	SetLayeredWindowAttributes(overlay_handle, 0, 200, LWA_ALPHA);






    //unused
    gui_handles_struct top_gui_handles = { 0 };
    top_gui_handles.top_gui = hwnd_top_gui;
    top_gui_handles.rec_button = hwnd_rec_button;
    top_gui_handles.play_button = hwnd_play_button;
    top_gui_handles.macro_list = hwnd_macro_list;

    return top_gui_handles;
}



