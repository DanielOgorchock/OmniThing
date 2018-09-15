#ifndef OMNI_COMMANDRETURNCODE_H
#define OMNI_COMMANDRETURNCODE_H

#include "InputBool.h"
#include "ObjectConfig.h"

namespace omni
{
    class CommandReturnCode : public InputBool
    {
        private:
            const char* m_CommandStr;

        public:
            CommandReturnCode(const char* commandStr);
            virtual ~CommandReturnCode();

            bool readBool();

            static InputBool* createFromJson(const char* json);


            static const char* Type;
            static ObjectConfig<InputBool> InputBoolConf;
    };
}
#endif
