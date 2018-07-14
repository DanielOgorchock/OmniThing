#include "RelativeHumidityMeasurement.h"

#include "OmniThing.h"
#include "Logger.h"
#include <string.h>
#include "frozen.h"
#include "InputFloat.h"

namespace omni
{
//private
    void RelativeHumidityMeasurement::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{uid: %u, type: \"%s\", humidity: \"%f\"}", getUid(), getType(), getHumidity());

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void RelativeHumidityMeasurement::read()
    {
        m_fVal = m_Input.readFloat();
    }

//protected
//public
    RelativeHumidityMeasurement::RelativeHumidityMeasurement(InputFloat& input):
        Device(false),
        m_Input(input),
        m_fVal(0.f)
    {
        read();
    }

    RelativeHumidityMeasurement::~RelativeHumidityMeasurement()
    {

    }

    void RelativeHumidityMeasurement::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            read();
            sendJsonPacket();
        }
    }

    void RelativeHumidityMeasurement::run()
    {

    }

    void RelativeHumidityMeasurement::init()
    {
        sendJsonPacket();
    }

    Device* RelativeHumidityMeasurement::createFromJson(const char* json)
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

        auto d = new RelativeHumidityMeasurement(*input);
        d->parseMisc(json);
        return d;
    }

    // commands
    const char* RelativeHumidityMeasurement::Cmd_Poll = "poll";

    const char* RelativeHumidityMeasurement::Type = "RelativeHumidityMeasurement";
    ObjectConfig<Device> RelativeHumidityMeasurement::DevConf(Type, createFromJson);
}


