#include <iostream>
#include <pigpio.h>
#include <frozen.h>
#include <unistd.h>

#include "DigitalInputPinRaspberryPi.h"
#include "DigitalOutputPinRaspberryPi.h"

#include "ContactSensor.h"
#include "Switch.h"


int main(int argc, char* argv[])
{
    using namespace omni;

    if(gpioInitialise() == PI_INIT_FAILED)
    {
        std::cerr << "Failed to initialize pigpio library" << std::endl;
    }
    std::cout << "Initialized pigpio\n";

    DigitalOutputPinRaspberryPi out(22, false, false);
    DigitalInputPinRaspberryPi in(7, false, DigitalInputPinRaspberryPi::PinMode::Pullup); 

    Switch sw(out);
    ContactSensor contact(in);

    while(true)
    {
        sleep(2);
        sw.recvJson("toggle", nullptr); 
        std::cout << std::endl;
        contact.recvJson("poll", nullptr);

        std::cout << std::endl << std::endl;
    }

    return 0;
}
