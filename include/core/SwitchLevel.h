#ifndef OMNI_SWITCHLEVEL_H
#define OMNI_SWITCHLEVEL_H

#include "Device.h"

namespace omni
{
    class OutputFloat;
    class InputFloat;

    class SwitchLevel : public Device
    {
        private:
            OutputFloat& m_rOutput;
            InputFloat* m_pInput;
            float m_fLevel;

            void sendJsonPacket();
            void writeNoUpdate(float level);

        protected:
        public:
            SwitchLevel(OutputFloat& output, float initialLevel, InputFloat* input);
            virtual ~SwitchLevel();

            virtual void recvJson(const char* cmd, const char* json);
            virtual void init();
            virtual void run();

            virtual const char* getType() const {return Type;}
            bool hasInput() const {return m_pInput;}
            virtual float read() const {return m_fLevel;}
            virtual void write(float level);

            static Device* createFromJson(const char* json);

            //Type
            static const char* Type;

            //json commands
            static const char* Cmd_Poll;
            static const char* Cmd_SetLevel;

            //events
            static const char* Event_Changed;

            static ObjectConfig<Device> DevConf;
    };
}
#endif
