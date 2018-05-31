#include "Logger.h"
#include "OmniThing.h"

#include "LoggerSerial.h"

auto& omnithing = omni::OmniThing::getInstance();

// the setup function runs once when you press reset or power the board
void setup() 
{
    using namespace omni;
    static omni::LoggerSerial logger(Serial, 115200);

    omnithing.setLogger(&logger);

    LOG << F("Running arduino setup() function\n");
    
    omnithing.init();    
}

// the loop function runs over and over again forever
void loop() 
{
    using namespace omni;

    omnithing.run();
}

