#include "include/cbs/deltams.h"

cbs::DeltaMeasure::DeltaMeasure()
{
	QueryPerformanceFrequency(&m_freq);
	QueryPerformanceCounter(&m_tick);
}


double cbs::DeltaMeasure::measureDelta()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double delta = (double)(now.QuadPart - m_tick.QuadPart) / m_freq.QuadPart;
	m_tick = now;
	return delta;
}
