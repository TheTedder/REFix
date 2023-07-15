#include "FixRank.h"
#include "Enums.h"

namespace REFix {
    FixRank::FixRank() : SimpleHook(REF::API::get()->tdb()->find_method("app.ropeway.GameRankSystem", "addRankPointDirect"), pre_add_rank_point_direct) {}

    int FixRank::pre_add_rank_point_direct(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys) {
        const RankPointType rankType = *(RankPointType*)&argv[3];

        if (rankType == RankPointType::FromFsm ||
            rankType == RankPointType::FromScript ||
            rankType == RankPointType::FromDirectSet ||
            rankType == RankPointType::FromDebug) {
            return REFRAMEWORK_HOOK_CALL_ORIGINAL;
        }

        return REFRAMEWORK_HOOK_SKIP_ORIGINAL;
    }
}
