#pragma once

#include <DGM/classes/Compatibility.hpp>

#ifdef ANDROID
#include <fmt/core.h>
#include <range/v3/range.hpp>
#endif

#ifdef ANDROID
namespace uni = fmt;
namespace uniranges = ranges;
#else
#include <ranges>

namespace uni = std;
namespace uniranges = std::ranges;
#endif
