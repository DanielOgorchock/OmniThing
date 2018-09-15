#include "PwmOutputEsp32.h"
#if defined(ARDUINO_ARCH_ESP32)

#include "OmniThing.h"
#include "Logger.h"

#include <Arduino.h>
#include <string.h>
#include "frozen.h"

namespace omni
{
//private
    void PwmOutputEsp32::setDutyCycle(float f)
    {
        unsigned long val = (f/100.f) * m_nMaxVal;
        ledcWrite(m_nChannel, val);
    }
//protected
//public
    PwmOutputEsp32::PwmOutputEsp32(unsigned short pin, unsigned short channel, unsigned long freq, unsigned short res):
        m_nPin(pin),
        m_nChannel(channel),
        m_nFrequency(freq),
        m_nResolutionBits(res)
    {
        m_nMaxVal = (1 << res) - 1;
        LOG << F("resolutionBits=") << res << F(" maxVal=") << m_nMaxVal << Logger::endl;

        if(channel > 7)
            LOG << F("ERROR: channel value must be between 0 and 7 (inclusive)\n");

        ledcAttachPin(pin, channel);
        ledcSetup(channel, freq, res);
        setDutyCycle(0);
    }

    PwmOutputEsp32::~PwmOutputEsp32()
    {

    }

    void PwmOutputEsp32::writeFloat(float percent)
    {
        setDutyCycle(percent);
    }

    OutputFloat* PwmOutputEsp32::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        unsigned short pin;
        unsigned short channel;
        unsigned long freq = 12000;
        unsigned short res = 8;

        if(json_scanf(json, len, "{pin: %hu, channel: %hu}", &pin, &channel) != 2)
        {
            return nullptr;
        }

        json_scanf(json, len, "{frequency: %lu}", &freq); //optional
        json_scanf(json, len, "{resolutionBits: %hu}", &res); //optional

        return new PwmOutputEsp32(pin, channel, freq, res);
    }

    const char* PwmOutputEsp32::Type = "PwmOutputEsp32";
    ObjectConfig<OutputFloat> PwmOutputEsp32::OutputFloatConf(Type, createFromJson);
}
#endif
