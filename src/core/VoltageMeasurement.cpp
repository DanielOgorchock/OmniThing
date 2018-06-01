#include "VoltageMeasurement.h"

#include "OmniThing.h"
#include "Logger.h"
#include <string.h>
#include "frozen.h"
#include "InputFloat.h"

namespace omni
{
//private
    void VoltageMeasurement::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{uid: %u, type: \"%s\", value: \"%f\"}", getUid(), getType(), getVoltage());

        LOG << buffer << Logger::endl;
        OmniThing::getInstance().sendJson(buffer);
    }

    void VoltageMeasurement::read()
    {
        m_fVal = m_Input.readFloat();
    }

//protected
//public
    VoltageMeasurement::VoltageMeasurement(InputFloat& input):
        Device(false),
        m_Input(input),
        m_fVal(0.f)
    {
        read();    
    }

    VoltageMeasurement::~VoltageMeasurement()
    {

    }

    void VoltageMeasurement::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            read();
            sendJsonPacket();
        }
    }

    void VoltageMeasurement::run()
    {

    }

    void VoltageMeasurement::init()
    {
        sendJsonPacket();
    }

    Device* VoltageMeasurement::createFromJson(const char* json)
    {
        unsigned int inputIndex;

        unsigned int len = strlen(json);

        if(json_scanf(json, len, "{inputIndex: %u}", &inputIndex) != 1)
        {
            return nullptr;
        }

        auto ifs = OmniThing::getInstance().getInputFloats();
        if(inputIndex >= ifs.getCount())
        {
            LOG << F("ERROR: inputIndex is too large\n");
            return nullptr;
        }

        return new VoltageMeasurement(*(ifs[inputIndex]));

    }

    // commands
    const char* VoltageMeasurement::Cmd_Poll = "poll";

    const char* VoltageMeasurement::Type = "VoltageMeasurement";
    ObjectConfig<Device> VoltageMeasurement::DevConf(Type, createFromJson);
}
