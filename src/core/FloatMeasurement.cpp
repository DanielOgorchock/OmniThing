#include "FloatMeasurement.h"

#include "OmniThing.h"
#include "Logger.h"
#include <string.h>
#include "frozen.h"
#include "InputFloat.h"

namespace omni
{
//private
//protected
    void FloatMeasurement::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        float val = getFloat();
        if(val > 1000 || val < -1000)
        {
            LOG << F("ERROR: float value too many digits\n");
            val = -1;
        }
        json_printf(&out, "{name: \"%s\", type: \"%s\", \"%s\": \"%f\"}", getName(), getType(), m_Attribute, val);

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void FloatMeasurement::read()
    {
        m_fVal = m_Input.readFloat();
    }

//public
    FloatMeasurement::FloatMeasurement(InputFloat& input, const char* attribute):
        Device(false),
        m_Input(input),
        m_fVal(0.f),
        m_Attribute(attribute)
    {
    }

    FloatMeasurement::~FloatMeasurement()
    {

    }

    void FloatMeasurement::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getName() << Logger::endl;
            read();
            sendJsonPacket();
        }
    }

    void FloatMeasurement::run()
    {

    }

    void FloatMeasurement::init()
    {
        sendJsonPacket();
    }

    FloatMeasurement* FloatMeasurement::createFromJson(const char* json, const char* attribute)
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

        auto d = new FloatMeasurement(*input, attribute);
        return d;
    }

    // commands
    const char* FloatMeasurement::Cmd_Poll = "poll";

    const char* FloatMeasurement::Type = "FloatMeasurement";
}

