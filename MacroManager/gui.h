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

struct gui_handles_struct
{
	HWND top_gui;
	HWND play_button;
	HWND rec_button;
	HWND macro_list;
	HWND bot_gui;
};

gui_handles_struct create_gui(HWND window_handle);

void create_window_classes();

gui_handles_struct create_bot_gui(HWND window_handle, RECT wind_dims);

gui_handles_struct create_top_gui(HWND window_handle, RECT wind_dims);
