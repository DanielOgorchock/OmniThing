#ifndef OMNI_MOMENTARY_H
#define OMNI_MOMENTARY_H

#include "Device.h"
#include "ObjectConfig.h"

namespace omni
{
    class OutputVoid;
    class Momentary : public Device
    {
        private:
            OutputVoid& m_rOutput;

            void sendJsonPacket(const char* event);
            void push();

        protected:
        public:
            Momentary(OutputVoid& output);
            virtual ~Momentary();
            
            virtual void recvJson(const char* cmd, const char* json);
            virtual void run();
            virtual void init();

            virtual const char* getType() const {return Type;}

            static Device* createFromJson(const char* json);

            // json commands
            static const char* Cmd_Poll;
            static const char* Cmd_Push;

            // events
            static const char* Event_Pushed;

            static const char* Type;
            static ObjectConfig<Device> DevConf;
    };
}
#endif
