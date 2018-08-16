#include "AnalogInputPinArduino.h"

#include <Arduino.h>
#include <string.h>
#include "frozen.h"

#if defined(ARDUINO_ARCH_ESP32)
    #define MAX_INTEGER_VAL 4095
#else
    #define MAX_INTEGER_VAL 1023
#endif

namespace omni
{
//private
    AnalogInputPinArduino* AnalogInputPinArduino::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        unsigned short pin;
        float minVoltage;
        float maxVoltage;

        if(json_scanf(json, len, "{pin: %hu, minVoltage: %f, maxVoltage: %f}", &pin, &minVoltage, &maxVoltage)!=3)
            return nullptr;

        LOG << F("minVoltage=") << minVoltage << F(" maxVoltage=") << maxVoltage << Logger::endl;
        return new AnalogInputPinArduino(pin, minVoltage, maxVoltage);
    }

//protected
    unsigned int AnalogInputPinArduino::readPin()
    {
        return analogRead(getPin());
    }

//public
    AnalogInputPinArduino::AnalogInputPinArduino(unsigned short pin, float minVolt, float maxVolt):
        AnalogInputPin(pin, MAX_INTEGER_VAL, minVolt, maxVolt)
    {

    }

    AnalogInputPinArduino::~AnalogInputPinArduino()
    {

    }

    InputUInt* AnalogInputPinArduino::createUIntFromJson(const char* json)
    {
        return createFromJson(json);
    }

    InputFloat* AnalogInputPinArduino::createFloatFromJson(const char* json)
    {
        return createFromJson(json);
    }

    const char* AnalogInputPinArduino::Type = "AnalogInputPinArduino";
    ObjectConfig<InputUInt> AnalogInputPinArduino::InputUIntConf(Type, createUIntFromJson);
    ObjectConfig<InputFloat> AnalogInputPinArduino::InputFloatConf(Type, createFloatFromJson);
}
