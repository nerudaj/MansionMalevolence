#pragma once

enum class [[nodiscard]] CardType
{
    Empty = 0,

    // Weapons
    Pistol,
    SilencedPistol,
    Shotgun,
    Crossbow,
    RocketLauncher,
    Ammo,
    PistolParts,

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
    WeaponLockerKey,
    LockedWeaponLocker,
    UnlockedWeaponLocker,

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
