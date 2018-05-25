#ifndef OMNI_NETWORKRECEIVER_H
#define OMNI_NETWORKRECEIVER_H

namespace omni
{
    class NetworkReceiver
    {
        private:
        protected:
        public:
            virtual ~NetworkReceiver();

            virtual void init() = 0;
            virtual void run()  = 0;
            virtual const char* getJsonString() = 0; //should return null if no new data

    };
}
#endif
