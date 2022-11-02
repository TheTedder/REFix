#pragma once
#include "REFix.h"
#include "AnimationCurveMutator.h"

namespace REFix {
    class AnimationCurveFlattener : public AnimationCurveMutator {
        float value;

    public:
        AnimationCurveFlattener(float v);

    protected:
        void operate(REF::API::ManagedObject* key_frame) const;
    };
}