#ifndef _Trace_H_
#define _Trace_H_
//#include "..\公共组件\服务核心\ServiceCoreHead.h"
#include "..\服务器组件\内核引擎\TraceService.h"

// 调试输出
#ifdef D_TRACE_OPEN

inline void D_TRACE(const TCHAR*fmt, ...)
{
	TCHAR szOutput[1024];
	va_list ap;
	va_start(ap, fmt);
	int nLen = _vsntprintf_s(szOutput, 1024, fmt, ap);
	va_end(ap);
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	std::wostringstream wos2;
	wos2 <<L"\r\n"<<L'[' <<  curTime.wMonth<<L"-"<< curTime.wDay <<L" " <<curTime.wHour<<L":"<<curTime.wMinute<<L":"<<curTime.wSecond << L']' <<szOutput ;

	CTraceService::TraceString(wos2.str().c_str(), TraceLevel_Debug); 
} // D_TRACE

#else
#define D_TRACE
#endif // D_TRACE_OPEN

// 一般信息输出
inline void I_TRACE(const TCHAR*fmt, ...)
{
 	TCHAR szOutput[1024];
 	va_list ap;
 	va_start(ap, fmt);
 	int nLen = _vsntprintf_s(szOutput, 1024, fmt, ap);
 	va_end(ap);
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	std::wostringstream wos2;
	wos2 <<L"\r\n"<<L'[' <<  curTime.wMonth<<L"-"<< curTime.wDay <<L" " <<curTime.wHour<<L":"<<curTime.wMinute<<L":"<<curTime.wSecond << L']' <<szOutput ;

	CTraceService::TraceString(wos2.str().c_str(), TraceLevel_Info); 
} // I_TRACE

// 警告信息输出
inline void W_TRACE(const TCHAR*fmt, ...)
{
	TCHAR szOutput[1024];
	va_list ap;
	va_start(ap, fmt);
	int nLen = _vsntprintf_s(szOutput, 1024, fmt, ap);
	va_end(ap);
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	std::wostringstream wos2;
	wos2 <<L"\r\n"<<L'[' <<  curTime.wMonth<<L"-"<< curTime.wDay <<L" " <<curTime.wHour<<L":"<<curTime.wMinute<<L":"<<curTime.wSecond << L']' <<szOutput ;

	CTraceService::TraceString(wos2.str().c_str(), TraceLevel_Warning); 
} // W_TRACE

#endif // _Trace_H_


////c99才支持
//#define D_TRACE(fmt, ...) \
//{ \
//	TCHAR szOutput[1024]; \
//	_sntprintf(szOutput, 1024, fmt, __VA_ARGS__); \
//	CTraceService::TraceString(szOutput, TraceLevel_Info); \
//}