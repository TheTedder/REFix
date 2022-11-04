#pragma once
#include "REFix.h"

namespace REFix {
    /// <summary>
    /// Removes all damping behavior from a DampingStruct.
    /// </summary>
    class Undamper {
        const REF::API::Field* damping_time;

    public:
        /// <param name="damping_struct_type">
        /// An instantiation of the generic type DampingStruct or a subclass thereof.
        /// </param>
        Undamper(const REF::API::TypeDefinition* damping_struct_type);

        /// <summary>
        /// "Undamps" the specified DampingStruct instance by settings its DampingTime to zero.
        /// </summary>
        /// <param name="damping_struct">The damping struct instance to act upon.
        /// Must have the type used to initialize this object as a parent type somewhere in its hierarchy.
        /// </param>
        void undamp(REF::API::ManagedObject* damping_struct) const;
    };
}