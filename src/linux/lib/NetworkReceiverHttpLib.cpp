#include "NetworkReceiverHttpLib.h"

#include <iostream>

namespace omni
{
//private
//protected
//public
    NetworkReceiverHttpLib::NetworkReceiverHttpLib():
        m_Server()
    {
        m_JsonBuffer[0] = 0; 
    }

    NetworkReceiverHttpLib::~NetworkReceiverHttpLib()
    {

    }

    void NetworkReceiverHttpLib::init()
    {
        m_Server.Post("/", [](const auto& req, auto& res)
        {
            std::cout << req.body() << std::endl;
            res.set_content("ok", "text/plain");
        });

        m_Server.listen("localhost", 80);
    }

    void NetworkReceiverHttpLib::run()
    {

    }

    const char* NetworkReceiverHttpLib::getJsonString()
    {
        return nullptr;
    }
}
