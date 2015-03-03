#pragma once

#include <cbs/d3d11.h>

class Main :public cbs::D3D11Device, public cbs::Assimp, public cbs::ModelRenderer
{
public:
	Main();
	~Main();

	void loop(); // override
	void setMaterial(const cbs::Material & mtl); // override
	void setWorld(const aiMatrix4x4 & matrix); // override

private:

	cbs::Buffer m_vbQuad; // ���� ����
	cbs::Buffer m_constbuffer; // ���̴� ��� ����

	cbs::VertexShader		m_vs; // ���� ���̴�
	cbs::PixelShader		m_ps; // �ȼ� ���̴�
	cbs::SamplerState		m_sam; // �ؽ�ó ���÷�
	cbs::DepthStencilState	m_depth; // ���� ���ٽ� ������Ʈ

	cbs::Model			m_model; // ��

	DWORD m_startTick;
};
