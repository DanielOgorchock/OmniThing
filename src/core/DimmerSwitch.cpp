#include "DimmerSwitch.h"

#include "OmniThing.h"
#include "frozen.h"
#include <cmath>
#include <string.h>
#include "OutputFloat.h"
#include "InputFloat.h"


namespace omni
{
//private
    void DimmerSwitch::sendJsonPacket()
    {
        char buffer[256] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{name: \"%s\", type: \"%s\", level: \"%f\", switch: \"%s\"}",
                    getName(), getType(), read(), m_bValue ? "on" : "off");

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void DimmerSwitch::writeNoUpdate(float level, bool turnOff)
    {
        if(!hasInput() && level != m_fLevel)
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
        if (!hasInput() && !turnOff)
            m_fLevel = level;
        m_bValue = level != 0;
        m_rOutput.writeFloat(level);
    }

//protected

//public
    DimmerSwitch::DimmerSwitch(OutputFloat& output, float initialLevel, InputFloat* input) :
        Device(false),
        m_rOutput(output),
        m_pInput(input),
        m_fInitial(initialLevel),
        m_fLevel(initialLevel),
        m_bValue(m_fLevel != 0)
    {
        writeNoUpdate(initialLevel, false);
        if(hasInput())
        {
            setRun(true);
        }
    }

    DimmerSwitch::~DimmerSwitch()
    {

    }

    void DimmerSwitch::recvJson(const char* cmd, const char* json)
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
                write(level, false);
        }
        else if(!strcmp(cmd, Cmd_On))
        {
            LOG << F("On triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            if(hasInput())
                m_fLevel = m_fInitial;
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
        if(hasInput())
        {
            m_fLevel = m_pInput->readFloat();
        }
        sendJsonPacket();
    }

    void DimmerSwitch::run()
    {
        if(!hasInput())
        {
            LOG << F("ERROR: How did we enable DimmerSwitch run() with null input? disabling...\n");
            setRun(false);
            return;
        }

        float tmp = m_pInput->readFloat();

        m_bValue = tmp;
        if(fabs(tmp - read()) >= 0.2)
        {
            emit(Event_Changed);
            if(tmp != 0 && read() == 0)
            {
                emit(Event_On);
            }
            else if(tmp == 0 && read() != 0)
            {
                emit(Event_Off);
            }
            m_fLevel = tmp;
            sendJsonPacket();
        }
    }

    void DimmerSwitch::write(float level, bool turnOff)
    {
        writeNoUpdate(level, turnOff);
        sendJsonPacket();
    }

    Device* DimmerSwitch::createFromJson(const char* json)
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
                LOG << F("WARNING: Failed to create optional DimmerSwitch input\n");
            }
        }

        auto d = new DimmerSwitch(*output, initial, input);
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

