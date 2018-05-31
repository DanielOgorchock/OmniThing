#include "OmniUtil.h"
#include <Arduino.h>

namespace omni
{
    unsigned long long getMillis()
    {
        return millis();
    }

    void sleepMillis(unsigned long long ms)
    {
        delay(ms);
    }

}

