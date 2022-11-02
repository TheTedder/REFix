#include "AnimCurveFlattener.h"

namespace REFix {
    extern const REF::API::Field* value_field;

    AnimationCurveFlattener::AnimationCurveFlattener(float v) {
        this->value = v;
    }

    void AnimationCurveFlattener::operate(REF::API::ManagedObject* key_frame) const {
        *(float*)value_field->get_data_raw(key_frame, true) = value;
    }
}