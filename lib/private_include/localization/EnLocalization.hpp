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
    { MenuLanguage, "menu language" },

    // Level select
    { LevelSelect, "select scenario" },
    { LevelTutorial1, "tutorial" },
    { LevelEasy, "easy scenario" },
    { LevelNormal, "normal scenario" },
    { LevelHard, "hard scenario" },
    { LevelNightmare, "nightmare scenario" },
    { LevelNightmare2, "one-room scenario" },
    { LevelCleared, "cleared" },

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
    { BackToLevelSelect, "back to scenarios" },

    // How to play
    { Prev, "prev" },
    { Next, "next" },
    { SkipText,
      "drag the main card over discard to skip it. beware - enemies might "
      "attack you "
      "when they are skipped!" },
    { TakeText,
      "drag the main card over your inventory to pick it up. certain cards "
      "might "
      "automatically combine if there is no space left." },
    { AttackText,
      "drag inventory card onto the main card to attack it, or use key item on "
      "it." },
    { CombineText,
      "drag inventory card onto another inventory card to reload weapons or "
      "craft new items." },
    { HealText,
      "drag healing item from the inventory onto your healthbar to heal." },
    { DiscardText,
      "you can discard a card from your inventory to make place for something "
      "more useful. Note that certain cards can combine upon taking even if "
      "inventory is "
      "full!" },
    { VaccineText,
      "find and use the vaccine on yourself to cure the infection!" },
    { DiamondsText,
      "diamonds on weapons indicate damage, amount of hitpoints healed on "
      "healing cards and both health and damage for monsters. damaging a "
      "monster will reduce the damage it can deal!" },
};
