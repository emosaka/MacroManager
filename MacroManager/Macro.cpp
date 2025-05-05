#include "Macro.h"
#include "gui.h"
#include "hooks.h"
#include "helpers.h"
#include "WindowProcs.h"

BOOL Macro::active = FALSE;
INPUT Macro:: stopcode = {0};
HHOOK Macro:: read_hook_handle = nullptr;
std::vector<std::unique_ptr<Macro>> Macro:: macro_list = {};
int Macro::recording_macro_index = -1;
HANDLE Macro::read_thread_handle;

std::vector<int>    Macro::active_macro_ML_indexes = {};
std::vector<INPUT>  Macro::active_macro_hotkeys = {};
std::vector<HANDLE> Macro::active_macro_event_handles = {};
std::vector<HANDLE> Macro::active_macro_thread_handles = {};

Macro::Macro()
{
	this->finalized = FALSE;
	this->macro_name = L"unnamed macro";
	this->non_finalized_inputs;
	this->inputs;
	this->delays;
	this->ML_index = -1;
	this->hotkey = { 0 };

}


int Macro::create_macro()
{
	Macro::macro_list.emplace_back( std::make_unique<Macro>() );

	int ML_index = Macro::macro_list.size() - 1;  //index of new macro in macro list
	Macro::macro_list[ML_index]->ML_index = ML_index;  //macro object knows its index

	return ML_index;
}


HANDLE Macro::read_stopcode()
{
	HANDLE thread_handle = NULL;
	if (!( Macro::read_hook_handle || Macro::read_thread_handle))
	{
		thread_handle = CreateThread(NULL, NULL,&Macro::read_stopcode_threadproc,nullptr,NULL,NULL);
		Macro::read_thread_handle = thread_handle;
	}
	return thread_handle;
}

DWORD Macro::read_stopcode_threadproc(LPVOID lpParameter)
{
	HHOOK read_stopcode_hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, &read_stopcode_hook, 0, 0);

	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL)) {

		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	CloseHandle(Macro::read_thread_handle);
	Macro::read_hook_handle = NULL;
	Macro::read_thread_handle = NULL;
	Macro::recording_macro_index = -1;

	return TRUE;
}

BOOL Macro::is_finalized() const
{
	return this->finalized;
}

void Macro::add_input(WPARAM wParam,LPARAM lParam)
{
	INPUT input = {0};
	KBDLLHOOKSTRUCT* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);


	input.type = INPUT_KEYBOARD;
	input.ki.wVk = kb->vkCode;
	input.ki.wScan = kb->scanCode;
	input.ki.time = kb->time;
	//keyboard_event.dwExtraInfo = kb ->dwExtraInfo; //look at docs later
	input.ki.dwFlags = 0;

	if (kb->flags & LLKHF_EXTENDED) {input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;}
	if (kb->flags & LLKHF_UP) {input.ki.dwFlags |= KEYEVENTF_KEYUP;}

	this->non_finalized_inputs.push_back(input);


}

HANDLE Macro::read_macro()
{
	if (
		Macro::read_hook_handle ||
		Macro::read_thread_handle ||
		Macro::stopcode.ki.wScan == NULL ||
		Macro::recording_macro_index != -1
		)
	{return NULL;}

	SetWindowTextW(overlay_handle, L"Reading...");
	ShowWindow(overlay_handle, SW_SHOW);
	UpdateWindow(overlay_handle);


	HANDLE thread_handle = CreateThread(NULL, NULL, &Macro::read_macro_threadproc, nullptr, CREATE_SUSPENDED, NULL);
	if (!thread_handle) { return NULL; }

	Macro::recording_macro_index = this->ML_index;
	Macro::read_thread_handle = thread_handle;

	this->first_timestamp = GetTickCount();
	ResumeThread(thread_handle);

	return thread_handle;
}

DWORD Macro::read_macro_threadproc(LPVOID lpParameter)
{
	Macro::read_hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, &read_macro_hook, 0, 0);

	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL)) {

		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	CloseHandle(Macro::read_thread_handle);
	UnhookWindowsHookEx(Macro::read_hook_handle);
	Macro::read_hook_handle = NULL;
	Macro::read_thread_handle = NULL;
	Macro::recording_macro_index = -1;

	return TRUE;
}

void Macro::stop_reading()
{
	Macro::macro_list[Macro::recording_macro_index]->finalize_inputs(GetTickCount());
}

void Macro::set_stopcode(WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	INPUT input = { 0 };

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = kb->vkCode;
	input.ki.wScan = kb->scanCode;
	input.ki.time = kb->time;
	//keyboard_event.dwExtraInfo = kb ->dwExtraInfo; //look at docs later
	input.ki.dwFlags = 0;

	if (kb->flags & LLKHF_EXTENDED) { input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY; }
	if (kb->flags & LLKHF_UP) { input.ki.dwFlags |= KEYEVENTF_KEYUP; }


	Macro::stopcode = input;

}

void Macro::finalize_inputs(DWORD last_timestamp)
{
	DWORD previous_timestamp = this->first_timestamp;
	int size = this->non_finalized_inputs.size();
	int i = 0;
	for (i = 0; i < size; i++)
	{
		this->inputs.push_back(this->non_finalized_inputs[i]);
		this->inputs[i].ki.time = 0;

		this->delays.push_back( this->non_finalized_inputs[i].ki.time - previous_timestamp );

		previous_timestamp = non_finalized_inputs[i].ki.time;
	}
	this->delays.push_back(last_timestamp - previous_timestamp);

	this->non_finalized_inputs.clear();
	this->finalized = TRUE;
	Macro::recording_macro_index = -1;
	Macro::read_hook_handle = NULL;

}

HANDLE Macro::read_hotkey()
{
	if (
		Macro::read_hook_handle ||
		Macro::read_thread_handle
		)
	{return NULL;}
	
		HANDLE thread_handle = CreateThread(NULL, NULL, &Macro::read_hotkey_threadproc, nullptr, CREATE_SUSPENDED, NULL);
		if (!thread_handle) { return NULL; }

		Macro::recording_macro_index = this->ML_index;
		Macro::read_thread_handle = thread_handle;

		ResumeThread(thread_handle);

	return thread_handle;
}

DWORD Macro::read_hotkey_threadproc(LPVOID lpParameter)
{
	HHOOK read_stopcode_hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, &read_hotkey_hook, 0, 0);

	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL)) {

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	CloseHandle(Macro::read_thread_handle);
	UnhookWindowsHookEx(Macro::read_hook_handle);
	Macro::read_hook_handle = NULL;
	Macro::read_thread_handle = NULL;
	Macro::recording_macro_index = -1;

	return TRUE;
}

DWORD Macro::play_threadproc(LPVOID lpParameter)
{
	int active_macro_index = (int)lpParameter;

	int ML_index = Macro::active_macro_ML_indexes[active_macro_index];
	HANDLE this_threads_event_handle = Macro::active_macro_event_handles[active_macro_index];
	HANDLE this_threads_handle = Macro::active_macro_thread_handles[active_macro_index]; // maybe make into struct?

	while (Macro::active) {
		WaitForSingleObjectEx(this_threads_event_handle, INFINITE, TRUE); //unsure of last parameter

		if (!Macro::active) { break; }

		Macro::macro_list[ML_index]->play();
	}
	return TRUE; //add one listener thread
}

HANDLE Macro::listen_hotkeys()
{
	HANDLE thread_handle = CreateThread(NULL, NULL, &Macro::listen_hotkeys_threadproc, nullptr, NULL, NULL);
	if (!thread_handle) { return NULL; }

	Macro::read_thread_handle = thread_handle;
	return thread_handle;
}

DWORD Macro::listen_hotkeys_threadproc(LPVOID lpParameter)
{
	Macro::read_hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, &listen_hotkeys_hook, 0, 0);

	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL)) {

		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	CloseHandle(Macro::read_thread_handle);
	UnhookWindowsHookEx(Macro::read_hook_handle);
	Macro::read_hook_handle = NULL;
	Macro::read_thread_handle = NULL;
	Macro::recording_macro_index = -1;


	return TRUE;
}

void Macro::set_hotkey(WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	INPUT input = { 0 };

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = kb->vkCode;
	input.ki.wScan = kb->scanCode;
	input.ki.time = kb->time;
	//keyboard_event.dwExtraInfo = kb ->dwExtraInfo; //look at docs later
	input.ki.dwFlags = 0;

	if (kb->flags & LLKHF_EXTENDED) { input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY; }
	if (kb->flags & LLKHF_UP) { input.ki.dwFlags |= KEYEVENTF_KEYUP; }

	this->hotkey = input;
}

BOOL Macro::play()
{
	if (this->is_finalized() == TRUE) {

		int size = this->inputs.size();
		int i = 0;
		for (i = 0; i < size; i++)
		{
			Sleep(this->delays[i]);

			SendInput(1, &this->inputs[i], sizeof(INPUT));
		}

		Sleep(this->delays[size]);

		return TRUE;

	}else{return FALSE;}
}

BOOL Macro::activate_macro()
{
	if (!Macro::active) { return FALSE; }

	Macro::active_macro_ML_indexes.push_back(this->ML_index);

	std::wstring event_name = L"macro event " + std::to_wstring(this->ML_index);
	Macro::active_macro_event_handles.push_back
	(
		CreateEventW(NULL, FALSE, FALSE, event_name.c_str())
	);

	Macro::active_macro_hotkeys.push_back(this->hotkey);

	Macro::active_macro_thread_handles.push_back
	(
		CreateThread(NULL, NULL, Macro::play_threadproc, (LPVOID)this->ML_index, NULL, NULL)
	);

}

BOOL Macro::save_macro()
{
	if (this->is_finalized() == FALSE) { return FALSE; }

	std::wstring directory = get_directory();
	CreateDirectoryW((directory + L"\\saved_macros").c_str(), NULL);

	std::wstring macro_path = directory + L"\\saved_macros\\" + this->macro_name + L"_macro.json";
	std::ofstream macro_file(macro_path);

	if (!macro_file.is_open()) { return FALSE; }

	nlohmann::ordered_json j;

	j["macro_name"] = this->macro_name;
	j["hotkey"] = input_to_ordered_json(this->hotkey);
	j["inputs"] = nlohmann::ordered_json::array();

	for (const INPUT& input : this->inputs)
	{
		j["inputs"].push_back(input_to_ordered_json(input));
	}

	j["delays"] = this->delays;
	j["finalized"] = TRUE;


	macro_file << j.dump(4);
	macro_file.close();

	return TRUE;
}

BOOL Macro::load_saved_macros()
{
	std::wstring directory = get_directory();

	WIN32_FIND_DATAW search_data = {0};


	HANDLE search_handle = FindFirstFileW((directory + L"\\saved_macros\\*_macro.json").c_str(), &search_data);

	if (search_handle == INVALID_HANDLE_VALUE) { return TRUE; } //no saved macros to load

	std::wstring file_path;
	do 
	{
		file_path = std::wstring(directory + L"\\saved_macros\\" + std::wstring(search_data.cFileName));
		Macro::load_macro(file_path);

	} while (FindNextFileW(search_handle,&search_data));

	return TRUE;
}

BOOL Macro::load_macro(std::wstring file_path)
{


	std::ifstream macro_file(file_path);

	if (!macro_file.is_open()) { return FALSE; }

	nlohmann::ordered_json j;

	macro_file >> j;

	int ML_index = Macro::create_macro();

	auto& this_macro = Macro::macro_list[ML_index];

    this_macro->macro_name = j["macro_name"].get<std::wstring>();

	this_macro->hotkey = ordered_json_to_input(j["hotkey"]);

	j["inputs"].size();

	for (nlohmann::ordered_json json_input : j["inputs"])
	{
		this_macro->inputs.emplace_back(ordered_json_to_input(json_input));
	}

    this_macro->delays = j["delays"].get<std::vector<int>>();

	this_macro->finalized = j["finalized"];

	this_macro->add_to_listview();

	return TRUE;
}

BOOL Macro::add_to_listview()
{
	HWND top_gui_handle = GetDlgItem(main_window_handle, TOP_GUI);
	HWND macro_listview_handle = GetDlgItem(top_gui_handle, MACRO_LISTBOX);

	LVITEMW macro_item = { 0 };
	macro_item.mask = LVIF_TEXT | LVIF_PARAM;
	macro_item.pszText = const_cast<LPWSTR>(this->macro_name.c_str());
	macro_item.lParam = this->ML_index;

	int LV_index = ListView_InsertItem(macro_listview_handle, &macro_item);

	std::wstring hotkey_name = get_input_name(&this->hotkey);

	ListView_SetItemText(macro_listview_handle, LV_index, 1, &hotkey_name[0]);

	return TRUE;
}

