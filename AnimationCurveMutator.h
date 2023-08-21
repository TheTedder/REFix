#pragma once
#include "REFix.h"

namespace REFix {
    /// <summary>
    /// Performs an operation on an AnimationCurve by iterating over its KeyFrames.
    /// </summary>
    class AnimationCurveMutator {
    public:
        /// <param name="animation_curve">the AnimationCurve to operate on. Note that even though this is a const pointer, this function will perform a destructive operation.</param>
        void mutate(const REF::API::ManagedObject* animation_curve) const;

    protected:
        virtual void operate(REF::API::ManagedObject* key_frame) const = 0;
    };
}