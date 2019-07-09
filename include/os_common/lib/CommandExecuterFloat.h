#ifndef OMNI_COMMANDEXECUTERFLOAT_H
#define OMNI_COMMANDEXECUTERFLOAT_H

#include "OutputFloat.h"
#include "ObjectConfig.h"

namespace omni
{
    class CommandExecuterFloat : public OutputFloat
    {
        private:
            const char* m_CommandStr;
            bool m_bMultithreaded;

        protected:
        public:
            CommandExecuterFloat(const char* command, bool multithread = false);
            virtual ~CommandExecuterFloat();

            void writeFloat(float val);

            static OutputFloat* createFromJson(const char* json);


            static const char* Type;
            static ObjectConfig<OutputFloat> OutputFloatConf;

    };
}
#endif
