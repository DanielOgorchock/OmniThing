#include "OmniThing.h"
#include "OmniUtil.h"

#include "Device.h"
#include "ObjectConfig.h"
#include "NetworkReceiver.h"
#include "NetworkSender.h"
#include "Logger.h"
#include "frozen.h"
#include "Triggerable.h"
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

    void OmniThing::printContainersDebug()
    {
        LOG << F("\n############# CONFIGURATIONS ##############\n");
        LOG << F("Device Configurations:\n");
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

        LOG << F("\n############# DEVICES ##############\n");
        LOG << F("Devices:\n");
        for(unsigned int i = 0; i < m_Devices.getCount(); ++i)
        {
            auto d = m_Devices[i];
            LOG << F("\tuid=") << d->getUid() << F(" type=") << d->getType() << F("\n");
        }

        LOG << F("\n############# TRIGGERS ##############\n");
        LOG << F("Triggers:\n");
        for(unsigned int i = 0; i < m_Triggers.getCount(); ++i)
        {
            auto& t = m_Triggers[i];
            LOG << F("\tinterval=") << t.interval;
            LOG << F(" command=") << t.cmd << Logger::endl;
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
            
            if(time - t.triggerTime >= t.interval)
            {
                t.triggerTime += t.interval;
                t.target->trigger(t.cmd);
            }
        }
        //remove all non-repeating triggers
        for(unsigned short i = 0; i < m_Triggers.getCount(); ++i)
        {
            if(!m_Triggers[i].repeating)
            {
                m_Triggers.removeElement(i);
                --i;
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
            
            t.triggerTime = time;
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

    bool OmniThing::addTrigger(Device* d, unsigned long interval, const char* cmd, bool repeat)
    {
        Trigger tmp(d, interval, cmd, repeat);
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


    // {
    //      "InputBools":       [ {"type": string, ... } , ... ],
    //      "InputFloats":      [ {"type": string, ... } , ... ],
    //      "InputUInts":       [ {"type": string, ... } , ... ],
    //      "OutputVoids":      [ {"type": string, ... } , ... ],
    //      "OutputBools":      [ {"type": string, ... } , ... ],
    //      "OutputFloats":     [ {"type": string, ... } , ... ],
    //      "OutputStrings":    [ {"type": string, ... } , ... ],
    //      "Devices":          [ {"type": string, ... } , ... ],
    //      "Triggers":         [ {"deviceIndex": int, "interval": int, "command": string}, ... ]
    // }
    //      
    bool OmniThing::loadJsonConfig(const char* json)
    {
        LOG << F("Parsing Json Config:\n") << json << Logger::endl;

        struct json_token t;
        unsigned int len = strlen(json);
        char buffer[100];
        buffer[0] = 0;

        // scan for InputBools
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".InputBools", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;
 
                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            } 

            bool found = false;
            for(unsigned int i = 0; i < m_InputBoolConfigs.getCount(); ++i)
            {
                auto conf = m_InputBoolConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    strncpy(buffer, t.ptr, t.len);
                    buffer[t.len]=0;
 
                    auto obj = conf->createFromJson(buffer);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << buffer << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addInputBool(obj); 
                        strncpy(buffer, t.ptr, t.len);
                        buffer[t.len]=0;

                        buffer[t.len] = 0;
                        LOG << F("Successfully created new ") << buffer << Logger::endl;
                    }
                    break; 
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }

        // scan for InputFloats
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".InputFloats", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;
 
                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            } 

            bool found = false;
            for(unsigned int i = 0; i < m_InputFloatConfigs.getCount(); ++i)
            {
                auto conf = m_InputFloatConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    strncpy(buffer, t.ptr, t.len);
                    buffer[t.len]=0;
 
                    auto obj = conf->createFromJson(buffer);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << buffer << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addInputFloat(obj); 
                        strncpy(buffer, t.ptr, t.len);
                        buffer[t.len]=0;

                        LOG << F("Successfully created new ") << buffer << Logger::endl;
                    }
                    break; 
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }

        // scan for InputUInts
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".InputUInts", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;
 
                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            } 

            bool found = false;
            for(unsigned int i = 0; i < m_InputUIntConfigs.getCount(); ++i)
            {
                auto conf = m_InputUIntConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    strncpy(buffer, t.ptr, t.len);
                    buffer[t.len]=0;
 
                    auto obj = conf->createFromJson(buffer);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << buffer << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addInputUInt(obj); 
                        strncpy(buffer, t.ptr, t.len);
                        buffer[t.len]=0;

                        LOG << F("Successfully created new ") << buffer << Logger::endl;
                    }
                    break; 
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }

        // scan for OutputVoids
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".OutputVoids", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;
 
                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            } 

            bool found = false;
            for(unsigned int i = 0; i < m_OutputVoidConfigs.getCount(); ++i)
            {
                auto conf = m_OutputVoidConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    strncpy(buffer, t.ptr, t.len);
                    buffer[t.len]=0;
 
                    auto obj = conf->createFromJson(buffer);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << buffer << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addOutputVoid(obj); 
                        strncpy(buffer, t.ptr, t.len);
                        buffer[t.len]=0;

                        LOG << F("Successfully created new ") << buffer << Logger::endl;
                    }
                    break; 
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }

        // scan for OutputBools
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".OutputBools", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;
 
                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            } 

            bool found = false;
            for(unsigned int i = 0; i < m_OutputBoolConfigs.getCount(); ++i)
            {
                auto conf = m_OutputBoolConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    strncpy(buffer, t.ptr, t.len);
                    buffer[t.len]=0;
 
                    auto obj = conf->createFromJson(buffer);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << buffer << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addOutputBool(obj); 
                        strncpy(buffer, t.ptr, t.len);
                        buffer[t.len]=0;

                        LOG << F("Successfully created new ") << buffer << Logger::endl;
                    }
                    break; 
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }

        // scan for OutputFloats
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".OutputFloats", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;
 
                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            } 

            bool found = false;
            for(unsigned int i = 0; i < m_OutputFloatConfigs.getCount(); ++i)
            {
                auto conf = m_OutputFloatConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    strncpy(buffer, t.ptr, t.len);
                    buffer[t.len]=0;
 
                    auto obj = conf->createFromJson(buffer);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << buffer << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addOutputFloat(obj); 
                        strncpy(buffer, t.ptr, t.len);
                        buffer[t.len]=0;

                        LOG << F("Successfully created new ") << buffer << Logger::endl;
                    }
                    break; 
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }

        // scan for OutputStrings
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".OutputStrings", i, &t) > 0; ++i)
        {
            if(json_scanf(t.ptr, t.len, "{type: %s}", buffer) <= 0)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;
 
                LOG << F("ERROR: failed to find \"type\" key/value pair: ") << buffer << Logger::endl;
                return false;
            } 

            bool found = false;
            for(unsigned int i = 0; i < m_OutputStringConfigs.getCount(); ++i)
            {
                auto conf = m_OutputStringConfigs[i];
                if(!strcmp(buffer, conf->getType()))
                {
                    found = true;
                    strncpy(buffer, t.ptr, t.len);
                    buffer[t.len]=0;
 
                    auto obj = conf->createFromJson(buffer);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << buffer << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addOutputString(obj); 
                        strncpy(buffer, t.ptr, t.len);
                        buffer[t.len]=0;

                        LOG << F("Successfully created new ") << buffer << Logger::endl;
                    }
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
                    strncpy(buffer, t.ptr, t.len);
                    buffer[t.len]=0;
 
                    auto obj = conf->createFromJson(buffer);
                    if(!obj)
                    {
                        LOG << F("ERROR: Failed to create from: \n") << buffer << Logger::endl;
                        return false;
                    }
                    else
                    {
                        addDevice(obj); 
                        strncpy(buffer, t.ptr, t.len);
                        buffer[t.len]=0;

                        LOG << F("Successfully created new ") << buffer << Logger::endl;
                    }
                    break; 
                }
            }
            if(!found)
            {
                LOG << F("ERROR: No config found for type: ") << buffer << Logger::endl;
                return false;
            }
        }

        // scan for Triggers
        for(unsigned int i = 0; json_scanf_array_elem(json, len, ".Triggers", i, &t) > 0; ++i)
        {
            unsigned int deviceIndex;
            unsigned long interval;

            if(json_scanf(t.ptr, t.len, "{deviceIndex: %u, interval: %u, command: %s}", &deviceIndex, &interval, buffer) != 3)
            {
                strncpy(buffer, t.ptr, t.len);
                buffer[t.len]=0;
 
                LOG << F("ERROR: failed to parse trigger: ") << buffer << Logger::endl;
                return false;
            } 

            if(deviceIndex >= m_Devices.getCount())
            {
                LOG << F("ERROR: trigger's device index=") << deviceIndex << F(" which is too large\n");
                return false;
            }

            addTrigger(m_Devices[deviceIndex], interval, buffer);
            strncpy(buffer, t.ptr, t.len);
            buffer[t.len]=0;
            LOG << F("Successfully created new ") << buffer << Logger::endl;
        }

        return true;
    }
}
