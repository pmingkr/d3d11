#pragma once

#include <xnamath.h>

/*
	float 4개에서 XMVECTORF32를 만들어서 반환
*/
inline XMVECTORF32 vector(float x, float y, float z, float w = 1)
{
	XMVECTORF32 v;
	v.f[0] = x;
	v.f[1] = y;
	v.f[2] = z;
	v.f[3] = w;
	return v;
}