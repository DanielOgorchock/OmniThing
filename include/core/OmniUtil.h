#ifndef OMNI_OMNIUTIL_H
#define OMNI_OMNIUTIL_H


namespace omni
{
    unsigned long long getMillis();
    void sleepMillis(unsigned long long ms);
    void sleepMillisBusy(unsigned long long ms);

    unsigned long long getMicros();
    void sleepMicros(unsigned long long us);
    void sleepMicrosBusy(unsigned long long us);
}


#endif
