#include "DigitalInputPinArduino.h"

#include <Arduino.h>
#include <string.h>
#include "frozen.h"

namespace omni
{
//private

//protected
    bool DigitalInputPinArduino::readPin()
    {
        return digitalRead(getPin());
    }

//public
    DigitalInputPinArduino::DigitalInputPinArduino(unsigned short pin, bool invertLogic, bool pullup):
        DigitalInputPin(pin, invertLogic),
        m_bPullup(pullup)
    {
        configure();
    }

    DigitalInputPinArduino::~DigitalInputPinArduino()
    {

    }

    bool DigitalInputPinArduino::configure()
    {
        if(m_bPullup)
            pinMode(getPin(), INPUT_PULLUP);
        else
            pinMode(getPin(), INPUT);
        return true;
    }

    InputBool* DigitalInputPinArduino::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        unsigned short pin;
        bool invert;
        bool pullup;

        if(json_scanf(json, len, "{pin: %hu, invert: %B, pullup: %B}", &pin, &invert, &pullup) != 3)
            return nullptr;

        return new DigitalInputPinArduino(pin, invert, pullup);
    }

    const char* DigitalInputPinArduino::Type = "DigitalInputPinArduino";
    ObjectConfig<InputBool> DigitalInputPinArduino::InputBoolConf(Type, createFromJson);
}
