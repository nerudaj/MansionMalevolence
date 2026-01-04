#pragma once

struct [[nodiscard]] GameStats final
{
    int turnsTaken = 0;
    int heartsRestored = 0;
    int enemiesKilled = 0;
    int shotsFired = 0;
    int damageTaken = 0;
};
