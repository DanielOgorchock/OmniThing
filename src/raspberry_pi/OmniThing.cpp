#include <iostream>
#include <pigpio.h>
#include <frozen.h>
#include <unistd.h>

#include "DigitalInputPinRaspberryPi.h"
#include "DigitalOutputPinRaspberryPi.h"


int main(int argc, char* argv[])
{
    using namespace omni;

    if(gpioInitialise() == PI_INIT_FAILED)
    {
        std::cerr << "Failed to initialize pigpio library" << std::endl;
    }
    std::cout << "Initialized pigpio\n";

    DigitalOutputPinRaspberryPi out(22, false, false);

    while(true)
    {
        sleep(1);
        std::cout << "Toggling pin\n";
        out.writeVoid(); 
    }

    return 0;
}
