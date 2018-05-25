#ifndef OMNI_DIGITALINPUTPINRASPBERRYPI_H
#define OMNI_DIGITALINPUTPINRASPBERRYPI_H

#include "DigtalInputPin.h"

namespace omni
{
    class DigitalInputPinRaspberryPi : public DigitalInputPin
    {
        private:

        protected:
            bool readPin() final;

        public:
            DigitalInputPinRaspberryPi(unsigned short pin, bool invertLogic, PinMode pm);

            virtual ~DigitalInputPinRaspberryPi();

            enum class PinMode {Normal, Pullup, Pulldown};
    };
}

#endif
