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
        m_Name(name),
        m_Periph(periph)
    {
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
        const char* compositeName;
        const char* paramName;
        if(json_scanf(json, strlen(json), "{compositeName: %M, paramName: %M}", compositeName, &paramName) != 2)
            return nullptr;

        CompositePeripheral* periph = OmniThing::getInstance().getCompositePeriph(compositeName);
        if(!periph)
        {
            LOG << F("ERROR: Composite Peripheral of name=") << compositeName << F(" does not exist\n");
            return nullptr;
        }

        return new InputFloatRef(*periph, paramName);
    }

    const char* InputFloatRef::Type = "InputFloatRef";
    ObjectConfig<InputFloat> InputFloatRef::InputFloatConf(Type, createFromJson);
}

