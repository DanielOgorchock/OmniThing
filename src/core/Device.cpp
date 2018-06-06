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
        m_nUid(0),
        m_bRun(run),
        m_Name(EmptyString)
    {
        static unsigned short nextUid = 0;
        m_nUid = nextUid++;
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
        LOG << F("emitting event=") << event << F(" deviceUID=") << getUid() << Logger::endl;
        OmniThing::getInstance().addEvent(getName(), event);
    }

    void Device::parseMisc(const char* json)
    {
        auto& omnithing = OmniThing::getInstance();
        unsigned int len = strlen(json);

        json_token t;

        char buf[30]; 

        // scanning for optional name
        if(json_scanf(json, len, "{name: %s}", &buf) == 1)
        {
            buf[29] = 0; //just in case
            char* name = new char[strlen(buf)+1];

            strcpy(name, buf);

            LOG << F("Naming device: ") << name << Logger::endl;
            m_Name = name;
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

            LOG << F("Successfully added subscription");
        }
    }
}
