#include "Triggerable.h"

namespace omni
{
//private
//protected
//public
    Triggerable::~Triggerable()
    {

    }

    void Triggerable::trigger(void* arg) 
    {
        LOG << F("No trigger() implemented\n");
    }
}
