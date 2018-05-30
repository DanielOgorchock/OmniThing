#ifndef OMNI_INPUTBOOL_H
#define OMNI_INPUTBOOL_H

#include "ObjectConfig.h"


namespace omni
{
    class InputBool
    {
        private:

        public:
            virtual ~InputBool();

            virtual bool readBool() = 0;
    };
}
#endif
