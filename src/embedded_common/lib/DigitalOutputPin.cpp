#include "DigitalOutputPin.h"

#if defined(OMNI_PLAT_RPI)
    #include "DigitalOutputPinRaspberryPi.h"
#elif !defined(OMNI_NOT_ARDUINO)
    #include "DigitalOutputPinArduino.h"
#endif

namespace omni
{
//private


//public
    DigitalOutputPin::DigitalOutputPin(unsigned short pin, bool initialValue, bool invertLogic):
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
        writeBool(!m_bValue);
    }

    void DigitalOutputPin::writeBool(bool b)
    {
        bool val = isInverted() ? !b : b;
        m_bValue = b;

        writePin(val);
    }

    DigitalOutputPin* DigitalOutputPin::create(unsigned short pin, bool initialValue, bool invertLogic)
    {
    #if defined(OMNI_PLAT_RPI)
        return new DigitalOutputPinRaspberryPi(pin, initialValue, invertLogic);
    #elif !defined(OMNI_NOT_ARDUINO)
        return new DigitalOutputPinArduino(pin, initialValue, invertLogic);
    #else
        LOG << F("ERROR: DigitalInputPin not supported on this platform\n");
        return nullptr;
    #endif
    }
}

