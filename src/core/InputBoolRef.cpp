#include "InputBoolRef.h"
#include "CompositePeripheral.h"
#include <string.h>
#include "Logger.h"

namespace omni
{
//private
//protected
//public
    InputBoolRef::InputBoolRef(CompositePeripheral& periph, const char* name):
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

    InputBoolRef::~InputBoolRef()
    {
        
    }

    bool InputBoolRef::readBool()
    {
        return m_Periph.getBool(m_Name);
    }
}
