#include "CommandExecuter.h"

#include "OmniThing.h"
#include "Logger.h"

#include <string.h>
#include "frozen.h"
#include <thread>

#include <stdio.h>
#include <stdlib.h>

namespace omni
{
//private
    void CommandExecuter::executeCommand()
    {
        LOG << F("Executing: ") << m_CommandStr << Logger::endl;
        if(system(nullptr))
        {
            LOG << F("ERROR: This platform does not support the system() function\n");
        }

        system(m_CommandStr);
    }
    
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
            std::thread commandRunner(&CommandExecuter::executeCommand, this);
            commandRunner.detach();
        }
        else
        {
            executeCommand();
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
