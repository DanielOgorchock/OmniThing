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
        m_Name(name),
        m_Periph(periph)
    {
    }

    InputUIntRef::~InputUIntRef()
    {

    }

    unsigned int InputUIntRef::readUInt()
    {
        unsigned int ui;

        if(!m_Periph.getUInt(m_Name, ui))
        {
            LOG << F("ERROR: Failed to get uint=") << m_Name << F(" from ") << m_Periph.getName() << F("\n");
        }

        return ui;
    }

    InputUInt* InputUIntRef::createFromJson(const char* json)
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

        return new InputUIntRef(*periph, paramName);
    }

    const char* InputUIntRef::Type = "InputUIntRef";
    ObjectConfig<InputUInt> InputUIntRef::InputUIntConf(Type, createFromJson);
}


