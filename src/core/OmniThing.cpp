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
        m_pNetworkReceiver(nullptr),
        m_pNetworkSender(nullptr),
        m_pDefaultLogger(new LoggerStdout()),
        m_pLogger(m_pDefaultLogger)
    {

    }

    void OmniThing::runScheduler()
    {
        //run all run-enabled devices
        for(unsigned short i = 0; i < m_Devices.getCount(); ++i)
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
        for(unsigned short i = 0; i < m_Triggers.getCount(); ++i)
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
        for(unsigned short i = 0; i < m_Devices.getCount(); ++i)
        {
            m_Devices[i]->init();
        }

    }

    void OmniThing::initScheduler()
    {
        LOG << F("Initializing scheduler...\n");

        unsigned long long time = getMillis();
        for(unsigned short i = 0; i < m_Triggers.getCount(); ++i)
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
        for(unsigned int i = 0; i < m_Devices.getCount(); ++i)
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
        // Provide some debug output
        LOG << F("\nDevice Configurations:\n");
        for(unsigned int i = 0; i < m_DeviceConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_DeviceConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nInputBool Configurations:\n");
        for(unsigned int i = 0; i < m_InputBoolConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_InputBoolConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nInputFloat Configurations:\n");
        for(unsigned int i = 0; i < m_InputFloatConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_InputFloatConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nInputUInt Configurations:\n");
        for(unsigned int i = 0; i < m_InputUIntConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_InputUIntConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nOutputVoid Configurations:\n");
        for(unsigned int i = 0; i < m_OutputVoidConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_OutputVoidConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nOutputBool Configurations:\n");
        for(unsigned int i = 0; i < m_OutputBoolConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_OutputBoolConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nOutputFloat Configurations:\n");
        for(unsigned int i = 0; i < m_OutputFloatConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_OutputFloatConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nOutputString Configurations:\n");
        for(unsigned int i = 0; i < m_OutputStringConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_OutputStringConfigs[i]->getType() << F("\n");
        }
        LOG << Logger::endl;


        // Initialize Network Sender/Receiver
        if(m_pNetworkSender)
            m_pNetworkSender->init();
        else
            LOG << F("WARNING: no NetworkSender configured\n");

        if(m_pNetworkReceiver)
            m_pNetworkReceiver->init();
        else
            LOG << F("WARNING: no NetworkReceiver configured\n");

        initDevices(); 
        initScheduler();
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

    bool OmniThing::addDevice(Device* dev)
    {
        if(m_Devices.addElement(dev))
            return true;
        else
        {
            LOG << F("Failed to add device uid=") << dev->getUid() << F(" type=") << dev->getType() << F("\n");
            return false;
        }
    }
    bool OmniThing::addInputBool(InputBool* e)
    {
        if(m_InputBools.addElement(e))
            return true;
        else
        {
            LOG << F("Failed to add InputBool (array full)\n");
            return false;
        }
    }

    bool OmniThing::addInputFloat(InputFloat* e)
    {
        if(m_InputFloats.addElement(e))
            return true;
        else
        {
            LOG << F("Failed to add InputFloat (array full)\n");
            return false;
        }
    }

    bool OmniThing::addInputUInt(InputUInt* e)
    {
        if(m_InputUInts.addElement(e))
            return true;
        else
        {
            LOG << F("Failed to add InputUInt (array full)\n");
            return false;
        }
    }

    bool OmniThing::addOutputVoid(OutputVoid* e)
    {
        if(m_OutputVoids.addElement(e))
            return true;
        else
        {
            LOG << F("Failed to add OutputVoid (array full)\n");
            return false;
        }
    }

    bool OmniThing::addOutputBool(OutputBool* e)
    {
        if(m_OutputBools.addElement(e))
            return true;
        else
        {
            LOG << F("Failed to add OutputBool (array full)\n");
            return false;
        }
    }

    bool OmniThing::addOutputFloat(OutputFloat* e)
    {
        if(m_OutputFloats.addElement(e))
            return true;
        else
        {
            LOG << F("Failed to add OutputFloat (array full)\n");
            return false;
        }
    }

    bool OmniThing::addOutputString(OutputString* e)
    {
        if(m_OutputStrings.addElement(e))
            return true;
        else
        {
            LOG << F("Failed to add OutputString (array full)\n");
            return false;
        }
    }

    bool OmniThing::addTrigger(Trigger& t)
    {
        if(m_Triggers.addElement(t))
            return true;
        else
        {
            LOG << F("Failed to add Trigger (array full)\n");
            return false;
        }
    }

    bool OmniThing::addTrigger(Device* d, unsigned long interval, const char* cmd, const char* json, bool repeat)
    {
        Trigger tmp(d, interval, cmd, json, repeat);
        return addTrigger(tmp);
    }

    bool OmniThing::addDeviceConfig(ObjectConfig<Device>* dc)
    {
        if(m_DeviceConfigs.addElement(dc))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << dc->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::addInputBoolConfig(ObjectConfig<InputBool>* c)
    {
        if(m_InputBoolConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::addInputFloatConfig(ObjectConfig<InputFloat>* c)
    {
        if(m_InputFloatConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::addInputUIntConfig(ObjectConfig<InputUInt>* c)
    {
        if(m_InputUIntConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::addOutputVoidConfig(ObjectConfig<OutputVoid>* c)
    {
        if(m_OutputVoidConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::addOutputBoolConfig(ObjectConfig<OutputBool>* c)
    {
        if(m_OutputBoolConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::addOutputFloatConfig(ObjectConfig<OutputFloat>* c)
    {
        if(m_OutputFloatConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::addOutputStringConfig(ObjectConfig<OutputString>* c)
    {
        if(m_OutputStringConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::loadJsonConfig(const char* json)
    {
        return true;
    }
}
