#include "include/cbs/d3d11/common.h"

HINSTANCE cbs::g_hInst;

cbs::vec::vec()
{
}
cbs::vec::vec(float x)
{
	m_vector.m128_f32[0] = x;
	m_vector.m128_f32[1] = x;
	m_vector.m128_f32[2] = x;
	m_vector.m128_f32[3] = 1.f;
}
cbs::vec::vec(float x, float y, float z, float w )
{
	m_vector.m128_f32[0] = x;
	m_vector.m128_f32[1] = y;
	m_vector.m128_f32[2] = z;
	m_vector.m128_f32[3] = w;
}

cbs::vec::operator const XMVECTOR&() const
{
	return m_vector;
}
cbs::vec::operator const aiVector3D&() const
{
	return (aiVector3D&)m_vector;
}