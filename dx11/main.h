#pragma once

#include "dx11.h"

class Main :public cbs::D3D11Device
{
public:
	Main();
	~Main();

	void loop();

private:

	cbs::Buffer m_vbQuad; // ���� ����
	cbs::Buffer m_constbuffer; // ���̴� ��� ����

	cbs::VertexShader	m_vs; // ���� ���̴�
	cbs::PixelShader	m_ps; // �ȼ� ���̴�

	cbs::Texture		m_tex; // �ؽ�ó
	cbs::SamplerState	m_sam; // �ؽ�ó ���÷�
};
