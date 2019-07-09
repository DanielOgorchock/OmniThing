#include "DimmerSwitch.h"

#include "OmniThing.h"
#include "frozen.h"
#include <string.h>
#include "OutputFloat.h"


namespace omni
{
//private
    void DimmerSwitch::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{name: \"%s\", type: \"%s\", level: \"%f\", switch: \"%s\"}",
                    getName(), getType(), read(), m_bValue ? "on" : "off");

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void DimmerSwitch::writeNoUpdate(float level, bool turnOff)
    {
        if(level != m_fLevel)
        {
            emit(Event_Changed);
            if(m_fLevel == 0)
            {
                emit(Event_On);
            }
            else if(level == 0)
            {
                emit(Event_Off);
            }
        }
        if (!turnOff)
            m_fLevel = level;
        m_bValue = level != 0;
        m_rOutput.writeFloat(level);
    }

//protected

//public
    DimmerSwitch::DimmerSwitch(OutputFloat& output, float initialLevel) :
        Device(false),
        m_rOutput(output),
        m_fLevel(initialLevel),
        m_bValue(m_fLevel != 0)
    {
        writeNoUpdate(initialLevel, false);
    }

    DimmerSwitch::~DimmerSwitch()
    {

    }

    void DimmerSwitch::recvJson(const char* cmd, const char* json)
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
                write(level, false);
        }
        else if(!strcmp(cmd, Cmd_On))
        {
            LOG << F("On triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            on();
        }
        else if(!strcmp(cmd, Cmd_Off))
        {
            LOG << F("Off triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            off();
        }
        else if(!strcmp(cmd, Cmd_Toggle))
        {
            LOG << F("Toggle triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            toggle();
        }
    }

    void DimmerSwitch::init()
    {
        sendJsonPacket();
    }

    void DimmerSwitch::write(float level, bool turnOff)
    {
        writeNoUpdate(level, turnOff);
        sendJsonPacket();
    }

    Device* DimmerSwitch::createFromJson(const char* json)
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

        auto d = new DimmerSwitch(*output, initial);
        if(!d->parseMisc(json))
            return nullptr;
        return d;
    }


    const char* DimmerSwitch::Type = "DimmerSwitch";

    const char* DimmerSwitch::Cmd_Poll       = "poll";
    const char* DimmerSwitch::Cmd_SetLevel   = "setLevel";
    const char* DimmerSwitch::Cmd_On         = "on";
    const char* DimmerSwitch::Cmd_Off        = "off";
    const char* DimmerSwitch::Cmd_Toggle     = "toggle";

    const char* DimmerSwitch::Event_On       = "on";
    const char* DimmerSwitch::Event_Off      = "off";
    const char* DimmerSwitch::Event_Changed  = "changed";

    ObjectConfig<Device> DimmerSwitch::DevConf(Type, createFromJson);
}

