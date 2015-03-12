#pragma once

#ifdef _MSC_VER

#ifdef DX11LIB_STATIC_LIBRARY

#define CBS_DX11LIB_EXPORT

#else

#ifdef CBS_DX11_LIBRARY
#define CBS_DX11LIB_EXPORT __declspec(dllexport)
#else
#define CBS_DX11LIB_EXPORT  __declspec(dllimport)
#endif

#endif

#define CBS_DEPRECATED __declspec(deprecated)

#else

#define DEPRECATED
#define CBS_DX11LIB_EXPORT

#endif