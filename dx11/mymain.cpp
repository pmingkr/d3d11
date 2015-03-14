#include "mymain.h"
#include <sstream>

using namespace std;
using namespace cbs;

MyMain * g_main;

MyModel::MyModel()
{
	m_time = 0;
}
void MyModel::load(const char * filename)
{
	m_model = Model(filename);
	m_time = 0;
}
void MyModel::setAnimationTPS(double animationTime)
{
	m_model.setAnimationTPS(animationTime);
}
void MyModel::draw(const XMMATRIX& matrix)
{
	// �ִϸ��̼� ���
	Model::Pose pose;
	Model::AnimationStatus status;
	
	status.animation = m_model.getAnimation(0); // �ִϸ��̼� ��������
	status.time = m_time + g_main->getDelta(); // �ִϸ��̼� �ð� �̵�
	pose.set(&status); // ���� ��������
	m_time = status.time; // �ִϸ��̼� �ð� ����
	pose.transform(matrix); // ���� ��� ����
	g_main->render(m_model, pose); // ������
}

MyMain::MyMain()
	:Main(800, 600)
{
	g_main = this;

	m_tiny.load("res/tiny_4anim.x");
	m_tiny.setAnimationTPS(4800.0);

	m_dwarf.load("models-nonbsd/X/dwarf.x");
	m_bob.load("models-nonbsd/MD5/Bob.md5mesh");
	m_fish.load("models-nonbsd/Ogre/OgreSDK/fish.mesh.xml");
	m_ninja.load("models-nonbsd/Ogre/OgreSDK/ninja.mesh.xml");
}

void MyMain::myLoop()
{
	// ī�޶� ��� ����
	XMMATRIX mVP = XMMatrixLookAtLH(vec(-800.f, 0.f, -400.f), vec(0, 0, 0), vec(0, 0, -1));
	mVP *= XMMatrixPerspectiveFovLH(XM_PI / 3, (float)getWidth() / getHeight(), 50.f, 10000.f);
	setViewProjection(mVP);


	XMMATRIX mRes;

	// Ÿ�̴� ������
	mRes = XMMatrixRotationZ(-XM_PI / 2);
	mRes *= XMMatrixTranslation(0.f, 450.f, 200.f);
	m_tiny.draw(mRes);

	// �� ������
	mRes = XMMatrixRotationX(XM_PI);
	mRes *= XMMatrixRotationZ(XM_PI / 2);
	mRes *= XMMatrixScaling(8.f, 8.f, 8.f);
	mRes *= XMMatrixTranslation(0.f, 150.f, 200.f);
	m_bob.draw(mRes);

	// ����� ������
	mRes = XMMatrixRotationX(-XM_PI / 2);
	mRes *= XMMatrixRotationZ(XM_PI / 2);
	mRes *= XMMatrixScaling(8.f, 8.f, 8.f);
	mRes *= XMMatrixTranslation(0.f, -150.f, 200.f);
	m_dwarf.draw(mRes);

	// ����� ������
	mRes = XMMatrixRotationX(-XM_PI / 2);
	mRes *= XMMatrixScaling(30.f, 30.f, 30.f);
	mRes *= XMMatrixTranslation(0.f, -450.f, 200.f);
	m_fish.draw(mRes);
	
	// ninja ������
	mRes = XMMatrixRotationX(-XM_PI / 2);
	mRes *= XMMatrixRotationZ(-XM_PI / 2);
	mRes *= XMMatrixScaling(2.5f, 2.5f, 2.5f);
	mRes *= XMMatrixTranslation(-450.f, 0.f, 200.f);
	m_ninja.draw(mRes);
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
		ss << L"������ ����. \r\n�����ڵ�: 0x" << hex << ex.hr << L"\r\n���� ����:" << ex.lineText;
		MessageBox(nullptr, ss.str().c_str(), nullptr, MB_OK | MB_ICONERROR);
		return (int)ex.hr;
	}
}
