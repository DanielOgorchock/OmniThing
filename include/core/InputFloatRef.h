#ifndef OMNI_INPUTFLOATREF_H
#define OMNI_INPUTFLOATREF_H
#include "InputFloat.h"

namespace omni
{
    class CompositePeripheral;

    class InputFloatRef : public InputFloat
    {
        private:
            char m_Name[20];
            CompositePeripheral& m_Periph;

        protected:
        public:
            InputFloatRef(CompositePeripheral& periph, const char* name);
            virtual ~InputFloatRef();

            float readFloat() final;
    };
}
#endif

