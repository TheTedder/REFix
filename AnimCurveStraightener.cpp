#include "AnimCurveStraightener.h"
#include <cstdint>

namespace REFix {
    extern const REF::API::Field* in_normal_field;
    extern const REF::API::Field* out_normal_field;

    void AnimationCurveStraightener::operate(void* key_frame) const {
        uint32_t& in_normal = in_normal_field->get_data<uint32_t>(key_frame, true);
        uint32_t& out_normal = out_normal_field->get_data<uint32_t>(key_frame, true);
        LOG_INFO("Anim key original normal: (%u, %u)", in_normal, out_normal);
        in_normal = 0U;
        out_normal = 0U;
    }
}