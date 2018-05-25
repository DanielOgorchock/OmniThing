#ifndef OMNI_CLOUDRECEIVER_H
#define OMNI_CLOUDRECEIVER_H

namespace omni
{
    class CloudReceiver
    {
        private:
        protected:
        public:
            void init() = 0;
            void run()  = 0;
            const char* getJsonString() = 0; //should return null if no new data

    };
}
#endif
