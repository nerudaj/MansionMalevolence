#pragma once

enum class [[nodiscard]] CardType
{
    // Weapons
    Pistol = 0,
    Shotgun,
    Ammo,

    // Healing
    GreenHerb,
    RedHerb,
    MixedHerbs,
    FirstAid,

    // Items
    Door,
    Key,

    // Enemies
    Zombie,
    CrimsonHead,
    Licker,

    BookCase,

    Max,
};
