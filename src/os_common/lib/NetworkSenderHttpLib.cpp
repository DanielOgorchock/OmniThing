#include "NetworkSenderHttpLib.h"
#include "httplib.h"

#include <iostream>

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

        std::cout << "Sending json: " << json << std::endl;
        if(res)
        {
            std::cout << "Return value: " << res->status << std::endl;
        }
        else
        {
            std::cout << "Error: No reply\n";
        }
    }
}
