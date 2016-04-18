#ifndef TIMER_H
#define TIMER_H


#ifdef WIN32

#include <sys/timeb.h>

inline long long getTimeMs()
{
    struct timeb t;
    ftime(&t);
    return t.time * 1000ll + t.millitm;
}

#else

#include <sys/time.h>

inline long long getTimeMs()
{
    timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec *  1000ll) +  time.tv_usec / 1000;
}

#endif


#endif // TIMER_H
