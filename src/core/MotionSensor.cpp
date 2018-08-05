#include "MotionSensor.h"
#include "InputBool.h"

#include <string.h>
#include "frozen.h"
#include "OmniThing.h"

#include "Logger.h"

namespace omni
{
//private
    void MotionSensor::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{name: \"%s\", type: \"%s\", motion: \"%s\"}", getName(), getType(), (read()?"inactive":"active"));

        LOG << buffer << Logger::endl;

        OmniThing::getInstance().sendJson(buffer);
    }

//protected
//public
    MotionSensor::MotionSensor(InputBool& input, bool invert, bool constantPoll):
        Device(constantPoll),
        m_rInput(input),
        m_bInvert(invert)
    {

    }

    MotionSensor::~MotionSensor()
    {

    }

    void MotionSensor::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            bool val = read();
            if(val != m_bLastVal)
            {
                emit(Event_Changed);

                if(val)
                    emit(Event_Active);
                else
                    emit(Event_Inactive);
            }
            m_bLastVal = val;
            sendJsonPacket();
        }

    }

    void MotionSensor::run()
    {
        bool val = read();
        if(val != m_bLastVal)
        {
            emit(Event_Changed);

            if(val)
                emit(Event_Active);
            else
                emit(Event_Inactive);

            sendJsonPacket();
        }
        m_bLastVal = val;
    }

    void MotionSensor::init()
    {
        m_bLastVal = read();
        sendJsonPacket();
    }

    bool MotionSensor::read()
    {
        bool val = m_rInput.readBool();

        val = isInverted() ? !val : val;
        return val;
    }

    Device* MotionSensor::createFromJson(const char* json)
    {
        bool invert;
        bool constantPoll;

        unsigned int len = strlen(json);
        json_token t;

        if(json_scanf(json, len, "{input: %T, invert: %B, constantPoll: %B}", &t, &invert, &constantPoll) != 3)
        {
            return nullptr;
        }

        auto input = OmniThing::getInstance().buildInputBool(t);
        if(!input)
        {
            LOG << F("ERROR: Failed to create input\n");
            return nullptr;
        }

        auto d = new MotionSensor(*input, invert, constantPoll);
        if(!d->parseMisc(json))
            return nullptr;
        return d;
    }


    //commands
    const char* MotionSensor::Cmd_Poll = "poll";

    //events
    const char* MotionSensor::Event_Active     = "active";
    const char* MotionSensor::Event_Inactive   = "inactive";
    const char* MotionSensor::Event_Changed    = "changed";

    const char* MotionSensor::Type = "MotionSensor";
    ObjectConfig<Device> MotionSensor::DevConf(Type, createFromJson);
}


