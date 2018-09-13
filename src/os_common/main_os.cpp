#ifdef OMNI_PLAT_RPI
    #include <pigpio.h>
#endif

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
    if(argc != 2)
    {
        std::cout << "Usage: ./OmniThing [config_file]\n\n";
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

    omnithing.loadJsonConfig(const_cast<char*>(buffer.str().c_str()));

    omnithing.init();

    while(true)
    {
        sleepMillis(10);

        omnithing.run();
    }

    return 0;
}

