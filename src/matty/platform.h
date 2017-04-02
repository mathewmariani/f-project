#pragma once

// platform
#if defined(WIN32)
	#define OS_WINDOWS
#elif defined(__APPLE__)
	#define OS_MACOS
#elif defined(__linux__)
	#define OS_LINUX
#endif

// dll exports
#if defined(OS_WINDOWS)
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

// debug
#if (_DEBUG)
	#define DEBUG 1
#endif