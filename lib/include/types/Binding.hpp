#pragma once

#include "types/Overloads.hpp"
#include "types/SemanticTypes.hpp"
#include <DGM/dgm.hpp>
#include <nlohmann/json.hpp>
#include <tuple>
#include <variant>

using KmbBinding =
    std::variant<sf::Keyboard::Key, sf::Mouse::Button, std::monostate>;

namespace nlohmann
{
    template<>
    struct adl_serializer<KmbBinding>
    {
        static void to_json(json& j, const KmbBinding& bnd)
        {
            std::visit(
                overloads {
                    [&j](sf::Keyboard::Key key)
                    {
                        j = json {};
                        j["type"] = "key";
                        j["value"] = std::to_underlying(key);
                    },
                    [&j](sf::Mouse::Button btn)
                    {
                        j = json {};
                        j["type"] = "mouse";
                        j["value"] = std::to_underlying(btn);
                    },
                    [&j](std::monostate)
                    {
                        j = json {};
                        j["type"] = "none";
                    },
                },
                bnd);
        }

        static void from_json(const json& j, KmbBinding& bnd)
        {
            if (!j.contains("type")) return;

            const std::string type = j["type"].get<std::string>();
            if (type == "key")
            {
                using T = sf::Keyboard::Key;
                bnd =
                    static_cast<T>(j["value"].get<std::underlying_type_t<T>>());
            }
            else if (type == "mouse")
            {
                using T = sf::Mouse::Button;
                bnd =
                    static_cast<T>(j["value"].get<std::underlying_type_t<T>>());
            }
            else if (type == "none")
            {
                bnd = std::monostate {};
            }
            else
            {
                assert(!"Deserialization not implemented for given type");
            }
        }
    };
} // namespace nlohmann

using GamepadAxis = std::pair<sf::Joystick::Axis, dgm::AxisHalf>;

using GamepadBinding = std::variant<GamepadButton, GamepadAxis, std::monostate>;

namespace nlohmann
{
    template<>
    struct adl_serializer<GamepadBinding>
    {
        static void to_json(json& j, const GamepadBinding& bnd)
        {
            std::visit(
                overloads {
                    [&j](GamepadButton btn)
                    {
                        j = json {};
                        j["type"] = "btn";
                        j["value"] = btn.get();
                    },
                    [&j](const GamepadAxis& pair)
                    {
                        j = json {};
                        j["type"] = "axis";
                        j["value"] = pair;
                    },
                    [&j](std::monostate)
                    {
                        j = json {};
                        j["type"] = "none";
                    },
                },
                bnd);
        }

        static void from_json(const json& j, GamepadBinding& bnd)
        {
            if (!j.contains("type")) return;

            const std::string type = j["type"].get<std::string>();
            if (type == "btn")
            {
                auto value = j["value"].get<unsigned>();
                bnd = GamepadButton(value);
            }
            else if (type == "axis")
            {
                const auto axis = j["value"].get<GamepadAxis>();
                bnd = axis;
            }
            else if (type == "none")
            {
                bnd = std::monostate {};
            }
            else
            {
                assert(!"Deserialization not implemented for given type");
            }
        }
    };
} // namespace nlohmann

using Binding = std::tuple<KmbBinding, GamepadBinding>;

static_assert(std::is_default_constructible_v<KmbBinding>);
static_assert(std::is_default_constructible_v<GamepadBinding>);
static_assert(std::is_default_constructible_v<Binding>);
