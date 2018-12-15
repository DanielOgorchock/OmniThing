#include "OmniUtil.h"

#include <chrono>
#include <thread>

namespace omni
{
    unsigned long long getMillis()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void sleepMillis(unsigned long long ms)
    {
        std::this_thread::sleep_for(std::chrono::duration<unsigned long long, std::milli>(ms));
    }

    void sleepMillisBusy(unsigned long long ms)
    {
        unsigned long long start = getMillis();
        while((getMillis() - start) < ms)
        {
            continue;
        }
    }

    unsigned long long getMicros()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    void sleepMicros(unsigned long long us)
    {
        std::this_thread::sleep_for(std::chrono::duration<unsigned long long, std::micro>(us));
    }

    void sleepMicrosBusy(unsigned long long us)
    {
        unsigned long long start = getMicros();
        while((getMicros() - start) < us)
        {
            continue;
        }
    }
}
