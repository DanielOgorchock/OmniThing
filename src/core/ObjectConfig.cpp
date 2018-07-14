#include "ObjectConfig.h"
#include "OmniThing.h"

#include "Logger.h"
#include "Device.h"
#include "InputBool.h"
#include "InputFloat.h"
#include "InputUInt.h"
#include "OutputVoid.h"
#include "OutputBool.h"
#include "OutputFloat.h"
#include "OutputString.h"

namespace omni
{
    // Overloads of registerConfig
    void registerConfig(ObjectConfig<Device>* conf)
    {
        OmniThing::getInstance().addDeviceConfig(conf);
    }

    void registerConfig(ObjectConfig<InputBool>* conf)
    {
        OmniThing::getInstance().addInputBoolConfig(conf);
    }

    void registerConfig(ObjectConfig<InputFloat>* conf)
    {
        OmniThing::getInstance().addInputFloatConfig(conf);
    }

    void registerConfig(ObjectConfig<InputUInt>* conf)
    {
        OmniThing::getInstance().addInputUIntConfig(conf);
    }

    void registerConfig(ObjectConfig<OutputVoid>* conf)
    {
        OmniThing::getInstance().addOutputVoidConfig(conf);
    }

    void registerConfig(ObjectConfig<OutputBool>* conf)
    {
        OmniThing::getInstance().addOutputBoolConfig(conf);
    }

    void registerConfig(ObjectConfig<OutputFloat>* conf)
    {
        OmniThing::getInstance().addOutputFloatConfig(conf);
    }

    void registerConfig(ObjectConfig<OutputString>* conf)
    {
        OmniThing::getInstance().addOutputStringConfig(conf);
    }

    void registerConfig(ObjectConfig<NetworkReceiver>* conf)
    {
        OmniThing::getInstance().addNetworkReceiverConfig(conf);
    }

    void registerConfig(ObjectConfig<NetworkSender>* conf)
    {
        OmniThing::getInstance().addNetworkSenderConfig(conf);
    }

    void registerConfig(ObjectConfig<CompositePeripheral>* conf)
    {
        OmniThing::getInstance().addCompositePeriphConfig(conf);
    }
}
