#include "Switch.h"
#include "OutputBool.h"

#include <string.h>
#include <iostream> //TODO: change this!!!

namespace omni
{
//private
//protected
//public
    Switch::Switch(OutputBool& output, bool invert, bool initial):
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
            read();
        }
        else if(!strcmp(cmd, Cmd_On))
        {
            std::cout << "On triggered for " << getType() << " " << getUid() << std::endl;
            read();
        }
        else if(!strcmp(cmd, Cmd_Off))
        {
            std::cout << "Off triggered for " << getType() << " " << getUid() << std::endl;
            read();
        }
        else if(!strcmp(cmd, Cmd_Toggle))
        {
            std::cout << "Toggle triggered for " << getType() << " " << getUid() << std::endl;
            read();
        }
        
    }

    void Switch::write(bool b)
    {
        m_bValue = b;
        m_rOutput.writeBool(isInverted() ? !b : b);

        std::cout << "Switch: write() state=" << b?"on":"off" << std::endl;
    }





    const char* Switch::Cmd_Poll    = "poll";
    const char* Switch::Cmd_On      = "on";
    const char* Switch::Cmd_Off     = "off";
    const char* Switch::Cmd_Toggle  = "toggle";
}
