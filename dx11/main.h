#pragma once

#define DX11LIB_STATIC_LIBRARY
#include <cbs/d3d11.h>
#include <cbs/deltams.h>

class Main :public cbs::D3D11Device, public cbs::Assimp, public cbs::ModelRenderer
{
public:
	Main(int width, int height);
	~Main();

	virtual void myLoop() = 0;
	void loop(); // override
	void setViewProjection(const XMMATRIX & matrix);
	void setMaterial(const cbs::Material & mtl); // override
	void setWorld(const XMMATRIX & matrix); // override
	void setBoneWorlds(const cbs::Matrix4x3 * matrix, size_t m4x3count); // override

	float getDelta();
	int getWidth();
	int getHeight();

private:

	int						m_width, m_height; // ȭ�� ũ��

	cbs::Buffer				m_cb_vsCommon; // ���� ���̴� ���� ��� ����
	cbs::Buffer				m_cb_vsBasic; // ���� ���̴� ��� ����
	cbs::Buffer				m_cb_vsSkinned; // ���� ���̴� ��� ����
	cbs::Buffer				m_cb_psBasic; // �ȼ� ���̴� ��� ����

	cbs::VertexShader		m_vs_basic; // ���� ���̴�
	cbs::VertexShader		m_vs_skinned; // ��Ų ���� ���̴�
	cbs::PixelShader		m_ps_basic; // �ȼ� ���̴�
	cbs::PixelShader		m_ps_noTexture; // �ؽ�ó ���� �ȼ� ���̴�
	cbs::SamplerState		m_sam; // �ؽ�ó ���÷�
	cbs::DepthStencilState	m_depth; // ���� ���ٽ� ������Ʈ

	float					m_delta;
	cbs::DeltaMeasure		m_deltaMeasure; // ��Ÿ ������
};
