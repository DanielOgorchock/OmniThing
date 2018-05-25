#include "Device.h"

#include <iostream> //TODO: replace with Logger class for arduino support

namespace omni
{
//private
//protected
//public
    Device::Device():
        m_nUid(0),
        m_bRun(true)
    {
        static unsigned short nextUid = 0;
        m_nUid = nextUid++;
    }

    Device::~Device()
    {

    }
}
