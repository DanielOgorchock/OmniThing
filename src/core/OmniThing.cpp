#include "OmniThing.h"
#include "OmniUtil.h"

#include "Device.h"
#include "CompositePeripheral.h"
#include "ObjectConfig.h"
#include "NetworkReceiver.h"
#include "NetworkSender.h"
#include "Logger.h"
#include "frozen.h"
#include "Triggerable.h"
#include <string.h>

namespace omni
{
//private
    OmniThing::OmniThing():
        m_pNetworkReceiver(nullptr),
        m_pNetworkSender(nullptr),
        m_pDefaultLogger(&Logger::StubbedLogger),
        m_pLogger(m_pDefaultLogger)
    {
        initOutgoingJson();
    }

    void OmniThing::printContainersDebug()
    {
        LOG << F("\n############# CONFIGURATIONS ##############\n");
        LOG << F("Device Configurations:\n");
        for(unsigned int i = 0; i < m_DeviceConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_DeviceConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nComposite Peripheral Configurations:\n");
        for(unsigned int i = 0; i < m_CompositePeriphConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_CompositePeriphConfigs[i]->getType() << F("\n");
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

        LOG << F("\nNetworkReceiver Configurations:\n");
        for(unsigned int i = 0; i < m_NetworkReceiverConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_NetworkReceiverConfigs[i]->getType() << F("\n");
        }

        LOG << F("\nNetworkSender Configurations:\n");
        for(unsigned int i = 0; i < m_NetworkSenderConfigs.getCount(); ++i)
        {
            LOG << F("\t") << m_NetworkSenderConfigs[i]->getType() << F("\n");
        }

        LOG << F("\n############# DEVICES ##############\n");
        LOG << F("Devices:\n");
        for(unsigned int i = 0; i < m_Devices.getCount(); ++i)
        {
            auto d = m_Devices[i];
            LOG << F("\tname=") << d->getName() << F(" type=") << d->getType()
                << F(" constantRun=") << d->isRunning() << F("\n");
        }

        LOG << F("\n############# TRIGGERS ##############\n");
        LOG << F("Triggers:\n");
        for(unsigned int i = 0; i < m_Triggers.getCount(); ++i)
        {
            auto& t = m_Triggers[i];
            LOG << F("\tinterval=") << t.interval << F(" offset=") << t.offset << Logger::endl;
        }

        LOG << F("\n############# SUBSCRIPTIONS ##############\n");
        LOG << F("Subscriptions:\n");
        for(unsigned int i = 0; i < m_Subscriptions.getCount(); ++i)
        {
            auto& t = m_Subscriptions[i];
            LOG << F("\tsource=")  << t.event.src
                << F(" event=")    << t.event.event
                << F(" sub_name=") << t.subscriber->getName()
                << F(" cmd=")      << t.cmd
                << Logger::endl;
        }
        LOG << Logger::endl;
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
        unsigned long long time = getMillis();
        for(unsigned short i = 0; i < m_Triggers.getCount(); ++i)
        {
            Trigger& t = m_Triggers[i];

            if(t.offset > 0)
            {
                if(time < t.offset + t.triggerTime)
                    continue;
                else
                {
                    t.triggerTime = time;
                    t.offset = 0;
                }
            }

            if(time - t.triggerTime >= t.interval)
            {
                t.triggerTime += t.interval;
                t.target->trigger(t.arg);
                LOG << Logger::endl; //splits up debug output from triggers nicely

                //remove this trigger if non-repeating
                if(!t.repeating)
                {
                    m_Triggers.removeElement(i);
                    --i;
                }
                else if(time - t.triggerTime >= t.interval) // prevent windup
                {
                    unsigned long difference = time - t.triggerTime;
                    unsigned long intervalsBehind = difference / t.interval;
                    t.triggerTime += intervalsBehind * t.interval;
                }
            }
        }
    }

    void OmniThing::checkEvents()
    {
        for(unsigned int i = 0; i < m_Events.getCount(); ++i)
        {
            auto& e = m_Events[i];
            for(unsigned int j = 0; j < m_Subscriptions.getCount(); ++j)
            {
                auto& s = m_Subscriptions[j];
                if(s.event == e)
                {
                    LOG << F("event source=") << e.src << F(" type=") << e.event;
                    LOG << F(" triggered subscription:");
                    LOG << F(" sub_type=") << s.subscriber->getType() <<
                           F(" sub_name=") << s.subscriber->getName() << F(" cmd=") << s.cmd << Logger::endl;

                    s.subscriber->trigger(s.cmd);
                }
            }
        }

        // clear events
        m_Events.resetCount();
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

            t.triggerTime = time;
        }

    }

    void OmniThing::parseJson(const char* json)
    {
        char name[OMNI_MAX_NAME_LENGTH + 1];
        name[0] = '\0';
        char cmd[24];

        int res = json_scanf(json, strlen(json), "{name:%" S(OMNI_MAX_NAME_LENGTH) "s, cmd:%s}", name, cmd);
        if(res != 2)
        {
            LOG << F("problem scanning err=") << res << F(" : ") << json << Logger::endl;
            return;
        }
        name[OMNI_MAX_NAME_LENGTH] = 0; //just in case

        Device* d = findDevice(name);
        if(!d)
        {
            LOG << F("No device found with name=") << name << Logger::endl;
        }
        else
        {
            d->recvJson(cmd, json);
        }
    }

    Device* OmniThing::findDevice(const char* name)
    {
        for(unsigned int i = 0; i < m_Devices.getCount(); ++i)
        {
            Device* d = m_Devices[i];
            if(!strcmp(d->getName(), name))
                return d;
        }

        return nullptr;
    }

    void OmniThing::initOutgoingJson()
    {
        m_OutgoingJsonBuf[0] = 0;
        strcat(m_OutgoingJsonBuf, "{\"updates\": [");
    }

    void OmniThing::sendOutgoingJson()
    {
        m_OutgoingJsonBuf[strlen(m_OutgoingJsonBuf)-1] = 0;
        strcat(m_OutgoingJsonBuf, "]}");

        sendJsonNow(m_OutgoingJsonBuf);

        initOutgoingJson();
    }

    void OmniThing::addOutgoingJson(const char* json)
    {
        if(strlen(m_OutgoingJsonBuf) + strlen(json) + 3 >= OMNI_OUTGOING_JSON_BUF_SIZE)
        {
            LOG << F("Adding more json would overflow outgoing buffer; sending now...\n");
            sendOutgoingJson();
            if(strlen(m_OutgoingJsonBuf) + strlen(json) + 3 >= OMNI_OUTGOING_JSON_BUF_SIZE)
            {
                LOG << F("This json object alone is too big for buffer; sending on its own...\n");
                if(m_pNetworkSender)
                {
                   m_pNetworkSender->sendJson(json);
                }
                return;
            }
        }

        strcat(m_OutgoingJsonBuf, json);
        strcat(m_OutgoingJsonBuf, ",");
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
        printContainersDebug();

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
        m_Events.resetCount();
        initScheduler();
    }

    void OmniThing::run()
    {
        runScheduler();
        checkEvents();

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

        if(strlen(m_OutgoingJsonBuf) > strlen("{\"updates\": ["))
        {
            sendOutgoingJson();
        }

        if(m_pLogger)
        {
            m_pLogger->flush();
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
        addOutgoingJson(json);
    }

    void OmniThing::sendJsonNow(const char* json)
    {
        if(m_pNetworkSender)
            m_pNetworkSender->sendJson(json);
    }

    bool OmniThing::addDevice(Device* dev)
    {
        if(m_Devices.addElement(dev))
            return true;
        else
        {
            LOG << F("Failed to add device name=") << dev->getName() << F(" type=") << dev->getType() << F("\n");
            return false;
        }
    }

    bool OmniThing::addCompositePeriph(CompositePeripheral* e)
    {
        if(m_CompositePeriphs.addElement(e))
            return true;
        else
        {
            LOG << F("Failed to add CompositePeripheral (array full)\n");
            return false;
        }
    }

    bool OmniThing::addTrigger(Trigger& t)
    {
        if(m_Triggers.addElement(t))
        {
            m_Triggers[m_Triggers.getCount()-1].triggerTime = getMillis();
            return true;
        }
        else
        {
            LOG << F("Failed to add Trigger (array full)\n");
            return false;
        }
    }

    bool OmniThing::addTrigger(Triggerable* t, unsigned long interval, void* arg, bool repeat, unsigned long offset)
    {
        Trigger tmp(t, interval, arg, repeat, offset);
        return addTrigger(tmp);
    }

    bool OmniThing::addEvent(const char* src, const char* event)
    {
        if(m_Events.addElement(Event(src, event)))
            return true;
        else
        {
            LOG << F("Failed to add event (array full)\n");
            return false;
        }
    }

    bool OmniThing::addSubscription(const Subscription& sub)
    {
        if(m_Subscriptions.addElement(sub))
            return true;
        else
        {
            LOG << F("Failed to add subscription (array full)\n");
            return false;
        }
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

    bool OmniThing::addCompositePeriphConfig(ObjectConfig<CompositePeripheral>* c)
    {
        if(m_CompositePeriphConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
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

    bool OmniThing::addNetworkReceiverConfig(ObjectConfig<NetworkReceiver>* c)
    {
        if(m_NetworkReceiverConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    bool OmniThing::addNetworkSenderConfig(ObjectConfig<NetworkSender>* c)
    {
        if(m_NetworkSenderConfigs.addElement(c))
            return true;
        else
        {
            LOG << F("Failed to add Config (array full) type=") << c->getType() << Logger::endl;
            return false;
        }
    }

    CompositePeripheral* OmniThing::getCompositePeriph(const char* name)
    {
        CompositePeripheral* periph = nullptr;
        for(unsigned int i = 0; i < m_CompositePeriphs.getCount(); ++i)
        {
            if(!strcmp(name, m_CompositePeriphs[i]->getName()))
            {
                periph = m_CompositePeriphs[i];
            }
        }

        return periph;
    }

    InputBool* OmniThing::buildInputBool(json_token& t)
    {
        char tmp = t.ptr[t.len];
        const_cast<char*>(t.ptr)[t.len] = 0;
        auto& configs = m_InputBoolConfigs;

        char type[30];

        if(json_scanf(t.ptr, t.len, "{type: %s}", type) != 1)
        {
            LOG << F("ERROR: Failed to find type in: ") << t.ptr << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        int index = getConfigIndex(configs, type);
        if(index < 0)
        {
            LOG << F("ERROR: no config found of type=") << type << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        auto ret = configs[index]->createFromJson(t.ptr);

        const_cast<char*>(t.ptr)[t.len] = tmp;
        return ret;
    }

    InputFloat* OmniThing::buildInputFloat(json_token& t)
    {
        char tmp = t.ptr[t.len];
        const_cast<char*>(t.ptr)[t.len] = 0;
        auto& configs = m_InputFloatConfigs;

        char type[30];

        if(json_scanf(t.ptr, t.len, "{type: %s}", type) != 1)
        {
            LOG << F("ERROR: Failed to find type in: ") << t.ptr << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        int index = getConfigIndex(configs, type);
        if(index < 0)
        {
            LOG << F("ERROR: no config found of type=") << type << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        auto ret = configs[index]->createFromJson(t.ptr);

        const_cast<char*>(t.ptr)[t.len] = tmp;
        return ret;
    }

    InputUInt* OmniThing::buildInputUInt(json_token& t)
    {
        char tmp = t.ptr[t.len];
        const_cast<char*>(t.ptr)[t.len] = 0;
        auto& configs = m_InputUIntConfigs;

        char type[30];

        if(json_scanf(t.ptr, t.len, "{type: %s}", type) != 1)
        {
            LOG << F("ERROR: Failed to find type in: ") << t.ptr << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        int index = getConfigIndex(configs, type);
        if(index < 0)
        {
            LOG << F("ERROR: no config found of type=") << type << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        auto ret = configs[index]->createFromJson(t.ptr);

        const_cast<char*>(t.ptr)[t.len] = tmp;
        return ret;
    }

    OutputVoid* OmniThing::buildOutputVoid(json_token& t)
    {
        char tmp = t.ptr[t.len];
        const_cast<char*>(t.ptr)[t.len] = 0;
        auto& configs = m_OutputVoidConfigs;

        char type[30];

        if(json_scanf(t.ptr, t.len, "{type: %s}", type) != 1)
        {
            LOG << F("ERROR: Failed to find type in: ") << t.ptr << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        int index = getConfigIndex(configs, type);
        if(index < 0)
        {
            LOG << F("ERROR: no config found of type=") << type << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        auto ret = configs[index]->createFromJson(t.ptr);

        const_cast<char*>(t.ptr)[t.len] = tmp;
        return ret;
    }

    OutputBool* OmniThing::buildOutputBool(json_token& t)
    {
        char tmp = t.ptr[t.len];
        const_cast<char*>(t.ptr)[t.len] = 0;
        auto& configs = m_OutputBoolConfigs;

        char type[30];

        if(json_scanf(t.ptr, t.len, "{type: %s}", type) != 1)
        {
            LOG << F("ERROR: Failed to find type in: ") << t.ptr << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        int index = getConfigIndex(configs, type);
        if(index < 0)
        {
            LOG << F("ERROR: no config found of type=") << type << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        auto ret = configs[index]->createFromJson(t.ptr);

        const_cast<char*>(t.ptr)[t.len] = tmp;
        return ret;
    }

    OutputFloat* OmniThing::buildOutputFloat(json_token& t)
    {
        char tmp = t.ptr[t.len];
        const_cast<char*>(t.ptr)[t.len] = 0;
        auto& configs = m_OutputFloatConfigs;

        char type[30];

        if(json_scanf(t.ptr, t.len, "{type: %s}", type) != 1)
        {
            LOG << F("ERROR: Failed to find type in: ") << t.ptr << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        int index = getConfigIndex(configs, type);
        if(index < 0)
        {
            LOG << F("ERROR: no config found of type=") << type << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        auto ret = configs[index]->createFromJson(t.ptr);

        const_cast<char*>(t.ptr)[t.len] = tmp;
        return ret;
    }

    OutputString* OmniThing::buildOutputString(json_token& t)
    {
        char tmp = t.ptr[t.len];
        const_cast<char*>(t.ptr)[t.len] = 0;
        auto& configs = m_OutputStringConfigs;

        char type[30];

        if(json_scanf(t.ptr, t.len, "{type: %s}", type) != 1)
        {
            LOG << F("ERROR: Failed to find type in: ") << t.ptr << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        int index = getConfigIndex(configs, type);
        if(index < 0)
        {
            LOG << F("ERROR: no config found of type=") << type << Logger::endl;
            const_cast<char*>(t.ptr)[t.len] = tmp;
            return nullptr;
        }

        auto ret = configs[index]->createFromJson(t.ptr);

        const_cast<char*>(t.ptr)[t.len] = tmp;
        return ret;
    }


    // {
    //      "NetworkReceiver":  {"type": string, ... },
    //      "NetworkSender":    {"type": string, ... },
    //      "CompositePeriphs": [ {"type": string, ... } , ... ],
    //      "Devices":          [ {"type": string, ... } , ... ],
    // }
    //
    bool OmniThing::loadJsonConfig(char* json)
    {
        LOG << F("Parsing Json Config:\n") << json << Logger::endl;

        struct json_token t;
        unsigned int len = strlen(json);
        char buffer[100];
        buffer[0] = 0;

        // scan for NetworkReceiver
        if(json_scanf(json, len, "{NetworkReceiver: %T}", &t) == 1)
        {
            LOG << "Found NetworkReceiver entry\n";
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) == 1)
            {
                bool found = false;
                for(unsigned int i = 0; i < m_NetworkReceiverConfigs.getCount(); ++i)
                {
                    auto conf = m_NetworkReceiverConfigs[i];
                    if(!strcmp(buffer, conf->getType()))
                    {
                        found = true;
                        char tmpC = json[(t.ptr-json)+t.len];
                        json[(t.ptr-json)+t.len] = 0;

                        LOG << F("Found configuration\n");

                        auto obj = conf->createFromJson(t.ptr);
                        if(!obj)
                        {
                            LOG << F("ERROR: Failed to create from: \n") << t.ptr << Logger::endl;
                            return false;
                        }
                        else
                        {
                            setNetworkReceiver(obj);
                            LOG << F("Successfully created new ") << t.ptr << Logger::endl;
                        }
                        json[(t.ptr-json)+t.len] = tmpC;
                        break;
                    }
                }
                if(!found)
                {
                    LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                    return false;
                }

            }
            else
            {
                LOG << "NetworkReceiver requires a specified type\n";
            }
        }

        // scan for NetworkSender
        if(json_scanf(json, len, "{NetworkSender: %T}", &t) == 1)
        {
            LOG << "Found NetworkSender entry\n";
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) == 1)
            {
                bool found = false;
                for(unsigned int i = 0; i < m_NetworkSenderConfigs.getCount(); ++i)
                {
                    auto conf = m_NetworkSenderConfigs[i];
                    if(!strcmp(buffer, conf->getType()))
                    {
                        found = true;
                        char tmpC = json[(t.ptr-json)+t.len];
                        json[(t.ptr-json)+t.len] = 0;

                        LOG << F("Found configuration\n");

                        auto obj = conf->createFromJson(t.ptr);
                        if(!obj)
                        {
                            LOG << F("ERROR: Failed to create from: \n") << t.ptr << Logger::endl;
                            return false;
                        }
                        else
                        {
                            setNetworkSender(obj);
                            LOG << F("Successfully created new ") << t.ptr << Logger::endl;
                        }
                        json[(t.ptr-json)+t.len] = tmpC;
                        break;
                    }
                }
                if(!found)
                {
                    LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                    return false;
                }

            }
            else
            {
                LOG << "NetworkSender requires a specified type\n";
            }
        }

        // scan for CompositePeripherals
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".CompositePeriphs", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;

                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            }

            bool found = false;
            for(unsigned int i = 0; i < m_CompositePeriphConfigs.getCount(); ++i)
            {
                auto conf = m_CompositePeriphConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    char tmpC = json[(t.ptr-json)+t.len];
                    json[(t.ptr-json)+t.len] = 0;

                    auto obj = conf->createFromJson(t.ptr);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << t.ptr << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addCompositePeriph(obj);
                        LOG << F("Successfully created new ") << t.ptr << Logger::endl;
                    }
                    json[(t.ptr-json)+t.len] = tmpC;
                    break;
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }

        // scan for Devices
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".Devices", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;

                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            }

            bool found = false;
            for(unsigned int i = 0; i < m_DeviceConfigs.getCount(); ++i)
            {
                auto conf = m_DeviceConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    char tmpC = json[(t.ptr-json)+t.len];
                    json[(t.ptr-json)+t.len] = 0;

                    auto obj = conf->createFromJson(t.ptr);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << t.ptr << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addDevice(obj);
                        LOG << F("Successfully created new ") << t.ptr << Logger::endl;
                    }
                    json[(t.ptr-json)+t.len] = tmpC;
                    break;
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }
        LOG << Logger::endl;
        return true;
    }

    bool operator==(const Event& l, const Event&r)
    {
        if(!l.src || !r.src || !l.event || !r.event)
            return false;
        return (!strcmp(l.src, r.src)) && (!strcmp(l.event, r.event));
    }
}
