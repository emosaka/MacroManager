#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <fstream>
#include "third_party/json.hpp" // https://github.com/nlohmann/json


class Macro
{
public:

	Macro();

	static BOOL active;

	static std::vector<std::unique_ptr<Macro>> macro_list;

	static INPUT stopcode;

	static int recording_macro_index;

	static HANDLE read_thread_handle;

	static int create_macro();

	static HANDLE read_stopcode();

	static DWORD read_stopcode_threadproc(LPVOID lpParameter);

	static void set_stopcode(WPARAM wParam, LPARAM lParam);

	static DWORD read_macro_threadproc(LPVOID lpParameter);

	static void stop_reading();

	static DWORD read_hotkey_threadproc(LPVOID lpParameter);

	static DWORD play_threadproc(LPVOID lpParameter);
	
	static HANDLE listen_hotkeys();

	static DWORD listen_hotkeys_threadproc(LPVOID lpParameter);

	static std::vector<int> active_macro_ML_indexes;

	static std::vector<INPUT> active_macro_hotkeys;

	static std::vector<HANDLE> active_macro_event_handles;

	static std::vector<HANDLE> active_macro_thread_handles;

	static BOOL load_saved_macros();

	static BOOL load_macro(std::wstring file_path);

	std::wstring macro_name;

	INPUT hotkey;

	int ML_index;

	std::vector<INPUT> inputs;

	std::vector<int> delays;

	BOOL is_finalized() const;

	void add_input(WPARAM wParam, LPARAM lParam);

	void set_hotkey(WPARAM wParam, LPARAM lParam);

	BOOL add_to_listview();

	HANDLE read_macro();

	HANDLE read_hotkey();

	BOOL play();

	std::vector<INPUT> non_finalized_inputs;

	BOOL save_macro();

	BOOL activate_macro();

private:

	static HHOOK read_hook_handle;


	BOOL finalized;

	DWORD first_timestamp = 0;



	void finalize_inputs(DWORD last_timestamp);







};

