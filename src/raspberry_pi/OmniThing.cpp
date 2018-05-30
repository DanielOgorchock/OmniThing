#include <iostream>
#include <pigpio.h>
#include <frozen.h>
#include <unistd.h>

#include "DigitalInputPinRaspberryPi.h"
#include "DigitalOutputPinRaspberryPi.h"

#include "OmniThing.h"
#include "OmniUtil.h"
#include "ContactSensor.h"
#include "Switch.h"

#include "NetworkReceiverHttpLib.h"
#include "NetworkSenderHttpLib.h"

#include "LoggerStdout.h"

#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
    using namespace omni;

    OmniThing& omnithing = OmniThing::getInstance();

    omnithing.setLogger(new LoggerStdout());

    const char* ip = "192.168.2.104";
    NetworkReceiverHttpLib receiver(ip, 1337);
    omnithing.setNetworkReceiver(&receiver);

    const char* destIp = "192.168.2.200";
    NetworkSenderHttpLib sender(destIp, 39500);
    omnithing.setNetworkSender(&sender);


    if(gpioInitialise() == PI_INIT_FAILED)
    {
        LOG << "Failed to initialize pigpio library" << Logger::endl;
        return 1;
    }
    LOG << "Initialized pigpio\n";

    std::ifstream f("example_configs/raspberry_pi.json");
    std::stringstream buffer;
    buffer << f.rdbuf();

    omnithing.loadJsonConfig(buffer.str().c_str());

    omnithing.init();

    while(true)
    {
        sleepMillis(10);

        omnithing.run();
    }

    return 0;
}
