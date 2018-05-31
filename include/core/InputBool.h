#ifndef OMNI_INPUTBOOL_H
#define OMNI_INPUTBOOL_H

#include "ObjectConfig.h"
#include "Triggerable.h"

namespace omni
{
    class InputBool : virtual public Triggerable
    {
        private:

        public:
            virtual ~InputBool();

            virtual bool readBool() = 0;
    };
}
#endif
