#pragma once

// localizations can be found under private_include/localizations

enum class [[nodiscard]] StringId
{
    PlayButton,
    Options,
    PauseTitle,
    ExitButton,
    Back,

    // Options
    Apply,
    VideoOptionsTab,
    AudioOptionsTab,
    InputOptionsTab,
    BindingsOptionsTab,
    EnableFullscreen,
    SetResolution,
    SetUiScale,
    SetTheme,
    SoundVolume,
    MusicVolume,
    GamepadDeadzone,
    CursorSpeed,
    ShowTouchControls,
    ReleaseAllInputs,
    PressAKeyOrEscape,
    BindingHeadingAction,
    BindingHeadingKMB,
    BindingsHeadingGamepad,

    // InputKind
    InputKind_BackButton,
    InputKind_ConfirmButton,
    InputKind_CursorUp,
    InputKind_CursorDown,
    InputKind_CursorLeft,
    InputKind_CursorRight,
    InputKind_MenuCycleLeft,
    InputKind_MenuCycleRight,
    InputKind_Left,
    InputKind_Right,
    InputKind_Jump,

    // Pause
    Resume,
    BackToMenu,

    MaxId, // Make sure this one is always the last!
};
