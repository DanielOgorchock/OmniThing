#include "DigitalOutputPinRaspberryPi.h"

#include <pigpio.h>

#include "Logger.h"

namespace omni
{
//private

//protected
    void DigitalOutputPinRaspberryPi::writePin(bool b)
    {
        LOG << "DigitalOutputPinRaspberryPi: pin=" << getPin() << " val=" << b << Logger::endl;
        int res = gpioWrite(getPin(), b);

        if(res == PI_BAD_GPIO || res == PI_BAD_LEVEL)
            LOG << "Problem writing pin " << getPin() << Logger::endl;
    }

//public
    DigitalOutputPinRaspberryPi::DigitalOutputPinRaspberryPi(unsigned short pin, bool initialVal, bool invertLogic):
        DigitalOutputPin(pin, initialVal, invertLogic)
    {
        if(gpioSetMode(pin, PI_OUTPUT))
        {
            LOG << "Failed to set pin " << pin << " to output\n";
        }

        writeBool(initialVal);
    }

    DigitalOutputPinRaspberryPi::~DigitalOutputPinRaspberryPi()
    {

    }

}
