#pragma once

#include <optional>
#include <string>
#include <typeinfo>
#include <variant>

struct [[nodiscard]] PopIfNotMenu final
{
};

using AppMessage = std::variant<PopIfNotMenu>;

template<class T>
concept IsAppMessage = std::constructible_from<AppMessage, T>;

class Messaging
{
public:
    template<IsAppMessage T>
    static const char* serialize()
    {
        return typeid(T).name();
    }

    static std::optional<AppMessage> deserialize(const std::string& str);
};
