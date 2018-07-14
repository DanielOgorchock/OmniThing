#ifndef OMNI_INPUTUINTREF_H
#define OMNI_INPUTUINTREF_H
#include "InputUInt.h"

namespace omni
{
    class CompositePeripheral;

    class InputUIntRef : public InputUInt
    {
        private:
            const char* m_Name;
            CompositePeripheral& m_Periph;

        protected:
        public:
            InputUIntRef(CompositePeripheral& periph, const char* name);
            virtual ~InputUIntRef();

            unsigned int readUInt() final;

            static InputUInt* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<InputUInt> InputUIntConf;
    };
}
#endif


