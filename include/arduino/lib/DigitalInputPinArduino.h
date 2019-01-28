#ifndef OMNI_DIGITALINPUTPINARDUINO_H
#define OMNI_DIGITALINPUTPINARDUINO_H

#include "DigitalInputPin.h"

namespace omni
{
    class DigitalInputPinArduino : public DigitalInputPin
    {
        private:
            bool m_bPullup;

        protected:
            bool readPin() final;

        public:
            DigitalInputPinArduino(unsigned short pin, bool invertLogic, bool pullup);

            virtual ~DigitalInputPinArduino();

            virtual bool configure();

            static InputBool* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<InputBool> InputBoolConf;
    };
}

#endif
