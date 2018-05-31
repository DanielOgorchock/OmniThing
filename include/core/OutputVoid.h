#ifndef OMNI_OUTPUTVOID_H
#define OMNI_OUTPUTVOID_H

#include "ObjectConfig.h"
#include "Triggerable.h"

namespace omni
{
    class OutputVoid : virtual public Triggerable
    {
        private:

        public:
            virtual ~OutputVoid();

            virtual void writeVoid() = 0;
    };
}
#endif
