#ifndef OMNI_TIMEDOUTPUTBOOL_H
#define OMNI_TIMEDOUTPUTBOOL_H

#include "OutputBool.h"
#include "OutputVoid.h"
#include "ObjectConfig.h"

namespace omni
{
    class TimedOutputBool : public OutputBool, public OutputVoid
    {
        private:
            OutputBool& m_rOutput;
            unsigned long m_nDuration;
            bool m_bValue;
            bool m_bTriggerPending;

            static TimedOutputBool* createFromJson(const char* json);
            void write(bool b);
            void startTimedOutput();

        protected:
        public:
            TimedOutputBool(OutputBool& output, unsigned long duration);
            ~TimedOutputBool();

            virtual void writeBool(bool b);
            virtual void writeVoid();

            virtual void trigger(void* arg);

            static OutputBool* createBoolFromJson(const char* json);
            static OutputVoid* createVoidFromJson(const char* json); 

            static const char* Type;
            static ObjectConfig<OutputBool> OutputBoolConf;
            static ObjectConfig<OutputVoid> OutputVoidConf;
    };
}

#endif
