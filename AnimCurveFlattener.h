#pragma once
#include "REFix.h"
#include "AnimationCurveMutator.h"

namespace REFix {
    /// <summary>
    /// Flattens an AnimationCurve by setting all its KeyFrames to one specific value.
    /// </summary>
    class AnimationCurveFlattener : public AnimationCurveMutator {
        const float value;

    public:
        /// <summary>
        /// Constructs a new AnimationCurveFlattener.
        /// </summary>
        /// <param name="v">The value to set every KeyFrame to.</param>
        AnimationCurveFlattener(float v);

    protected:
        void operate(REF::API::ManagedObject* key_frame) const;
    };
}