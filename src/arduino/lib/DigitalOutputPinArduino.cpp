#include "DigitalOutputPinArduino.h"

#include <Arduino.h>
#include <string.h>
#include "frozen.h"

namespace omni
{
//private
    DigitalOutputPinArduino* DigitalOutputPinArduino::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        unsigned short pin;
        bool initial;
        bool invert;

        if(json_scanf(json, len, "{pin: %hu, initial: %B, invert: %B}", &pin, &initial, &invert) != 3)
        {
            return nullptr;
        }

        return new DigitalOutputPinArduino(pin, initial, invert);
    }

//protected
    void DigitalOutputPinArduino::writePin(bool b)
    {
        digitalWrite(getPin(), b);
    }

//public
    DigitalOutputPinArduino::DigitalOutputPinArduino(unsigned short pin, bool initialVal, bool invertLogic):
        DigitalOutputPin(pin, initialVal, invertLogic)
    {
        configure();
        writeBool(initialVal);
    }

    DigitalOutputPinArduino::~DigitalOutputPinArduino()
    {

    }

    bool DigitalOutputPinArduino::configure()
    {
        pinMode(getPin(), OUTPUT);
        return true;
    }

    OutputVoid* DigitalOutputPinArduino::createVoidFromJson(const char* json)
    {
        return createFromJson(json);
    }

    OutputBool* DigitalOutputPinArduino::createBoolFromJson(const char* json)
    {
        return createFromJson(json);
    }

    const char* DigitalOutputPinArduino::Type = "DigitalOutputPinArduino";
    ObjectConfig<OutputVoid> DigitalOutputPinArduino::OutputVoidConf(Type, createVoidFromJson);
    ObjectConfig<OutputBool> DigitalOutputPinArduino::OutputBoolConf(Type, createBoolFromJson);

}
