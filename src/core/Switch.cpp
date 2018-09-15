#include "Switch.h"
#include "OutputBool.h"
#include "InputBool.h"

#include <string.h>
#include "frozen.h"
#include "OmniThing.h"

#include "Logger.h"

namespace omni
{
//private
    void Switch::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{name: \"%s\", type: \"%s\", switch: \"%s\"}", getName(), getType(), (read()?"on":"off"));

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void Switch::writeNoUpdate(bool b)
    {
        if(b == m_bValue && m_bIgnoreRedundant)
        {
            LOG << F("Ignoring redundant switch command\n");
            return;
        }
        if(!hasInput() && b != m_bValue)
        {
            emit(Event_Changed);
            if(b)
                emit(Event_On);
            else
                emit(Event_Off);
        }

        if(!hasInput())
            m_bValue = b;

        m_rOutput.writeBool(isInverted() ? !b : b);

        LOG << F("Switch: write() state=") << (b?F("on"):F("off")) << Logger::endl;
    }

//protected
//public
    Switch::Switch(OutputBool& output, bool invert, bool initial, InputBool* input, bool ignoreRedundant):
        Device(false),
        m_rOutput(output),
        m_pInput(input),
        m_bInvert(invert),
        m_bIgnoreRedundant(ignoreRedundant),
        m_bValue(initial)
    {
        writeNoUpdate(m_bValue);
        if(hasInput())
        {
            setRun(true);
        }
    }

    Switch::~Switch()
    {

    }

    void Switch::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            sendJsonPacket();
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

    void Switch::init()
    {
        if(hasInput())
        {
            bool tmp = m_pInput->readBool();
            tmp = isInverted() ? !tmp : tmp;
            m_bValue = tmp;
        }
        sendJsonPacket();
    }

    void Switch::run()
    {
        if(!hasInput())
        {
            LOG << F("ERROR: How did we enable switch run() with null input? disabling...\n");
            setRun(false);
            return;
        }

        bool tmp = m_pInput->readBool();
        tmp = isInverted() ? !tmp : tmp;

        if(tmp != read())
        {
            LOG << F("Detected switch ") << getName() << F(" change to state=") << (tmp ? F("on\n") : F("off\n"));
            m_bValue = tmp;
            sendJsonPacket();
            emit(Event_Changed);
            if(tmp)
                emit(Event_On);
            else
                emit(Event_Off);
        }
    }

    void Switch::write(bool b)
    {
        writeNoUpdate(b);
        sendJsonPacket();
    }

    Device* Switch::createFromJson(const char* json)
    {
        bool invert;
        bool initial;
        InputBool* input = nullptr;
        bool ignoreRedundant = false;

        unsigned int len = strlen(json);
        json_token t;

        if(json_scanf(json, len, "{output: %T, invert: %B, initial: %B}", &t, &invert, &initial) != 3)
        {
            return nullptr;
        }

        auto output = OmniThing::getInstance().buildOutputBool(t);
        if(!output)
        {
            LOG << F("ERROR: Failed to create output\n");
            return nullptr;
        }

        //optional input
        if(json_scanf(json, len, "{input: %T}", &t) == 1)
        {
            LOG << F("Optional input provided\n");

            input = OmniThing::getInstance().buildInputBool(t);
            if(!input)
            {
                LOG << F("WARNING: Failed to create optional switch input\n");
            }
        }

        json_scanf(json, len, "{ignoreRedundant: %B}", &ignoreRedundant); //optional

        auto d = new Switch(*output, invert, initial, input, ignoreRedundant);
        if(!d->parseMisc(json))
            return nullptr;
        return d;
    }




    //commands
    const char* Switch::Cmd_Poll    = "poll";
    const char* Switch::Cmd_On      = "on";
    const char* Switch::Cmd_Off     = "off";
    const char* Switch::Cmd_Toggle  = "toggle";

    //events
    const char* Switch::Event_On        = "on";
    const char* Switch::Event_Off       = "off";
    const char* Switch::Event_Changed   = "changed";

    const char* Switch::Type        = "Switch";
    ObjectConfig<Device> Switch::DevConf(Type, createFromJson); 
}
