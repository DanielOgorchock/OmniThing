#include "ObjectConfig.h"
#include "OmniThing.h"

#include "Logger.h"
#include "Device.h"

namespace omni
{
    // Overloads of registerConfig
    void registerConfig(ObjectConfig<Device>* conf)
    {
        OmniThing::getInstance().addDeviceConfig(conf);
    }
}
