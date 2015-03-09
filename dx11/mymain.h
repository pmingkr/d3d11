#pragma once

#include "main.h"

class MyMain:public Main
{
public:
	MyMain();

	void myLoop(); // override
	void drawModel(cbs::Model * model, double * time, const XMMATRIX& matrix);

private:

	cbs::Model			m_tiny; // 모델/타이니
	double				m_tinyTime; // 애니메이션 진행 변수

	cbs::Model			m_dwarf; // 모델/드워프
	double				m_dwarfTime; // 애니메이션 진행 변수

	cbs::Model			m_bob; // 모델/밥
	double				m_bobTime; // 애니메이션 진행 변수

	cbs::Model			m_fish; // 모델/물고기
	double				m_fishTime; // 애니메이션 진행 변수
};

extern MyMain * g_main;