#include "main.h"
#include <sstream>
#include <iomanip>

using namespace std;

int forSEH()
{
	try
	{
		Main main;
		int res = main.messageLoop();
		_CrtDumpMemoryLeaks();
		return res;
	}
	catch (cbs::DXException & ex)
	{
		wstringstream ss;
		ss << L"������ ����. \r\n�����ڵ�: 0x" << hex << (HRESULT)ex;
		MessageBox(nullptr, ss.str().c_str(), nullptr, MB_OK | MB_ICONERROR);
		return (int)(HRESULT)ex;
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
