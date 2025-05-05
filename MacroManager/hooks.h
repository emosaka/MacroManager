#pragma once

#include <Windows.h>


//LowLevelKeyboardProc in docs
LRESULT CALLBACK read_macro_hook(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT read_stopcode_hook(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT read_hotkey_hook(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT listen_hotkeys_hook(int nCode, WPARAM wParam, LPARAM lParam);