#include "main.h"
#include <sstream>
#include <iomanip>

using namespace std;

int forSEH()
{
	try
	{
		Main main;
		int res = main.messageLoop(); // 메세지 루프
		_CrtDumpMemoryLeaks(); // 메모리 누수 감지
		return res;
	}
	catch (cbs::DXException & ex)
	{
		wstringstream ss;
		ss << ex.filename << L'(' << dec << ex.line << L"): 오류가 낫소. (오류코드: 0x" << hex << ex.hr << L')' << endl;
		OutputDebugString(ss.str().c_str());

		ss.str(L"");
		ss << L"오류가 났소. \r\n오류코드: 0x" << hex << ex.hr;
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
		MessageBox(nullptr, L"죽었소 ㅠㅠ", nullptr, MB_OK | MB_ICONERROR);
		return -1;
	}
}
