#ifndef OMNI_INPUTFLOAT_H
#define OMNI_INPUTFLOAT_H

#include "ObjectConfig.h"


namespace omni
{
    class InputFloat
    {
        private:

        public:
            virtual ~InputFloat();

            virtual float readFloat() = 0;
    };
}
#endif
