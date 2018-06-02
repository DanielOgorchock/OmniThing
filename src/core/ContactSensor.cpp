#include "ContactSensor.h"
#include "InputBool.h"

#include <string.h>
#include "frozen.h"
#include "OmniThing.h"

#include "Logger.h"

namespace omni
{
//private
    void ContactSensor::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{uid: %u, type: \"%s\", value: \"%s\"}", getUid(), getType(), (read()?"closed":"open"));

        LOG << buffer << Logger::endl;

        OmniThing::getInstance().sendJson(buffer);
    }

//protected
//public
    ContactSensor::ContactSensor(InputBool& input, bool invert, bool constantPoll):
        Device(constantPoll),
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
            LOG << F("Poll triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            bool val = read(); 
            if(val != m_bLastVal)
            {
                emit(Event_Changed);

                if(val)
                    emit(Event_Open);
                else
                    emit(Event_Closed);
            }
            m_bLastVal = val; 
            sendJsonPacket();
        }

    }

    void ContactSensor::run()
    {
        bool val = read(); 
        if(val != m_bLastVal)
        {
            emit(Event_Changed);

            if(val)
                emit(Event_Open);
            else
                emit(Event_Closed);

            sendJsonPacket();
        }
        m_bLastVal = val; 
    }

    void ContactSensor::init()
    {
        m_bLastVal = read(); 
        sendJsonPacket();
    }

    bool ContactSensor::read()
    {
        bool val = m_rInput.readBool();

        val = isInverted() ? !val : val;
        return val;
    }

    Device* ContactSensor::createFromJson(const char* json)
    {
        unsigned int inputIndex;
        bool invert;
        bool constantPoll;

        unsigned int len = strlen(json);

        if(json_scanf(json, len, "{inputIndex: %u, invert: %B, constantPoll: %B}", &inputIndex, &invert, &constantPoll) != 3)
        {
            return nullptr;
        }

        auto& ibs = OmniThing::getInstance().getInputBools();
        if(inputIndex >= ibs.getCount())
        {
            LOG << F("ContactSensor: InputBool index out of range\n");
            return nullptr;
        }

        return new ContactSensor(*(ibs[inputIndex]), invert, constantPoll); 
    }


    //commands
    const char* ContactSensor::Cmd_Poll = "poll";

    //events
    const char* ContactSensor::Event_Open       = "open";
    const char* ContactSensor::Event_Closed     = "closed";
    const char* ContactSensor::Event_Changed    = "changed";

    const char* ContactSensor::Type     = "ContactSensor";
    ObjectConfig<Device> ContactSensor::DevConf(Type, createFromJson); 
}
