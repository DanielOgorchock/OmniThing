#ifndef OMNI_INPUTFLOATREF_H
#define OMNI_INPUTFLOATREF_H
#include "InputFloat.h"

namespace omni
{
    class CompositePeripheral;

    class InputFloatRef : public InputFloat
    {
        private:
            const char* m_Name;
            CompositePeripheral& m_Periph;

        protected:
        public:
            InputFloatRef(CompositePeripheral& periph, const char* name);
            virtual ~InputFloatRef();

            float readFloat() final;

            static InputFloat* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<InputFloat> InputFloatConf;
    };
}
#endif

