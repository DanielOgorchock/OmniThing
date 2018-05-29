#include "DigitalInputPinStub.h"

#include "Logger.h"

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
        LOG << F("DigitalInputPinStub: Constructor(): pin=") << pin << F("  invertLogic=") << invertLogic << Logger::endl;
    }

    DigitalInputPinStub::~DigitalInputPinStub()
    {

    }

}
