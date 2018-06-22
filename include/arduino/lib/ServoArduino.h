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

        protected:
        public:
            ServoArduino(unsigned short pin, float initialPercent = 90.f);
            virtual ~ServoArduino();

            virtual void writeFloat(float percent);

            static OutputFloat* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<OutputFloat> OutputFloatConf;
    };
}
#endif
