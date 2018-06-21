#ifndef OMNI_NETWORKMANAGERESP8266_H
#define OMNI_NETWORKMANAGERESP8266_H

#if defined(ARDUINO_ARCH_ESP8266)

#include "NetworkReceiver.h"
#include "NetworkSender.h"
#include "ObjectConfig.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define OMNI_ESP_JSON_BUF_SIZE 2000

namespace omni
{
    class NetworkManagerEsp8266 : public NetworkReceiver, public NetworkSender
    {
        private:
            NetworkManagerEsp8266();
            NetworkManagerEsp8266(const NetworkManagerEsp8266& m);

            void enableOTA();
            void waitUntilConnected();
            void serverRun();
            static void handleConnection();

            static void scanForCredentials(const char* json);

            char m_JsonBuf[OMNI_ESP_JSON_BUF_SIZE];
            char m_DestIp[50];
            unsigned short m_nDestPort;
            ESP8266WebServer* m_WebServer;
            bool m_bClearBuffer;

        protected:
        public:
            virtual ~NetworkManagerEsp8266();

            static NetworkManagerEsp8266& getInstance();

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

                enableOTA();
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
#endif
