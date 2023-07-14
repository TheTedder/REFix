#pragma once
#include "REFix.h"

namespace REFix {
    class Module {
    public:
        /// <summary>
        /// Enable the module.
        /// </summary>
        /// <returns>a boolean indicating whether the module was successfully enabled or not.</returns>
        virtual bool enable() { return true; }

        /// <summary>
        /// Disable the module.
        /// </summary>
        /// <returns>a boolean indicating whether the module was successfully disabled or not.</returns>
        virtual bool disable() { return true; }

        /// <summary>
        /// Get the display name of the module.
        /// </summary>
        virtual const char* display_name() const = 0;

        /// <summary>
        /// Get the internal name of the module. This should consist of letters, numbers, and underscores.
        /// </summary>
        /// <returns></returns>
        virtual const char* internal_name() const = 0;
    };

    bool enable_with_error(Module* m);
    bool disable_with_error(Module* m);
}