#include "InputUIntRef.h"
#include "CompositePeripheral.h"
#include <string.h>
#include "Logger.h"
#include "frozen.h"

namespace omni
{
//private
//protected
//public
    InputUIntRef::InputUIntRef(CompositePeripheral& periph, const char* name):
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

    InputUIntRef::~InputUIntRef()
    {
        
    }

    unsigned int InputUIntRef::readUInt()
    {
        return m_Periph.getUInt(m_Name);
    }

    InputUInt* InputUIntRef::createFromJson(const char* json)
    {
        unsigned int compositeIndex;
        char name[20];
        if(json_scanf(json, strlen(json), "{compositeIndex: %u, paramName: %s}", &compositeIndex, &name) != 2)
            return nullptr;

        auto composites = OmniThing::getInstance().getCompositePeriphs();
        if(compositeIndex >= composites.getCount())
        {
            LOG << F("ERROR: compositeIndex=") << compositeIndex << F(" is out of bounds.\n");
            return nullptr;
        } 

        return new InputUIntRef(*(composites[compositeIndex]), name);
    }

    const char* InputUIntRef::Type = "InputUIntRef";
    ObjectConfig<InputUInt> InputUIntRef::InputUIntConf(Type, createFromJson);
}


