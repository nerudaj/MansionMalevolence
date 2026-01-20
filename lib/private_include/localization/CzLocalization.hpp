#pragma once

#include "strings/StringId.hpp"
#include "types/StringTypes.hpp"

using enum StringId;

const std::map<StringId, StringViewType> CZ_LOCALIZATION = {
    { PlayButton, "hrat" },
    { Options, "nastaveni" },
    { PauseTitle, "hra pozastavena" },
    { HowToPlay, "jak hrat" },
    { CardGallery, "galerie karet" },
    { ExitButton, "ukoncit" },
    { Back, "zpet" },

    // Options
    { Apply, "pouzit" },
    { VideoOptionsTab, "video" },
    { AudioOptionsTab, "audio" },
    { InputOptionsTab, "ovladani" },
    { BindingsOptionsTab, "prirazeni" },
    { EnableFullscreen, "cela obrazovka" },
    { SetResolution, "rozliseni" },
    { SetUiScale, "velikost ui" },
    { SetTheme, "motiv" },
    { SoundVolume, "hlasitost efektu" },
    { MusicVolume, "hlasitost hudby" },
    { GamepadDeadzone, "mrtva zona gamepadu" },
    { CursorSpeed, "rychlost kurzoru" },
    { ShowTouchControls, "zobrazit dotykove ovladani" },
    { ReleaseAllInputs, "uvolnit vsechny vstupy" },

#ifdef ANDROID
    { PressAKeyOrEscape,
      "stiskni klavesu\nnebo klikni mimo\nzpravu pro zruseni" },
#else
    { PressAKeyOrEscape, "stiskni klavesu\nnebo escape pro zruseni" },
#endif

    { BindingHeadingAction, "akce" },
    { BindingHeadingKMB, "klavesnice/mys" },
    { BindingsHeadingGamepad, "gamepad" },
    { MenuLanguage, "jazyk menu" },

    // Level select
    { LevelSelect, "vyber scenare" },
    { LevelTutorial1, "tutorial" },
    { LevelEasy, "lehky scenar" },
    { LevelNormal, "normalni scenar" },
    { LevelHard, "tezky scenar" },
    { LevelNightmare, "nocni mura" },
    { LevelNightmare2, "jedna mistnost" },
    { LevelCleared, "splneno" },

    // InputKind
    { InputKind_BackButton, "zpet" },
    { InputKind_ConfirmButton, "potvrdit" },
    { InputKind_CursorUp, "kurzor nahoru" },
    { InputKind_CursorDown, "kurzor dolu" },
    { InputKind_CursorLeft, "kurzor vlevo" },
    { InputKind_CursorRight, "kurzor vpravo" },
    { InputKind_MenuCycleLeft, "listovat vlevo" },
    { InputKind_MenuCycleRight, "listovat vpravo" },
    { InputKind_Left, "beh vlevo" },
    { InputKind_Right, "beh vpravo" },
    { InputKind_Jump, "skok" },

    // Pause
    { Resume, "pokracovat" },
    { BackToMenu, "zpet do hlavniho menu" },

    // EndGameScreen
    { YouSurvived, "prezil jsi" },
    { YouDied, "zemrel jsi" },
    { Retry, "znovu" },
    { TurnsTaken, "provedene tahy" },
    { ShotsFired, "vystrelene rany" },
    { EnemiesRouted, "porazeni nepratele" },
    { DamageTaken, "utrzena zraneni" },
    { KilledByInfection, "infekce te premenila v zombie" },
    { KilledByZombie, "zombie ti sezral mozek" },
    { KilledByCerberus, "cerberus te ulovil jako kralika" },
    { KilledByCrisonHead, "crimson head te dostal" },
    { KilledByLicker, "licker te roztrhal na kusy" },
    { KilledByTyrant, "tyrant te rozpulil" },
    { BackToLevelSelect, "zpet na scenare" },

    // How to play
    { Prev, "predchozi" },
    { Next, "dalsi" },

    { SkipText,
      "pretahni hlavni kartu na odhazovaci hromadku pro preskoceni. pozor - "
      "nepratele mohou utocit kdyz jsou preskoceni!" },

    { TakeText,
      "pretahni hlavni kartu do inventare pro sebrani. nektere karty se mohou "
      "automaticky spojit pokud neni misto." },

    { AttackText,
      "pretahni kartu z inventare na hlavni kartu pro utok, nebo pouzij "
      "klicovy predmet." },

    { CombineText,
      "pretahni kartu z inventare na jinou kartu pro nabiti zbrane nebo vyrobu "
      "predmetu." },

    { HealText,
      "pretahni lecivy predmet z inventare na ukazatel zdravi pro leceni." },

    { DiscardText,
      "muzes odhodit kartu z inventare aby sis udelal misto na neco "
      "uzitecnejsiho." },

    { VaccineText,
      "najdi a pouzij vakcinu na sebe aby ses vylecil z infekce!" },

    { DiamondsText,
      "diamanty na zbranich znaci poskozeni, na lecivych kartach mnozstvi "
      "obnoveneho zdravi a u monster zdravi i poskozeni. poskozeni monstra "
      "snizuje jeho utocnou silu!" },

    { AmmoText, "zbrane maji limitovanou munici, strilej s rozmyslem!" },
};
