#include "AnalogInputPin.h"

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
}

