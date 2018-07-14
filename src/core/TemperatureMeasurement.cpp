#include "TemperatureMeasurement.h"

#include "OmniThing.h"
#include "Logger.h"
#include <string.h>
#include "frozen.h"
#include "InputFloat.h"

namespace omni
{
//private
    void TemperatureMeasurement::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{uid: %u, type: \"%s\", temperature: \"%f\"}", getUid(), getType(), getTemperature());

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void TemperatureMeasurement::read()
    {
        m_fVal = m_Input.readFloat();
    }

//protected
//public
    TemperatureMeasurement::TemperatureMeasurement(InputFloat& input):
        Device(false),
        m_Input(input),
        m_fVal(0.f)
    {
        read();
    }

    TemperatureMeasurement::~TemperatureMeasurement()
    {

    }

    void TemperatureMeasurement::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            read();
            sendJsonPacket();
        }
    }

    void TemperatureMeasurement::run()
    {

    }

    void TemperatureMeasurement::init()
    {
        sendJsonPacket();
    }

    Device* TemperatureMeasurement::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        json_token t;

        if(json_scanf(json, len, "{input: %T}", &t) != 1)
        {
            return nullptr;
        }

        auto input = OmniThing::getInstance().buildInputFloat(t);
        if(!input)
        {
            LOG << F("ERROR: Failed to create input\n");
            return nullptr;
        }

        auto d = new TemperatureMeasurement(*input);
        d->parseMisc(json);
        return d;
    }

    // commands
    const char* TemperatureMeasurement::Cmd_Poll = "poll";

    const char* TemperatureMeasurement::Type = "TemperatureMeasurement";
    ObjectConfig<Device> TemperatureMeasurement::DevConf(Type, createFromJson);
}

