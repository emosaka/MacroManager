#include <Windows.h>
#include <iostream>
#include <string>

#include "WindowProcs.h"
#include "hooks.h"
#include "Macro.h"


//LowLevelKeyboardProc in docs
LRESULT CALLBACK read_macro_hook(int nCode, WPARAM wParam, LPARAM lParam) {


    if (nCode != HC_ACTION) { return CallNextHookEx(nullptr, nCode, wParam, lParam); }


    KBDLLHOOKSTRUCT* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);


    if (kb->scanCode == Macro::stopcode.ki.wScan) {

		ShowWindow(overlay_handle, SW_HIDE);
		UpdateWindow(overlay_handle);

        Macro::stop_reading();
        PostQuitMessage(NULL);
    }
    else {
        Macro::macro_list[Macro::recording_macro_index]->add_input(wParam, lParam);
    }


    return 0; //message processed. (not 0, so that nothing is typed etc.)
}

LRESULT read_stopcode_hook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode != HC_ACTION) { return CallNextHookEx(nullptr, nCode, wParam, lParam); }

	KBDLLHOOKSTRUCT* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	Macro::set_stopcode(wParam,lParam);

    HWND top_gui_handle = GetDlgItem(main_window_handle, TOP_GUI);
    HWND stopcode_button_handle = GetDlgItem(top_gui_handle, STOPCODE_BUTTON);

    wchar_t key_name[32] = {0};

    LONG key_flags = kb->scanCode << 16;
    if (kb->flags & LLKHF_EXTENDED) { key_flags = key_flags | (1 << 24); }

    if (!GetKeyNameTextW(key_flags, key_name, 32))
    {
        wcscpy_s(key_name,L"unknown key");
    }

    SetWindowTextW(stopcode_button_handle, key_name);

    PostQuitMessage(NULL);

	return -1;
}


LRESULT read_hotkey_hook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode != HC_ACTION) { return CallNextHookEx(nullptr, nCode, wParam, lParam); }

	KBDLLHOOKSTRUCT* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	Macro::macro_list[Macro::recording_macro_index]->set_hotkey(wParam, lParam);

	wchar_t hotkey_name[32] = { 0 };

	LONG key_flags = kb->scanCode << 16;
	if (kb->flags & LLKHF_EXTENDED) { key_flags = key_flags | (1 << 24); }

	if (!GetKeyNameTextW(key_flags, hotkey_name, 32))
	{
		wcscpy_s(hotkey_name, L"unknown key");
	}

	HWND top_gui_handle = GetDlgItem(main_window_handle, TOP_GUI);
	HWND listview_handle = GetDlgItem(top_gui_handle, MACRO_LISTBOX);

    LVFINDINFOW find_info = { 0 };
    find_info.flags = LVFI_PARAM;
    find_info.lParam = Macro::recording_macro_index;

    int LB_index = ListView_FindItem(listview_handle, -1, &find_info);
    ListView_SetItemText(listview_handle, LB_index, 1, hotkey_name);

	PostQuitMessage(NULL);

	return -1;
}

LRESULT listen_hotkeys_hook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode != HC_ACTION) { return CallNextHookEx(nullptr, nCode, wParam, lParam); }

	KBDLLHOOKSTRUCT* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    int size = Macro::active_macro_hotkeys.size();

    for (int i = 0; i < size; i++)
    {
        if (kb->scanCode == Macro::active_macro_hotkeys[i].ki.wScan &&
           (kb->flags & LLKHF_EXTENDED) == (Macro::active_macro_hotkeys[i].ki.dwFlags & KEYEVENTF_EXTENDEDKEY) &&
           (kb->flags & LLKHF_UP) == FALSE)
            
        {
            SetEvent(Macro::active_macro_event_handles[i]);
            return -1;
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}