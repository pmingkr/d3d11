#pragma once

#include "dx11.h"

class Main :public D3D11Device
{
public:
	Main();
	~Main();

	void loop();

private:

	Buffer m_vb;
	Buffer m_constbuffer;

	AutoRelease<ID3D11VertexShader>	m_vs;
	AutoRelease<ID3D11PixelShader>	m_ps;
	AutoRelease<ID3D11InputLayout>	m_layout;

	XMMATRIX m_viewmatrix;
};
