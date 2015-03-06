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

	cbs::Buffer				m_vbQuad; // 정점 버퍼
	cbs::Buffer				m_cb_vsCommon; // 정점 셰이더 공용 상수 버퍼
	cbs::Buffer				m_cb_vsBasic; // 정점 셰이더 상수 버퍼
	cbs::Buffer				m_cb_vsSkinned; // 정점 셰이더 상수 버퍼
	cbs::Buffer				m_cb_psBasic; // 픽셀 셰이더 상수 버퍼

	cbs::VertexShader		m_vs_basic; // 정점 셰이더
	cbs::VertexShader		m_vs_skinned; // 스킨 정점 셰이더
	cbs::PixelShader		m_ps_basic; // 픽셀 셰이더
	cbs::PixelShader		m_ps_noTexture; // 텍스처 없는 픽셀 셰이더
	cbs::SamplerState		m_sam; // 텍스처 샘플러
	cbs::DepthStencilState	m_depth; // 뎁스 스텐실 스테이트

	cbs::Model			m_model; // 모델/타이니
	double				m_modelTime; // 모델의 애니메이션 진행 변수

	cbs::Model			m_dwarf; // 모델/드워프
	double				m_dwarfTime; // 모델의 애니메이션 진행 변수

	DeltaMeasure		m_delta; // 델타 측정기
};
