#pragma once

#include "strings/StringId.hpp"
#include "types/StringTypes.hpp"

using enum StringId;

const std::map<StringId, StringViewType> EN_LOCALIZATION = {
    { PlayButton, "play" },
    { Options, "options" },
    { PauseTitle, "game paused" },
    { HowToPlay, "how to play" },
    { CardGallery, "card gallery" },
    { ExitButton, "exit" },
    { Back, "back" },

    // Options
    { Apply, "apply" },
    { VideoOptionsTab, "video" },
    { AudioOptionsTab, "audio" },
    { InputOptionsTab, "input" },
    { BindingsOptionsTab, "bindings" },
    { EnableFullscreen, "fullscreen" },
    { SetResolution, "resolution" },
    { SetUiScale, "set ui scale" },
    { SetTheme, "set theme" },
    { SoundVolume, "sound volume" },
    { MusicVolume, "music volume" },
    { GamepadDeadzone, "gamepad deadzone" },
    { CursorSpeed, "cursor speed" },
    { ShowTouchControls, "show touch controls" },
    { ReleaseAllInputs, "release all inputs" },
#ifdef ANDROID
    { PressAKeyOrEscape,
      "press a key\nor click outside of\nthis message to cancel" },
#else
    { PressAKeyOrEscape, "press a key\nor Escape to cancel" },
#endif
    { BindingHeadingAction, "action" },
    { BindingHeadingKMB, "keyboard/Mouse" },
    { BindingsHeadingGamepad, "gamepad" },

    // Level select
    { LevelSelect, "level select" },
    { LevelTutorial1, "tutorial" },
    { LevelEasy, "easy scenario" },
    { LevelNormal, "normal scenario" },
    { LevelHard, "hard scenario" },
    { LevelNightmare, "nightmare scenario" },

    // InputKind
    { InputKind_BackButton, "back" },
    { InputKind_ConfirmButton, "confirm" },
    { InputKind_CursorUp, "cursor up" },
    { InputKind_CursorDown, "cursor down" },
    { InputKind_CursorLeft, "cursor left" },
    { InputKind_CursorRight, "cursor right" },
    { InputKind_MenuCycleLeft, "menu cycle left" },
    { InputKind_MenuCycleRight, "menu cycle right" },
    { InputKind_Left, "run left" },
    { InputKind_Right, "run right" },
    { InputKind_Jump, "jump" },

    // Pause
    { Resume, "resume game" },
    { BackToMenu, "back to main menu" },

    // EndGameScreen
    { YouSurvived, "you survived" },
    { YouDied, "you died" },
    { Retry, "retry" },
    { TurnsTaken, "turns taken" },
    { ShotsFired, "shots fired" },
    { EnemiesRouted, "enemies routed" },
    { DamageTaken, "damage taken" },
    { KilledByInfection, "infection had turned you into a zombie" },
    { KilledByZombie, "zombie ate your brain" },
    { KilledByCerberus, "cerberus hunted you like a rabbit" },
    { KilledByCrisonHead, "crimson head chased you down" },
    { KilledByLicker, "licker has torn you into pieces" },
    { KilledByTyrant, "tyrant has slashed you in half" },

    // How to play
    { Prev, "prev" },
    { Next, "next" },
    { SkipText, "swipe right to skip card you can't deal with right now." },
    { TakeText,
      "swipe down to take card into your inventory. some cards automatically "
      "combine if there is no space left." },
    { InventoryText,
      "you can combine cards in your inventory, use them on the main card, "
      "heal yourself, or put the inventory card back into deck." },
    { VaccineText,
      "find and use the vaccine on yourself to heal the infection!" },
};
