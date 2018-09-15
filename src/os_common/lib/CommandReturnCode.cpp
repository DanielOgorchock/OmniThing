#include "CommandReturnCode.h"

#include "OmniOSUtil.h"

namespace omni
{
//private

//public
    CommandReturnCode::CommandReturnCode(const char* commandStr):
        m_CommandStr(commandStr)
    {

    }

    CommandReturnCode::~CommandReturnCode()
    {

    }

    bool CommandReturnCode::readBool()
    {
        return (processRun(m_CommandStr, false) == 0);
    }

    InputBool* CommandReturnCode::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        char* command;

        if(json_scanf(json, len, "{command: %Q}", &command) != 1)
        {
            return nullptr;
        }

        return new CommandReturnCode(command);
    }

    const char* CommandReturnCode::Type = "CommandReturnCode";
    ObjectConfig<InputBool> CommandReturnCode::InputBoolConf(Type, createFromJson);
}
