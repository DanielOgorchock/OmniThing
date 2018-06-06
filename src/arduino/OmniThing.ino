#include "Logger.h"
#include "OmniThing.h"
#include "OmniUtil.h"

#include "LoggerSerial.h"
#include "TriggerableFunction.h"

#include "ArduinoJsonConfig.h"

#if defined(ARDUINO_ARCH_AVR)
    #include <avr/pgmspace.h>
#endif

#if defined(ARDUINO_ARCH_ESP8266)
    #include <pgmspace.h>
#endif

#include "frozen.h"

auto& omnithing = omni::OmniThing::getInstance();

long freeRam()
{
#if defined(ARDUINO_ARCH_AVR) 
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
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

void configWithProgmem(const char* json)
{
    using namespace omni;

    static char buffer[3000];
    printFreeRam();

    strcpy_P(buffer, json);
    omnithing.loadJsonConfig(buffer);
}

// the setup function runs once when you press reset or power the board
void setup() 
{
    using namespace omni;
    static LoggerSerial logger(Serial, 115200);

    omnithing.setLogger(&logger);

    static TriggerableFunction freeRamTrigger(printFreeRam);
    omnithing.addTrigger(&freeRamTrigger, 30000, nullptr, true);

    printFreeRam();

    LOG << F("Running arduino setup() function\n");
    LOG << F("A0 = ") << A0 << Logger::endl;

    for(unsigned int i = 0; i < Num_Json_Strings; ++i)
    {
        configWithProgmem(Config_Json_Strings[i]);
    }

    omnithing.init();    

    LOG << F("Entering main loop\n");

    delay(10);
}

// the loop function runs over and over again forever
void loop() 
{
    using namespace omni;
    omnithing.run();
}

