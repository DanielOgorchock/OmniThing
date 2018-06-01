#ifndef OMNI_NETWORKRECEIVERHTTPLIB_H
#define OMNI_NETWORKRECEIVERHTTPLIB_H

#include "NetworkReceiver.h"
#include "httplib.h"
#include "ObjectConfig.h"

#include <thread>

namespace omni
{
    class NetworkReceiverHttpLib : public NetworkReceiver
    {
        private:
            httplib::Server m_Server;
            char m_JsonBuffer[2048]; //TODO: make size configurable
            char m_IP[30];
            unsigned short m_nPort;
            std::thread* m_Thread;
            std::mutex m_BufferMutex;
			bool m_bUnlockMutex;
            bool m_bWipeBuffer;

            void listenerThread();

        protected:

        public:
            NetworkReceiverHttpLib(const char* ip, unsigned short port);
            virtual ~NetworkReceiverHttpLib();

            virtual void init();
            virtual void run();
            virtual const char* getJsonString();

            static NetworkReceiver* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<NetworkReceiver> NetworkReceiverConf;
    };
}
#endif
