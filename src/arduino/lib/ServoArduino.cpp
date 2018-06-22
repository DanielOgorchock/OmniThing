#include "ServoArduino.h"

#include <string.h>
#include "frozen.h"

namespace omni
{
//private
//protected
//public
    ServoArduino::ServoArduino(unsigned short pin, float initialPercent) :
        m_Servo()
    {
        m_Servo.attach(pin);
        writeFloat(initialPercent);
    }

    ServoArduino::~ServoArduino()
    {

    }

    void ServoArduino::writeFloat(float percent)
    {
        if(percent < 0.f)
            percent = 0;
        if(percent > 100.f)
            percent = 100;

        int angle = percent*180;
        m_Servo.write(angle);
    }

    OutputFloat* ServoArduino::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        unsigned short pin;
        float initialPercent = 90.f;

        if(json_scanf(json, len, "{pin: %u}", &pin) != 1)
        {
            return nullptr;
        }
                
        json_scanf(json, len, "{initialPercent: %f}", &initialPercent); // optional param

        return new ServoArduino(pin, initialPercent);
    }

    const char* ServoArduino::Type = "ServoArduino";
    ObjectConfig<OutputFloat> ServoArduino::OutputFloatConf(Type, createFromJson);
}
