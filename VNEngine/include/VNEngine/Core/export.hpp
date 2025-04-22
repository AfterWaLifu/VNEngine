#pragma once

#ifdef BUILDING_LIB
#define VN_ENGINE_API __declspec(dllexport)
#else
#define VN_ENGINE_API __declspec(dllimport)
#endif