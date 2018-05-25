#include "Switch.h"
#include "OutputBool.h"

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
}
