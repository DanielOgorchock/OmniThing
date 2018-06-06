#include "TimedOutputBool.h"

#include "OmniThing.h"
#include "Logger.h"
#include <string.h>
#include "frozen.h"

namespace omni
{
//private
    TimedOutputBool* TimedOutputBool::createFromJson(const char* json)
    {
        auto& omnithing = OmniThing::getInstance();
        unsigned int len = strlen(json);

        json_token t;
        unsigned long duration;

        if(json_scanf(json, len, "{output: %T, duration: %lu}", &t, &duration) != 2)
        {
            return nullptr;
        }

        auto output = omnithing.buildOutputBool(t);
        if(!output)
        {
            LOG << F("ERROR: Failed to build output\n");
            return nullptr;
        }

        return new TimedOutputBool(*output, duration);
    }

    void TimedOutputBool::write(bool b)
    {
        m_bValue = b;
        m_rOutput.writeBool(b);
    }

    void TimedOutputBool::startTimedOutput()
    {
        if(m_bTriggerPending)
        {
            LOG << F("TimedOutputBool timer already pending; ignoring request\n");
            return;
        }

        m_bTriggerPending = true;
        write(true);
        OmniThing::getInstance().addTrigger(this, m_nDuration, nullptr, false);
    }

//protected
//public
    TimedOutputBool::TimedOutputBool(OutputBool& output, unsigned long duration):
        m_rOutput(output),
        m_nDuration(duration),
        m_bValue(false),
        m_bTriggerPending(false)
    {
        write(false);
    }

    TimedOutputBool::~TimedOutputBool()
    {

    }

    void TimedOutputBool::writeBool(bool b)
    {
        startTimedOutput();    
    }

    void TimedOutputBool::writeVoid()
    {
        startTimedOutput(); 
    }

    void TimedOutputBool::trigger(void* arg)
    {
        m_bTriggerPending = false;
        write(false);
    }

    OutputBool* TimedOutputBool::createBoolFromJson(const char* json)
    {
        return createFromJson(json);
    }
    
    OutputVoid* TimedOutputBool::createVoidFromJson(const char* json)
    {
        return createFromJson(json);
    }
    
    
    const char* TimedOutputBool::Type = "TimedOutputBool";
    ObjectConfig<OutputBool> TimedOutputBool::OutputBoolConf(Type, createBoolFromJson);
    ObjectConfig<OutputVoid> TimedOutputBool::OutputVoidConf(Type, createVoidFromJson);
}




