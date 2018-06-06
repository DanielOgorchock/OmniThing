#include "DigitalInputPin.h"

#if defined(OMNI_PLAT_RPI)
    #include "DigitalInputPinRaspberryPi.h"
#elif !defined(OMNI_NOT_ARDUINO)
    #include "DigitalInputPinArduino.h"
#endif

namespace omni
{
//private


//public
    DigitalInputPin::DigitalInputPin(unsigned short pin, bool invertLogic):
        m_nPin(pin),
        m_bInvertLogic(invertLogic)
    {

    }       

    DigitalInputPin::~DigitalInputPin()
    {

    }

    bool DigitalInputPin::readBool()
    {
        bool result = readPin();

        return isInverted() ? !result : result;
    }

    DigitalInputPin* DigitalInputPin::create(unsigned short pin, bool invertLogic, bool pullup)
    {
    #if defined(OMNI_PLAT_RPI)
        DigitalInputPinRaspberryPi::PinMode pm_normal = DigitalInputPinRaspberryPi::PinMode::Normal;
        DigitalInputPinRaspberryPi::PinMode pm_pullup = DigitalInputPinRaspberryPi::PinMode::Pullup;
        return new DigitalInputPinRaspberryPi(pin, invertLogic, (pullup ? pm_pullup : pm_normal));
    #elif !defined(OMNI_NOT_ARDUINO)
        return new DigitalInputPinArduino(pin, invertLogic, pullup);
    #else
        LOG << F("ERROR: DigitalInputPin not supported on this platform\n");
        return nullptr;
    #endif
    }
}
