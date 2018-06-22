#ifndef OMNI_OUTPUTBOOLTOFLOAT_H
#define OMNI_OUTPUTBOOLTOFLOAT_H

#include "OutputBool.h"
#include "OutputFloat.h"
#include "ObjectConfig.h"

namespace omni
{
    class OutputBoolToFloat : public OutputBool
    {
        private:
            OutputFloat& m_rOutputFloat;
            float m_fTrueVal;
            float m_fFalseVal;

        protected:
        public:
            OutputBoolToFloat(OutputFloat& outputFloat, float trueVal, float falseVal);
            virtual ~OutputBoolToFloat();

            void writeBool(bool b);

            static OutputBool* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<OutputBool> OutputBoolConf;
    };
}

#endif
