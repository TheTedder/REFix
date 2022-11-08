#include "Undamper.h"

namespace REFix {
    Undamper::Undamper(const REF::API::TypeDefinition* damping_struct_type) {
        this->damping_time = damping_struct_type->find_field("DampingTime");
    }

    void Undamper::undamp(REF::API::ManagedObject* damping_struct) const {
        *(float*)this->damping_time->get_data_raw(damping_struct) = 0.0f;
    }
}