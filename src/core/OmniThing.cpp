#include "OmniThing.h"
#include "OmniUtil.h"

#include "Device.h"
#include "ObjectConfig.h"
#include "NetworkReceiver.h"
#include "NetworkSender.h"
#include "Logger.h"
#include "frozen.h"
#include <string.h>

#include "LoggerStdout.h" //TODO: REMOVE THIS

namespace omni
{
//private
    OmniThing::OmniThing():
        m_nDeviceCount(0),
        m_nTriggerCount(0),
        m_nDeviceConfigCount(0),
        m_nInputBoolConfigCount(0),
        m_nInputFloatConfigCount(0),
        m_nInputUIntConfigCount(0),
        m_nOutputVoidConfigCount(0),
        m_nOutputBoolConfigCount(0),
        m_nOutputFloatConfigCount(0),
        m_nOutputStringConfigCount(0),
        m_pNetworkReceiver(nullptr),
        m_pNetworkSender(nullptr),
        m_pDefaultLogger(new LoggerStdout()),
        m_pLogger(m_pDefaultLogger)
    {

    }

    void OmniThing::runScheduler()
    {
        //run all run-enabled devices
        for(unsigned short i = 0; i < m_nDeviceCount; ++i)
        {
            if(m_Devices[i]->isRunning())
            {
                m_Devices[i]->run();
            }
        }

        //check for triggers
        //TODO: account for overflow in the for loop
        //TODO: account for non-repeating triggers
        unsigned long long time = getMillis();
        for(unsigned short i = 0; i < m_nTriggerCount; ++i)
        {
            Trigger& t = m_Triggers[i];
            
            if(time >= t.triggerTime)
            {
                t.triggerTime += t.interval;
                t.dev->recvJson(t.cmd, t.json);
            }
        }
    }

    void OmniThing::initDevices()
    {
        LOG << F("Initializing devices...\n");

        //run init on all devices
        for(unsigned short i = 0; i < m_nDeviceCount; ++i)
        {
            m_Devices[i]->init();
        }

    }

    void OmniThing::initScheduler()
    {
        LOG << F("Initializing scheduler...\n");

        unsigned long long time = getMillis();
        for(unsigned short i = 0; i < m_nTriggerCount; ++i)
        {
            Trigger& t = m_Triggers[i];
            
            t.triggerTime = time + t.interval;
        }

    }

    void OmniThing::parseJson(const char* json)
    {
        unsigned int uid;
        char cmd[24]; 
        
        int res = json_scanf(json, strlen(json), F("{uid:%u, cmd:%s}"), &uid, cmd);
        if(res != 2)
        {
            LOG << F("problem scanning err=") << res << F(" : ") << json << Logger::endl;
        }

        Device* d = findDevice(uid);
        if(!d)
        {
            LOG << F("No device found with uid=") << uid << Logger::endl;
        }
        else
        {
            d->recvJson(cmd, json);
        }
    }

    Device* OmniThing::findDevice(unsigned int uid)
    {
        for(unsigned int i = 0; i < m_nDeviceCount; ++ i)
        {
            Device* d = m_Devices[i];
            if(d->getUid() == uid)
                return d;
        }

        return nullptr;
    }

//public
    OmniThing& OmniThing::getInstance()
    {
        static OmniThing instance;

        return instance;
    }

    void OmniThing::init()
    {
        initDevices(); 
        initScheduler();

        if(m_pNetworkSender)
            m_pNetworkSender->init();
        else
            LOG << F("WARNING: no NetworkSender configured\n");

        if(m_pNetworkReceiver)
            m_pNetworkReceiver->init();
        else
            LOG << F("WARNING: no NetworkReceiver configured\n");

        // Provide some debug output
        LOG << F("Device Configurations:\n");
        for(unsigned int i = 0; i < m_nDeviceConfigCount; ++i)
        {
            LOG << F("\t") << m_DeviceConfigs[i]->getType() << F("\n");
        }

        LOG << F("InputBool Configurations:\n");
        for(unsigned int i = 0; i < m_nInputBoolConfigCount; ++i)
        {
            LOG << F("\t") << m_InputBoolConfigs[i]->getType() << F("\n");
        }

        LOG << F("InputFloat Configurations:\n");
        for(unsigned int i = 0; i < m_nInputFloatConfigCount; ++i)
        {
            LOG << F("\t") << m_InputFloatConfigs[i]->getType() << F("\n");
        }

        LOG << F("InputUInt Configurations:\n");
        for(unsigned int i = 0; i < m_nInputUIntConfigCount; ++i)
        {
            LOG << F("\t") << m_InputUIntConfigs[i]->getType() << F("\n");
        }

        LOG << F("OutputVoid Configurations:\n");
        for(unsigned int i = 0; i < m_nOutputVoidConfigCount; ++i)
        {
            LOG << F("\t") << m_OutputVoidConfigs[i]->getType() << F("\n");
        }

        LOG << F("OutputBool Configurations:\n");
        for(unsigned int i = 0; i < m_nOutputBoolConfigCount; ++i)
        {
            LOG << F("\t") << m_OutputBoolConfigs[i]->getType() << F("\n");
        }

        LOG << F("OutputFloat Configurations:\n");
        for(unsigned int i = 0; i < m_nOutputFloatConfigCount; ++i)
        {
            LOG << F("\t") << m_OutputFloatConfigs[i]->getType() << F("\n");
        }

        LOG << F("OutputString Configurations:\n");
        for(unsigned int i = 0; i < m_nOutputStringConfigCount; ++i)
        {
            LOG << F("\t") << m_OutputStringConfigs[i]->getType() << F("\n");
        }


    }

    void OmniThing::run()
    {
        runScheduler();

        if(m_pNetworkSender)
        {
            m_pNetworkSender->run();
        }

        if(m_pNetworkReceiver)
        {
            m_pNetworkReceiver->run();

            const char* json_rcvd = m_pNetworkReceiver->getJsonString();
            if(json_rcvd)
            {
                LOG << F("Received json: ") << json_rcvd << Logger::endl;
                parseJson(json_rcvd);
            }
        }
    }

    void OmniThing::setNetworkReceiver(NetworkReceiver* nr)
    {
        m_pNetworkReceiver = nr;
    }

    void OmniThing::setNetworkSender(NetworkSender* ns)
    {
        m_pNetworkSender = ns;
    }

    void OmniThing::setLogger(Logger* l)
    {
        m_pLogger = l;
    }

    Logger& OmniThing::getLogger()
    {
        return *m_pLogger;
    }

    void OmniThing::sendJson(const char* json)
    {
        if(m_pNetworkSender)
        {
            m_pNetworkSender->sendJson(json);
        }
    } 

    //TODO: make all max sizes below configurable!
    bool OmniThing::addDevice(Device* dev)
    {
        if(m_nDeviceCount >= 20)
        {
            LOG << F("Device array full\n");
            return false;
        }
        m_Devices[m_nDeviceCount++] = dev;
        return true;
    }

    bool OmniThing::addTrigger(Trigger& t)
    {
        if(m_nTriggerCount >= 10)
        {
            LOG << F("Trigger array full\n");
            return false;
        }
        m_Triggers[m_nTriggerCount++] = t;
        return true;
    }

    bool OmniThing::addTrigger(Device* d, unsigned long interval, const char* cmd, const char* json, bool repeat)
    {
        Trigger tmp(d, interval, cmd, json, repeat);
        return addTrigger(tmp);
    }

    bool OmniThing::addDeviceConfig(ObjectConfig<Device>* dc)
    {
        if(m_nDeviceConfigCount >= 20)
        {
            LOG << F("Full array; could not add ") << dc->getType() << Logger::endl;
            return false;
        }
        m_DeviceConfigs[m_nDeviceConfigCount++] = dc;
        return true;
    }

    bool OmniThing::addInputBoolConfig(ObjectConfig<InputBool>* c)
    {
        if(m_nInputBoolConfigCount >= 10)
        {
            LOG << F("Full array; could not add ") << c->getType() << Logger::endl;
            return false;
        }
        m_InputBoolConfigs[m_nInputBoolConfigCount++]; 
        return true;
    }

    bool OmniThing::addInputFloatConfig(ObjectConfig<InputFloat>* c)
    {
        if(m_nInputFloatConfigCount >= 10)
        {
            LOG << F("Full array; could not add ") << c->getType() << Logger::endl;
            return false;
        }
        m_InputFloatConfigs[m_nInputFloatConfigCount++]; 
        return true;
    }

    bool OmniThing::addInputUIntConfig(ObjectConfig<InputUInt>* c)
    {
        if(m_nInputUIntConfigCount >= 10)
        {
            LOG << F("Full array; could not add ") << c->getType() << Logger::endl;
            return false;
        }
        m_InputUIntConfigs[m_nInputUIntConfigCount++]; 
        return true;
    }

    bool OmniThing::addOutputVoidConfig(ObjectConfig<OutputVoid>* c)
    {
        if(m_nOutputVoidConfigCount >= 10)
        {
            LOG << F("Full array; could not add ") << c->getType() << Logger::endl;
            return false;
        }
        m_OutputVoidConfigs[m_nOutputVoidConfigCount++]; 
        return true;
    }

    bool OmniThing::addOutputBoolConfig(ObjectConfig<OutputBool>* c)
    {
        if(m_nOutputBoolConfigCount >= 10)
        {
            LOG << F("Full array; could not add ") << c->getType() << Logger::endl;
            return false;
        }
        m_OutputBoolConfigs[m_nOutputBoolConfigCount++]; 
        return true;
    }

    bool OmniThing::addOutputFloatConfig(ObjectConfig<OutputFloat>* c)
    {
        if(m_nOutputFloatConfigCount >= 10)
        {
            LOG << F("Full array; could not add ") << c->getType() << Logger::endl;
            return false;
        }
        m_OutputFloatConfigs[m_nOutputFloatConfigCount++]; 
        return true;
    }

    bool OmniThing::addOutputStringConfig(ObjectConfig<OutputString>* c)
    {
        if(m_nOutputStringConfigCount >= 10)
        {
            LOG << F("Full array; could not add ") << c->getType() << Logger::endl;
            return false;
        }
        m_OutputStringConfigs[m_nOutputStringConfigCount++]; 
        return true;
    }

}
