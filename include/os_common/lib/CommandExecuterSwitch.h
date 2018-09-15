#ifndef OMNI_COMMANDEXECUTERSWITCH_H
#define OMNI_COMMANDEXECUTERSWITCH_H

#include "OutputBool.h"
#include "ObjectConfig.h"

namespace omni
{
    class CommandExecuterSwitch : public OutputBool
    {
        private:
            const char* m_CommandStrOn;
            const char* m_CommandStrOff;
            bool m_bMultithreaded;

            void executeCommandOn();
            void executeCommandOff();
        protected:
        public:
            CommandExecuterSwitch(const char* commandOn, const char* commandOff, bool multithread = false);
            virtual ~CommandExecuterSwitch();

            void writeBool(bool b);

            static OutputBool* createFromJson(const char* json);


            static const char* Type;
            static ObjectConfig<OutputBool> OutputBoolConf;

    };
}
#endif

