#include "helpers.h"






std::wstring get_input_name(const INPUT* input)
{
	std::wstring input_name(32, L'\0');

	LONG key_flags = input->ki.wScan << 16;
	if (input->ki.dwFlags & KEYEVENTF_EXTENDEDKEY) { key_flags = key_flags | (1 << 24); }

	if (!GetKeyNameTextW(key_flags, &input_name[0], 32))
	{
		input_name = L"unknown key";
		if (input->ki.wScan == NULL) { input_name = L"None"; }
	}

	return input_name;
}




nlohmann::ordered_json input_to_ordered_json(const INPUT& input)
{
	nlohmann::ordered_json input_json;


	input_json["type"] = input.type;

	input_json["mi"]["dx"] = input.mi.dx;
	input_json["mi"]["dy"] = input.mi.dy;
	input_json["mi"]["mouseData"] = input.mi.mouseData;
	input_json["mi"]["dwFlags"] = input.mi.dwFlags;
	input_json["mi"]["time"] = input.mi.time;
	input_json["mi"]["dwExtraInfo"] = input.mi.dwExtraInfo;

	input_json["ki"]["wVk"] = input.ki.wVk;
	input_json["ki"]["wScan"] = input.ki.wScan;
	input_json["ki"]["dwFlags"] = input.ki.dwFlags;
	input_json["ki"]["time"] = input.ki.time;
	input_json["ki"]["dwExtraInfo"] = input.ki.dwExtraInfo;

	input_json["hi"]["uMsg"] = input.hi.uMsg;
	input_json["hi"]["wParamL"] = input.hi.wParamL;
	input_json["hi"]["wParamH"] = input.hi.wParamH;


	return input_json;
}


INPUT ordered_json_to_input(const nlohmann::ordered_json& input_json)
{

	INPUT input = { 0 };

	input.type = input_json["type"];

	input.mi.dx = input_json["mi"]["dx"];
	input.mi.dy = input_json["mi"]["dy"];
	input.mi.mouseData = input_json["mi"]["mouseData"];
	input.mi.dwFlags = input_json["mi"]["dwFlags"];
	input.mi.time = input_json["mi"]["time"];
	input.mi.dwExtraInfo = input_json["mi"]["dwExtraInfo"];

	input.ki.wVk = input_json["ki"]["wVk"];
	input.ki.wScan = input_json["ki"]["wScan"];
	input.ki.dwFlags = input_json["ki"]["dwFlags"];
	input.ki.time = input_json["ki"]["time"];
	input.ki.dwExtraInfo = input_json["ki"]["dwExtraInfo"];

	input.hi.uMsg = input_json["hi"]["uMsg"];
	input.hi.wParamL = input_json["hi"]["wParamL"];
	input.hi.wParamH = input_json["hi"]["wParamH"];

	return input;
}


std::wstring get_directory() {

	wchar_t directory[MAX_PATH];
	GetModuleFileNameW(NULL, directory, MAX_PATH);

	std::wstring::size_type pos = std::wstring(directory).find_last_of(L"\\/");
	return std::wstring(directory).substr(0, pos);
}


BOOL set_overlay_text()
{

	//ShowWindow(hwnd_overlay, SW_SHOW);
	//UpdateWindow(hwnd_overlay);



}
