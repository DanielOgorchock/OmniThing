#ifndef OMNI_INPUTBOOLREF_H
#define OMNI_INPUTBOOLREF_H
#include "InputBool.h"

namespace omni
{
    class CompositePeripheral;

    class InputBoolRef : public InputBool
    {
        private:
            const char* m_Name;
            CompositePeripheral& m_Periph;

        protected:
        public:
            InputBoolRef(CompositePeripheral& periph, const char* name);
            virtual ~InputBoolRef();

            bool readBool() final;

            static InputBool* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<InputBool> InputBoolConf;
    };
}
#endif
