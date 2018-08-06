#include "Device.h"

#include "OmniThing.h"
#include "Logger.h"
#include "frozen.h"
#include <string.h>

namespace omni
{
//private
    const char Device::EmptyString[] = "";

//protected
//public
    Device::Device(bool run):
        m_bRun(run),
        m_Name(EmptyString)
    {
    }

    Device::~Device()
    {

    }

    void Device::trigger(void* arg)
    {
        recvJson(static_cast<const char*>(arg), nullptr);
    }

    void Device::emit(const char* event)
    {
        LOG << F("emitting event=") << event << F(" deviceName=") << getName() << Logger::endl;
        OmniThing::getInstance().addEvent(getName(), event);
    }

    bool Device::parseMisc(const char* json)
    {
        auto& omnithing = OmniThing::getInstance();
        unsigned int len = strlen(json);

        json_token t;

        // scanning for required name
        if(json_scanf(json, len, "{name: %Q}", &m_Name) == 1)
        {
            if(strlen(m_Name) > OMNI_MAX_NAME_LENGTH)
            {
                LOG << F("ERROR: name=") << m_Name << F(" exceeds max length of ")
                    << OMNI_MAX_NAME_LENGTH << Logger::endl;
                delete m_Name;
                return false;
            }
            LOG << F("Naming device: ") << m_Name << Logger::endl;
        }
        else
        {
            LOG << F("ERROR: name is required for device\n");
            return false;
        }

        // scan for triggers
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".triggers", i, &t) > 0; ++i)
        {
            unsigned long interval;
            char* cmd;

            // interval and command are required params
            if(json_scanf(t.ptr, t.len, "{interval: %lu, command: %Q}", &interval, &cmd) != 2)
            {
                LOG << F("ERROR: Failed to parse trigger\n");
                continue;
            }

            // check for optional offset
            unsigned int offset;
            if(json_scanf(t.ptr, t.len, "{offset: %u}", &offset) != 1)
                offset = 0;

            // will fail if this would exceed max triggers
            if(!omnithing.addTrigger(this, interval, cmd, true, offset))
            {
                LOG << F("ERROR: addTrigger() returned false; free cmd[]\n");
                delete[] cmd;
                continue;
            }

            LOG << F("Successfully added trigger\n");
        }

        // scan for subscriptions
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".subscriptions", i, &t) > 0; ++i)
        {
            char* src, *ev, *cmd;

            if(json_scanf(t.ptr, t.len, "{source: %Q, event: %Q, command: %Q}", &src, &ev, &cmd) != 3)
            {
                LOG << F("ERROR: Failed to parse subscription\n");
                continue;
            }

            Event e(src, ev);
            if(!omnithing.addSubscription(Subscription(e, this, cmd)))
            {
                LOG << F("ERROR: Failed to add subscription; freeing strings\n");
                delete[] ev;
                delete[] src;
                delete[] cmd;
            }

            LOG << F("Successfully added subscription\n");
        }

        return true;
    }
}
