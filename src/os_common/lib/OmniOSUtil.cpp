#include "OmniOSUtil.h"

#include <stdio.h>
#include <stdlib.h>

#include "OmniThing.h"
#include "Logger.h"

#ifdef OMNI_PLAT_LINUX
    #define OMNI_POPEN popen
    #define OMNI_PCLOSE pclose
#elif defined(OMNI_PLAT_WINDOWS)
    #define OMNI_POPEN _popen
    #define OMNI_PCLOSE _pclose
    #define WEXITSTATUS(x) (x)
#endif


namespace omni
{
    int processRun(const char* command, bool print)
    {
        char buf[512];
        FILE* pipe = OMNI_POPEN(command, "r");
        if(!pipe)
        {
            LOG << F("ERROR: Failed to execute command: ") << command << Logger::endl;
            return -1;
        }

        if(print)
            LOG << F("Executing command: ") << command << Logger::endl;

        while(fgets(buf, sizeof(buf), pipe) != nullptr)
        {
            if(print)
                LOG << buf;
        }

        int status = OMNI_PCLOSE(pipe);
        int ret = WEXITSTATUS(status);
        if(print)
            LOG << F("\nprocess returned: ") << ret << Logger::endl;
        return ret;
    }
}
