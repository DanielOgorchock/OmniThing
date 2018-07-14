#include "InputBoolRef.h"
#include "CompositePeripheral.h"
#include <string.h>
#include "Logger.h"
#include "OmniThing.h"
#include "frozen.h"

namespace omni
{
//private
//protected
//public
    InputBoolRef::InputBoolRef(CompositePeripheral& periph, const char* name):
        m_Name(name),
        m_Periph(periph)
    {
    }

    InputBoolRef::~InputBoolRef()
    {

    }

    bool InputBoolRef::readBool()
    {
        bool b;

        if(!m_Periph.getBool(m_Name, b))
        {
            LOG << F("ERROR: Failed to get bool=") << m_Name << F(" from ") << m_Periph.getName() << F("\n");
        }

        return b;
    }

    InputBool* InputBoolRef::createFromJson(const char* json)
    {
        const char* compositeName;
        const char* paramName;
        if(json_scanf(json, strlen(json), "{compositeName: %Q, paramName: %Q}", &compositeName, &paramName) != 2)
            return nullptr;

        CompositePeripheral* periph = OmniThing::getInstance().getCompositePeriph(compositeName);
        if(!periph)
        {
            LOG << F("ERROR: Composite Peripheral of name=") << compositeName << F(" does not exist\n");
            return nullptr;
        }

        return new InputBoolRef(*periph, paramName);
    }

    const char* InputBoolRef::Type = "InputBoolRef";
    ObjectConfig<InputBool> InputBoolRef::InputBoolConf(Type, createFromJson);
}
