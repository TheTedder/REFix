#pragma once
#include "REFix.h"
#include "AnimationCurveMutator.h"

namespace REFix {
    class AnimationCurveStraightener : public AnimationCurveMutator {
    protected:
        void operate(REF::API::ManagedObject* key_frame) const;
    };
}