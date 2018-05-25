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

            void sendJsonPacket();

        protected:
        public:
            ContactSensor(InputBool& input, bool invert = false);
            virtual ~ContactSensor();

            virtual void recvJson(const char* cmd, const char* json);

            const char* getType() const {return "ContactSensor";}
            bool isInverted() const {return m_bInvert;}
            virtual bool read();

            //json commands/format strings
            static const char* Cmd_Poll;

    };
}

#endif
