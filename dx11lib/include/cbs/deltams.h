#pragma once

#include "dllconfig.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace cbs
{

	/*
	�ð� ������ ������ִ� Ŭ����
	*/
	class DeltaMeasure
	{
	private:
		LARGE_INTEGER m_tick;
		LARGE_INTEGER m_freq;

	public:
		inline DeltaMeasure()
		{
			QueryPerformanceFrequency(&m_freq);
			QueryPerformanceCounter(&m_tick);
		}
		inline double measureDelta()
		{
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			double delta = (double)(now.QuadPart - m_tick.QuadPart) / m_freq.QuadPart;
			m_tick = now;
			return delta;
		}
	};

}
