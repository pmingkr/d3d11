#pragma once

namespace cbs
{
	 size_t cnvcpy(char * dest, size_t limit, const wchar_t * src, int cp = 0);
	size_t cnvcpy(wchar_t * dest, size_t limit, const char * src, int cp = 0);
	size_t cnvlen(const wchar_t * src, int cp = 0);
	size_t cnvlen(const char * src, int cp = 0);

	template <size_t limit>
	size_t cnvcpy(char(&dest)[limit], const wchar_t * src, int cp = 0)
	{
		return cnvcpy(dest, limit, src, cp);
	}
	template <size_t limit>
	size_t cnvcpy(wchar_t(&dest)[limit], const char * src, int cp = 0)
	{
		return cnvcpy(dest, limit, src, cp);
	}
}