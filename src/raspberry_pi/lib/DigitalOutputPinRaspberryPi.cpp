#include "DigitalOutputPinRaspberryPi.h"

#include <pigpio.h>

#include "Logger.h"
#include "frozen.h"
#include <string.h>

namespace omni
{
//private
    DigitalOutputPinRaspberryPi* DigitalOutputPinRaspberryPi::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        unsigned short pin;
        bool initial;
        bool invert;

        if(json_scanf(json, len, "{pin: %hu, initial: %B, invert: %B}", &pin, &initial, &invert) != 3)
        {
            return nullptr;
        }

        return new DigitalOutputPinRaspberryPi(pin, initial, invert);
    }

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
        configure();
        writeBool(initialVal);
    }

    DigitalOutputPinRaspberryPi::~DigitalOutputPinRaspberryPi()
    {

    }

    bool DigitalOutputPinRaspberryPi::configure()
    {
        if(gpioSetMode(getPin(), PI_OUTPUT))
        {
            LOG << "Failed to set pin " << getPin() << " to output\n";
            return false;
        }
        return true;
    }

    OutputVoid* DigitalOutputPinRaspberryPi::createVoidFromJson(const char* json)
    {
        return createFromJson(json);
    }

    OutputBool* DigitalOutputPinRaspberryPi::createBoolFromJson(const char* json)
    {
        return createFromJson(json);
    }

    const char* DigitalOutputPinRaspberryPi::Type = "DigitalOutputPinRaspberryPi";
    ObjectConfig<OutputVoid> DigitalOutputPinRaspberryPi::OutputVoidConf(Type, createVoidFromJson);
    ObjectConfig<OutputBool> DigitalOutputPinRaspberryPi::OutputBoolConf(Type, createBoolFromJson);
}
