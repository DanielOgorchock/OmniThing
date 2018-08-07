#include "CompositePeripheral.h"

#include "OmniThing.h"
#include "Logger.h"

#include <string.h>
#include "frozen.h"

namespace omni
{
//private
//protected
//public
    CompositePeripheral::CompositePeripheral(const char* name):
        m_Name(name)
    {

    }

    CompositePeripheral::~CompositePeripheral()
    {

    }

    const char* CompositePeripheral::parseName(const char* json)
    {
        unsigned int len = strlen(json);

        const char* name = nullptr;

        // scanning for optional name
        if(json_scanf(json, len, "{name: %Q}", &name) != 1)
        {
            LOG << F("ERROR: CompositePeriph name not found\n");
        }

        return name;
    }
}
