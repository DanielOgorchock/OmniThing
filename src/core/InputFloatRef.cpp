#include "InputFloatRef.h"
#include "CompositePeripheral.h"
#include <string.h>
#include "Logger.h"
#include "frozen.h"

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

    InputFloat* InputFloatRef::createFromJson(const char* json)
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

        return new InputFloatRef(*(composites[compositeIndex]), name);
    }

    const char* InputFloatRef::Type = "InputFloatRef";
    ObjectConfig<InputFloat> InputFloatRef::InputFloatConf(Type, createFromJson);
}

