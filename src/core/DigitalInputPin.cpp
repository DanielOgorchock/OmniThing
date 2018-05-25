#include "DigitalInputPin.h"

namespace omni
{
//private


//public
    DigitalInputPin::DigitalInputPin(unsigned short pin, bool invertLogic):
        m_nPin(pin),
        m_bInvertLogic(invertLogic)
    {

    }       

    DigitalInputPin::~DigitalInputPin()
    {

    }

    bool DigitalInputPin::readBool()
    {
        bool result = readPin();

        return isInverted() ? !result : result;
    }
}
