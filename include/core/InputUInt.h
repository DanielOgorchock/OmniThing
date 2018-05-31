#ifndef OMNI_INPUTUINT_H
#define OMNI_INPUTUINT_H

#include "ObjectConfig.h"
#include "Triggerable.h"

namespace omni
{
    class InputUInt : virtual public Triggerable
    {
        private:

        public:
            virtual ~InputUInt();

            virtual unsigned int readUInt() = 0;
    };
}
#endif

