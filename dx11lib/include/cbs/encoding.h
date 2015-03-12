#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>

namespace cbs
{
	inline size_t cnvcpy(char * dest, size_t limit, const wchar_t * src, int cp = 0)
	{
		assert(limit != 0);
		if (limit >= 0x7fffffff) limit = 0x7fffffff;
		size_t sz = wcslen(src);
		assert(sz <= 0x7fffffff);
		size_t len = WideCharToMultiByte(cp, 0, src, (int)sz, dest, (int)limit - 1, nullptr, nullptr);
		dest[len] = '\0';
		return len;
	}
	inline size_t cnvcpy(wchar_t * dest, size_t limit, const char * src, int cp = 0)
	{
		assert(limit != 0);
		if (limit >= 0x7fffffff) limit = 0x7fffffff;
		size_t sz = strlen(src);
		assert(sz <= 0x7fffffff);
		size_t len = MultiByteToWideChar(cp, 0, src, (int)sz, dest, (int)limit - 1);
		dest[len] = '\0';
		return len;
	}
	inline size_t cnvlen(const wchar_t * src, int cp = 0)
	{
		size_t sz = wcslen(src);
		assert(sz <= 0x7fffffff);
		return WideCharToMultiByte(cp, 0, src, (int)sz, nullptr, 0, nullptr, nullptr);
	}
	inline size_t cnvlen(const char * src, int cp = 0)
	{
		size_t sz = strlen(src);
		assert(sz <= 0x7fffffff);
		return MultiByteToWideChar(cp, 0, src, (int)sz, nullptr, 0);
	}

	template <size_t limit>
	inline size_t cnvcpy(char(&dest)[limit], const wchar_t * src, int cp = 0)
	{
		return cnvcpy(dest, limit, src, cp);
	}
	template <size_t limit>
	inline size_t cnvcpy(wchar_t(&dest)[limit], const char * src, int cp = 0)
	{
		return cnvcpy(dest, limit, src, cp);
	}
}