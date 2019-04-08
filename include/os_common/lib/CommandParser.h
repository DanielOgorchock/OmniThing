#ifndef OMNI_COMMANDPARSER_H
#define OMNI_COMMANDPARSER_H

#include "InputFloat.h"
#include "ObjectConfig.h"

namespace omni
{
    class CommandParser : public InputFloat
    {
        private:
            const char* m_CommandStr;

        public:
            CommandParser(const char* commandStr);
            virtual ~CommandParser();

            float readFloat();

            static InputFloat* createFromJson(const char* json);


            static const char* Type;
            static ObjectConfig<InputFloat> InputFloatConf;
    };
}
#endif
