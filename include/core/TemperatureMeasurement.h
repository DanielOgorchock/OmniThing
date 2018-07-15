#ifndef OMNI_TEMPERATUREMEASUREMENT_H
#define OMNI_TEMPERATUREMEASUREMENT_H

#include "FloatMeasurement.h"

namespace omni
{
    class TemperatureMeasurement : public FloatMeasurement
    {
        private:

        protected:
        public:
            TemperatureMeasurement(FloatMeasurement& fm);
            virtual ~TemperatureMeasurement();

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
