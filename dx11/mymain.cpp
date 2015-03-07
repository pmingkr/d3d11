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

	m_tiny = Model("res/tiny_4anim.x");
	m_tiny.setAnimationTPS(4800.0);
	m_tinyTime = 0;

	m_dwarf = Model("models-nonbsd/X/dwarf.x");
	m_dwarfTime = 0;

	m_bob = Model("models-nonbsd/MD5/Bob.md5mesh");
	m_bobTime = 0;
	
	m_fish = Model("models-nonbsd/Ogre/OgreSDK/fish.mesh.xml");
	m_fishTime = 0;
}

void MyMain::myLoop()
{
	// 카메라 행렬 설정
	{
		XMMATRIX mVP = XMMatrixLookAtLH(vec(-800.f, 0.f, -400.f), vec(0, 0, 0), vec(0, 0, -1));
		mVP *= XMMatrixPerspectiveFovLH(XM_PI / 3, (float)getWidth() / getHeight(), 50.f, 10000.f);
		setViewProjection(mVP);
	}

	XMMATRIX mRes;

	m_rotation += getDelta() * XM_PI*2.f / 10.f;
	XMMATRIX mCommon = XMMatrixRotationZ(m_rotation);
	 
	// 타이니 렌더링
	mRes = mCommon;
	mRes *= XMMatrixTranslation(0.f, 450.f, 200.f);
	drawModel(&m_tiny, &m_tinyTime, mRes);

	// 밥 렌더링
	mRes = XMMatrixScaling(8.f, 8.f, 8.f);
	mRes *= XMMatrixRotationX(XM_PI);
	mRes *= mCommon;
	mRes *= XMMatrixTranslation(0.f, 150.f, 200.f);
	drawModel(&m_bob, &m_bobTime, mRes);

	// 드워프 렌더링
	mRes = XMMatrixScaling(8.f, 8.f, 8.f);
	mRes *= XMMatrixRotationX(-XM_PI / 2);
	mRes *= mCommon;
	mRes *= XMMatrixTranslation(0.f, -150.f, 200.f);
	drawModel(&m_dwarf, &m_dwarfTime, mRes);

	// 물고기 렌더링
	mRes = XMMatrixScaling(30.f, 30.f, 30.f);
	mRes *= XMMatrixRotationX(-XM_PI / 2);
	mRes *= mCommon;
	mRes *= XMMatrixTranslation(0.f, -450.f, 200.f);
	drawModel(&m_fish, &m_fishTime, mRes);
}

void MyMain::drawModel(cbs::Model * model, double * time, const XMMATRIX& matrix)
{
	// 애니메이션 재생
	Model::Pose pose;
	Model::AnimationStatus status;
	
	status.animation = model->getAnimation(0); // 애니메이션 가져오기
	status.time = *time + getDelta(); // 애니메이션 시간 이동
	pose.set(&status); // 포즈 가져오기
	*time = status.time; // 애니메이션 시간 보정
	pose.transform(matrix); // 월드 행렬 설정
	render(*model, pose); // 렌더링
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
