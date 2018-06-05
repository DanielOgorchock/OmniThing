#ifndef OMNI_DIGITALINPUTPINARDUINO_H
#define OMNI_DIGITALINPUTPINARDUINO_H

#include "DigitalInputPin.h"

namespace omni
{
    class DigitalInputPinArduino : public DigitalInputPin
    {
        private:
            bool m_bPullup;
            
            void configure();
        protected:
            bool readPin() final;

        public:
            DigitalInputPinArduino(unsigned short pin, bool invertLogic, bool pullup);

            virtual ~DigitalInputPinArduino();

            static InputBool* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<InputBool> InputBoolConf;
    };
}

#endif
