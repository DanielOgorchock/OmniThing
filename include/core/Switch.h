#ifndef OMNI_SWITCH_H
#define OMNI_SWITCH_H

namespace omni
{
    class OutputBool;

    class Switch : public Device
    {
        private:
            static const char* Type = "Switch";

            //json commands/format strings
            static const char* Cmd_Poll     = "poll";
            static const char* Cmd_On       = "on";
            static const char* Cmd_Off      = "off";
            static const char* Cmd_Toggle   = "toggle";

            OutputBool& m_rOutput;
            bool m_bInvert;
            bool m_bValue;
        protected:
        public:
            Switch(OutputBool& output, bool invert = false, bool initial = false);
            virtual ~Switch();

            virtual void recvJson(const char* cmd, const char* json);

            const char* getType() const {return Type;}
            bool isInverted() const {return m_bInvert;}
            virtual bool read() const {return m_bValue;}
            virtual void write(bool b);
            virtual void on() {write(true);}
            virtual void off() {write(false);} 
            virtual void toggle() {write(!read());}
    };
}

#endif
