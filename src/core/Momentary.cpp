#include "Momentary.h"

#include "Logger.h"
#include "OmniThing.h"
#include "OutputVoid.h"
#include <string.h>
#include "frozen.h"

namespace omni
{
//private
    void Momentary::sendJsonPacket(const char* event)
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{name: \"%s\", type: \"%s\", value: \"%s\"}", getName(), getType(), event);

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void Momentary::push()
    {
        m_rOutput.writeVoid();
        sendJsonPacket(Event_Pushed);
    }

//protected
//public
    Momentary::Momentary(OutputVoid& output):
        Device(false),
        m_rOutput(output)
    {

    }

    Momentary::~Momentary()
    {

    }

    void Momentary::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            sendJsonPacket("polled");
        }
        else if(!strcmp(cmd, Cmd_Push))
        {
            LOG << F("Push triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            push();
        }
    }

    void Momentary::run()
    {

    }

    void Momentary::init()
    {
        sendJsonPacket("polled");
    }

    Device* Momentary::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        json_token t;

        if(json_scanf(json, len, "{output: %T}", &t) != 1)
        {
            return nullptr;
        }

        auto output = OmniThing::getInstance().buildOutputVoid(t);
        if(!output)
        {
            LOG << F("ERROR: Failed to create output\n");
            return nullptr;
        }

        auto d = new Momentary(*output);
        d->parseMisc(json);
        return d;
    }

    // commands
    const char* Momentary::Cmd_Poll = "poll";
    const char* Momentary::Cmd_Push = "push";

    // events
    const char* Momentary::Event_Pushed = "pushed";

    const char* Momentary::Type = "Momentary";
    ObjectConfig<Device> Momentary::DevConf(Type, createFromJson);
}

