#include "encoding.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>

size_t cbs::cnvcpy(char * dest, size_t limit, const wchar_t * src, int cp)
{
	assert(limit != 0);
	if(limit >= 0x7fffffff) limit = 0x7fffffff;
	size_t len = WideCharToMultiByte(cp, 0, src, wcslen(src), dest, limit - 1, nullptr, nullptr);
	dest[len] = '\0';
	return len;
}
size_t cbs::cnvcpy(wchar_t * dest, size_t limit, const char * src, int cp)
{
	assert(limit != 0);
	if (limit >= 0x7fffffff) limit = 0x7fffffff;
	size_t len = MultiByteToWideChar(cp, 0, src, strlen(src), dest, limit-1);
	dest[len] = '\0';
	return len;
}
size_t cbs::cnvlen(const wchar_t * src, int cp)
{
	return WideCharToMultiByte(cp, 0, src, wcslen(src), nullptr, 0, nullptr, nullptr);
}
size_t cbs::cnvlen(const char * src, int cp)
{
	return MultiByteToWideChar(cp, 0, src, strlen(src), nullptr, 0);
}
