#ifndef OMNI_PWMOUTPUT_H
#define OMNI_PWMOUTPUT_H

#include "OutputFloat.h"
#include "ObjectConfig.h"

namespace omni
{
    class PwmOutput : public OutputFloat
    {
        private:
            unsigned short m_nPin;

            void setDutyCycle(float f);

        protected:
        public:
            PwmOutput(unsigned short pin);
            virtual ~PwmOutput();

            virtual void writeFloat(float percent);

            static OutputFloat* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<OutputFloat> OutputFloatConf;
    };
}
#endif
