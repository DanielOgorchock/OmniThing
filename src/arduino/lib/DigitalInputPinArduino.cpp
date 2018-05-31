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
        DigitalInputPin(pin, invertLogic)
    {
        if(pullup)
            pinMode(pin, INPUT_PULLUP);
        else
            pinMode(pin, INPUT);
    }

    DigitalInputPinArduino::~DigitalInputPinArduino()
    {

    }

    InputBool* DigitalInputPinArduino::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        unsigned int pin;
        bool invert;
        bool pullup;

        if(json_scanf(json, len, "{pin: %u, invert: %B, pullup: %B}", &pin, &invert, &pullup) != 3)
            return nullptr;

        return new DigitalInputPinArduino(pin, invert, pullup);
    }

    const char* DigitalInputPinArduino::Type = "DigitalInputPinArduino";
    ObjectConfig<InputBool> DigitalInputPinArduino::InputBoolConf(Type, createFromJson);
}
