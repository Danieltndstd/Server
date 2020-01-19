/*
	�ļ�	��LogSystem.h
	���	����־ϵͳ���࣬һ���Ե�����ʽ����
	---------------------------------------------------------------------------------------------
*/
#ifndef	_LOG_SYSTEM_H_
#define	_LOG_SYSTEM_H_

#include "LogFile.h"

#define MAX_LOG_FILE_NUM 16

#if defined (_M_IX86)
#define _DbgBreak() __asm { int 3 }
#elif defined (_M_IA64)
void __break(int);
#pragma intrinsic (__break)
#define _DbgBreak() __break(0x80016)
#else  /* defined (_M_IA64) */
#define _DbgBreak() DebugBreak()
#endif  /* defined (_M_IA64) */


enum enum_LOG_FILE_ID
{
	LOG_DEBUG,
	LOG_ASSERT,
	LOG_ERROR,
	LOG_FUNCTION,
	LOG_SEND,
	LOG_LUA_ERROR,
    LOG_DSSDK,
	LOG_MONEY,
	LOG_ITEM,
	LOG_AUDIT,
    LOG_COST,
    LOG_STATISTIC,
};


class ILogger
{
public:
    ILogger(){};
    virtual ~ILogger(){};
};

class CLogSystem :public Singleton<CLogSystem>, public ILogger
{
public:
    CLogSystem(): m_eFileLogLevel(LOG_LEVEL_DEBUG),m_eConsoleLogLevel(LOG_LEVEL_DEBUG)
      {
          for (INT i = 0; i < MAX_LOG_FILE_NUM; ++i)
              m_pLogFiles[i] = NULL;
      }
	~CLogSystem()
    {
        if (m_pDefaultFile != NULL)
        {
            delete m_pDefaultFile;
            m_pDefaultFile = NULL;
        }
    }

public:
    // ��ʼ����־ϵͳ,ָ���Ƿ񴴽�������logĿ¼���Ƿ�log�ļ������ʱ�䡢���ڵȵĺ�׺
    bool Initialize(bool bCreateLogDir = TRUE, bool bLogFilenameAddPostfix = TRUE)
    {
        if (bCreateLogDir)
        {
            if (!CLogFile::CheckDirExsist("./Log"))
            {
                CLogFile::DSCreateDirectory("./Log");
            }
        }
        //��ʼ��Ĭ����־�ļ�
        m_pDefaultFile = new CLogFile("Default.log", "./Log");
        m_pDefaultFile->SetConsoleLogLevel(m_eConsoleLogLevel);
        m_pDefaultFile->SetFileLogLevel(m_eFileLogLevel);

        return TRUE;
    }

	//��ʼ��һ����־�ļ���ָ��һ���ļ�id������־ʱ�ṩ����ļ�id��������ļ�
	bool InitLogFile(INT logFileId, const CHAR* szFileName, const CHAR* szLogPath )
    {
        if (logFileId < 0 || logFileId >= MAX_LOG_FILE_NUM )
        {
            return FALSE;
        }
        if (m_pLogFiles[logFileId] != NULL)
        {
            return FALSE;
        }
        m_pLogFiles[logFileId] = new CLogFile(szFileName, szLogPath);
        m_pLogFiles[logFileId]->SetConsoleLogLevel(m_eConsoleLogLevel);
        m_pLogFiles[logFileId]->SetFileLogLevel(m_eFileLogLevel);
        return TRUE;
    }
	//�����ļ�id������־�ļ�
	CLogFile* GetLogFlieById(INT logFileId)
    {
        if (logFileId < 0 || logFileId >= MAX_LOG_FILE_NUM )
        {
            return m_pDefaultFile;
        }
        if(m_pLogFiles[logFileId] == NULL)
        {
            return m_pDefaultFile;
        }
        return m_pLogFiles[logFileId];
    }


	//������־�ļ�����־�ȼ����ߵ�������ȼ�����־�Żᱻ�����ļ�
	VOID SetFileLogLevel(enum_LOG_LEVEL	logLevel) { m_eFileLogLevel = logLevel; }

	//���ÿ���̨�������־�ȼ����ߵ�������ȼ�����־�Żᱻ�������̨
	VOID SetConsoleLogLevel(enum_LOG_LEVEL logLevel) { m_eConsoleLogLevel = logLevel; }

	//ˢ��������־��backbuf���ļ�
	VOID Flush()
    {
        for (INT i = 0; i < MAX_LOG_FILE_NUM; ++i)
        {
            if(m_pLogFiles[i] != NULL)
            {
                m_pLogFiles[i]->WriteAllBufToFile();
            }
        }
    }
	// ��ú������е�ջ
	const string GetStackString()
	{
		string stackString;

		//#ifndef WIN32
		//	void *array[20];
		//	size_t size;
		//	char **strings;
		//
		//	size = backtrace(array, 20);
		//	strings = backtrace_symbols(array, size);
		//	for(int i=0; i<size; i++)
		//	{
		//		stackString += strings[i];
		//		stackString += "\n";
		//	}
		//	free(strings);
		//#endif

		return stackString;
	}
	VOID Exit()
    {
        for (INT i = 0; i < MAX_LOG_FILE_NUM; i++)
        {
            if (m_pLogFiles[i] != NULL)
            {
                delete m_pLogFiles[i];
                m_pLogFiles[i] = NULL;
            }
        }
        if (m_pDefaultFile != NULL)
        {
            delete m_pDefaultFile;
            m_pDefaultFile = NULL;
        }
    }
#ifdef  WIN32
	static void  AssertMessageBox( const char* expr )
	{
		int nCode = MessageBoxA(NULL, expr, "Assert", MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);

		/* Abort: abort the program */
		if (nCode == IDABORT)
		{
			/* raise abort signal */
			raise(SIGABRT);

			/* We usually won't get here, but it's possible that
			SIGABRT was ignored.  So exit the program anyway. */

			_exit(3);
		}

		/* Retry: call the debugger */
		if (nCode == IDRETRY)
		{
			_DbgBreak();
			/* return to user code */
			return;
		}

		/* Ignore: continue execution */
		if (nCode == IDIGNORE)
			return;

		abort();
	}
#endif
private:
	enum_LOG_LEVEL		m_eFileLogLevel;
	enum_LOG_LEVEL		m_eConsoleLogLevel;
    CLogFile*			m_pLogFiles[MAX_LOG_FILE_NUM];

	//Ĭ����־�ļ�����ָ�����ļ�id�д��Ҳ�����Ӧ�ļ�ʱ��������ļ�
	CLogFile*			m_pDefaultFile;

};

#define DiskLog_WARNNING(fileId, format, ...) CLogSystem::GetInstancePtr()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_WARNNING, format, ##__VA_ARGS__)
#define DiskLog_DEBUG(fileId, format, ...) CLogSystem::GetInstancePtr()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define DiskLog_ERROR(fileId, format, ...) CLogSystem::GetInstancePtr()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#define CacheLog_WARNNING(fileId, format, ...) CLogSystem::GetInstancePtr()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_WARNNING, format, ##__VA_ARGS__)
#define CacheLog_DEBUG(fileId, format, ...) CLogSystem::GetInstancePtr()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define CacheLog_ERROR(fileId, format, ...) CLogSystem::GetInstancePtr()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#endif
