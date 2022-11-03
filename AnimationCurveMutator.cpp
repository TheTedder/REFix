#include "AnimationCurveMutator.h"
#include <cstdint>

namespace REFix {
    extern const REF::API::TypeDefinition* key_frame_type;
    extern const REF::API::Method* get_keys_count;
    extern const REF::API::Method* get_keys;
    extern const REF::API::Method* set_keys;

    void AnimationCurveMutator::mutate(const REF::API::ManagedObject* animation_curve) const {
        const REF::API::VMContext* const context = REF::API::get()->get_vm_context();

        REF::API::ManagedObject* const key_frame = key_frame_type->create_instance();
        const uint32_t key_count = get_keys_count->call<uint32_t>(context, animation_curve);

        for (uint32_t i = 0U; i < key_count; i++) {
            const REF::API::ManagedObject* const keys_result = get_keys->call<REF::API::ManagedObject*>(key_frame, context, animation_curve, i);

            if (keys_result == 0) {
                REF::API::get()->log_warn("[REFix] Failed to retrieve KeyFrame %u for animation curve %p", i, animation_curve);
                continue;
            }

            operate(key_frame);
            set_keys->call(context, animation_curve, i, key_frame);
        }
    }
}