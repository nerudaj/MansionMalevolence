#pragma once

enum class [[nodiscard]] AnimationKind
{
    SkipCard,
    TakeCard,
    TrashMainCard,
    EnemyAttack,
    EnemyDamaged,
    EnemyDodgedAttack,
};
