#ifndef OMNI_NETWORKSENDERHTTPLIB_H
#define OMNI_NETWORKSENDERHTTPLIB_H

#include "NetworkSender.h"
#include "ObjectConfig.h"

namespace omni
{
    class NetworkSenderHttpLib : public NetworkSender
    {
        private:
            char m_Address[100];
            unsigned int m_nPort;

        protected:
        public:
            NetworkSenderHttpLib(const char* address, unsigned int port);
            virtual ~NetworkSenderHttpLib();

            virtual void init();
            virtual void run();
            virtual void sendJson(const char* json);

            static NetworkSender* createFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<NetworkSender> NetworkSenderConf;
    };
}
#endif
