#include "SwitchLevel.h"

#include "OmniThing.h"
#include "frozen.h"
#include <cmath>
#include <string.h>
#include "OutputFloat.h"
#include "InputFloat.h"


namespace omni
{
//private
    void SwitchLevel::sendJsonPacket()
    {
        char buffer[256] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{name: \"%s\", type: \"%s\", level: \"%f\"}", getName(), getType(), read());

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void SwitchLevel::writeNoUpdate(float level)
    {
        if(!hasInput() && level != m_fLevel)
        {
            emit(Event_Changed);
        }

        if(!hasInput())
            m_fLevel = level;

        m_rOutput.writeFloat(level);
    }

//protected

//public
    SwitchLevel::SwitchLevel(OutputFloat& output, float initialLevel, InputFloat* input) :
        Device(false),
        m_rOutput(output),
        m_pInput(input),
        m_fLevel(initialLevel)
    {
        writeNoUpdate(initialLevel);
        if(hasInput())
        {
            setRun(true);
        }
    }

    SwitchLevel::~SwitchLevel()
    {

    }

    void SwitchLevel::recvJson(const char* cmd, const char* json)
    {
        unsigned int len;

        if(json)
            len = strlen(json);

        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            sendJsonPacket();
        }
        else if(!strcmp(cmd, Cmd_SetLevel) && json)
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
        if(hasInput())
        {
            m_fLevel = m_pInput->readFloat();
        }
        sendJsonPacket();
    }

    void SwitchLevel::run()
    {
        if(!hasInput())
        {
            LOG << F("ERROR: How did we enable SwitchLevel run() with null input? disabling...\n");
            setRun(false);
            return;
        }

        float tmp = m_pInput->readFloat();

        if(fabs(tmp - read()) >= 0.2)
        {
            emit(Event_Changed);
            m_fLevel = tmp;
            sendJsonPacket();
        }
    }

    void SwitchLevel::write(float level)
    {
        writeNoUpdate(level);
        sendJsonPacket();
    }

    Device* SwitchLevel::createFromJson(const char* json)
    {
        float initial;
        InputFloat* input = nullptr;

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

        //optional input
        if(json_scanf(json, len, "{input: %T}", &t) == 1)
        {
            LOG << F("Optional input provided\n");

            input = OmniThing::getInstance().buildInputFloat(t);
            if(!input)
            {
                LOG << F("WARNING: Failed to create optional SwitchLevel input\n");
            }
        }

        auto d = new SwitchLevel(*output, initial, input);
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

