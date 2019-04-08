#include "CommandExecuter.h"

#include "OmniThing.h"
#include "Logger.h"
#include "OmniOSUtil.h"

#include <string.h>
#include "frozen.h"
#include <thread>

namespace omni
{
//private

//protected
//public
    CommandExecuter::CommandExecuter(const char* command, bool multithread):
        m_CommandStr(command),
        m_bMultithreaded(multithread)
    {

    }

    CommandExecuter::~CommandExecuter()
    {

    }

    void CommandExecuter::writeVoid()
    {
        if(m_bMultithreaded)
        {
            std::thread commandRunner(processRun, m_CommandStr, true, nullptr, nullptr);
            commandRunner.detach();
        }
        else
        {
            processRun(m_CommandStr, true);
        }
    }

    OutputVoid* CommandExecuter::createFromJson(const char* json)
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

        return new CommandExecuter(command, multithread);
    }

    const char* CommandExecuter::Type = "CommandExecuter";
    ObjectConfig<OutputVoid> CommandExecuter::OutputVoidConf(Type, createFromJson);
}
