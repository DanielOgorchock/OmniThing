#include "OmniThing.h"
#include "OmniUtil.h"

#include "Device.h"

namespace omni
{
//private
    OmniThing::OmniThing():
        m_nDeviceCount(0),
        m_nTriggerCount(0)
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
        //run init on all devices
        for(unsigned short i = 0; i < m_nDeviceCount; ++i)
        {
            m_Devices[i]->init();
        }

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
    }

    void OmniThing::run()
    {
        runScheduler();
    }

    void OmniThing::sendJson(const char* json)
    {
        //TODO: THIS
    } 

    void OmniThing::addDevice(Device* dev)
    {
        m_Devices[m_nDeviceCount++] = dev;
    }

    void OmniThing::addTrigger(Trigger& t)
    {
        m_Triggers[m_nTriggerCount++] = t;
    }
}
