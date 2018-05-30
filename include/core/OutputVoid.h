#ifndef OMNI_OUTPUTVOID_H
#define OMNI_OUTPUTVOID_H

#include "ObjectConfig.h"

namespace omni
{
    class OutputVoid
    {
        private:

        public:
            virtual ~OutputVoid();

            virtual void writeVoid() = 0;
    };
}
#endif
