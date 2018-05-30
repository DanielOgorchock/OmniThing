#include <frozen.h>

#include "OmniThing.h"
#include "OmniUtil.h"
#include "ContactSensor.h"
#include "Switch.h"

#include "NetworkReceiverHttpLib.h"
#include "NetworkSenderHttpLib.h"
#include "DigitalInputPinStub.h"

#include "LoggerStdout.h"

int main(int argc, char* argv[])
{
    using namespace omni;

    OmniThing& omnithing = OmniThing::getInstance();

    omnithing.setLogger(new LoggerStdout());

    const char* ip = "192.168.2.105";
    NetworkReceiverHttpLib receiver(ip, 1337);
    omnithing.setNetworkReceiver(&receiver);

    const char* destIp = "192.168.2.200";
    NetworkSenderHttpLib sender(destIp, 39500);
    omnithing.setNetworkSender(&sender);

    omnithing.loadJsonConfig("{\"InputBools\": [{\"type\": \"DigitalInputPinStub\", \"pin\": 12, \"invertLogic\": true}], \"Devices\": [{\"type\": \"ContactSensor\", \"input\": 0, \"invert\": false, \"constantPoll\": true}] }");

    omnithing.init();

    while(true)
    {
        sleepMillis(10);

        omnithing.run();
    }

    return 0;
}

