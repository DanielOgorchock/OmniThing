#include "Switch.h"
#include "OutputBool.h"

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

        json_printf(&out, "{uid: %u, type: \"%s\", value: \"%s\"}", getUid(), getType(), (read()?"on":"off"));

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void Switch::writeNoUpdate(bool b)
    {
        if(b != m_bValue)
        {
            emit(Event_Changed);
            if(b)
                emit(Event_On);
            else
                emit(Event_Off);
        }

        m_bValue = b;
        m_rOutput.writeBool(isInverted() ? !b : b);

        LOG << F("Switch: write() state=") << (b?F("on"):F("off")) << Logger::endl;
    }

//protected
//public
    Switch::Switch(OutputBool& output, bool invert, bool initial):
        Device(false),
        m_rOutput(output),
        m_bInvert(invert),
        m_bValue(initial)
    {
        writeNoUpdate(m_bValue);
    }

    Switch::~Switch()
    {

    }

    void Switch::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            sendJsonPacket();
        }
        else if(!strcmp(cmd, Cmd_On))
        {
            LOG << F("On triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            on();
        }
        else if(!strcmp(cmd, Cmd_Off))
        {
            LOG << F("Off triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            off();
        }
        else if(!strcmp(cmd, Cmd_Toggle))
        {
            LOG << F("Toggle triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            toggle();
        }

    }

    void Switch::init()
    {
        sendJsonPacket();
    }

    void Switch::write(bool b)
    {
        writeNoUpdate(b);
        sendJsonPacket();
    }

    Device* Switch::createFromJson(const char* json)
    {
        unsigned int outputIndex;
        bool invert;
        bool initial;

        unsigned int len = strlen(json);

        if(json_scanf(json, len, "{outputIndex: %u, invert: %B, initial: %B}", &outputIndex, &invert, &initial) != 3)
        {
            return nullptr;
        }

        auto& obs = OmniThing::getInstance().getOutputBools();
        if(outputIndex >= obs.getCount())
        {
            LOG << F("ERROR: outputIndex is too large\n");
            return nullptr;
        }

        return new Switch(*(obs[outputIndex]), invert, initial);
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
