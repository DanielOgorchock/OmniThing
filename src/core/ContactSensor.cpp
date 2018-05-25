#include "ContactSensor.h"
#include "InputBool.h"

#include <iostream> //TODO: replace!!!
#include <string.h>

namespace omni
{
//private
//protected
//public
    ContactSensor(InputBool& input, bool invert):
        m_rInput(input),
        m_bInvert(invert)
    {

    }

    ContactSensor::~ContactSensor()
    {

    }

    void ContactSensor::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            std::cout << "Poll triggered for " << getType() << " " << getUid() << std::endl;
            read();
        }

    }

    bool read()
    {
        bool val = m_rInput.readBool();

        val = isInverted() ? !val : val;
        std::cout << "ContactSensor: read() state=" << val?"closed":"open" << std::endl;
        return val;
    }
}
