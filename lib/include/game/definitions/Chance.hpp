#pragma once

#include <cstdlib>

struct [[nodiscard]] Chance final
{
    int evasiveChanceSkew = 0;
    int crimsonHeadChanceSkew = 0;
    int dodgeChanceSkew = 0;
    int blindDodgeChanceSkew = 0;
    int vigilantDodgeChanceSkew = 0;
    int critChanceSkew = 0;

    bool rollForEvasion()
    {
        return rollForChanceSkewed(2, 1, evasiveChanceSkew);
    }

    bool rollForCrimsonHead()
    {
        return rollForChanceSkewed(3, 1, crimsonHeadChanceSkew);
    }

    bool rollForDodge()
    {
        return rollForChanceSkewed(2, 1, dodgeChanceSkew);
    }

    bool rollForBlindDodge()
    {
        return rollForChanceSkewed(3, 2, blindDodgeChanceSkew);
    }

    bool rollForVigilantDodge()
    {
        return rollForChanceSkewed(3, 1, vigilantDodgeChanceSkew);
    }

    bool rollForCrit()
    {
        return rollForChanceSkewed(3, 1, critChanceSkew);
    }

    static int rollForChanceSkewed(int diceSize, int failThreshold, int& skew)
    {
        const bool success = skew + rand() % diceSize < failThreshold;
        skew += success ? 1 : -1;
        return success;
    }
};
