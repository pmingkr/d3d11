#pragma once

#include "main.h"

class MyModel
{
public:
	MyModel();
	void load(const char * filename);
	void setAnimationTPS(double animationTime);
	void draw(const XMMATRIX& matrix);

private:
	cbs::Model			m_model; // 모델
	double				m_time; // 애니메이션 진행 변수
};

class MyMain:public Main
{
public:
	MyMain();

	virtual void myLoop(); // override

private:

	MyModel m_tiny; // 타이니
	MyModel m_bob; // 밥
	MyModel m_dwarf; // 드워프
	MyModel m_fish; // 물고기
	MyModel m_ninja; // ninja
};

extern MyMain * g_main;