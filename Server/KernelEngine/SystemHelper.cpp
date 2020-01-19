#include "StdAfx.h"
__declspec(thread) char thrd_g_msg[8192] = { 0 };

__declspec(dllexport)  void WINAPI HxVSDebugPrintA(LPCSTR _Format, va_list _Args) {
	memset(thrd_g_msg, 0, sizeof(thrd_g_msg));
	vsprintf(thrd_g_msg, _Format, _Args);
	OutputDebugStringA(thrd_g_msg);
}

__declspec(dllexport) void  WINAPI HxDebugPrintA(LPCSTR _Format, ...) {
	va_list args;
	va_start(args, _Format);
	HxVSDebugPrintA(_Format, args);
	va_end(args);
}
static DWORD HashString32(LPCSTR stringKey) {
	DWORD hash = 0;
	if (stringKey == NULL || stringKey[0] == 0)
		return 0;
	for (int i = 0; i < 33; i++) {
		char c = stringKey[i];
		if (c == 0) {
			break;
		}
		hash = (hash << 5) + hash + tolower(c);
	}
	return hash;
}

__declspec(dllexport) DWORD WINAPI HashStringA(LPCSTR _Format, ...) {
	CHAR str[33] = { 0 };
	va_list vp;
	va_start(vp, _Format);
	_vsnprintf(str, 33, _Format, vp);
	va_end(vp);
	return HashString32(str);
}
__declspec(dllexport) DWORD WINAPI HashStringW(LPCWSTR _Format, ...) {
	WCHAR str[33] = { 0 };
	va_list vp;
	va_start(vp, _Format);
	_vsnwprintf(str, 33, _Format, vp);
	va_end(vp);
	return HashString32(CW2A(str));
}