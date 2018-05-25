#ifndef OMNI_NETWORKSENDERHTTPLIB_H
#define OMNI_NETWORKSENDERHTTPLIB_H

#include "NetworkSender.h"

namespace omni
{
    class NetworkSenderHttpLib : public NetworkSender
    {
        private:
            const char* m_Address;
            unsigned int m_nPort;

        protected:
        public:
            NetworkSenderHttpLib(const char* address, unsigned int port);
            virtual ~NetworkSenderHttpLib();

            virtual void init();
            virtual void run();
            virtual void sendJson(const char* json);
    };
}
#endif
