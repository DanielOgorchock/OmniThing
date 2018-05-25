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
            char m_JsonBuffer[128]; //TODO: make size configurable
            m_nPort;

        protected:

        public:
            NetworkReceiverHttpLib(unsigned short port);
            virtual ~NetworkReceiverHttpLib();

            virtual void init();
            virtual void run();
            virtual const char* getJsonString();

    };
}
#endif
