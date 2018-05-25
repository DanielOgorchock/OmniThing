#include "DigitalInputPinStub.h"


int main(int argc, char* argv[])
{
    using namespace omni; 

    InputBool* inBool = new DigitalInputPinStub(10, true);

    inBool->readBool();

    return 0;
}
