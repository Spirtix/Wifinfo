#if defined(_WIN32)
#include <Windows.h>
#define NativeHandle HWND
#else
#define NativeHandle void*
#endif