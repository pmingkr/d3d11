#pragma once

#include "main.h"

class MyMain:public Main
{
public:
	MyMain();

	void myLoop(); // override
	void drawModel(cbs::Model * model, double * time, aiMatrix4x4 matrix);

private:

	float				m_rotation;

	cbs::Model			m_tiny; // 모델/타이니
	double				m_tinyTime; // 애니메이션 진행 변수

	cbs::Model			m_dwarf; // 모델/드워프
	double				m_dwarfTime; // 애니메이션 진행 변수

	cbs::Model			m_fish; // 모델/물고기
	double				m_fishTime; // 애니메이션 진행 변수
};

extern MyMain * g_main;