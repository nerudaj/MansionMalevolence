#pragma once

#include <variant>

struct [[nodiscard]] DummyGameEvent final
{
};

using GameEvent = std::variant<DummyGameEvent>;
