#include "DigitalInputPinStub.h"

// TODO: replace this with a LOGGER class
#include <iostream>

namespace omni
{
//private


//protected
    bool DigitalInputPinStub::readPin()
    {
        static bool bogusVal = false;
        
        bogusVal = bogusVal;
        //std::cout << "DigitalInputPinStub: readPin(): " << bogusVal << std::endl; 
        return bogusVal;
    }

//public
    DigitalInputPinStub::DigitalInputPinStub(unsigned short pin, bool invertLogic):
        DigitalInputPin(pin, invertLogic)
    {
        std::cout << "DigitalInputPinStub: Constructor(): pin=" << pin << "  invertLogic=" << invertLogic << std::endl;
    }

    DigitalInputPinStub::~DigitalInputPinStub()
    {

    }

}
