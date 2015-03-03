#include "main.h"
#include <sstream>
#include <iomanip>

using namespace std;

int forSEH()
{
	try
	{
		Main main;
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
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int)
{
	cbs::g_hInst = hInstance;
	__try
	{
		return forSEH();
	}
	__except (GetExceptionCode() != EXCEPTION_BREAKPOINT ? EXCEPTION_ACCESS_VIOLATION : EXCEPTION_EXECUTE_HANDLER)
	{
		MessageBox(nullptr, L"�׾��� �Ф�", nullptr, MB_OK | MB_ICONERROR);
		return -1;
	}
}

#pragma warning(disable:4074)
#pragma init_seg(compiler)

struct __StaticRun
{
	__StaticRun()
	{
		//_crtBreakAlloc = 252;
	}
	~__StaticRun()
	{
		_CrtDumpMemoryLeaks(); // �޸� ���� ����
	}
} __staticRun;
