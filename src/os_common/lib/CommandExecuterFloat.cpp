#include "CommandExecuterFloat.h"

#include "OmniThing.h"
#include "Logger.h"
#include "OmniOSUtil.h"

#include <string.h>
#include "frozen.h"
#include <thread>
#include <sstream>

namespace omni
{
//private

//protected
//public
    CommandExecuterFloat::CommandExecuterFloat(const char* command, bool multithread):
        m_CommandStr(command),
        m_bMultithreaded(multithread)
    {

    }

    CommandExecuterFloat::~CommandExecuterFloat()
    {

    }

    void CommandExecuterFloat::writeFloat(float val)
    {
        std::stringstream ss;

        ss << "OMNITHING_FLOAT_VAL=" << val << "; " << m_CommandStr;
        std::string s = ss.str();

        if(m_bMultithreaded)
        {
            std::thread commandRunner(processRun, s.c_str(), true, nullptr, nullptr);
            commandRunner.detach();
        }
        else
        {
            processRun(s.c_str(), true);
        }
    }

    OutputFloat* CommandExecuterFloat::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        char* command;
        bool multithread;

        if(json_scanf(json, len, "{command: %Q}", &command) != 1)
        {
            return nullptr;
        }

        if(json_scanf(json, len, "{multithread: %B}", &multithread) != 1)
        {
            multithread = false;
        }

        return new CommandExecuterFloat(command, multithread);
    }

    const char* CommandExecuterFloat::Type = "CommandExecuterFloat";
    ObjectConfig<OutputFloat> CommandExecuterFloat::OutputFloatConf(Type, createFromJson);
}
