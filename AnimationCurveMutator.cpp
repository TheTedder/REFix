#include "AnimationCurveMutator.h"
#include <cstdint>
#include <cstdlib>

namespace REFix {
    extern const REF::API::TypeDefinition* key_frame_type;
    extern const REF::API::Method* get_keys_count;
    extern const REF::API::Method* get_keys;
    extern const REF::API::Method* set_keys;

    void AnimationCurveMutator::mutate(const REF::API::ManagedObject* animation_curve) const {
        void* const key_frame = std::malloc(key_frame_type->get_valuetype_size());
        const uint32_t key_count = get_keys_count->call<uint32_t>(VMC(), animation_curve);

        for (uint32_t i = 0U; i < key_count; i++) {
            const REF::API::ManagedObject* const keys_result = get_keys->call<REF::API::ManagedObject*>(key_frame, VMC(), animation_curve, i);

            if (keys_result == nullptr) {
               LOG_WARN("Failed to retrieve KeyFrame %u for animation curve %p", i, animation_curve);
                continue;
            }

            operate(key_frame);
            set_keys->call(VMC(), animation_curve, i, key_frame);
        }

        std::free(key_frame);
    }
}