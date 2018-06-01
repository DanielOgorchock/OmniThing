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

            virtual const char* getType() const {return Type;}
            bool isInverted() const {return m_bInvert;}
            virtual bool read();

            static Device* createFromJson(const char* json);

            //Type
            static const char* Type;

            //json commands
            static const char* Cmd_Poll;

            //events
            static const char* Event_Open;
            static const char* Event_Closed;
            static const char* Event_Changed;
           

            static ObjectConfig<Device> DevConf;
    };
}

#endif
