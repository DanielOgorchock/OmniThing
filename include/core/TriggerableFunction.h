#ifndef OMNI_TRIGGERABLEFUNCTION_H
#define OMNI_TRIGGERABLEFUNCTION_H

#include "Triggerable.h"

namespace omni
{
    class TriggerableFunction : public Triggerable
    {
        private:
            void (*m_pNoArg) (void); 
            void (*m_pYesArg) (void*);
        protected:
        public:
            TriggerableFunction(void (*noArg) (void), void (*yesArg) (void*) = nullptr);
            
            virtual void trigger(void* arg);
    };
}
#endif
