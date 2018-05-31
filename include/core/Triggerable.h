#ifndef OMNI_TRIGGERABLE_H
#define OMNI_TRIGGERABLE_H

#include "Logger.h"

namespace omni
{
    class Triggerable
    {
        private:
        protected:
        public:
            virtual ~Triggerable();

            virtual void trigger(void* arg);

    };
}
#endif
