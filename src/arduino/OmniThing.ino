#include "Logger.h"
#include "OmniThing.h"
#include "OmniUtil.h"

#include "LoggerSerial.h"
#include "TriggerableFunction.h"

auto& omnithing = omni::OmniThing::getInstance();

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

void printFreeRam()
{
    omnithing.getLogger() << F("Free ram: ") << freeRam() << F(" bytes\n");
}

// the setup function runs once when you press reset or power the board
void setup() 
{
    using namespace omni;
    static LoggerSerial logger(Serial, 115200);

    omnithing.setLogger(&logger);

    static TriggerableFunction freeRamTrigger(printFreeRam);
    omnithing.addTrigger(&freeRamTrigger, 10000, nullptr, true);

    printFreeRam();

    LOG << F("Running arduino setup() function\n");
    
    omnithing.init();    
}

// the loop function runs over and over again forever
void loop() 
{
    using namespace omni;

    omnithing.run();
}

