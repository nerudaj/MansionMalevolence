#pragma once

enum class [[nodiscard]] CardType
{
    Empty = 0,

    // Weapons
    Pistol,
    Shotgun,
    RocketLauncher,
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
    RedJewel,
    RedJewelBox,
    YellowJewel,
    YellowJewelBox,
    Vaccine,

    // Enemies
    Zombie,
    CrimsonHead,
    Licker,
    Cerberus,
    Hunter,
    Tyrant,

    Crate,

    Max,
};
