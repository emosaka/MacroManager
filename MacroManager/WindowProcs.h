#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#define TOP_GUI            1
#define REC_BUTTON         2
#define PLAY_BUTTON        3
#define NEW_MACRO_BUTTON   4
#define ACTIVATE_BUTTON    5
#define SAVE_BUTTON        6
#define MACRO_LISTBOX      7
#define STOPCODE_BUTTON    8
#define BOT_GUI            9

extern HWND main_window_handle;
extern HWND overlay_handle;


BOOL CALLBACK resize_enum_child_proc(HWND hWnd, LPARAM lParam);


LRESULT main_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


LRESULT top_gui_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


LRESULT top_gui_control_command(HWND hWnd, WPARAM wParam, LPARAM lParam);


LRESULT top_gui_control_notification(HWND top_gui_handle, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK overlay_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



