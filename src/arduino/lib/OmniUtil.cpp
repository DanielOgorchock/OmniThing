#include "OmniUtil.h"
#include <Arduino.h>

namespace omni
{
    unsigned long long getMillis()
    {
        return getMillis();
    }

    void sleepMillis(unsigned long long ms)
    {
        delay(ms);
    }

    long freeRam()
    {
    #if defined(ARDUINO_ARCH_AVR) 
        extern int __heap_start, *__brkval;
        int v;
        return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
    #elif defined(ARDUINO_ARCH_ESP8266)
        return ESP.getFreeHeap();
    #elif defined(ARDUINO_ARCH_SAMD)
        char top;
        return &top - reinterpret_cast<char*>(sbrk(0));
    #else
        return -1;
    #endif // !
    }
}

