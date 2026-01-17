#pragma once

// localizations can be found under private_include/localizations

enum class [[nodiscard]] StringId
{
    PlayButton,
    Options,
    PauseTitle,
    HowToPlay,
    CardGallery,
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

    // Level select
    LevelSelect,
    LevelTutorial1,
    LevelEasy,
    LevelNormal,
    LevelHard,
    LevelNightmare,
    LevelNightmare2,
    LevelCleared,

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

    // EndGameScreen
    YouSurvived,
    YouDied,
    Retry,
    TurnsTaken,
    ShotsFired,
    EnemiesRouted,
    DamageTaken,
    KilledByInfection,
    KilledByZombie,
    KilledByCerberus,
    KilledByCrisonHead,
    KilledByLicker,
    KilledByTyrant,
    BackToLevelSelect,

    // How to play
    Prev,
    Next,
    SkipText,
    TakeText,
    AttackText,
    CombineText,
    HealText,
    DiscardText,
    VaccineText,
    DiamondsText,

    MaxId, // Make sure this one is always the last!
};
