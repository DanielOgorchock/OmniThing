#include "NetworkReceiverHttpLib.h"

#include <iostream>
#include <string>
#include <thread>
#include <string.h>

namespace omni
{
//private
    void NetworkReceiverHttpLib::listenerThread()
    {
        std::cout << "Starting http server thread...\n";

        m_Server.Post("/", [this](const auto& req, auto& res)
        {
            std::string body(req.body); 

            m_BufferMutex.lock();

            std::cout << (std::string("Received message: ") + body + "\n");
            res.set_content("ok\n", "text/plain");

            strcpy(m_JsonBuffer, body.c_str());

            m_BufferMutex.unlock();
        });

        std::cout << "Starting http server...\n";
		if (!m_Server.listen(m_IP, m_nPort))
		{
			std::cout << "Failed to bind socket\n";
		}
        std::cerr << "Server Failed\n";
    }

//protected
//public
    NetworkReceiverHttpLib::NetworkReceiverHttpLib(const char* ip, unsigned short port):
        m_Server(),
        m_IP(ip),
        m_nPort(port),
        m_bWipeBuffer(false),
		m_bUnlockMutex(false)
    {
        m_JsonBuffer[0] = 0; 
    }

    NetworkReceiverHttpLib::~NetworkReceiverHttpLib()
    {

    }

    void NetworkReceiverHttpLib::init()
    {
        m_Thread = new std::thread(&NetworkReceiverHttpLib::listenerThread, this);
    }

    void NetworkReceiverHttpLib::run()
    {
        if(m_bWipeBuffer)
        {
            m_JsonBuffer[0] = 0;
            m_bWipeBuffer = false;
        }

		if (m_bUnlockMutex)
		{
			m_bUnlockMutex = false;
			m_BufferMutex.unlock();
		}
    }

    const char* NetworkReceiverHttpLib::getJsonString()
    {
        if(m_JsonBuffer[0] == 0)
        {
            return nullptr;
        }

        m_BufferMutex.lock();
		m_bUnlockMutex = true;

        m_bWipeBuffer = true;
        return m_JsonBuffer;
    }
}
