#include "OutputBoolToFloat.h"

#include <string.h>
#include "frozen.h"

namespace omni
{
//private
//protected
//public
    OutputBoolToFloat::OutputBoolToFloat(OutputFloat& outputFloat, float trueVal, float falseVal):
        m_rOutputFloat(outputFloat),
        m_fTrueVal(trueVal),
        m_fFalseVal(falseVal)
    {

    }

    OutputBoolToFloat::~OutputBoolToFloat()
    {

    }

    void OutputBoolToFloat::writeBool(bool b)
    {
        if(b)
        {
            m_rOutputFloat.writeFloat(m_fTrueVal);
        }
        else
        {
            m_rOutputFloat.writeFloat(m_fFalseVal);
        }
    }

    OutputBool* OutputBoolToFloat::createFromJson(const char* json)
    {
        float trueVal;
        float falseVal;

        unsigned int len = strlen(json);
        json_token t;

        if(json_scanf(json, len, "{output: %T, trueVal: %f, falseVal: %f}", &t, &trueVal, &falseVal) != 3)
        {
            return nullptr;
        }

        auto output = OmniThing::getInstance().buildOutputFloat(t);
        if(!output)
        {
            LOG << F("ERROR: Failed to create output\n");
            return nullptr;
        }

        return new OutputBoolToFloat(*output, trueVal, falseVal);
    }

    const char* OutputBoolToFloat::Type = "OutputBoolToFloat";
    ObjectConfig<OutputBool> OutputBoolToFloat::OutputBoolConf(Type, createFromJson);
}
