#include "mymain.h"
#include <sstream>

using namespace std;
using namespace cbs;

MyMain * g_main;

MyMain::MyMain()
	:Main(800, 600)
{
	g_main = this;

	m_rotation = 0.f;

	//m_tiny = Model("models/Ogre/TheThing/Mesh.mesh.xml");
	m_tiny = Model("res/tiny_4anim.x");
	m_tiny.setAnimationTPS(4800.0);
	m_tinyTime = 0;

	//m_dwarf = Model("models-nonbsd/MD5/Bob.md5mesh");
	m_dwarf = Model("models-nonbsd/X/dwarf.x");
	m_dwarfTime = 0;
	
	m_fish = Model("models-nonbsd/Ogre/OgreSDK/fish.mesh.xml");
	m_fishTime = 0;
}

void MyMain::myLoop()
{
	aiMatrix4x4 mTmp;
	aiMatrix4x4 mRes;
	aiMatrix4x4 mCommon;

	m_rotation += getDelta() * XM_PI*2.f / 10.f;
	
	mCommon *= aiMatrix4x4::RotationY(XM_PI / 5, mTmp);
	mCommon *= aiMatrix4x4::RotationZ(m_rotation, mTmp);

	// Ÿ�̴� ������
	mRes = aiMatrix4x4::Translation(vec(0.f, 300.f, 200.f), mTmp);
	mRes *= mCommon;
	drawModel(&m_tiny, &m_tinyTime, mRes);

	// ����� ������
	mRes = aiMatrix4x4::Translation(vec(0.f, -300.f, 200.f), mTmp);
	mRes *= mCommon;
	mRes *= aiMatrix4x4::RotationX(-XM_PI / 2, mTmp);
	mRes *= aiMatrix4x4::Scaling(vec(8.f), mTmp);
	drawModel(&m_dwarf, &m_dwarfTime, mRes);

	// ����� ������
	mRes = aiMatrix4x4::Translation(vec(0.f, 0.f, 200.f), mTmp);
	mRes *= aiMatrix4x4::Scaling(vec(30.f), mTmp);
	mRes *= mCommon;
	mRes *= aiMatrix4x4::RotationX(-XM_PI / 2, mTmp);
	drawModel(&m_fish, &m_fishTime, mRes);
}

void MyMain::drawModel(cbs::Model * model, double * time, aiMatrix4x4 matrix)
{
	// �ִϸ��̼� ���
	Model::Pose pose;
	Model::AnimationStatus status;
	status.animation = model->getAnimation(0); // �ִϸ��̼� ��������
	status.time = *time + getDelta(); // �ִϸ��̼� �ð� �̵�
	pose.set(&status); // ���� ��������
	*time = status.time; // �ִϸ��̼� �ð� ����
	pose.transform(matrix); // ���� ��� ����
	render(*model, pose); // ������
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int)
{
	cbs::g_hInst = hInstance;
	try
	{
		MyMain main;
		return main.messageLoop(); // �޼��� ����
	}
	catch (cbs::DXException & ex)
	{
		wstringstream ss;
		ss << ex.filename << L'(' << dec << ex.line << L"): ������ ����. (�����ڵ�: 0x" << hex << ex.hr << L')' << endl;
		OutputDebugString(ss.str().c_str());

		ss.str(L"");
		ss << L"������ ����. \r\n�����ڵ�: 0x" << hex << ex.hr;
		MessageBox(nullptr, ss.str().c_str(), nullptr, MB_OK | MB_ICONERROR);
		return (int)ex.hr;
	}
}
