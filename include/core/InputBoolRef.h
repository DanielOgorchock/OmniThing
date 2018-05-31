#ifndef OMNI_INPUTBOOLREF_H
#define OMNI_INPUTBOOLREF_H
#include "InputBool.h"

namespace omni
{
    class CompositePeripheral;

    class InputBoolRef : public InputBool
    {
        private:
            char m_Name[20];
            CompositePeripheral& m_Periph;

        protected:
        public:
            InputBoolRef(CompositePeripheral& periph, const char* name);
            virtual ~InputBoolRef();

            bool readBool() final;
    };
}
#endif
