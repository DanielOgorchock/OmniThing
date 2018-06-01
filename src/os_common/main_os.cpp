#ifdef OMNI_PLAT_RPI
    #include <pigpio.h>
#endif

#include <unistd.h>

#include "OmniThing.h"
#include "OmniUtil.h"

#include "NetworkReceiverHttpLib.h"
#include "NetworkSenderHttpLib.h"

#include "LoggerStdout.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "frozen.h"

int main(int argc, char* argv[])
{
    using namespace omni;

    //parse commandline args
    if(argc != 4)
    {
        std::cout << "Usage: ./OmniThing [computer_ip] [hub_ip] [config_file]\n\n"; 
        return 1;
    }

    OmniThing& omnithing = OmniThing::getInstance();

    omnithing.setLogger(new LoggerStdout());

    const char* ip = argv[1];
    NetworkReceiverHttpLib receiver(ip, 1337);
    omnithing.setNetworkReceiver(&receiver);

    const char* destIp = argv[2];
    NetworkSenderHttpLib sender(destIp, 39500);
    omnithing.setNetworkSender(&sender);


#ifdef OMNI_PLAT_RPI
    if(gpioInitialise() == PI_INIT_FAILED)
    {
        LOG << "Failed to initialize pigpio library" << Logger::endl;
        return 1;
    }
    LOG << "Initialized pigpio\n";
#endif

    std::cout << "Attempting to open configuration file: " << argv[3] << std::endl;

    std::ifstream f(argv[3]);
    if(!f.is_open())
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }

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

