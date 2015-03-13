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
	cbs::Model			m_model; // ��
	double				m_time; // �ִϸ��̼� ���� ����
};

class MyMain:public Main
{
public:
	MyMain();

	virtual void myLoop(); // override

private:

	MyModel m_tiny; // Ÿ�̴�
	MyModel m_bob; // ��
	MyModel m_dwarf; // �����
	MyModel m_fish; // �����
	MyModel m_ninja; // ninja
};

extern MyMain * g_main;