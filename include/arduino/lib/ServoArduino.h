#ifndef OMNI_SERVOARDUINO_H
#define OMNI_SERVOARDUINO_H

#include "OutputFloat.h"
#include "ObjectConfig.h"
#include <Servo.h>

namespace omni
{
    class ServoArduino : public OutputFloat
    {
        private:        
            Servo m_Servo;
            unsigned short m_nPin;
            float m_fInitial;
            bool m_bRevert;
            unsigned long m_nRevertTime;
            bool m_bShutoff;
            unsigned long m_nShutoffTime;

            void writeFloatNoRevert(float percent);

            static char* Cmd_Detach;
            static char* Cmd_Revert;
        protected:
        public:
            ServoArduino(unsigned short pin, float initialPercent, bool revert, unsigned long revertTime, bool shutoff, unsigned long shutoffTime);
            virtual ~ServoArduino();

            virtual void writeFloat(float percent);

            virtual void trigger(void* arg);

            static OutputFloat* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<OutputFloat> OutputFloatConf;
    };
}
#endif
