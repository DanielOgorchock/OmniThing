#ifndef OMNI_CONTACTSENSOR_H
#define OMNI_CONTACTSENSOR_H

#include "Device.h"

namespace omni
{
    class InputBool;

    class ContactSensor : public Device
    {
        private:
            InputBool& m_rInput;
            bool m_bInvert;
            bool m_bLastVal;

            void sendJsonPacket();

        protected:
        public:
            ContactSensor(InputBool& input, bool invert = false, bool constantPoll = true);
            virtual ~ContactSensor();

            virtual void recvJson(const char* cmd, const char* json);
            virtual void run();
            virtual void init();

            const char* getType() const {return "ContactSensor";}
            bool isInverted() const {return m_bInvert;}
            virtual bool read();

            //json commands/format strings
            static const char* Cmd_Poll;

    };
}

#endif
