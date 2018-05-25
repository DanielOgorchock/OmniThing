#include "DigitalInputPinRaspberryPi.h"

#include <iostream>
#include <pigpio.h>

namespace omni
{
//private

//protected
    bool DigitalInputPinRaspberryPi::readPin()
    {
        int val = gpioRead(getPin());
        if(val == PI_BAD_GPIO)
        {
            std::cerr << "Problem reading pin " << getPin() << std::endl;
            val = 0;
        }

        return val;
    }

//public
    DigitalInputPinRaspberryPi::DigitalInputPinRaspberryPi(unsigned short pin, bool invertLogic, PinMode pm):
        DigitalInputPin(pin, invertLogic)
    {
        gpioSetMode(pin, PI_INPUT);
        
        unsigned short pullupdown = PI_PUD_DOWN;
        if(pm == PinMode::Normal)
            pullupdown = PI_PUD_OFF;
        else if(pm == PinMode::Pullup)
            pullupdown = PI_PUD_UP

        gpioSetPullUpDown(pin, pullupdown);
             
    }

    DigitalInputPinRaspberryPi::~DigitalInputPinRaspberryPi()
    {

    }
}
