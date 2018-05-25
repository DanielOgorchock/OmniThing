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


int main(int argc, char* argv[])
{
    using namespace omni;

    OmniThing& omnithing = OmniThing::getInstance();

    NetworkReceiverHttpLib receiver(1337);

    omnithing.setNetworkReceiver(&receiver);

    if(gpioInitialise() == PI_INIT_FAILED)
    {
        std::cerr << "Failed to initialize pigpio library" << std::endl;
    }
    std::cout << "Initialized pigpio\n";

    DigitalOutputPinRaspberryPi out(22, false, false);
    DigitalInputPinRaspberryPi in(7, false, DigitalInputPinRaspberryPi::PinMode::Pullup); 

    Switch sw(out);
    ContactSensor contact(in);

    Trigger t_sw(&sw, 1000, Switch::Cmd_Toggle);
    Trigger t_contact(&contact, 1000, ContactSensor::Cmd_Poll);

    omnithing.addDevice(&sw);
    omnithing.addDevice(&contact);

    omnithing.addTrigger(t_sw);
    omnithing.addTrigger(t_contact);

    omnithing.init();

    while(true)
    {
        sleepMillis(10);

        omnithing.run();
    }

    return 0;
}
