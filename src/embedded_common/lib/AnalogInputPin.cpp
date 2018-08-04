#include "AnalogInputPin.h"

#if !defined(OMNI_NOT_ARDUINO)
    #include "AnalogInputPinArduino.h"
#endif

namespace omni
{
//private


//public
    AnalogInputPin::AnalogInputPin(unsigned short pin, unsigned int maxVal, float minVolt, float maxVolt):
        m_nPin(pin),
        m_nMaxValue(maxVal),
        m_fMinVoltage(minVolt),
        m_fMaxVoltage(maxVolt)
    {

    }

    AnalogInputPin::~AnalogInputPin()
    {

    }

    unsigned int AnalogInputPin::readUInt()
    {
        unsigned int result = readPin();

        return result;
    }

    float AnalogInputPin::readFloat()
    {
        float percentage = readPin() / static_cast<float>(m_nMaxValue);

        float difference = m_fMaxVoltage - m_fMinVoltage;

        return m_fMinVoltage + percentage*difference;
    }

    AnalogInputPin* AnalogInputPin::create(unsigned short pin, float minVolt, float maxVolt)
    {
    #if !defined(OMNI_NOT_ARDUINO)
        return new AnalogInputPinArduino(pin, minVolt, maxVolt);
    #else
        LOG << F("ERROR: AnalogInputPin not supported on this platform\n");
        return nullptr;
    #endif

    }
}

