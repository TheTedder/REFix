#pragma once
#include <cstdint>

namespace REFix {
    enum class RankPointType : int32_t {
        PlAttackHit = 0,
        PlGetItem = 1,
        PlDamage = 2,
        PlRetry = 3,
        FromFsm = 4,
        FromScript = 5,
        FromDirectSet = 6,
        PlUseWeapon = 7,
        PlUseDefenceItem = 8,
        FromDebug = 9
    };
}