#pragma once
#include <sstream>
#include <Windows.h>

enum { CSFC_LOG_MAX = 1024 };	// 日志最大长度。

enum LogLevel_	// 日志级别。
{
	LL_INFO,
	LL_WARN,
	LL_ERR,
};


template <LogLevel_> struct DebugLogLevel;

template <> struct DebugLogLevel<LL_INFO>
{
	static const wchar_t* MARK;
	static const wchar_t* ICON;
};

template <> struct DebugLogLevel<LL_WARN>
{
	static const wchar_t* MARK;
	static const wchar_t* ICON;
};

template <> struct DebugLogLevel<LL_ERR>
{
	static const wchar_t* MARK;
	static const wchar_t* ICON;
};


template <int> struct TraceServiceLevel { enum { LEVEL = LL_INFO }; };
template <> struct TraceServiceLevel<2> { enum { LEVEL = LL_WARN }; };
template <> struct TraceServiceLevel<3> { enum { LEVEL = LL_ERR  }; };

#define TRACESTRING(s,l) CTraceService::TraceString( s, (enTraceLevel)l );

template <LogLevel_ LEVEL>
class DebugLog : public std::wostringstream
{
public:
	DebugLog(const wchar_t* caption, int line, const wchar_t* file, const wchar_t* function, const wchar_t* date, const wchar_t* time, const wchar_t* type);
	~DebugLog(void);

public:
	void operator() (const wchar_t* format, ...);
private:
	const wchar_t* const m_caption;

	const int            m_line;
	const wchar_t* const m_file;
	const wchar_t* const m_function;
	const wchar_t* const m_date;
	const wchar_t* const m_time;
	const wchar_t* const m_type;
	SYSTEMTIME m_curTime;
};


template <LogLevel_ LEVEL>
DebugLog<LEVEL>::DebugLog(const wchar_t* caption, int line, const wchar_t* file, const wchar_t* function, const wchar_t* date, const wchar_t* time, const wchar_t* type)
	: m_caption(caption)
	, m_line(line)
	, m_file(file)
	, m_function(function)
	, m_date(date)
	, m_time(time)
	, m_type(type)
{
	GetLocalTime(&m_curTime);
}


template <LogLevel_ LEVEL>
DebugLog<LEVEL>::~DebugLog(void)
{
	std::wostringstream wos2;
 		//wos2 <<  str() <<  L" "  << m_function << " " << m_file <<L'(' << m_line << L"): ";
 		wos2 <<L"\r\n"<<L'[' <<  m_curTime.wMonth<<L"-"<< m_curTime.wDay <<L" " <<m_curTime.wHour<<L":"<<m_curTime.wMinute<<L":"<<m_curTime.wSecond << L']' << str() << L" "  << m_function << " " << m_file <<L'(' << m_line << L"): " ;

	TRACESTRING( wos2.str().c_str(), LEVEL);
}

template <LogLevel_ LEVEL>
void DebugLog<LEVEL>::operator() (const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);

	wchar_t buffer[CSFC_LOG_MAX] = { L'\0' };
	_vsnwprintf_s(buffer, CSFC_LOG_MAX, CSFC_LOG_MAX - 1, format, (va_list)args);
	*this << buffer;

	va_end(args);
}

#ifdef GAME_NAME
#	define DEFAULT_LOG_CAPTION L"────[" GAME_NAME L"]────"
#	define LOG_CAPTION DEFAULT_LOG_CAPTION
#	ifdef _WINDLL
#		define LOG_TYPE L"[Server]"
#	else
#		define LOG_TYPE L"[Client]"
#	endif
#else
#	ifndef LOG_CAPTION
#		define LOG_CAPTION L"───────────────"
#	endif
#	ifndef LOG_TYPE
#		define LOG_TYPE L"────"
#	endif
#endif


#define DEBUG_LOG(level) DebugLog<level>(LOG_CAPTION, __LINE__, TEXT(__FILE__), TEXT(__FUNCSIG__), TEXT(""), TEXT(""), LOG_TYPE)
#define INFO DEBUG_LOG(LL_INFO)
#define WARN DEBUG_LOG(LL_WARN)
#define ERR  DEBUG_LOG(LL_ERR)

#define ASSERT_WARN(f) ((void)((f) || (WARN << L"断言失败：" << TEXT(#f), true)))
#define ASSERT_ERR(f) ((void)((f) || (ERR << L"断言失败：" << TEXT(#f), true)))

#define TRACE_LOG(str, level) ( DEBUG_LOG((LogLevel_)TraceServiceLevel<level>::LEVEL) << str )

//#define DISK_LOG( format, ...) CTraceService::DiskString(format, ##__VA_ARGS__)
#define DISK_LOG( format, ...) 1
