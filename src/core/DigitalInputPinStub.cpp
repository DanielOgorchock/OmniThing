#include "DigitalInputPinStub.h"

#include "Logger.h"
#include "frozen.h"
#include <string.h>

namespace omni
{
//private


//protected
    bool DigitalInputPinStub::readPin()
    {
        static bool bogusVal = false;
        
        bogusVal = bogusVal;
        return bogusVal;
    }

//public
    DigitalInputPinStub::DigitalInputPinStub(unsigned short pin, bool invertLogic):
        DigitalInputPin(pin, invertLogic)
    {
//        LOG << F("DigitalInputPinStub: Constructor(): pin=") << pin << F("  invertLogic=") << invertLogic << Logger::endl;
    }

    DigitalInputPinStub::~DigitalInputPinStub()
    {

    }


    InputBool* DigitalInputPinStub::createFromJson(const char* json)
    {
        unsigned int pin;
        bool invertLogic;
        if(json_scanf(json, strlen(json), "{pin: %u, invertLogic: %B}", &pin, &invertLogic) != 2)
            return nullptr;

        return new DigitalInputPinStub(pin, invertLogic);
    }

    const char* DigitalInputPinStub::Type = "DigitalInputPinStub";
    ObjectConfig<InputBool> DigitalInputPinStub::InputBoolConf(Type, createFromJson);
}
