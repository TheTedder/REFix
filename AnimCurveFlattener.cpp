#include "AnimCurveFlattener.h"

namespace REFix {
    extern const REF::API::Field* value_field;

    AnimationCurveFlattener::AnimationCurveFlattener(float v) : value(v) {}

        *(float*)value_field->get_data_raw(key_frame, true) = value;
    void AnimationCurveFlattener::operate(void* key_frame) const {
    }
}