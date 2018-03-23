#pragma once

#include <stdint.h>
#if defined(WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <sys/time.h>
#endif

class Stopwatch
{
public:
    Stopwatch();
    ~Stopwatch();
    
    void Start();
    uint64_t GetElapsedTime();
    uint64_t GetTimeSinceStart() const;
    void Stop();
    void Reset();
    bool IsRunning() const;
    
    void WaitForTimePassed(double a_MilliSeconds);
private:
#if defined(WIN32)
	LARGE_INTEGER m_StartTime;
    LARGE_INTEGER m_LastQueryTime;
    LARGE_INTEGER m_Frequency;
#elif defined (__linux__)
	timeval m_StartTime;
	timeval m_LastQueryTime;
#endif
	bool m_FirstQuery;
    bool m_Running;
};
