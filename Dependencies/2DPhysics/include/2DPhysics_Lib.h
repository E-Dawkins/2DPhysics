#pragma once

#ifdef PHYSICSLIB_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif


PHYSICS_API void PrintTest();