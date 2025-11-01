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
    FirstAid,

    // Items
    Door,
    Key,

    // Enemies
    Zombie,
    Licker,

    Max,
};
