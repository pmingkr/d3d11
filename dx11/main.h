#pragma once

#include "dx11.h"

class Main :public cbs::D3D11Device
{
public:
	Main();
	~Main();

	void loop();

private:

	cbs::Buffer m_vbQuad; // 정점 버퍼
	cbs::Buffer m_constbuffer; // 셰이더 상수 버퍼

	cbs::VertexShader	m_vs; // 정점 셰이더
	cbs::PixelShader	m_ps; // 픽셀 셰이더

	cbs::Texture		m_tex; // 텍스처
	cbs::SamplerState	m_sam; // 텍스처 샘플러
};
