#pragma once

enum class [[nodiscard]] CardType
{
    Empty = 0,

    // Weapons
    Pistol,
    Shotgun,
    Ammo,

    // Healing
    GreenHerb,
    RedHerb,
    MixedHerbs,
    FirstAid,

    // Items
    MoonCrestLeft,
    MoonCrestRight,
    MoonCrest,
    MoonCrestDoor,
    ShieldKey,
    ShieldDoor,

    // Enemies
    Zombie,
    CrimsonHead,
    Licker,
    Cerberus,
    Hunter,
    Tyrant,

    Crate,
    BookCase,

    Max,
};
