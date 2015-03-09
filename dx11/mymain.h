#pragma once

#include "main.h"

class MyMain:public Main
{
public:
	MyMain();

	void myLoop(); // override
	void drawModel(cbs::Model * model, double * time, const XMMATRIX& matrix);

private:

	cbs::Model			m_tiny; // ��/Ÿ�̴�
	double				m_tinyTime; // �ִϸ��̼� ���� ����

	cbs::Model			m_dwarf; // ��/�����
	double				m_dwarfTime; // �ִϸ��̼� ���� ����

	cbs::Model			m_bob; // ��/��
	double				m_bobTime; // �ִϸ��̼� ���� ����

	cbs::Model			m_fish; // ��/�����
	double				m_fishTime; // �ִϸ��̼� ���� ����
};

extern MyMain * g_main;