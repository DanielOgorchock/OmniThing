#ifndef OMNI_DIGITALINPUTPIN_H
#define OMNI_DIGITALINPUTPIN_H

#include "InputBool.h"

namespace omni
{
    class DigitalInputPin : public InputBool
    {
        private:
            unsigned short m_nPin;
            bool m_bInvertLogic;

        protected:
            virtual bool readPin() = 0;

        public:
            DigitalInputPin(unsigned short pin, bool invertLogic);

            virtual ~DigitalInputPin();

            unsigned short getPin() const {return m_nPin;}
            bool isInverted() const {return m_bInvertLogic;}

            bool readBool() final; //DigitalInputPin definitions should overwrite readPin, not this
    };
}

#endif
