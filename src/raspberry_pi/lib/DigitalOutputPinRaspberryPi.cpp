#include "DigitalOutputPinRaspberryPi.h"

#include <iostream>
#include <pigpio.h>

namespace omni
{
//private

//protected
    void DigitalOutputPinRaspberryPi::writePin(bool b)
    {
        int res = gpioWrite(getPin(), b);

        if(res == PI_BAD_GPIO || res == PI_BAD_LEVEL)
            std::cerr << "Problem writing pin " << getPin() << std::endl;
    }

//public
    DigitalOutputPinRaspberryPi::DigitalOutputPinRaspberryPi(unsigned short pin, bool initialVal, bool invertLogic):
        DigitalOutputPin(pin, initialVal, invertLogic)
    {
        gpioSetMode(pin, PI_OUTPUT);
    }

    DigitalOutputPinRaspberryPi::~DigitalOutputPinRaspberryPi()
    {

    }

}
