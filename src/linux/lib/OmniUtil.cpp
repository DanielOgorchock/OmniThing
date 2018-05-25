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
        std::this_thread::sleep_for(std::chrono::duration<time_unit, std::milli>(duration));
    }
}
