#ifndef OMNI_SERVOMOTOR_H
#define OMNI_SERVOMOTOR_H

#include "OutputFloat.h"
#include "ObjectConfig.h"

#if defined(OMNI_PLAT_RPI)
    #include <pigpio.h>
#elif !defined(OMNI_NOT_ARDUINO)
    #ifndef ARDUINO_ARCH_ESP32
        #include <Servo.h>
    #else
        #include "ESP32_Servo.h"
    #endif
#endif

namespace omni
{
    class ServoMotor : public OutputFloat
    {
        private:
        #ifndef OMNI_NOT_ARDUINO
            Servo m_Servo;
        #endif
            unsigned short m_nPin;
            float m_fInitial;
            bool m_bRevert;
            unsigned long m_nRevertTime;
            bool m_bShutoff;
            unsigned long m_nShutoffTime;
            bool m_bNoStartup;
            unsigned long m_nMinPulse;
            unsigned long m_nMaxPulse;
            unsigned int m_nShutoffId;
            unsigned int m_nShutoffIdTriggers;
            unsigned int m_nRevertId;
            unsigned int m_nRevertIdTriggers;

            void writeFloatNoRevert(float percent);
            void attach();
            void detach();
            void writePulse(unsigned long pulseWidth);

            static char Cmd_Detach[];
            static char Cmd_Revert[];
            static char Cmd_Startup[];
        protected:
        public:
            ServoMotor(unsigned short pin, float initialPercent, bool revert, unsigned long revertTime, bool shutoff, unsigned long shutoffTime, bool noStartup, unsigned long minPulse, unsigned long maxPulse);
            virtual ~ServoMotor();

            virtual void writeFloat(float percent);

            virtual void trigger(void* arg);

            static OutputFloat* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<OutputFloat> OutputFloatConf;
    };
}
#endif
