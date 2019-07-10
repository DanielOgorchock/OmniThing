#ifndef OMNI_DIMMERSWITCH_H
#define OMNI_DIMMERSWITCH_H

#include "Device.h"

namespace omni
{
    class OutputFloat;
    class InputFloat;

    class DimmerSwitch : public Device
    {
        private:
            OutputFloat& m_rOutput;
            InputFloat* m_pInput;
            float m_fInitial;
            float m_fLevel;
            bool m_bValue;

            void sendJsonPacket();
            void writeNoUpdate(float level, bool turnOff);

        protected:
        public:
            DimmerSwitch(OutputFloat& output, float initialLevel, InputFloat* input);
            virtual ~DimmerSwitch();

            virtual void recvJson(const char* cmd, const char* json);
            virtual void init();
            virtual void run();

            virtual const char* getType() const {return Type;}
            bool hasInput() const {return m_pInput;}
            virtual float read() const {return m_fLevel;}
            virtual void write(float level, bool turnOff);
            virtual void on() {write(m_fLevel, false);}
            virtual void off() {write(0, true);}
            virtual void toggle() {if(m_bValue) off(); else on();}

            static Device* createFromJson(const char* json);

            //Type
            static const char* Type;

            //json commands
            static const char* Cmd_Poll;
            static const char* Cmd_SetLevel;
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
