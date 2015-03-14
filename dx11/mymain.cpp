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
	// 애니메이션 재생
	Model::Pose pose;
	Model::AnimationStatus status;
	
	status.animation = m_model.getAnimation(0); // 애니메이션 가져오기
	status.time = m_time + g_main->getDelta(); // 애니메이션 시간 이동
	pose.set(&status); // 포즈 가져오기
	m_time = status.time; // 애니메이션 시간 보정
	pose.transform(matrix); // 월드 행렬 설정
	g_main->render(m_model, pose); // 렌더링
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
	// 카메라 행렬 설정
	XMMATRIX mVP = XMMatrixLookAtLH(vec(-800.f, 0.f, -400.f), vec(0, 0, 0), vec(0, 0, -1));
	mVP *= XMMatrixPerspectiveFovLH(XM_PI / 3, (float)getWidth() / getHeight(), 50.f, 10000.f);
	setViewProjection(mVP);


	XMMATRIX mRes;

	// 타이니 렌더링
	mRes = XMMatrixRotationZ(-XM_PI / 2);
	mRes *= XMMatrixTranslation(0.f, 450.f, 200.f);
	m_tiny.draw(mRes);

	// 밥 렌더링
	mRes = XMMatrixRotationX(XM_PI);
	mRes *= XMMatrixRotationZ(XM_PI / 2);
	mRes *= XMMatrixScaling(8.f, 8.f, 8.f);
	mRes *= XMMatrixTranslation(0.f, 150.f, 200.f);
	m_bob.draw(mRes);

	// 드워프 렌더링
	mRes = XMMatrixRotationX(-XM_PI / 2);
	mRes *= XMMatrixRotationZ(XM_PI / 2);
	mRes *= XMMatrixScaling(8.f, 8.f, 8.f);
	mRes *= XMMatrixTranslation(0.f, -150.f, 200.f);
	m_dwarf.draw(mRes);

	// 물고기 렌더링
	mRes = XMMatrixRotationX(-XM_PI / 2);
	mRes *= XMMatrixScaling(30.f, 30.f, 30.f);
	mRes *= XMMatrixTranslation(0.f, -450.f, 200.f);
	m_fish.draw(mRes);
	
	// ninja 렌더링
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
		return main.messageLoop(); // 메세지 루프
	}
	catch (cbs::DXException & ex)
	{
		wstringstream ss;
		ss << ex.filename << L'(' << dec << ex.line << L"): 오류가 낫소. (오류코드: 0x" << hex << ex.hr << L')' << endl;
		OutputDebugString(ss.str().c_str());

		ss.str(L"");
		ss << L"오류가 났소. \r\n오류코드: 0x" << hex << ex.hr << L"\r\n오류 라인:" << ex.lineText;
		MessageBox(nullptr, ss.str().c_str(), nullptr, MB_OK | MB_ICONERROR);
		return (int)ex.hr;
	}
}
