#pragma once
#include "REFix.h"
#include "AnimationCurveMutator.h"

namespace REFix {
    /// <summary>
    /// Straightens an AnimationCurve by setting the normal vectors on all its KeyFrames to <0,0>.
    /// </summary>
    class AnimationCurveStraightener : public AnimationCurveMutator {
    protected:
        void operate(REF::API::ManagedObject* key_frame) const;
    };
}