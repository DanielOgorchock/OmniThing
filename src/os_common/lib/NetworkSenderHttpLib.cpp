#include "NetworkSenderHttpLib.h"
#include "httplib.h"
#include "Logger.h"

#include <string.h>
#include "frozen.h"

namespace omni
{
//private
//protected
//public
    NetworkSenderHttpLib::NetworkSenderHttpLib(const char* address, unsigned int port):
        m_nPort(port)
    {
        strncpy(m_Address, address, 100);
    }

    NetworkSenderHttpLib::~NetworkSenderHttpLib()
    {

    }

    void NetworkSenderHttpLib::init()
    {

    }

    void NetworkSenderHttpLib::run()
    {

    }

    void NetworkSenderHttpLib::sendJson(const char* json)
    {
        httplib::Client client(m_Address, m_nPort);
        auto res = client.Post("/", json, "application/json"); 

        LOG << "Sending json: " << json << Logger::endl;
        if(res)
        {
            LOG << "Return value: " << res->status << Logger::endl;
        }
        else
        {
            LOG << "Error: No reply\n";
        }
    }

    NetworkSender* NetworkSenderHttpLib::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        char ip[20];
        unsigned int port;

        if(json_scanf(json, len, "{ip: %s, port: %u}", ip, &port) != 2)
        {
            return nullptr;
        }

        LOG << "ip=" << ip <<" port=" << port << Logger::endl;
        return new NetworkSenderHttpLib(ip, port);
    }

    const char* NetworkSenderHttpLib::Type = "NetworkSenderHttpLib";
    ObjectConfig<NetworkSender> NetworkSenderHttpLib::NetworkSenderConf(Type, createFromJson);
}
