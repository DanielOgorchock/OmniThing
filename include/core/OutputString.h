#ifndef OMNI_OUTPUTSTRING_H
#define OMNI_OUTPUTSTRING_H

#include "ObjectConfig.h"
#include "Triggerable.h"

namespace omni
{
    class OutputString : virtual public Triggerable
    {
        private:
        protected:
        public:
            virtual ~OutputString();

            virtual void writeString(const char*) = 0;
    };
}
#endif
