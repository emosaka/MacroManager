#pragma once

#include <windows.h>
#include "Macro.h"

std::wstring get_input_name(const INPUT* input);

nlohmann::ordered_json input_to_ordered_json(const INPUT& input);

INPUT ordered_json_to_input(const nlohmann::ordered_json& j);

std::wstring get_directory();