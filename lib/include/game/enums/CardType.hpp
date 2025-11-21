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
    SunCrest,
    CrestDoorEmpty,
    CrestDoorWithOneCrest,
    ShieldKey,
    ShieldDoor,
    DiamondKey,
    DiamondDoor,

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
