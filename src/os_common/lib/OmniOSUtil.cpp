#include "OmniOSUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int processRun(const char* command, bool print, char* buffer, size_t *len)
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

        size_t buf_len = 0;
        if(buffer && len && *len > 0)
            buffer[0] = '\0';

        while(fgets(buf, sizeof(buf), pipe) != nullptr)
        {
            if(print)
                LOG << buf;
            if(buffer && len)
            {
                size_t tmp = strlen(buffer);
                if(tmp + buf_len >= *len)
                    continue;
                strcat(buffer, buf);
                buf_len += tmp;
            }
        }

        if(len)
            *len = buf_len;

        int status = OMNI_PCLOSE(pipe);
        int ret = WEXITSTATUS(status);
        if(print)
            LOG << F("\nprocess returned: ") << ret << Logger::endl;
        return ret;
    }
}
