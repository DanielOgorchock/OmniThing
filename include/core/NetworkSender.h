#ifndef OMNI_NETWORKSENDER_H
#define OMNI_NETWORKSENDER_H

namespace omni
{
    class NetworkSender
    {
        private:
        protected:
        public:
            virtual ~NetworkSender();

            virtual void init() = 0;
            virtual void run() = 0;
            virtual void sendJson(const char* json) = 0;
    };
}

#endif
