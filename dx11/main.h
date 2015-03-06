#pragma once

#include <cbs/d3d11.h>
#include "deltams.h"
#include "commonutil.h"

class Main :public cbs::D3D11Device, public cbs::Assimp, public cbs::ModelRenderer
{
public:
	Main();
	~Main();

	void loop(); // override
	void setMaterial(const cbs::Material & mtl); // override
	void setWorld(const aiMatrix4x4 & matrix); // override
	void setBoneWorlds(const cbs::Matrix4x3 * matrix, size_t m4x3count); // override
	void drawModel(float delta, cbs::Model * model, double * time, aiMatrix4x4 matrix);

private:

	cbs::Buffer				m_vbQuad; // ���� ����
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

	cbs::Model			m_model; // ��/Ÿ�̴�
	double				m_modelTime; // ���� �ִϸ��̼� ���� ����

	cbs::Model			m_dwarf; // ��/�����
	double				m_dwarfTime; // ���� �ִϸ��̼� ���� ����

	DeltaMeasure		m_delta; // ��Ÿ ������
};
