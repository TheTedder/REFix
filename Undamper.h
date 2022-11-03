#pragma once
#include "REFix.h"

namespace REFix {
    class Undamper {
        const REF::API::Field* damping_time;

    public:
        Undamper(const REF::API::TypeDefinition* damping_struct_type);
        void undamp(REF::API::ManagedObject* damping_struct) const;
    };
}