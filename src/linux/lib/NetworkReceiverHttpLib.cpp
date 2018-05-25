#include "NetworkReceiverHttpLib.h"

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
            res.set_content("ok", "text/plain");
        });
    }

    void NetworkReceiverHttpLib::run()
    {

    }

    const char* NetworkReceiverHttpLib::getJsonString()
    {
        return nullptr;
    }
}
