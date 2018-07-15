#ifndef OMNI_RELATIVEHUMIDITYMEASUREMENT_H
#define OMNI_RELATIVEHUMIDITYMEASUREMENT_H

#include "FloatMeasurement.h"

namespace omni
{
    class RelativeHumidityMeasurement : public FloatMeasurement
    {
        private:

        protected:
        public:
            RelativeHumidityMeasurement(FloatMeasurement& fm);
            virtual ~RelativeHumidityMeasurement();

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
