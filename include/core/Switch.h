#ifndef OMNI_SWITCH_H
#define OMNI_SWITCH_H

#include "Device.h"

namespace omni
{
    class OutputBool;
    class InputBool;

    class Switch : public Device
    {
        private:
            OutputBool& m_rOutput;
            InputBool* m_pInput;
            bool m_bInvert;
            bool m_bIgnoreRedundant;
            bool m_bValue;

            void sendJsonPacket();

            void writeNoUpdate(bool b);

        protected:
        public:
            Switch(OutputBool& output, bool invert, bool initial, InputBool* input, bool ignoreRedundant);
            virtual ~Switch();

            virtual void recvJson(const char* cmd, const char* json);
            virtual void init();
            virtual void run();

            virtual const char* getType() const {return Type;}
            bool isInverted() const {return m_bInvert;}
            bool hasInput() const {return m_pInput;}
            virtual bool read() const {return m_bValue;}
            virtual void write(bool b);
            virtual void on() {write(true);}
            virtual void off() {write(false);}
            virtual void toggle() {write(!read());}

            static Device* createFromJson(const char* json);

            //Type
            static const char* Type;

            //json commands
            static const char* Cmd_Poll;
            static const char* Cmd_On;
            static const char* Cmd_Off;
            static const char* Cmd_Toggle;

            //events
            static const char* Event_On;
            static const char* Event_Off;
            static const char* Event_Changed;

            static ObjectConfig<Device> DevConf;
    };
}

#endif
