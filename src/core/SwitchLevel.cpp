#include "SwitchLevel.h"

#include "OmniThing.h"
#include "frozen.h"
#include <string.h>
#include "OutputFloat.h"


namespace omni
{
//private
    void SwitchLevel::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{name: \"%s\", type: \"%s\", level: \"%f\"}", getName(), getType(), read());

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void SwitchLevel::writeNoUpdate(float level)
    {
        if(level != m_fLevel)
        {
            emit(Event_Changed);
        }
        m_fLevel = level;
        m_rOutput.writeFloat(level);
    }

//protected

//public
    SwitchLevel::SwitchLevel(OutputFloat& output, float initialLevel) :
        Device(false),
        m_rOutput(output),
        m_fLevel(initialLevel)
    {
        writeNoUpdate(initialLevel);
    }

    SwitchLevel::~SwitchLevel()
    {

    }

    void SwitchLevel::recvJson(const char* cmd, const char* json)
    {
        unsigned int len = strlen(json);
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            sendJsonPacket();
        }
        else if(!strcmp(cmd, Cmd_SetLevel))
        {
            LOG << F("SetLevel triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            float level;
            if(json_scanf(json, len, "{level: %f}", &level) != 1)
                LOG << F("ERROR: Failed to find level in json\n");
            else
                write(level);
        }
    }

    void SwitchLevel::init()
    {
        sendJsonPacket();
    }

    void SwitchLevel::write(float level)
    {
        writeNoUpdate(level);
        sendJsonPacket();
    }

    Device* SwitchLevel::createFromJson(const char* json)
    {
        float initial;

        unsigned int len = strlen(json);
        json_token t;

        if(json_scanf(json, len, "{output: %T, initial: %f}", &t, &initial) != 2)
        {
            return nullptr;
        }

        auto output = OmniThing::getInstance().buildOutputFloat(t);
        if(!output)
        {
            LOG << F("ERROR: Failed to create output\n");
            return nullptr;
        }

        auto d = new SwitchLevel(*output, initial);
        if(!d->parseMisc(json))
            return nullptr;
        return d;
    }


    const char* SwitchLevel::Type = "SwitchLevel";

    const char* SwitchLevel::Cmd_Poll       = "poll";
    const char* SwitchLevel::Cmd_SetLevel   = "setLevel";

    const char* SwitchLevel::Event_Changed  = "changed";

    ObjectConfig<Device> SwitchLevel::DevConf(Type, createFromJson);
}

