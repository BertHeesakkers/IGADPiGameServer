#include "Stopwatch.h"

#include "AssertMessage.h"

#if defined(__linux__)
#include <sys/time.h>
#endif

Stopwatch::Stopwatch()
	: m_StartTime({ 0 })
	, m_LastQueryTime({ 0 })
#if defined(WIN32)
	, m_Frequency({ 0 })
#endif
	, m_FirstQuery(true)
	, m_Running(false)
{

}

Stopwatch::~Stopwatch()
{

}

void Stopwatch::Start()
{
	AssertMessage(!m_Running, "Attempt to start a stopwatch that is already running!");
#if defined(WIN32)
	::QueryPerformanceFrequency(&m_Frequency);
	::QueryPerformanceCounter(&m_StartTime);
#elif defined(__linux__)
	gettimeofday(&m_StartTime, NULL);
#endif

	m_Running = true;
	m_FirstQuery = true;
}

uint64_t Stopwatch::GetElapsedTime()
{
	AssertMessage(m_Running, "Attempt to retrieve elapsed time from a stopwatch that is not running!");
	uint64_t elapsedTime = 0;
	if (m_FirstQuery)
	{
#if defined(WIN32)
		::QueryPerformanceCounter(&m_LastQueryTime);
		elapsedTime = static_cast<uint64_t>((m_LastQueryTime.QuadPart - m_StartTime.QuadPart) * 1000.0 / m_Frequency.QuadPart);
#elif defined(__linux__)
		gettimeofday(&m_LastQueryTime, NULL);
		elapsedTime = static_cast<uint64_t>((m_LastQueryTime.tv_sec - m_StartTime.tv_sec) * 1000.0 + (m_LastQueryTime.tv_usec - m_StartTime.tv_usec) / 1000.0);
#endif
		m_FirstQuery = false;
	}
	else
	{
#if defined(WIN32)
		LARGE_INTEGER queryTime;
		::QueryPerformanceCounter(&queryTime);
		elapsedTime = static_cast<uint64_t>((queryTime.QuadPart - m_LastQueryTime.QuadPart) * 1000.0 / m_Frequency.QuadPart);
		m_LastQueryTime = queryTime;
#elif defined(__linux__)
		timeval queryTime;
		gettimeofday(&queryTime, NULL);
		elapsedTime = static_cast<uint64_t>((queryTime.tv_sec - m_LastQueryTime.tv_sec) * 1000.0 + (queryTime.tv_usec - m_LastQueryTime.tv_usec) / 1000.0);
		m_LastQueryTime = queryTime;
#endif
	}
	return elapsedTime;
}

// milliseconds
uint64_t Stopwatch::GetTimeSinceStart() const
{
	AssertMessage(m_Running, "Attempt to retrieve running time from a stopwatch that is not running!");
	uint64_t elapsedTime = 0;
#if defined(WIN32)
	LARGE_INTEGER currentTime;
	::QueryPerformanceCounter(&currentTime);
	elapsedTime = static_cast<uint64_t>((currentTime.QuadPart - m_StartTime.QuadPart) * 1000.0 / m_Frequency.QuadPart);
#elif defined(__linux__)
	timeval currentTime;
	gettimeofday(&currentTime, NULL);
	elapsedTime = static_cast<uint64_t>((currentTime.tv_sec - m_StartTime.tv_sec) * 1000.0 + (currentTime.tv_usec - m_StartTime.tv_usec) / 1000.0);
#endif
	return elapsedTime;
}

void Stopwatch::Stop()
{
	AssertMessage(m_Running, "Attempt to stop a stopwatch that is not running!");
	m_FirstQuery = true;
	m_Running = false;
}

void Stopwatch::Reset()
{
	AssertMessage(m_Running, "Attempt to reset a stopwatch that is not running!");
#if defined(WIN32)
	::QueryPerformanceCounter(&m_StartTime);
#elif defined(__linux__)
	gettimeofday(&m_StartTime, NULL);
#endif
	m_FirstQuery = true;
}

bool Stopwatch::IsRunning() const
{
	return m_Running;
}

void Stopwatch::WaitForTimePassed(double a_MilliSeconds)
{
	AssertMessage(m_Running, "Attempt to wait for time to pass for a stopwatch that is not running!");
	uint64_t elapsedTime = 0;
	if (m_FirstQuery)
	{
		do
		{
#if defined(WIN32)
			::QueryPerformanceCounter(&m_LastQueryTime);
			elapsedTime = static_cast<uint64_t>((m_LastQueryTime.QuadPart - m_StartTime.QuadPart) * 1000.0 / m_Frequency.QuadPart);
#elif defined(__linux__)
			gettimeofday(&m_LastQueryTime, NULL);
			elapsedTime = static_cast<uint64_t>((m_LastQueryTime.tv_sec - m_StartTime.tv_sec) * 1000.0 + (m_LastQueryTime.tv_usec - m_StartTime.tv_usec) / 1000.0);
#endif
		} while (elapsedTime < a_MilliSeconds);
		m_FirstQuery = false;
	}
	else
	{
#if defined(WIN32)
		LARGE_INTEGER queryTime;
		do
		{
			::QueryPerformanceCounter(&queryTime);
			elapsedTime = static_cast<uint64_t>((queryTime.QuadPart - m_LastQueryTime.QuadPart) * 1000.0 / m_Frequency.QuadPart);
		} while (elapsedTime < a_MilliSeconds);
		m_LastQueryTime = queryTime;
#elif defined(__linux__)
		timeval queryTime;
		do
		{
			gettimeofday(&queryTime, NULL);
			elapsedTime = static_cast<uint64_t>((queryTime.tv_sec - m_LastQueryTime.tv_sec) * 1000.0 + (queryTime.tv_usec - m_LastQueryTime.tv_usec) / 1000.0);
		} while (elapsedTime < a_MilliSeconds);
		m_LastQueryTime = queryTime;
#endif
	}
}
