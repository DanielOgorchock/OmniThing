#ifndef OMNI_VOLTAGEMEASUREMENT_H
#define OMNI_VOLTAGEMEASUREMENT_H

#include "FloatMeasurement.h"

namespace omni
{
    class VoltageMeasurement : public FloatMeasurement
    {
        private:

        protected:
        public:
            VoltageMeasurement(FloatMeasurement& fm);
            virtual ~VoltageMeasurement();

            static Device* createFromJson(const char* json);

            virtual const char* getType() const {return Type;}

            // json commands

            // events
            // none as of now


            static const char* Attribute;

            static const char* Type;
            static ObjectConfig<Device> DevConf;
    };
}
#endif
