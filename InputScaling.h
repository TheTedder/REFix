#pragma once
#include "SimpleHook.h"

namespace REFix {
    class InputScaling : public Module {
        static const REF::API::Field* camera_param_field;
        static const REF::API::Field* field_of_view_field;

        class UpdatePitch : public SimpleHook {
        public:
            UpdatePitch(const REF::API::TypeDefinition*);

            const char* display_name() const override {
                return "Scale Pitch with FOV";
            }

            const char* internal_name() const override {
                return "Scale_Pitch_FOV";
            }
        };

        class UpdateYaw : public SimpleHook {
        public:
            UpdateYaw(const REF::API::TypeDefinition*);

            const char* display_name() const override {
                return "Scale Yaw with FOV";
            }

            const char* internal_name() const override {
                return "Scale_Yaw_FOV";
            }
        };

        UpdatePitch update_pitch;
        UpdateYaw update_yaw;

    public:
        InputScaling();

        bool enable() override;
        bool disable() override;

        const char* display_name() const override {
            return "Scale Input with FOV";
        }

        const char* internal_name() const override {
            return "Scale_Input_FOV";
        }

        static int pre_update_pitch_yaw(int argc, void** argv, REFrameworkTypeDefinitionHandle* arg_tys);
    };
}