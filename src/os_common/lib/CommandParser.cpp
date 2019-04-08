#include "CommandParser.h"

#include "OmniOSUtil.h"
#include <string.h>
#include <stdio.h>

namespace omni
{
//private

//public
    CommandParser::CommandParser(const char* commandStr):
        m_CommandStr(commandStr)
    {

    }

    CommandParser::~CommandParser()
    {

    }

    float CommandParser::readFloat()
    {
        char buffer[128];
        size_t len = sizeof(buffer);
        float value = -1;

        processRun(m_CommandStr, false, buffer, &len);
        if(sscanf(buffer, "%f", &value) != 1)
            LOG << "Warning: Failed to parse float in command output: " << buffer << Logger::endl;

        return value;
    }

    InputFloat* CommandParser::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        char* command;

        if(json_scanf(json, len, "{command: %Q}", &command) != 1)
        {
            return nullptr;
        }

        return new CommandParser(command);
    }

    const char* CommandParser::Type = "CommandParser";
    ObjectConfig<InputFloat> CommandParser::InputFloatConf(Type, createFromJson);
}
