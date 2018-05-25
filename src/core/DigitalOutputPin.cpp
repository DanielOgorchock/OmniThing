#include "DigitalOutputPin.h"

namespace omni
{
//private


//public
    DigitalOutputPin::DigitalOutputPin(unsigned char pin, bool initialValue, bool invertLogic):
        m_nPin(pin),
        m_bValue(initialValue),
        m_bInvertLogic(invertLogic)
    {

    }       

    DigitalOutputPin::~DigitalOutputPin()
    {

    }

    void DigitalOutputPin::writeVoid() //behaves as a toggle
    {
        m_bValue = !m_bValue;
        writePin(isInverted() ? !m_bValue : m_bValue);
    }

    void DigitalOutputPin::writeBool(bool b)
    {
        bool val = isInverted() ? !b : b;
        m_bValue = b;

        writePin(val);
    }
}

