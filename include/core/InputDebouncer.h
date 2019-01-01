#ifndef OMNI_INPUTDEBOUNCER_H
#define OMNI_INPUTDEBOUNCER_H
#include "InputBool.h"

namespace omni
{
    class InputDebouncer : public InputBool
    {
        public:
            enum class DebounceMode {Rising, Falling, Both};
        private:
            bool m_bFirst;
            InputBool& m_Input;
            DebounceMode m_Mode;
            bool m_bPrev;
            bool m_bVal;
            unsigned long m_nDebounceDuration; // milliseconds
            unsigned long long m_nLastChanged; // milliseconds

        protected:
        public:
            InputDebouncer(InputBool& input, DebounceMode mode, unsigned long duration);
            virtual ~InputDebouncer();

            virtual bool readBool();

            static InputBool* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<InputBool> InputBoolConf;
    };
}
#endif

