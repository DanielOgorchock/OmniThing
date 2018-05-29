#include "DeviceConfig.h"
#include "OmniThing.h"

#include "Logger.h"
#include "Device.h"

namespace omni
{
//private
//protected
//public
    DeviceConfig::DeviceConfig(const char* type, Device* (*createFromJson) (const char*)):
        m_Type(type),
        m_CreateFromJson(createFromJson)
    {
        OmniThing::getInstance().addDeviceConfig(this);
    }

    DeviceConfig::~DeviceConfig()
    {

    }

    Device* DeviceConfig::createFromJson(const char* json)
    {
        return m_CreateFromJson(json);
    }
}
