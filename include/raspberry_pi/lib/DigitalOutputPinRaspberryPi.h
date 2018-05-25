#ifndef OMNI_DIGITALOUTPUTPINRASPBERRYPI_H
#define OMNI_DIGITALOUTPUTPINRASPBERRYPI_H

#include "DigitalOutputPin.h"

namespace omni
{
    class DigitalOutputPinRaspberryPi : public DigitalOutputPin
    {
        private:

        protected:
            void writePin(bool b) final;

        public:
            DigitalOutputPinRaspberryPi(unsigned short pin, bool invertLogic);

            virtual ~DigitalOutputPinRaspberryPi();
    };
}

#endif

