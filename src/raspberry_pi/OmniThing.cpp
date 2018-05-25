#include <iostream>
#include <pigpio.h>
#include <frozen.h>

#include "DigitalInputPinRaspberryPi.h"
#include "DigitalOutputPinRaspberryPi.h"


int main(int argc, char* argv[])
{
    if(gpioInitialize() == PI_INIT_FAILED)
    {
        std::cerr << "Failed to initialize pigpio library" << std::endl;
    }

    return 0;
}
