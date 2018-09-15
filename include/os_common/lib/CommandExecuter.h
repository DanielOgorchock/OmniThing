#ifndef OMNI_COMMANDEXECUTER_H
#define OMNI_COMMANDEXECUTER_H

#include "OutputVoid.h"
#include "ObjectConfig.h"

namespace omni
{
    class CommandExecuter : public OutputVoid
    {
        private:
            const char* m_CommandStr;
            bool m_bMultithreaded;

        protected:
        public:
            CommandExecuter(const char* command, bool multithread = false);
            virtual ~CommandExecuter();

            void writeVoid();

            static OutputVoid* createFromJson(const char* json);


            static const char* Type;
            static ObjectConfig<OutputVoid> OutputVoidConf;

    };
}
#endif
