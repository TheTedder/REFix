#include "Undamper.h"

namespace REFix {
    Undamper::Undamper(const REF::API::TypeDefinition* damping_struct_type) : damping_time(damping_struct_type->find_field("DampingTime")) {}

    void Undamper::undamp(REF::API::ManagedObject* damping_struct) const {
        float& damptime = this->damping_time->get_data<float>(damping_struct);
        damptime = 0.0f;
    }
}