#include "PwmOutput.h"

#ifndef OMNI_NOT_ARDUINO
    #include <Arduino.h>
#elif defined(OMNI_PLAT_RPI)
    #include <pigpio.h>
#endif

#include <string.h>
#include "frozen.h"
#include "OmniThing.h"

namespace omni
{
//private
    void PwmOutput::setDutyCycle(float f)
    {
        unsigned int ds = (f/100.f) * 255;

    #ifndef OMNI_NOT_ARDUINO
        #if defined(ARDUINO_ARCH_ESP32)
            LOG << F("ERROR: PwmOutput doesn't support ESP32. Use PwmOutputEsp32\n");
        #else
            analogWrite(m_nPin, ds);
        #endif
    #elif defined(OMNI_PLAT_RPI)
        if(gpioPWM(m_nPin, ds))
            LOG << F("ERROR: Failed to set pwm on pin=") << m_nPin << F(" ds=") << ds << Logger::endl;
    #endif
    }

//protected

//public
    PwmOutput::PwmOutput(unsigned short pin):
        m_nPin(pin)
    {
        setDutyCycle(0);
    }

    PwmOutput::~PwmOutput()
    {

    }

    void PwmOutput::writeFloat(float percent)
    {
        setDutyCycle(percent);
    }

    OutputFloat* PwmOutput::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        unsigned short pin;

        if(json_scanf(json, len, "{pin: %hu}", &pin) != 1)
        {
            return nullptr;
        }

        return new PwmOutput(pin);
    }


    const char* PwmOutput::Type = "PwmOutput";
    ObjectConfig<OutputFloat> PwmOutput::OutputFloatConf(Type, createFromJson);
}
