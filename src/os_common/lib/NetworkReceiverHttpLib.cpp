#include "NetworkReceiverHttpLib.h"

#include <string>
#include <thread>
#include <string.h>

#include "Logger.h"
#include "OmniUtil.h"

#include "frozen.h"

namespace omni
{
//private
    void NetworkReceiverHttpLib::listenerThread()
    {
        LOG << "Starting http server thread...\n";

        m_Server.Post("/", [this](const auto& req, auto& res)
        {
            std::string body(req.body);

            m_BufferMutex.lock();

            LOG << "Received message: " << body.c_str() << "\n";
            res.set_content("ok\n", "text/plain");

            strcpy(m_JsonBuffer, body.c_str());

            m_BufferMutex.unlock();
        });

        while(true)
        {
            LOG << "Starting http server...\n";
            if (!m_Server.listen("0.0.0.0", m_nPort))
            {
                LOG << "Failed to bind socket\n";
            }
            LOG << "Server Failed\n";

            sleepMillis(10000);
        }
    }

//protected
//public
    NetworkReceiverHttpLib::NetworkReceiverHttpLib(unsigned short port):
        m_Server(),
        m_nPort(port),
		m_bUnlockMutex(false),
        m_bWipeBuffer(false)
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

    NetworkReceiver* NetworkReceiverHttpLib::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        unsigned int port;

        if(json_scanf(json, len, "{port: %u}", &port) != 1)
        {
            return nullptr;
        }

        LOG << "port=" << port << Logger::endl;
        return new NetworkReceiverHttpLib(port);
    }

    const char* NetworkReceiverHttpLib::Type = "NetworkReceiverHttpLib";
    ObjectConfig<NetworkReceiver> NetworkReceiverHttpLib::NetworkReceiverConf(Type, createFromJson);

}
