/*
	文件	：LogFile.h
	简介	：

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/
#ifndef	_LOG_FILE_H_
#define	_LOG_FILE_H_

#include "BaseType.h"
#include "DateTime.h"
#include "AssertDefine.h"

#define MAX_FILE_NAME	256
#define MAX_LOG_LINE	8192
#define LOG_BUF_LEN		1024*1024

enum enum_LOG_LEVEL
{
    LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_WARNNING,
	LOG_LEVEL_ERROR,
};

class CLogFile
{
public:
	CLogFile(const CHAR* szFileName, const CHAR* szPath )
    {
        //CTimeSystem* pTimeSystem = CTimeSystem::Instance();

        m_eFileLogLevel = LOG_LEVEL_DEBUG;
        m_eConsoleLogLevel = LOG_LEVEL_DEBUG;

        if (szPath == NULL || strlen(szPath) <= 0)
        {
            strncpy(m_szFilePath, "./Log/", MAX_FILE_NAME);
        }
        else
        {
            _snprintf(m_szFilePath, MAX_FILE_NAME, "%s", szPath);
        }

        if (!CheckDirExsist(m_szFilePath))
        {
            DSCreateDirectory(m_szFilePath);
        }

        _snprintf(m_szFileName, MAX_FILE_NAME, "%s", szFileName);

        memset(m_pBuf, 0, 2*LOG_BUF_LEN);
        m_pFrontBuf = m_pBuf;
        m_pBackBuf = m_pBuf + LOG_BUF_LEN;
        m_pWriteBuf = m_pFrontBuf;

        m_bBackBufAvailable = FALSE;

        //超过512M，新开一个文件
        m_nMaxLogFileLen = 512*1024*1024;
        m_nDate = DateTime().GetDay();

        MakeLogfilename();
    }
	~CLogFile()
    {

    }
public:
	//日志写到内存缓冲区，等缓冲满时，或一定的时间间隔时，统一写入文件
	VOID CacheLog(enum_LOG_LEVEL logLevel, const CHAR* pLogString , ...)
    {
        //等级太低，不需打印控制台和文件
        if (logLevel < m_eConsoleLogLevel && logLevel < m_eFileLogLevel)
        {
            return ;
        }

        CHAR szLine[MAX_LOG_LINE] = {0};
        va_list	va;
        va_start(va,pLogString);
#ifndef WIN32
        vsprintf(szLine,pLogString,va);
#else
        vsprintf_s(szLine,MAX_LOG_LINE,pLogString,va);
#endif
        va_end(va);

        DateTime now;
        CHAR szLogStr[MAX_LOG_LINE] = {'\0'};
        _snprintf(szLogStr, MAX_LOG_LINE-1, "%2d/%02d/%02d %02d:%02d:%02d.%03d	tId:%ld	%s\n",
            now.GetYear(),
            now.GetMonth(),
            now.GetDay(),
            now.GetHour(),
            now.GetMinute(),
            now.GetSecond(),
            now.GetMilliSecond(),
            IThread::GetThreadID(),
            szLine);
        //高等与控制台等级，则打印到控制台
        if (logLevel >= m_eConsoleLogLevel)
        {
            printf("%s", szLogStr);
        }

        //高等与文件等级，则打印到文件
        if (logLevel >= m_eFileLogLevel)
        {
            CSmartLock sLock(m_lock);

            int strLength = strlen(szLogStr);
            if (m_pWriteBuf + strLength - m_pFrontBuf >=  LOG_BUF_LEN)
            {
                //front 缓冲满了
                if (m_bBackBufAvailable)
                {
                    //两块全满了，这条要丢。
                    printf("LOG LOST!!, %s", szLogStr);
                    return;
                }
                //back空着，将front交换到back
                SwitchBuf();
            }

            //写入front
            strcpy( m_pWriteBuf, szLogStr );
            m_pWriteBuf += strLength;
        }
    }

	//直接写入文件中，若缓冲区有未写入
	VOID DiskLog(enum_LOG_LEVEL	logLevel, const CHAR* pLogString , ...)
    {
        //等级太低，不需打印控制台和文件
        if (logLevel < m_eConsoleLogLevel && logLevel < m_eFileLogLevel)
        {
            return ;
        }

        CHAR szLine[MAX_LOG_LINE] = {0};
        va_list	va;
        va_start(va,pLogString);
#ifndef WIN32
        vsprintf(szLine,pLogString,va);
#else
        vsprintf_s(szLine,MAX_LOG_LINE,pLogString,va);
#endif
        va_end(va);

        DateTime now;
        CHAR szLogStr[MAX_LOG_LINE] = {'\0'};
        _snprintf(szLogStr, MAX_LOG_LINE, "%2d/%02d/%02d %02d:%02d:%02d.%03d	tId:%ld	%s\n", 
            now.GetYear(),
            now.GetMonth(),
            now.GetDay(),
            now.GetHour(),
            now.GetMinute(),
            now.GetSecond(),
            now.GetMilliSecond(),
            IThread::GetThreadID(),
            szLine);

        //高等与控制台等级，则打印到控制台
        if (logLevel >= m_eConsoleLogLevel)
        {
            printf("%s", szLogStr);
        }

        //高等与文件等级，则打印到文件
        if (logLevel >= m_eFileLogLevel)
        {
            CSmartLock sLock(m_lock);

            INT nLogStrLen = strlen(szLogStr);

            if ((m_bBackBufAvailable) && 
                (m_pWriteBuf + nLogStrLen - m_pFrontBuf >=  LOG_BUF_LEN))
            {
                //如果back有东西， front还写不下，就要写2次文件了，不过这种情况应该很少
                WriteBackBufToFile();
            }

            if (m_pWriteBuf + nLogStrLen - m_pFrontBuf >=  LOG_BUF_LEN)
            {
                //front写不下，那么back肯定空着
                SwitchBuf();
            }

            //写入front
            strcpy( m_pWriteBuf, szLogStr );
            m_pWriteBuf += nLogStrLen;

            //全写入文件中
            WriteAllBufToFile();
        }
    }

	//检测back缓冲区，有内容就写入文件
	VOID WriteBackBufToFile()
    {
        CSmartLock sLock(m_lock);

        CheckLogFileStat();

        if (m_bBackBufAvailable)
        {
            ofstream logFile;
            logFile.open(m_szLogFileName, ios::app);
            INT outLen = strlen(m_pBackBuf);
            logFile.write(m_pBackBuf, (outLen>LOG_BUF_LEN)?LOG_BUF_LEN:outLen);
            logFile.close();

            *m_pBackBuf = '\0';
            m_bBackBufAvailable = FALSE;
        }
    }

	//front 和 back 缓冲区，都写入文件
	VOID WriteAllBufToFile()
    {
        CSmartLock sLock(m_lock);	

        CheckLogFileStat();

        ofstream logFile;
        if (m_bBackBufAvailable)
        {
            logFile.open(m_szLogFileName, ios::app);
            INT outLen = strlen(m_pBackBuf);
            logFile.write(m_pBackBuf, (outLen>LOG_BUF_LEN)?LOG_BUF_LEN:outLen);
            *m_pBackBuf = '\0';

            if (*m_pFrontBuf != '\0' && m_pWriteBuf != m_pFrontBuf)
            {
                outLen = strlen(m_pFrontBuf);
                logFile.write(m_pFrontBuf, (outLen>LOG_BUF_LEN)?LOG_BUF_LEN:outLen);
                *m_pFrontBuf = '\0';
                m_pWriteBuf = m_pFrontBuf;
            }
            logFile.close();

            m_bBackBufAvailable = FALSE;
        }	
        else
        {
            if (*m_pFrontBuf != '\0' && m_pWriteBuf != m_pFrontBuf)
            {
                logFile.open(m_szLogFileName, ios::app);
                INT outLen = strlen(m_pFrontBuf);
                logFile.write(m_pFrontBuf, (outLen>LOG_BUF_LEN)?LOG_BUF_LEN:outLen);
                logFile.close();
                *m_pFrontBuf = '\0';
                m_pWriteBuf = m_pFrontBuf;
            }
        }
    }

	//检查文件大小，如果需要的话，就新建一个文件
	VOID CheckLogFileStat()
    {
        bool bNeedNewFile = FALSE;
        DateTime now;

        if ( now.GetDay() != m_nDate)
        {
            //检查是否过了一天
            m_nDate = now.GetDay();
            bNeedNewFile = TRUE;
        }
        else
        {
            //检查文件大小
            fstream logFile;
            logFile.open(m_szLogFileName, ios::in);
            logFile.seekg(0, ios::end);
            INT length = logFile.tellg();
            logFile.close();

            if (length > m_nMaxLogFileLen)
            {
                bNeedNewFile = TRUE;
            }
        }

        if (bNeedNewFile)
        {
            //需要新开一个文件了
            MakeLogfilename();
        }
    }

	VOID SetFileLogLevel(enum_LOG_LEVEL	logLevel) { m_eFileLogLevel = logLevel; }
	VOID SetConsoleLogLevel(enum_LOG_LEVEL logLevel) { m_eConsoleLogLevel = logLevel; }


    static bool CheckDirExsist(const CHAR* szPath)
    {
        if (szPath == NULL)
        {
            return false;
        }
        INT nRet = 0;
#ifndef WIN32
        nRet = access(szPath, F_OK);
#else
//         WIN32_FIND_DATA wfd;
//         HANDLE hFind = FindFirstFile(szPath, &wfd);
//         if (hFind != INVALID_HANDLE_VALUE && wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
//         {
//             nRet = 0;
//         }
//         else
//         {
//             nRet = 1;
//         }
#endif
        return nRet == 0 ? TRUE : FALSE;
    }

   static  bool DSCreateDirectory(const CHAR* szPath)
    {
        bool bRet = true;
#ifndef WIN32
        if (mkdir(szPath, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
        {
            bRet = false;
        }
#else
        bRet = CreateDirectoryA(szPath, NULL)?true:false;
        if (!bRet && GetLastError() == ERROR_ALREADY_EXISTS)
        {
            bRet = true;
        }
#endif
        return bRet;
    }

private:
	VOID SwitchBuf()
    {
        if (m_pFrontBuf != m_pWriteBuf && m_bBackBufAvailable == FALSE)
        {
            //有东西，交换2页，front -> back.
            swap( m_pFrontBuf, m_pBackBuf );
            m_pWriteBuf = m_pFrontBuf;

            *m_pFrontBuf = '\0';

            m_bBackBufAvailable = TRUE;
        }
    }
    VOID MakeLogfilename()
    {
         MakeLogfilenameWithPostfix();
    }
    VOID MakeLogfilenameWithPostfix()
    {
        DateTime now;

        _snprintf(m_szLogFileName,MAX_FILE_NAME, "%s/%s_%.4d%.2d%.2d_%.2d%.2d%.2d_%.2d.log", m_szFilePath, m_szFileName, now.GetYear(), now.GetMonth(), 
            now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond(), now.GetMilliSecond());

        printf("%s\n", m_szLogFileName);
    }
private:
	enum_LOG_LEVEL		m_eFileLogLevel;
	enum_LOG_LEVEL		m_eConsoleLogLevel;

	CHAR		m_szLogFileName[MAX_FILE_NAME];
	CHAR		m_szFileName[MAX_FILE_NAME];
	CHAR        m_szFilePath[MAX_FILE_NAME];
	CHAR		m_pBuf[2*LOG_BUF_LEN];
	CHAR*		m_pFrontBuf;
	CHAR*		m_pBackBuf;
	CHAR*		m_pWriteBuf;
	
	bool		m_bBackBufAvailable;
	CLock		m_lock;

	//日志文件超过这个大小，就新开一个文件
	INT			m_nMaxLogFileLen;
	//记录当前的日期，如果发现跨过一天，就新开一个文件
	INT			m_nDate;
};

#endif
