#ifndef OMNI_OUTPUTFLOAT_H
#define OMNI_OUTPUTFLOAT_H

#include "ObjectConfig.h"
#include "Triggerable.h"


namespace omni
{
    class OutputFloat : virtual public Triggerable
    {
        private:

        public:
            virtual ~OutputFloat();

            virtual void writeFloat(float f) = 0;
    };
}
#endif

