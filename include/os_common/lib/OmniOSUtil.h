#ifndef OMNI_OMNIOSUTIL_H
#define OMNI_OMNIOSUTIL_H

#include <cstdlib>

namespace omni
{
    int processRun(const char* command, bool print, char* buffer = nullptr, size_t *len = nullptr);
}

#endif
