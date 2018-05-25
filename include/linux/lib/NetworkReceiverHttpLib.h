#ifndef OMNI_NETWORKRECEIVERHTTPLIB_H
#define OMNI_NETWORKRECEIVERHTTPLIB_H

#include "NetworkReceiver.h"
#include "httplib.h"

namespace omni
{
    class NetworkReceiverHttpLib : public NetworkReceiver
    {
        private:
            httplib::Server m_Server;
            char m_JsonBuffer[128] //TODO: make size configurable

        protected:

        public:
            NetworkReceiverHttpLib();
            virtual ~NetworkReceiverHttpLib();

            virtual void init();
            virtual void run();
            virtual const char* getJsonString();

    };
}
#endif
