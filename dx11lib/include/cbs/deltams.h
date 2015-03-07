#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace cbs
{

	/*
	시간 간격을 계산해주는 클래스
	*/
	class DeltaMeasure
	{
	public:
		DeltaMeasure();

		double measureDelta();

	private:
		LARGE_INTEGER m_tick;
		LARGE_INTEGER m_freq;
	};

}
