#ifndef OMNI_SWITCH_H
#define OMNI_SWITCH_H

#include "Device.h"

namespace omni
{
    class OutputBool;

    class Switch : public Device
    {
        private:
            OutputBool& m_rOutput;
            bool m_bInvert;
            bool m_bValue;

            void sendJsonPacket();

        protected:
        public:
            Switch(OutputBool& output, bool invert = false, bool initial = false);
            virtual ~Switch();

            virtual void recvJson(const char* cmd, const char* json);
            virtual void init();

            const char* getType() const {return Type;}
            bool isInverted() const {return m_bInvert;}
            virtual bool read() const {return m_bValue;}
            virtual void write(bool b);
            virtual void on() {write(true);}
            virtual void off() {write(false);} 
            virtual void toggle() {write(!read());}

            static Device* createFromJson(const char* json);

            //Type
            static const char* Type;
            
            //json commands/format strings
            static const char* Cmd_Poll;
            static const char* Cmd_On;
            static const char* Cmd_Off;
            static const char* Cmd_Toggle;

            static DeviceConfig DevConf;
    };
}

#endif
