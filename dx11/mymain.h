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

	cbs::Model			m_tiny; // ��/Ÿ�̴�
	double				m_tinyTime; // �ִϸ��̼� ���� ����

	cbs::Model			m_dwarf; // ��/�����
	double				m_dwarfTime; // �ִϸ��̼� ���� ����

	cbs::Model			m_fish; // ��/�����
	double				m_fishTime; // �ִϸ��̼� ���� ����
};

extern MyMain * g_main;