#ifndef OMNI_DIGITALOUTPUTPINARDUINO_H
#define OMNI_DIGITALOUTPUTPINARDUINO_H

#include "DigitalOutputPin.h"

namespace omni
{
    class DigitalOutputPinArduino : public DigitalOutputPin
    {
        private:
            static DigitalOutputPinArduino* createFromJson(const char* json);

        protected:
            void writePin(bool b) final;

        public:
            DigitalOutputPinArduino(unsigned short pin, bool initialVal, bool invertLogic);

            virtual ~DigitalOutputPinArduino();

            virtual bool configure();

            static OutputVoid* createVoidFromJson(const char* json);
            static OutputBool* createBoolFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<OutputVoid> OutputVoidConf;
            static ObjectConfig<OutputBool> OutputBoolConf;
    };
}
#endif
