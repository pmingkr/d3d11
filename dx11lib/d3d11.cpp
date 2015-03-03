#include "include/cbs/d3d11.h"
#include <fstream>


#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3d11.lib")

using namespace std;

using namespace cbs;

DXException::DXException(HRESULT hr)
	:hr(hr), filename(nullptr), line(0)
{
}
DXException::DXException(HRESULT hr, const wchar_t * filename, int line)
	:hr(hr), filename(filename), line(line)
{
}