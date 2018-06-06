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
            char* name = new char[strlen(buf)];

            strcpy(name, buf);

            LOG << F("Naming device: ") << name << Logger::endl;
            m_Name = name;
        } 

        // scan for triggers
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".Triggers", i, &t) > 0; ++i)
        {
            unsigned long interval;
            char command[20];

            // interval and command are required params
            if(json_scanf(t.ptr, t.len, "{interval: %lu, command: %s}", &interval, command) != 2)
            {
                LOG << F("ERROR: Failed to parse trigger\n");
                continue;
            }
            command[19] = 0; //just in case

            // copy command onto heap
            char* cmd = new char[strlen(command)];
            strcpy(cmd, command);

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
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".Subscriptions", i, &t) > 0; ++i)
        {
            char event[20];
            char source[30];
            char command[20];

            if(json_scanf(t.ptr, t.len, "{source: %s, event: %s, command: %s}", source, event, command) != 3)
            {
                LOG << F("ERROR: Failed to parse subscription\n");
                continue;
            }
            event[19]   = 0; //just in case
            source[29]  = 0;
            command[19] = 0;

            char* ev = new char[strlen(event)];
            strcpy(ev, event);

            char* src = new char[strlen(source)];
            strcpy(src, source);

            char* cmd = new char[strlen(command)];
            strcpy(cmd, command);

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
