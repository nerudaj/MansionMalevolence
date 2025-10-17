#pragma once

#include "input/InputKind.hpp"
#include "strings/StringId.hpp"
#include "strings/StringProvider.hpp"

class [[nodiscard]] InputKindToStringMapper final
{
public:
    InputKindToStringMapper(const StringProvider& strings) : strings(strings) {}

    InputKindToStringMapper(InputKindToStringMapper&&) = delete;
    InputKindToStringMapper(const InputKindToStringMapper&) = delete;

public:
    const CharType* inputKindToString(InputKind kind)
    {
        switch (kind)
        {
        case InputKind::BackButton:
            return strings.getString(StringId::InputKind_BackButton);
        case InputKind::ConfirmButton:
            return strings.getString(StringId::InputKind_ConfirmButton);
        case InputKind::CursorUp:
            return strings.getString(StringId::InputKind_CursorUp);
        case InputKind::CursorDown:
            return strings.getString(StringId::InputKind_CursorDown);
        case InputKind::CursorLeft:
            return strings.getString(StringId::InputKind_CursorLeft);
        case InputKind::CursorRight:
            return strings.getString(StringId::InputKind_CursorRight);
        case InputKind::MenuCycleLeft:
            return strings.getString(StringId::InputKind_MenuCycleLeft);
        case InputKind::MenuCycleRight:
            return strings.getString(StringId::InputKind_MenuCycleRight);
        case InputKind::Left:
            return strings.getString(StringId::InputKind_Left);
        case InputKind::Right:
            return strings.getString(StringId::InputKind_Right);
        case InputKind::Jump:
            return strings.getString(StringId::InputKind_Jump);
        default:
            return "--error--";
        }
    }

private:
    const StringProvider& strings;
};