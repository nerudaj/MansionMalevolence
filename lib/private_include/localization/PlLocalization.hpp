#pragma once

#include "strings/StringId.hpp"
#include "types/StringTypes.hpp"

using enum StringId;

const std::map<StringId, StringViewType> PL_LOCALIZATION = {
    { PlayButton, "graj" },
    { Options, "opcje" },
    { PauseTitle, "gra wstrzymana" },
    { HowToPlay, "jak grac" },
    { CardGallery, "galeria kart" },
    { ExitButton, "wyjscie" },
    { Back, "powrot" },

    // Options
    { Apply, "zastosuj" },
    { VideoOptionsTab, "wideo" },
    { AudioOptionsTab, "audio" },
    { InputOptionsTab, "sterowanie" },
    { BindingsOptionsTab, "przypisania" },
    { EnableFullscreen, "pelny ekran" },
    { SetResolution, "rozdzielczosc" },
    { SetUiScale, "skaluj ui" },
    { SetTheme, "motyw" },
    { SoundVolume, "glosnosc efektow" },
    { MusicVolume, "glosnosc muzyki" },
    { GamepadDeadzone, "martwa strefa gamepada" },
    { CursorSpeed, "predkosc kursora" },
    { ShowTouchControls, "pokaz sterowanie dotykowe" },
    { ReleaseAllInputs, "zwolnij wszystkie wejscia" },

#ifdef ANDROID
    { PressAKeyOrEscape,
      "nacisnij klawisz\nlub kliknij poza\nwiadomoscia aby anulowac" },
#else
    { PressAKeyOrEscape, "nacisnij klawisz\nlub escape aby anulowac" },
#endif

    { BindingHeadingAction, "akcja" },
    { BindingHeadingKMB, "klawiatura/mysz" },
    { BindingsHeadingGamepad, "gamepad" },
    { MenuLanguage, "jezyk menu" },

    // Level select
    { LevelSelect, "wybierz scenariusz" },
    { LevelTutorial1, "samouczek" },
    { LevelEasy, "latwy scenariusz" },
    { LevelNormal, "normalny scenariusz" },
    { LevelHard, "trudny scenariusz" },
    { LevelNightmare, "koszmarny scenariusz" },
    { LevelNightmare2, "scenariusz jednopokojowy" },
    { LevelCleared, "ukonczono" },

    // InputKind
    { InputKind_BackButton, "powrot" },
    { InputKind_ConfirmButton, "zatwierdz" },
    { InputKind_CursorUp, "kursor w gore" },
    { InputKind_CursorDown, "kursor w dol" },
    { InputKind_CursorLeft, "kursor w lewo" },
    { InputKind_CursorRight, "kursor w prawo" },
    { InputKind_MenuCycleLeft, "przewin menu w lewo" },
    { InputKind_MenuCycleRight, "przewin menu w prawo" },
    { InputKind_Left, "bieg w lewo" },
    { InputKind_Right, "bieg w prawo" },
    { InputKind_Jump, "skok" },

    // Pause
    { Resume, "wznow gre" },
    { BackToMenu, "powrot do menu glownego" },

    // EndGameScreen
    { YouSurvived, "przetrwales" },
    { YouDied, "zginales" },
    { Retry, "sprobuj ponownie" },
    { TurnsTaken, "wykonane tury" },
    { ShotsFired, "oddane strzaly" },
    { EnemiesRouted, "pokonani wrogowie" },
    { DamageTaken, "otrzymane obrazenia" },
    { KilledByInfection, "infekcja zamienila cie w zombie" },
    { KilledByZombie, "zombie zjadl twoj mozg" },
    { KilledByCerberus, "cerberus upolowal cie jak krolika" },
    { KilledByCrisonHead, "crimson head dopadl cie" },
    { KilledByLicker, "licker rozszarpal cie na kawalki" },
    { KilledByTyrant, "tyrant przecial cie na pol" },
    { BackToLevelSelect, "powrot do scenariuszy" },

    // How to play
    { Prev, "poprz" },
    { Next, "nastepny" },

    { SkipText,
      "przeciagnij glowna karte na stos odrzutow aby ja pominac. uwaga - "
      "wrogowie moga cie zaatakowac gdy sa pomijani!" },

    { TakeText,
      "przeciagnij glowna karte do ekwipunku aby ja podniesc. niektore karty "
      "moga laczyc sie automatycznie gdy nie ma miejsca." },

    { AttackText,
      "przeciagnij karte z ekwipunku na glowna karte aby ja zaatakowac, albo "
      "uzyj przedmiotu kluczowego." },

    { CombineText,
      "przeciagnij karte z ekwipunku na inna karte aby przeladowac bron lub "
      "stworzyc nowe przedmioty." },

    { HealText,
      "przeciagnij przedmiot leczacy z ekwipunku na pasek zdrowia aby sie "
      "uleczyc." },

    { DiscardText,
      "mozesz odrzucic karte z ekwipunku aby zrobic miejsce na cos bardziej "
      "przydatnego. pamietaj ze niektore karty lacza sie automatycznie nawet "
      "gdy ekwipunek jest pelny!" },

    { VaccineText,
      "znajdz i uzyj szczepionki na sobie aby wyleczyc infekcje!" },

    { DiamondsText,
      "diamenty na broniach oznaczaja obrazenia, na kartach leczenia ilosc "
      "punktow zdrowia, a na potworach zdrowie i obrazenia. zadawanie obrazen "
      "potworowi zmniejsza obrazenia jakie moze zadac!" },

};
