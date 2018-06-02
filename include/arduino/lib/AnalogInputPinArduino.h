#ifndef OMNI_ANALOGINPUTPINARDUINO_H
#define OMNI_ANALOGINPUTPINARDUINO_H

#include "AnalogInputPin.h"

namespace omni
{
    class AnalogInputPinArduino : public AnalogInputPin
    {
        private:
            static AnalogInputPinArduino* createFromJson(const char* json);

        protected:
            unsigned int readPin() final;

        public:
            AnalogInputPinArduino(unsigned short pin, float minVolt, float maxVolt);

            virtual ~AnalogInputPinArduino();

            static InputUInt* createUIntFromJson(const char* json);
            static InputFloat* createFloatFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<InputUInt> InputUIntConf;
            static ObjectConfig<InputFloat> InputFloatConf; 
    };
}
#endif
