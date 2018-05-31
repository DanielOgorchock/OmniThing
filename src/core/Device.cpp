#include "Device.h"


namespace omni
{
//private
//protected
//public
    Device::Device(bool run):
        m_nUid(0),
        m_bRun(run)
    {
        static unsigned short nextUid = 0;
        m_nUid = nextUid++;
    }

    Device::~Device()
    {

    }

    void Device::trigger(void* arg)
    {
        recvJson(static_cast<const char*>(arg), nullptr);
    }
}
