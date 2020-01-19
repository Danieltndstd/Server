#pragma once

//////////////////////////////////////////////////////////////////////////////////
//MFC �ļ�

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxDisp.h>
#include <AfxDtctl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <AfxCmn.h>
#endif

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//ƽ̨����
#include "..\..\ȫ�ֶ���\Platform.h"
#include "..\..\��Ϣ����\CMD_Correspond.h"
#include "..\..\��Ϣ����\CMD_GameServer.h"
#include "..\..\��Ϣ����\CMD_LogonServer.h"

//�������
#include "..\..\�������\�������\ServiceCoreHead.h"
#include "..\..\���������\��Ϸ����\GameServiceHead.h"
#include "..\..\���������\��������\MatchServiceHead.h"
#include "..\..\���������\�ں�����\KernelEngineHead.h"
#include "..\..\���������\ģ�����\ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/GameService.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/KernelEngine.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ModuleManager.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/GameService.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/KernelEngine.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ModuleManager.lib")


#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/GameServiceD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/KernelEngineD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ModuleManagerD.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/GameServiceD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/KernelEngineD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ModuleManagerD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#define LOG_CAPTION L"����������[��Ϸ������]����������"
#define LOG_TYPE L"[Server]"
#include "..\..\DebugLog\DebugLog.h"

#define D_TRACE_OPEN
#include "..\..\ȫ�ֶ���\Trace.h"

#include <vector>
#include <list>
#include <unordered_map>

#include <string>
#include "..\..\���������\libcurl\include\curl.h"

#include "DataCenter.h"


#include "..\redisdb\RedisClient.h"

#define LOG_FILE
#include "..\..\DebugLog\Log.h"
extern TCHAR g_szLogRoot[MAX_PATH];
using namespace std;