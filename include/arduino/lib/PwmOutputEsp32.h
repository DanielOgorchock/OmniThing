#ifndef OMNI_PWMOUTPUTESP32_H
#define OMNI_PWMOUTPUTESP32_H
#if defined(ARDUINO_ARCH_ESP32)

#include "OutputFloat.h"
#include "ObjectConfig.h"

namespace omni
{
    class PwmOutputEsp32 : public OutputFloat
    {
        private:
            unsigned short m_nPin;
            unsigned short m_nChannel;
            unsigned long  m_nFrequency;
            unsigned short m_nResolutionBits;
            unsigned long  m_nMaxVal;

            void setDutyCycle(float f);

        protected:
        public:
            PwmOutputEsp32(unsigned short pin, unsigned short channel, unsigned long freq, unsigned short res);
            virtual ~PwmOutputEsp32();

            virtual void writeFloat(float percent);

            static OutputFloat* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<OutputFloat> OutputFloatConf;
    };
}

#endif
#endif
