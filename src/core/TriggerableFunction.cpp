#include "TriggerableFunction.h"

#include "Logger.h"

namespace omni
{
//private
//protected
//public
    TriggerableFunction::TriggerableFunction(void (*noArg) (void), void (*yesArg) (void*)):
        m_pNoArg(noArg),
        m_pYesArg(yesArg)
    {

    }
    
    void TriggerableFunction::trigger(void* arg)
    {
        if(m_pNoArg)
            m_pNoArg();

        if(m_pYesArg)
            m_pYesArg(arg);
    }

}
