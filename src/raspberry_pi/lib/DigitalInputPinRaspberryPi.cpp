#include "DigitalInputPinRaspberryPi.h"

#include <pigpio.h>

#include "Logger.h"

namespace omni
{
//private

//protected
    bool DigitalInputPinRaspberryPi::readPin()
    {
        int val = gpioRead(getPin());
        if(val == PI_BAD_GPIO)
        {
            LOG << "Problem reading pin " << getPin() << Logger::endl;
            val = 0;
        }

        //std::cout << "DigitalInputPinRaspberryPi: pin=" << getPin() << " val=" << val << std::endl;

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
            pullupdown = PI_PUD_UP;

        gpioSetPullUpDown(pin, pullupdown);
             
    }

    DigitalInputPinRaspberryPi::~DigitalInputPinRaspberryPi()
    {

    }

    InputBool* DigitalInputPinRaspberryPi::createFromJson(const char* json)
    {
        return nullptr;
    }

    const char* DigitalInputPinRaspberryPi::Type = "DigitalInputPinRaspberryPi";
    ObjectConfig<InputBool> DigitalInputPinRaspberryPi::InputBoolConf(Type, createFromJson);
}
