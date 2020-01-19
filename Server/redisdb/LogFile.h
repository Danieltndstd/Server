/*
	�ļ�	��LogFile.h
	���	��

	����޸�����	�޸���		�޸�����
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

        //����512M���¿�һ���ļ�
        m_nMaxLogFileLen = 512*1024*1024;
        m_nDate = DateTime().GetDay();

        MakeLogfilename();
    }
	~CLogFile()
    {

    }
public:
	//��־д���ڴ滺�������Ȼ�����ʱ����һ����ʱ����ʱ��ͳһд���ļ�
	VOID CacheLog(enum_LOG_LEVEL logLevel, const CHAR* pLogString , ...)
    {
        //�ȼ�̫�ͣ������ӡ����̨���ļ�
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
        //�ߵ������̨�ȼ������ӡ������̨
        if (logLevel >= m_eConsoleLogLevel)
        {
            printf("%s", szLogStr);
        }

        //�ߵ����ļ��ȼ������ӡ���ļ�
        if (logLevel >= m_eFileLogLevel)
        {
            CSmartLock sLock(m_lock);

            int strLength = strlen(szLogStr);
            if (m_pWriteBuf + strLength - m_pFrontBuf >=  LOG_BUF_LEN)
            {
                //front ��������
                if (m_bBackBufAvailable)
                {
                    //����ȫ���ˣ�����Ҫ����
                    printf("LOG LOST!!, %s", szLogStr);
                    return;
                }
                //back���ţ���front������back
                SwitchBuf();
            }

            //д��front
            strcpy( m_pWriteBuf, szLogStr );
            m_pWriteBuf += strLength;
        }
    }

	//ֱ��д���ļ��У�����������δд��
	VOID DiskLog(enum_LOG_LEVEL	logLevel, const CHAR* pLogString , ...)
    {
        //�ȼ�̫�ͣ������ӡ����̨���ļ�
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

        //�ߵ������̨�ȼ������ӡ������̨
        if (logLevel >= m_eConsoleLogLevel)
        {
            printf("%s", szLogStr);
        }

        //�ߵ����ļ��ȼ������ӡ���ļ�
        if (logLevel >= m_eFileLogLevel)
        {
            CSmartLock sLock(m_lock);

            INT nLogStrLen = strlen(szLogStr);

            if ((m_bBackBufAvailable) && 
                (m_pWriteBuf + nLogStrLen - m_pFrontBuf >=  LOG_BUF_LEN))
            {
                //���back�ж����� front��д���£���Ҫд2���ļ��ˣ������������Ӧ�ú���
                WriteBackBufToFile();
            }

            if (m_pWriteBuf + nLogStrLen - m_pFrontBuf >=  LOG_BUF_LEN)
            {
                //frontд���£���ôback�϶�����
                SwitchBuf();
            }

            //д��front
            strcpy( m_pWriteBuf, szLogStr );
            m_pWriteBuf += nLogStrLen;

            //ȫд���ļ���
            WriteAllBufToFile();
        }
    }

	//���back�������������ݾ�д���ļ�
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

	//front �� back ����������д���ļ�
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

	//����ļ���С�������Ҫ�Ļ������½�һ���ļ�
	VOID CheckLogFileStat()
    {
        bool bNeedNewFile = FALSE;
        DateTime now;

        if ( now.GetDay() != m_nDate)
        {
            //����Ƿ����һ��
            m_nDate = now.GetDay();
            bNeedNewFile = TRUE;
        }
        else
        {
            //����ļ���С
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
            //��Ҫ�¿�һ���ļ���
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
            //�ж���������2ҳ��front -> back.
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

	//��־�ļ����������С�����¿�һ���ļ�
	INT			m_nMaxLogFileLen;
	//��¼��ǰ�����ڣ�������ֿ��һ�죬���¿�һ���ļ�
	INT			m_nDate;
};

#endif
