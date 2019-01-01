#include "DigitalInputPinRaspberryPi.h"

#include <pigpio.h>

#include "Logger.h"
#include "frozen.h"
#include <string.h>

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
        DigitalInputPin(pin, invertLogic),
        m_PinMode(pm)
    {
        configure();
    }

    DigitalInputPinRaspberryPi::~DigitalInputPinRaspberryPi()
    {

    }

    bool DigitalInputPinRaspberryPi::configure()
    {
        gpioSetMode(getPin(), PI_INPUT);

        unsigned short pullupdown = PI_PUD_DOWN;
        if(m_PinMode == PinMode::Normal)
            pullupdown = PI_PUD_OFF;
        else if(m_PinMode == PinMode::Pullup)
            pullupdown = PI_PUD_UP;

        gpioSetPullUpDown(getPin(), pullupdown);
        return true;
    }

    InputBool* DigitalInputPinRaspberryPi::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        unsigned short pin;
        bool invert;
        char pinmodeBuf[20];

        if(json_scanf(json, len, "{pin: %hu, invert: %B, pinMode: %s}", &pin, &invert, pinmodeBuf) != 3)
            return nullptr;

        PinMode pm = PinMode::Normal;

        if(!strcmp(pinmodeBuf, "Normal"))
            pm = PinMode::Normal;
        else if(!strcmp(pinmodeBuf, "Pullup"))
            pm = PinMode::Pullup;
        else if(!strcmp(pinmodeBuf, "Pulldown"))
            pm = PinMode::Pulldown;
        else
        {
            LOG << "ERROR: invalid pinmode=" << pinmodeBuf << Logger::endl;
            return nullptr;
        }
            
        return new DigitalInputPinRaspberryPi(pin, invert, pm);
    }

    const char* DigitalInputPinRaspberryPi::Type = "DigitalInputPinRaspberryPi";
    ObjectConfig<InputBool> DigitalInputPinRaspberryPi::InputBoolConf(Type, createFromJson);
}
