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
            DigitalOutputPinRaspberryPi(unsigned short pin, bool initialVal, bool invertLogic);

            virtual ~DigitalOutputPinRaspberryPi();

            static OutputVoid* createVoidFromJson(char* json);
            static OutputBool* createBoolFromJson(char* json);

            static ObjectConfig<OutputVoid> OutputVoidConf;
            static ObjectConfig<OutputBool> OutputBoolConf;
    };
}

#endif

