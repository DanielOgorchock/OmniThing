#include "Device.h"

#include <iostream> //TODO: replace with Logger class for arduino support

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
}
