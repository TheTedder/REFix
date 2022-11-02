#pragma once
#include "REFix.h"

namespace REFix {
    class AnimationCurveMutator {
    public:
        void mutate(const REF::API::ManagedObject* animation_curve) const;

    protected:
        virtual void operate(REF::API::ManagedObject* key_frame) const = 0;
    };
}