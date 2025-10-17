#pragma once

#include "strings/StringId.hpp"
#include <map>
#include <string>

using CharType = char;
using StringType = std::basic_string<CharType>;
using StringViewType = std::basic_string_view<CharType>;
using Localization = std::map<StringId, StringViewType>;
