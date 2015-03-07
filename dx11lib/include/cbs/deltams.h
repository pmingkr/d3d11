#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace cbs
{

	/*
	�ð� ������ ������ִ� Ŭ����
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
