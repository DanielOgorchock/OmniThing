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

    unsigned long long getMicros()
    {
        return micros();
    }

    void sleepMicros(unsigned long long us)
    {
        delayMicroseconds(us); 
    }
}

