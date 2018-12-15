#ifndef OMNI_DIGITALOUTPUTPINRASPBERRYPI_H
#define OMNI_DIGITALOUTPUTPINRASPBERRYPI_H

#include "DigitalOutputPin.h"

namespace omni
{
    class DigitalOutputPinRaspberryPi : public DigitalOutputPin
    {
        private:
            static DigitalOutputPinRaspberryPi* createFromJson(const char* json);

        protected:
            void writePin(bool b) final;

        public:
            DigitalOutputPinRaspberryPi(unsigned short pin, bool initialVal, bool invertLogic);

            virtual ~DigitalOutputPinRaspberryPi();

            virtual bool configure();

            static OutputVoid* createVoidFromJson(const char* json);
            static OutputBool* createBoolFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<OutputVoid> OutputVoidConf;
            static ObjectConfig<OutputBool> OutputBoolConf;
    };
}

#endif

