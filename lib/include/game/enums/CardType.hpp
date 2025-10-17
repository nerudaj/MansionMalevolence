#pragma once

enum class [[nodiscard]] CardType
{
    // Weapons
    Pistol,
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
};
