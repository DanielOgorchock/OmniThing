#include "NetworkSenderHttpLib.h"
#include "httplib.h"
#include "Logger.h"


namespace omni
{
//private
//protected
//public
    NetworkSenderHttpLib::NetworkSenderHttpLib(const char* address, unsigned int port):
        m_Address(address),
        m_nPort(port)
    {

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
}
