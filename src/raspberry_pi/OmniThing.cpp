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

int main(int argc, char* argv[])
{
    using namespace omni;

    OmniThing& omnithing = OmniThing::getInstance();

    const char* ip = "192.168.2.104";
    NetworkReceiverHttpLib receiver(ip, 1337);
    omnithing.setNetworkReceiver(&receiver);

    const char* destIp = "192.168.2.200";
    NetworkSenderHttpLib sender(destIp, 80);
    omnithing.setNetworkSender(&sender);


    if(gpioInitialise() == PI_INIT_FAILED)
    {
        std::cerr << "Failed to initialize pigpio library" << std::endl;
    }
    std::cout << "Initialized pigpio\n";

    DigitalOutputPinRaspberryPi out(22, false, false);
    DigitalInputPinRaspberryPi in(7, false, DigitalInputPinRaspberryPi::PinMode::Pullup); 

    Switch sw(out);
    ContactSensor contact(in);

    Trigger t_sw(&sw, 15000, Switch::Cmd_Toggle);
    Trigger t_contact(&contact, 10000, ContactSensor::Cmd_Poll);

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
