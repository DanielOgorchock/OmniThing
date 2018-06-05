#if defined(ARDUINO_ARCH_ESP32)

#include "NetworkSender.h"
#include "NetworkReceiver.h"
#include "WiFi.h"
#include "ObjectConfig.h"
#include "ESP32WebServer.h"

#define OMNI_ESP_JSON_BUF_SIZE 2000

namespace omni
{
    class NetworkManagerEsp32 : public NetworkSender, public NetworkReceiver
    {
        private:
            NetworkManagerEsp32();
            NetworkManagerEsp32(const NetworkManagerEsp32&);

            void waitUntilConnected();
            void serverRun();
            static void handleConnection();

            static void scanForCredentials(const char* json);

            char m_JsonBuf[OMNI_ESP_JSON_BUF_SIZE];
            char m_DestIp[50];
            unsigned short m_nDestPort;
            ESP32WebServer* m_WebServer;
            bool m_bClearBuffer;

        protected:
        public:
            virtual ~NetworkManagerEsp32();

            static NetworkManagerEsp32& getInstance();

            static void printDebug();

            void setCredentials(const char* ssid, const char* passwd);

            void setDestIp(const char* ip);
            void setDestPort(unsigned int port);

            void setServerPort(unsigned int port);

    // Network Receiver Functions
            virtual void init()
            {
                static bool first = true;
                if(!first)
                    return;

                first = false; 

                waitUntilConnected();

                if(m_WebServer)
                {
                    LOG << "Starting web server\n";
                    m_WebServer->on("/", handleConnection);
                    m_WebServer->begin();
                }
                else
                    LOG << "ERROR: web server not configured\n";

                printDebug();
            }

            virtual void run()
            {
                serverRun(); 
            }

            virtual const char* getJsonString();


            virtual void sendJson(const char* json);


    // Configuration
            static NetworkReceiver* createReceiverFromJson(const char* json);
            static NetworkSender* createSenderFromJson(const char* json);

            static const char* Type;
            static ObjectConfig<NetworkReceiver> NetworkReceiverConf;
            static ObjectConfig<NetworkSender> NetworkSenderConf;


    };
}

#endif
