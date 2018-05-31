#include "InputFloatRef.h"
#include "CompositePeripheral.h"
#include <string.h>
#include "Logger.h"

namespace omni
{
//private
//protected
//public
    InputFloatRef::InputFloatRef(CompositePeripheral& periph, const char* name):
        m_Periph(periph)
    {
        if(strlen(name) > 19)
        {
            LOG << F("WARNING: \"") << name << F("\" has too many characters to store\n");
            m_Name[0] = 0;
        }
        else
        {
            strcpy(m_Name, name);
        }
    }

    InputFloatRef::~InputFloatRef()
    {
        
    }

    float InputFloatRef::readFloat()
    {
        return m_Periph.getBool(m_Name);
    }
}

