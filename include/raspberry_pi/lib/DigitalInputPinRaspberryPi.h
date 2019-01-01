#ifndef OMNI_DIGITALINPUTPINRASPBERRYPI_H
#define OMNI_DIGITALINPUTPINRASPBERRYPI_H

#include "DigitalInputPin.h"

namespace omni
{
    class DigitalInputPinRaspberryPi : public DigitalInputPin
    {
        public:
            enum class PinMode {Normal, Pullup, Pulldown};

        private:
            PinMode m_PinMode;

        protected:
            bool readPin() final;

        public:

            DigitalInputPinRaspberryPi(unsigned short pin, bool invertLogic, PinMode pm);

            virtual ~DigitalInputPinRaspberryPi();

            virtual bool configure();

            static InputBool* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<InputBool> InputBoolConf;
    };
}

#endif
