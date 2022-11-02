#include "AnimCurveStraightener.h"
#include <cstdint>

namespace REFix {
    extern const REF::API::Field* in_normal_field;
    extern const REF::API::Field* out_normal_field;

    void AnimationCurveStraightener::operate(REF::API::ManagedObject* key_frame) const {
        *(uint32_t*)in_normal_field->get_data_raw(key_frame, true) = 0U;
        *(uint32_t*)out_normal_field->get_data_raw(key_frame, true) = 0U;
    }
}