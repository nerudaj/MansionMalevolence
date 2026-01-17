#pragma once

#include <nlohmann/json.hpp>
#include <set>
#include <string>

struct [[nodiscard]] SavePosition final
{
    std::set<std::string> clearedScenarioNames;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SavePosition, clearedScenarioNames);
