#if defined(ARDUINO_ARCH_ESP32)

#include "NetworkSender.h"
#include "NetworkReceiver.h"
#include "WiFi.h"
#include "ObjectConfig.h"

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

            static void scanForCredentials(const char* json);

            char m_JsonBuf[OMNI_ESP_JSON_BUF_SIZE];
            char m_DestIp[50];
            unsigned short m_nDestPort;
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

                //TODO: init webserver

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
