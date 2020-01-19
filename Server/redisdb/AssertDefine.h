/*
	文件	：AssertDefine.h
	简介	：服务器异常处理的宏定义，请注意在每个函数的开头加上，在结尾时加上，这样每当有Assert时将会加快追逐的速度。

	---------------------------------------------------------------------------------------------
*/

#ifndef __ASSERTDEFINE_H__
#define __ASSERTDEFINE_H__

#include "Lock.h"
#include "Singleton.h"
#include "IThread.h"
#include "LogSystem.h"
 
#define ASSERT_RETURN(cond, ret) \
	{ \
	if(!cond) return ret; \
	}

#ifdef  WIN32
	#define Assert(expr)               if(!(expr)) {                                 \
		CHAR szBuff[MAX_LOG_LINE] = {0};        \
		_snprintf(szBuff, MAX_LOG_LINE-1, "Assert in:%s; File:%s:%d; Function:%s", #expr, __FILE__, __LINE__, __FUNCTION__);    \
		CLogSystem::AssertMessageBox(szBuff);                \
		DiskLog_ERROR(LOG_ASSERT, szBuff );throw 0;}

	#define AssertEx(expr, assertMsg)		if(!(expr)) {                           \
		CHAR szBuff[MAX_LOG_LINE] = {0};        \
		_snprintf(szBuff, MAX_LOG_LINE-1, "Assert in:%s; File:%s:%d; Function:%s; Reason:%s", #expr, __FILE__, __LINE__, __FUNCTION__, assertMsg);    \
		CLogSystem::AssertMessageBox(szBuff);                \
		DiskLog_ERROR(LOG_ASSERT, szBuff );throw 0;}	
#else

	#define Assert(expr)				if(!(expr)) { DiskLog_ERROR(LOG_ASSERT, " Assert in:"#expr"; File:%s:%d; Function:%s\n%s", __FILE__, __LINE__, __FUNCTION__, CLogSystem::GetInstancePtr()->GetStackString().c_str() );throw 0;}
	#define AssertEx(expr, assertMsg)	if(!(expr)) { DiskLog_ERROR(LOG_ASSERT, " Assert in:"#expr"; File:%s:%d; Function:%s; Reason:%s\n%s", __FILE__, __LINE__, __FUNCTION__, assertMsg, CLogSystem::GetInstancePtr()->GetStackString().c_str());throw 0;}
	#define AssertExcept(expr)			if(!(expr)) { DiskLog_ERROR(LOG_ASSERT, "Stack: File:%s:%d; Function:%s", __FILE__, __LINE__, __FUNCTION__ );throw;}

#endif

#define AssertExcept(expr)         if(!(expr)) { DiskLog_ERROR(LOG_ASSERT, "Stack: File:%s:%d; Function:%s", __FILE__, __LINE__, __FUNCTION__ );throw;}


#endif // __ASSERTDEFINE_H__
