#ifndef OMNI_INPUTFLOAT_H
#define OMNI_INPUTFLOAT_H

#include "ObjectConfig.h"
#include "Triggerable.h"


namespace omni
{
    class InputFloat : virtual public Triggerable
    {
        private:

        public:
            virtual ~InputFloat();

            virtual float readFloat() = 0;
    };
}
#endif
