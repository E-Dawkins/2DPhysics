#pragma once

#ifdef DLL_EXPORT
#define 2DPHYSICS_API __declspec(dllexport)
#else
#define 2DPHYSICS_API __declspec(dllimport)
#endif