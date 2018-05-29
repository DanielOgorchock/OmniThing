#include "Switch.h"
#include "OutputBool.h"

#include <string.h>
#include <iostream> //TODO: change this!!!
#include "frozen.h"
#include "OmniThing.h"

namespace omni
{
//private
    void Switch::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{uid: %u, type: \"%s\", state: \"%s\"}", getUid(), getType(), (read()?"on":"off"));

        OmniThing::getInstance().sendJson(buffer);
    }

//protected
//public
    Switch::Switch(OutputBool& output, bool invert, bool initial):
        Device(false),
        m_rOutput(output),
        m_bInvert(invert),
        m_bValue(initial)
    {
        write(m_bValue);
    }

    Switch::~Switch()
    {

    }

    void Switch::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            std::cout << "Poll triggered for " << getType() << " " << getUid() << std::endl;
            sendJsonPacket();
        }
        else if(!strcmp(cmd, Cmd_On))
        {
            std::cout << "On triggered for " << getType() << " " << getUid() << std::endl;
            on();
        }
        else if(!strcmp(cmd, Cmd_Off))
        {
            std::cout << "Off triggered for " << getType() << " " << getUid() << std::endl;
            off();
        }
        else if(!strcmp(cmd, Cmd_Toggle))
        {
            std::cout << "Toggle triggered for " << getType() << " " << getUid() << std::endl;
            toggle();
        }

    }

    void Switch::init()
    {
        sendJsonPacket();
    }

    void Switch::write(bool b)
    {
        m_bValue = b;
        m_rOutput.writeBool(isInverted() ? !b : b);

        std::cout << "Switch: write() state=" << (b?"on":"off") << std::endl;
        sendJsonPacket();
    }





    const char* Switch::Cmd_Poll    = "poll";
    const char* Switch::Cmd_On      = "on";
    const char* Switch::Cmd_Off     = "off";
    const char* Switch::Cmd_Toggle  = "toggle";
    const char* Switch::Type        = "Switch";
}
