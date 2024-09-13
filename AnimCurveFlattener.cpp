#include "AnimCurveFlattener.h"

namespace REFix {
    extern const REF::API::Field* value_field;

    AnimationCurveFlattener::AnimationCurveFlattener(float v) : value(v) {}

    void AnimationCurveFlattener::operate(void* key_frame) const {
        float& data = value_field->get_data<float>(key_frame, true);
        data = value;
    }
}