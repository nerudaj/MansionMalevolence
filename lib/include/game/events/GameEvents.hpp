#pragma once

#include <variant>

struct DummyGameEvent
{
    std::string soundName = "";
};

using GameEvent = std::variant<DummyGameEvent>;
