#ifndef OMNI_DIGITALINPUTPINSTUB_H
#define OMNI_DIGITALINPUTPINSTUB_H

#include "DigitalInputPin.h"

namespace omni
{
    class DigitalInputPinStub : public DigitalInputPin
    {
        private:

        protected:
            bool readPin() final;

        public:
            DigitalInputPinStub(unsigned short pin, bool invertLogic);

            virtual ~DigitalInputPinStub();
    };
}

#endif
