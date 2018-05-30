#ifndef OMNI_OUTPUTFLOAT_H
#define OMNI_OUTPUTFLOAT_H

#include "ObjectConfig.h"


namespace omni
{
    class OutputFloat
    {
        private:

        public:
            virtual ~OutputFloat();

            virtual void writeFloat(float f) = 0;
    };
}
#endif

