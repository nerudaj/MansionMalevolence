#pragma once

#include <cmath>

class Easing
{
public:
    static float easeInOut(float x)
    {
        return x < 0.5f ? 4 * std::pow(x, 3.f)
                        : 1 - std::pow(-2 * x + 2, 3.f) / 2;
    }

    static float easeOutThenBack(float x)
    {
        return -4.f * std::pow(x - 0.5f, 2.f) + 1;
    }

    static float easeAttack(float x)
    {
        // note: wolframalpha.com
        // prompt: interpolating polynomial | {point1} ... {pointN}
        if (x < 0.4f)
            return 12.5f * x * x - 5.f * x;
        else if (x < 0.6f)
            return 5.f * (x - 0.4f);
        else if (x < 0.7f)
            return 1.f;
        return 193.333f * std::pow(x, 3.f) - 491.f * x * x + 407.967f * x
               - 110.3f;
    }

    static float easeDamage(float x)
    {
        return std::sin(x * 10.f);
    }

    static float easeValley(float x)
    {
        return 4 * x * x - 4 * x + 1;
    }

    static float easeValley2(float x)
    {
        if (x < 0.5f) return 16.f * x * x - 8.f * x + 1.f;
        return 1.f;
    }
};
