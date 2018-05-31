#ifndef OMNI_OUTPUTBOOL_H
#define OMNI_OUTPUTBOOL_H

#include "ObjectConfig.h"
#include "Triggerable.h"


namespace omni
{
    class OutputBool : virtual public Triggerable
    {
        private:

        public:
            virtual ~OutputBool();

            virtual void writeBool(bool b) = 0;
    };
}
#endif
