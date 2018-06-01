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
    if(argc > 4 || argc < 2)
    {
        std::cout << "Usage: ./OmniThing [config_file] [computer_ip <optional>] [hub_ip <optional>]\n\n"; 
        return 1;
    }

    OmniThing& omnithing = OmniThing::getInstance();

    omnithing.setLogger(new LoggerStdout());


#ifdef OMNI_PLAT_RPI
    if(gpioInitialise() == PI_INIT_FAILED)
    {
        LOG << "Failed to initialize pigpio library" << Logger::endl;
        return 1;
    }
    LOG << "Initialized pigpio\n";
#endif

    std::cout << "Attempting to open configuration file: " << argv[1] << std::endl;

    std::ifstream f(argv[1]);
    if(!f.is_open())
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << f.rdbuf();

    omnithing.loadJsonConfig(buffer.str().c_str());

    if(argc >= 3)
    {
        const char* ip = argv[2];
        LOG << "Configuring http server with cli ip: " << ip << Logger::endl;
        omnithing.setNetworkReceiver(new NetworkReceiverHttpLib(ip, 1337));
    }

    if(argc >= 4)
    {
        const char* destIp = argv[3];
        LOG << "Configuring http sender with cli ip: " << destIp << Logger::endl;
        omnithing.setNetworkSender(new NetworkSenderHttpLib(destIp, 39500));
    }

    omnithing.init();

    while(true)
    {
        sleepMillis(10);

        omnithing.run();
    }

    return 0;
}

