#include "deltams.h"

DeltaMeasure::DeltaMeasure()
{
	QueryPerformanceFrequency(&m_freq);
	QueryPerformanceCounter(&m_tick);
}


double DeltaMeasure::measureDelta()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double delta = (double)(now.QuadPart - m_tick.QuadPart) / m_freq.QuadPart;
	m_tick = now;
	return delta;
}