#include "StdAfx.h"
#include "GlobalUnits.h"
#include "ClientKernel.h"
#include "DlgGameClose.h"
#include "GameFrameView.h"
#include "GameGladMessage.h"
#include "MatchFrameControl.h"


//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_USER_CLOCK_PULSE		100									//�û�ʱ��

#define MAX_SHORT_COUNT				16									//�����Ŀ

#define IPC_SUB_GF_CLIENT_INSURE_SHORT		900							//Ԫ�����㣬֪ͨ����
#define IPC_CMD_GR_CONTROL					10							

TCHAR							g_szLogRoot[MAX_PATH];					//��־��Ŀ¼
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CClientKernel, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CClientKernel::CClientKernel()
{
	//״̬����
	m_bService=false;
	ZeroMemory(&m_UserAttribute,sizeof(m_UserAttribute));

	//ʱ����Ϣ
	m_wClockID=0L;
	m_nElapseCount=0L;
	m_wClockChairID=INVALID_CHAIR;

	//��Ϸ״̬
	m_bAllowLookon=false;
	m_pIMySelfUserItem=NULL;
	m_cbGameStatus=GAME_STATUS_FREE;

	//�ں˱���
	m_hProcessWnd=NULL;
	m_pIStringMessage=NULL;
	m_pIClientKernelSink=NULL;
	m_pIClientPropertySink=NULL;

	m_hWebPlaza = NULL;
	m_lCellScore=0;
	m_ServerTypeEx=0;

	//��������
	ZeroMemory(&m_GameAttribute,sizeof(m_GameAttribute));
	ZeroMemory(&m_ServerAttribute,sizeof(m_ServerAttribute));

	return;
}

//��������
CClientKernel::~CClientKernel()
{
}

//�ӿڲ�ѯ
VOID * CClientKernel::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientKernel,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessControlSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IClientKernel,Guid,dwQueryVer);
	return NULL;
}

//�ں�����
bool CClientKernel::InitClientKernel(LPCTSTR lpszComLine)
{
	//Ч�����
	ASSERT(lpszComLine!=NULL);
	if (lpszComLine==NULL) return false;

	//��������
	CWHCommandLine CommandLine;
	TCHAR szTransmittal[32]=TEXT("");
	TCHAR szServerType[32]=TEXT("");

	//��������
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
	}

	//��ȡ����
	if (CommandLine.SearchCommandItem(lpszComLine,TEXT("/Transmittal:"),szTransmittal,CountArray(szTransmittal))==true)
	{
		//���ò���
		m_hProcessWnd=(HWND)(_tstoi64(szTransmittal));

		//Ч��״̬
		ASSERT(IsWindow(m_hProcessWnd));
		if (IsWindow(m_hProcessWnd)==FALSE) m_hProcessWnd=NULL;
	}

	if ( CommandLine.SearchCommandItem(lpszComLine,TEXT("/ServerType:"),szServerType,CountArray(szServerType)) == true )
	{
		OutputDebugString(szServerType);
		m_ServerAttribute.wServerType = _tstoi(szServerType);
	}

	//�������
	if (m_ProcessControlModule.CreateInstance()==false) return false;
	if (m_GameUserManagerModule.CreateInstance()==false) return false;

	//ע�����
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_USER_MANAGER,m_GameUserManagerModule.GetInterface());

	//�������
	if (m_GameUserManagerModule->SetUserManagerSink(QUERY_ME_INTERFACE(IUnknownEx))==false) return false;
	if (m_ProcessControlModule->SetProcessControlSink(QUERY_ME_INTERFACE(IUnknownEx))==false) return false;

	//����ģʽ
	if (m_hProcessWnd!=NULL)
	{
		//ʱ������
		m_TimerPulseEngine.StartService();
		m_TimerPulseEngine.SetKernelWnd(m_hWnd);

		//��������
		if (m_ProcessControlModule->InitializeLink()==false) return false;
	}

	//��Ƶ����
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	if (pVideoServiceManager!=NULL) pVideoServiceManager->SetClientKernel(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	TCHAR myDocumentsPath[MAX_PATH];

	auto hResult = SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, myDocumentsPath);

	const wstring myDocPath = wstring(myDocumentsPath) + TEXT("/8099/");

	if(!PathFileExists(myDocPath.c_str()))
	{
		if(!CreateDirectory(myDocPath.c_str(), nullptr))
		{
			_stprintf(g_szLogRoot,TEXT("./Games/SG/"));
			return true;
		}
	}
	_stprintf(g_szLogRoot,TEXT("%s/GameFrame"),myDocPath.c_str());

	return true;
}

//���ýӿ�
bool CClientKernel::SetUserEventSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserEventSink)!=NULL);
	IUserEventSink * pIUserEventSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserEventSink);

	//�ɹ��ж�
	if (pIUserEventSink!=NULL)
	{
		m_UserEventSinkArray.Add(pIUserEventSink);
		return true;
	}

	return false;
}

//���ýӿ�
bool CClientKernel::SetStringMessage(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IStringMessage)!=NULL);
		m_pIStringMessage=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IStringMessage);

		//�ɹ��ж�
		if (m_pIStringMessage==NULL) return false;

		//��Ƶ����
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
		if (pVideoServiceManager!=NULL) pVideoServiceManager->SetMessageProxy(QUERY_OBJECT_PTR_INTERFACE(m_pIStringMessage,IUnknownEx));
	}
	else m_pIStringMessage=NULL;

	return true;
}

//���ýӿ�
bool CClientKernel::SetClientKernelSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientKernelSink)!=NULL);
		m_pIClientKernelSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientKernelSink);

		//�ɹ��ж�
		if (m_pIClientKernelSink==NULL) return false;
	}
	else m_pIClientKernelSink=NULL;

	return true;
}

//���ýӿ�
bool CClientKernel::SetClientPropertySink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientPropertySink)!=NULL);
		m_pIClientPropertySink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientPropertySink);

		//�ɹ��ж�
		if (m_pIClientPropertySink==NULL) return false;
	}
	else m_pIClientPropertySink=NULL;

	return true;
}

//��Ϸ����
bool CClientKernel::SetGameAttribute(WORD wKindID, WORD wPlayerCount, DWORD dwClientVersion, HICON hGameIcon, LPCTSTR pszGameName)
{
	//����ͼ��
	AfxGetMainWnd()->SetIcon(hGameIcon,TRUE);
	AfxGetMainWnd()->SetIcon(hGameIcon,FALSE);

	//���ñ���
	m_GameAttribute.wKindID=wKindID;
	m_GameAttribute.wChairCount=wPlayerCount;
	m_GameAttribute.dwClientVersion=dwClientVersion;
	lstrcpyn(m_GameAttribute.szGameName,pszGameName,CountArray(m_GameAttribute.szGameName));

	//��ȡ����
	TCHAR szModuleName[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szModuleName,CountArray(szModuleName));

	//�汾Ч��
	if (VERSION_EFFICACY==TRUE)
	{
		//��ȡ�汾
		DWORD dwProcessVersion=0L;
		CWHService::GetModuleVersion(szModuleName,dwProcessVersion);

		//Ч��汾
		ASSERT(m_GameAttribute.dwClientVersion==dwProcessVersion);
		if (m_GameAttribute.dwClientVersion!=dwProcessVersion) throw TEXT("��Ϸ����汾��Ϣ���Ƿ��޸�");
	}

	//���ñ���
	SetGameFrameTitle();

	return true;
}

//���Ӵ���
bool CClientKernel::CreateConnect()
{
	//Ч��״̬
	if (IsSingleMode()==true) return false;

	HWND   hMainWnd = AfxGetMainWnd()->GetSafeHwnd();

	//������Ϣ
	SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLIENT_READY,&hMainWnd,sizeof(hMainWnd));

	//��ȡ����
	CWnd * pMainWnd=AfxGetMainWnd();

	//��ԭ����
	if (pMainWnd->IsIconic()) 
	{
		pMainWnd->ShowWindow(SW_RESTORE);
	}

	//�ö�����
	pMainWnd->SetActiveWindow(); 
	pMainWnd->BringWindowToTop(); 
	pMainWnd->SetForegroundWindow();

	return true;
}

//�ж�����
bool CClientKernel::IntermitConnect()
{
	//Ч��״̬
	if (IsSingleMode()==true) return false;

	//������Ϣ
	SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLIENT_CLOSE);

	//�ر�����
	if (m_ProcessControlModule.GetInterface()!=NULL) m_ProcessControlModule->UnInitializeLink();

	return true;
}

//����ģʽ
bool CClientKernel::IsSingleMode()
{
	//�����ж�
	if (m_hProcessWnd==NULL) return true;
	if (m_ProcessControlModule.GetInterface()==NULL) return true;

	return false;
}

//�Թ��û�
bool CClientKernel::IsLookonMode()
{
	if (m_pIMySelfUserItem==NULL) return true;
	return (m_pIMySelfUserItem->GetUserStatus()==US_LOOKON);
}

//�����Թ�
bool CClientKernel::IsAllowLookon()
{
	//����Ա���������Թ۵�����
	if(m_pIMySelfUserItem==NULL || m_pIMySelfUserItem->GetMasterOrder() > 0)
	{
		return true;
	}

	//�Ƹ���Ϸ�����Ƿ������Թ۾������Կ�������
	if(m_ServerAttribute.wServerType==GAME_GENRE_GOLD)
	{
		return false;
	}

	return m_bAllowLookon;
}

//����״̬
bool CClientKernel::IsServiceStatus()
{
	return m_bService;
}

//�Լ�λ��
WORD CClientKernel::GetMeChairID()
{
	//Ч��״̬
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return INVALID_CHAIR;

	return m_pIMySelfUserItem->GetChairID();
}

//�Լ�λ��
IClientUserItem * CClientKernel::GetMeUserItem()
{
	return m_pIMySelfUserItem;
}

//��Ϸ�û�
IClientUserItem * CClientKernel::GetTableUserItem(WORD wChariID)
{
	//Ч��״̬
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//��ȡ�û�
	return m_GameUserManagerModule->GetTableUserItem(wChariID);
}

//�����û�
IClientUserItem * CClientKernel::SearchUserByUserID(DWORD dwUserID)
{
	//Ч��״̬
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//�����û�
	IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(dwUserID);

	return pIClientUserItem;
}

//�����û�
IClientUserItem * CClientKernel::SearchUserByGameID(DWORD dwGameID)
{
	//Ч��״̬
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//�����û�
	IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByGameID(dwGameID);

	return pIClientUserItem;
}

//�����û�
IClientUserItem * CClientKernel::SearchUserByNickName(LPCTSTR szNickName)
{
	//Ч��״̬
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//�����û�
	IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByNickName(szNickName);

	return pIClientUserItem;
}

//�Թ��û�
IClientUserItem * CClientKernel::EnumLookonUserItem(WORD wEnumIndex)
{
	//Ч��״̬
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//�Թ��û�
	IClientUserItem * pIClientUserItem=m_GameUserManagerModule->EnumLookonUserItem(wEnumIndex);

	return pIClientUserItem;
}

//���ͺ���
bool CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч��״̬
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//��������
	IPC_GF_SocketSend SocketSend;
	ZeroMemory(&SocketSend,sizeof(SocketSend));

	//��������
	SocketSend.CommandInfo.wSubCmdID=wSubCmdID;
	SocketSend.CommandInfo.wMainCmdID=wMainCmdID;

	//��������
	WORD wSendSize=sizeof(SocketSend)-sizeof(SocketSend.cbBuffer);
	m_ProcessControlModule->SendData(m_hProcessWnd,IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_SEND,&SocketSend,wSendSize);

	return true;
}

//���ͺ���
bool CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;

	//Ч��״̬
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//��������
	IPC_GF_SocketSend SocketSend;
	ZeroMemory(&SocketSend,sizeof(SocketSend));

	//��������
	SocketSend.CommandInfo.wSubCmdID=wSubCmdID;
	SocketSend.CommandInfo.wMainCmdID=wMainCmdID;

	//��������
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketSend.cbBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(SocketSend)-sizeof(SocketSend.cbBuffer)+wDataSize;
	m_ProcessControlModule->SendData(m_hProcessWnd,IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_SEND,&SocketSend,wSendSize);

	return true;
}

//��������
bool CClientKernel::SendProcessData(WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч��״̬
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//��������
	m_ProcessControlModule->SendData(m_hProcessWnd,wMainCmdID,wSubCmdID);

	return true;
}

//��������
bool CClientKernel::SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//��������
	m_ProcessControlModule->SendData(m_hProcessWnd,wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//ִ������
bool CClientKernel::SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//ִ������
	m_ProcessControlModule->SendCommand(m_hProcessWnd,wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//����׼��
bool CClientKernel::SendUserReady(VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;
	
	//����׼��
	SendSocketData(MDM_GF_FRAME,SUB_GF_USER_READY,pData,wDataSize);

	return true;
}

//�Թۿ���
bool CClientKernel::SendUserLookon(DWORD dwUserID, bool bAllowLookon)
{
	//������Ϣ
	if (IsLookonMode()==false)
	{
		//��������
		CMD_GF_LookonConfig LookonConfig;
		ZeroMemory(&LookonConfig,sizeof(LookonConfig));

		//���ñ���
		LookonConfig.dwUserID=dwUserID;
		LookonConfig.cbAllowLookon=(bAllowLookon==true)?TRUE:FALSE;

		//��������
		SendSocketData(MDM_GF_FRAME,SUB_GF_LOOKON_CONFIG,&LookonConfig,sizeof(LookonConfig));
	}

	return true;
}

//���ͱ���
bool CClientKernel::SendUserExpression(DWORD dwTargetUserID, WORD wItemIndex)
{
	//��������
	CMD_GF_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//������Ϣ
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;

	//��������
	SendSocketData(MDM_GF_FRAME,SUB_GF_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//��������
bool CClientKernel::SendUserChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//Ч���û�
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//������Ϣ
	CMD_GF_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//��������
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//��������
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	SendSocketData(MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(TCHAR));

	return true;
}

//������
VOID CClientKernel::ActiveGameFrame()
{
	//��ȡ����
	CWnd * pFocusWnd=GetFocus();
	CWnd * pMainWnd=AfxGetMainWnd();

	if (pMainWnd == NULL)
	{
		AfxMessageBox(L"pMainWnd ��ָ��");
		return;
	}
	//�����ж�
	if ((pFocusWnd==NULL)||(pMainWnd->IsChild(pFocusWnd)==FALSE)) pMainWnd->FlashWindow(TRUE);

	return;
}

//�л�����
WORD CClientKernel::SwitchViewChairID(WORD wChairID)
{
	//�����ж�
	if (wChairID==INVALID_CHAIR) return INVALID_CHAIR;
	if (m_UserAttribute.dwUserID==0L) return INVALID_CHAIR;

	//ת������
	WORD wChairCount=m_GameAttribute.wChairCount;
	WORD wViewChairID=(wChairID+wChairCount*3/2-m_UserAttribute.wChairID)%wChairCount;

	return wViewChairID;
}

//ʱ�ӱ�ʶ
UINT CClientKernel::GetClockID()
{
	return m_wClockID;
}

//ʱ��λ��
WORD CClientKernel::GetClockChairID()
{
	return m_wClockChairID;
}

//ɾ��ʱ��
VOID CClientKernel::KillGameClock(WORD wClockID)
{
	//Ч��״̬
	ASSERT(m_hWnd!=NULL);
	ASSERT(m_pIClientKernelSink!=NULL);

	//�߼�����
	if ((m_wClockID!=0)&&((m_wClockID==wClockID)||(wClockID==0)))
	{
		//���ý���
		m_TimerPulseEngine.StopTimerPulse(m_wClockID);

		//�¼�֪ͨ
		if (m_wClockChairID!=INVALID_CHAIR)
		{
			ASSERT(m_wClockChairID<m_GameAttribute.wChairCount);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->OnEventGameClockKill(m_wClockChairID);
		}

		//���ñ���
		m_wClockID=0L;
		m_nElapseCount=0L;
		m_wClockChairID=INVALID_CHAIR;
	}

	return;
}

//����ʱ��
VOID CClientKernel::SetGameClock(WORD wChairID, WORD wClockID, UINT nElapse)
{
	//Ч�����
	ASSERT(m_hWnd!=NULL);
	ASSERT(m_pIClientKernelSink!=NULL);

	//ɾ��ʱ��
	if (m_wClockID!=0L) KillGameClock(m_wClockID);

	//����ʱ��
	if ((wChairID<m_GameAttribute.wChairCount)&&(nElapse>0L))
	{
		//���ñ���
		m_wClockID=wClockID;
		m_nElapseCount=nElapse;
		m_wClockChairID=wChairID;

		//����ʱ��
		m_TimerPulseEngine.StartTimerPulse(wClockID);

		//�¼�֪ͨ
		if (m_pIClientKernelSink!=NULL)
		{
			m_pIClientKernelSink->OnEventGameClockInfo(m_wClockChairID,nElapse,wClockID);
		}
	}

	return;
}

//��������
bool CClientKernel::OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)
{
	switch (wMainCmdID)
	{
	case IPC_CMD_GF_SOCKET:		//������Ϣ
		{
			//��Ϣ����
			bool bSuccess=OnProcessMainSocket(wSubCmdID,pData,wDataSize);

			//������
			if (bSuccess==false)
			{
				//g_Log.LogToFile(g_szLogRoot,TEXT("������:%d, ������:%d"),wMainCmdID,wSubCmdID);
				ASSERT(m_pIClientKernelSink!=NULL);
				if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->CloseGameClient();
			}

			return true;
		}
	case IPC_CMD_GF_CONTROL:	//������Ϣ
		{
			return OnProcessMainControl(wSubCmdID,pData,wDataSize);
		}
	case IPC_CMD_GF_CONFIG:		//������Ϣ
		{
			return OnProcessMainConfig(wSubCmdID,pData,wDataSize);
		}
	case IPC_CMD_GF_USER_INFO:	//�û���Ϣ
		{
			return OnProcessMainUserInfo(wSubCmdID,pData,wDataSize);
		}
	case IPC_CMD_GF_PROPERTY_INFO: //������Ϣ
		{
			return OnProcessMainPropertyInfo(wSubCmdID,pData,wDataSize);
		}
	}

	return false;
}

//�û�����
VOID CClientKernel::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//Ч��״̬
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//�����û�
	if (m_pIMySelfUserItem==NULL)
	{
		DWORD dwUserID=pIClientUserItem->GetUserID();
		if (dwUserID==m_UserAttribute.dwUserID) m_pIMySelfUserItem=pIClientUserItem;

		//��½��Ƶ
		if (pIClientUserItem->GetUserStatus()!=US_LOOKON)
		{
			CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
			if (pVideoServiceManager!=NULL) pVideoServiceManager->LoginVideoSystem();
		}
	}

	//��������
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);
	IPC_GR_GameUserFace GameUserFace;
	ZeroMemory(&GameUserFace,sizeof(GameUserFace));	

	//�û�֪ͨ
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		if ( pIUserEventSink )
			pIUserEventSink->OnEventUserEnter(pIClientUserItem,bLookonUser);
		//else g_Log.LogToFile(g_szLogRoot,TEXT("��ȡָ��ʧ��"));
	}

	return;
}

//�û�ɾ��
VOID CClientKernel::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//Ч��״̬
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//��������
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);
	IPC_GR_GameUserFace GameUserFace;
	ZeroMemory(&GameUserFace,sizeof(GameUserFace));

	//�û�֪ͨ
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		if ( pIUserEventSink )
			pIUserEventSink->OnEventUserLeave(pIClientUserItem,bLookonUser);
		//else g_Log.LogToFile(g_szLogRoot,TEXT("��ȡָ��ʧ�� 2"));
	}

	return;
}

//�û�����
VOID CClientKernel::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//Ч��״̬
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//��������
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//�û�֪ͨ
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		if (pIUserEventSink)
			pIUserEventSink->OnEventCustomFace(pIClientUserItem,bLookonUser);
	}

	return;
}

VOID CClientKernel::OnUserTaskUpdate(IClientUserItem * pIClientUserItem)
{
	//Ч��״̬
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//��������
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//�û�֪ͨ
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		if (pIUserEventSink)
			pIUserEventSink->OnUserTaskUpdate(pIClientUserItem,bLookonUser);
	}
	return;
}

//�û�����
VOID CClientKernel::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//Ч��״̬
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//��������
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//�û�֪ͨ
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		if (pIUserEventSink)
			pIUserEventSink->OnEventUserScore(pIClientUserItem,bLookonUser);
	}

	return;
}

//�û�����
VOID CClientKernel::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//Ч��״̬
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//��������
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//�û�֪ͨ
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		if (pIUserEventSink)
			pIUserEventSink->OnEventUserStatus(pIClientUserItem,bLookonUser);
	}

	//��ʾ��Ϣ
	if(m_pIStringMessage)
	{
		if(pIClientUserItem->GetUserStatus()==US_OFFLINE)
		{
			m_pIStringMessage->InsertUserOffLine(pIClientUserItem->GetUserInfo()->szNickName);
		}
		else if(pIClientUserItem->GetUserStatus()==US_PLAYING && LastStatus.cbUserStatus==US_OFFLINE)
		{
			TCHAR strMsg[64]={0};
			_sntprintf(strMsg,CountArray(strMsg),TEXT("%s ���߻����ˣ�"), pIClientUserItem->GetUserInfo()->szNickName);
			m_pIStringMessage->InsertSystemString(strMsg);
		}
	}

	return;
}

//�û�����
VOID CClientKernel::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib)
{
	//Ч��״̬
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//��������
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//�û�֪ͨ
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		if (pIUserEventSink)
			pIUserEventSink->OnEventUserAttrib(pIClientUserItem,bLookonUser);
	}

	return;
}

//�����¼�
bool CClientKernel::OnGameOptionChange()
{
	if(m_pIClientKernelSink)
	{
		return m_pIClientKernelSink->OnGameOptionChange();
	}

	return true;
}

//������Ϣ
bool CClientKernel::OnProcessMainSocket(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//�������
	if (wSubCmdID==IPC_SUB_GF_SOCKET_RECV)
	{
		//Ч������
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize < sizeof(TCP_Command))
			return false;

		//��ȡ����
		WORD wPacketSize=wDataSize-sizeof(TCP_Command);
		IPC_GF_SocketRecv * pSocketRecv=(IPC_GF_SocketRecv *)pData;

		//��Ϸ����
		WORD wSubCmdID=pSocketRecv->CommandInfo.wSubCmdID;
		WORD wMainCmdID=pSocketRecv->CommandInfo.wMainCmdID;
		bool bSuccess=m_pIClientKernelSink->OnEventSocket(wMainCmdID,wSubCmdID,pSocketRecv->cbBuffer,wPacketSize);

		//�ں˴���
		if ((bSuccess==false)&&(wMainCmdID==MDM_GF_FRAME))
		{
			switch (wSubCmdID)
			{
			case SUB_GF_USER_CHAT:			//�û�����
				{
					return OnSocketSubUserChat(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_USER_EXPRESSION:	//�û�����
				{
					return OnSocketSubExpression(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_GAME_STATUS:		//��Ϸ״̬
				{
					return OnSocketSubGameStatus(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_GAME_SCENE:			//��Ϸ����
				{
					return OnSocketSubGameScene(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_LOOKON_STATUS:		//�Թ�״̬
				{
					return OnSocketSubLookonStatus(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_SYSTEM_MESSAGE:		//ϵͳ��Ϣ
				{
					return OnSocketSubSystemMessage(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_ACTION_MESSAGE:		//������Ϣ
				{
					return OnSocketSubActionMessage(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_USER_READY:			//�û�׼��
				{
					if(m_pIMySelfUserItem==NULL || m_pIMySelfUserItem->GetUserStatus()>=US_READY)
						return true;

					SendUserReady(NULL,0);
					m_pIClientKernelSink->OnEventSocket(MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,NULL,0);
					return true;
				}
				
			}

			return true;
		}

		//TCHAR buffer[MAX_PATH] = { 0 };
		//_sntprintf(buffer, sizeof(buffer), TEXT("������� - �����%d, �����%d"), wMainCmdID, wSubCmdID);
		//AfxMessageBox(buffer);

		return bSuccess;
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//������Ϣ
bool CClientKernel::OnProcessMainControl(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case IPC_SUB_GF_CLOSE_PROCESS:	//�رս���
		{
			//��ֹ����
			m_hProcessWnd=NULL;

			//�رս���
			ASSERT(m_pIClientKernelSink!=NULL);
			//g_Log.LogToFile(g_szLogRoot,TEXT("�رս���"));
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->CloseGameClient();

			return true;
		}
	case IPC_SUB_GF_ACTIVE_PROCESS:	//�������
		{
			//��ȡ����
			CWnd * pMainWnd=AfxGetMainWnd();

			//��ԭ����
			if (pMainWnd->IsIconic()) 
			{
				pMainWnd->ShowWindow(SW_RESTORE);
			}

			//�ö�����
			pMainWnd->SetActiveWindow(); 
			pMainWnd->BringWindowToTop(); 
			pMainWnd->SetForegroundWindow();
		
			return true;
		}
	case IPC_SUB_GF_BOSS_COME:		//�ϰ�����
		{
			//��ȡ����
			CWnd * pMainWnd=AfxGetMainWnd();

			//���ش���
			pMainWnd->ShowWindow(SW_MINIMIZE);
			pMainWnd->ShowWindow(SW_HIDE);

			return true;
		}
	case IPC_SUB_GF_BOSS_LEFT:		//�ϰ�����
		{
			//��ȡ����
			CWnd * pMainWnd=AfxGetMainWnd();

			//���ش���
			pMainWnd->ShowWindow(SW_RESTORE);
			pMainWnd->ShowWindow(SW_SHOW);

			//�ö�����
			pMainWnd->SetActiveWindow(); 
			pMainWnd->BringWindowToTop();
			pMainWnd->SetForegroundWindow();

			return true;
		}
	//case IPC_SUB_GF_MATCH_READY:	//������ʼ��ʱ������
	//	{
	//		CMatchFrameControl::GetInstance()->SetMatchOverStatus(FALSE);
	//		return true;
	//	}
	//case IPC_SUB_GF_MATCH_OUT:		//�����̭��ʱ��ֹͣ
	//	{
	//		CMatchFrameControl::GetInstance()->SetMatchOverStatus(TRUE);
	//		return true;
	//	}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//������Ϣ
bool CClientKernel::OnProcessMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case IPC_SUB_GF_LEVEL_INFO:		//�ȼ���Ϣ
		{
			//��������
			IPC_GF_LevelInfo * pLevelInfo=(IPC_GF_LevelInfo *)pData;
			WORD wHeadSize=sizeof(IPC_GF_LevelInfo)-sizeof(pLevelInfo->LevelItem);

			//Ч�����
			ASSERT((wHeadSize+pLevelInfo->cbItemCount*sizeof(pLevelInfo->LevelItem[0]))==wDataSize);
			if ((wHeadSize+pLevelInfo->cbItemCount*sizeof(pLevelInfo->LevelItem[0]))!=wDataSize) return false;

			//��ȡ����
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
			IGameLevelParser * pIGameLevelParser=pGlobalUnits->m_GameLevelParserModule.GetInterface();

			//���ý���
			pIGameLevelParser->SetKindName(m_GameAttribute.szGameName);
			pIGameLevelParser->SetGameLevelItem(pLevelInfo->LevelItem,pLevelInfo->cbItemCount);

			return true;
		}
	case IPC_SUB_GF_COLUMN_INFO:	//�б���Ϣ
		{
			//��������
			IPC_GF_ColumnInfo * pColumnInfo=(IPC_GF_ColumnInfo *)pData;
			WORD wHeadSize=sizeof(IPC_GF_ColumnInfo)-sizeof(pColumnInfo->ColumnItem);

			//Ч�����
			ASSERT((wHeadSize+pColumnInfo->cbColumnCount*sizeof(pColumnInfo->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pColumnInfo->cbColumnCount*sizeof(pColumnInfo->ColumnItem[0]))!=wDataSize) return false;

			//�б���Ϣ
			BYTE cbColumnCount=0;
			tagColumnItem ColumnItem[MAX_COLUMN];

			//�����б�
			for (BYTE i=0;i<pColumnInfo->cbColumnCount;i++)
			{
				//����ж�
				if (cbColumnCount>=CountArray(ColumnItem)) 
				{
					ASSERT(FALSE);
					break;
				}

				//�ų�����
				if (pColumnInfo->ColumnItem[i].cbDataDescribe==UD_TABLE) continue;
				if (pColumnInfo->ColumnItem[i].cbDataDescribe==UD_CHAIR) continue;
				if (pColumnInfo->ColumnItem[i].cbDataDescribe==UD_NOTE_INFO) continue;

				//�����б�
				ColumnItem[cbColumnCount++]=pColumnInfo->ColumnItem[i];
			}

			//�����Թ�
// 			if (cbColumnCount<CountArray(ColumnItem))
// 			{
// 				BYTE cbIndex=cbColumnCount++;
// 				ColumnItem[cbIndex].cbColumnWidth=100;
// 				ColumnItem[cbIndex].cbDataDescribe=UD_LOOKON_USER;
// 				lstrcpyn(ColumnItem[cbIndex].szColumnName,TEXT("�Թ��û�"),CountArray(ColumnItem[cbIndex].szColumnName));
// 			}

			//�����б�
			ASSERT(m_pIClientKernelSink!=NULL);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->SchemeColumnItem(ColumnItem,cbColumnCount);

			return true;
		}
	case IPC_SUB_GF_SERVER_INFO:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GF_ServerInfoEx));
			if (wDataSize!=sizeof(IPC_GF_ServerInfoEx)) return false;

			//��������
			IPC_GF_ServerInfoEx * pServerInfo=(IPC_GF_ServerInfoEx *)pData;

			//�û�����
			m_UserAttribute.dwUserID=pServerInfo->dwUserID;
			m_UserAttribute.wTableID=pServerInfo->wTableID;
			m_UserAttribute.wChairID=pServerInfo->wChairID;

			//Ȩ������
			m_UserAttribute.dwUserRight=pServerInfo->dwUserRight;
			m_UserAttribute.dwMasterRight=pServerInfo->dwMasterRight;

			//��������
			m_ServerAttribute.wKindID=pServerInfo->wKindID;
			m_ServerAttribute.wServerID=pServerInfo->wServerID;
			//m_ServerAttribute.wServerType=pServerInfo->wServerType;
			m_ServerAttribute.dwServerRule=pServerInfo->dwServerRule;
			lstrcpyn(m_ServerAttribute.szServerName,pServerInfo->szServerName,CountArray(m_ServerAttribute.szServerName));

			//��Ƶ����
			m_ServerAttribute.wAVServerPort=pServerInfo->wAVServerPort;
			m_ServerAttribute.dwAVServerAddr=pServerInfo->dwAVServerAddr;
			m_lCellScore = pServerInfo->lCellScore;
			m_ServerTypeEx = pServerInfo->wServerTypeEx;

			//��Ƶ��ַ
			CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
			if ( pVideoServiceManager!=NULL) pVideoServiceManager->SetVideoServiceInfo(pServerInfo->dwUserID,(pServerInfo->dwAVServerAddr),pServerInfo->wAVServerPort);

			return true;
		}
	case IPC_SUB_GF_PROPERTY_INFO:	//������Ϣ
		{
			//��������
			IPC_GF_PropertyInfo * pPropertyInfo=(IPC_GF_PropertyInfo *)pData;
			WORD wHeadSize=sizeof(IPC_GF_PropertyInfo)-sizeof(pPropertyInfo->PropertyInfo);

			//Ч�����
			ASSERT((wHeadSize+pPropertyInfo->cbPropertyCount*sizeof(pPropertyInfo->PropertyInfo[0]))==wDataSize);
			if ((wHeadSize+pPropertyInfo->cbPropertyCount*sizeof(pPropertyInfo->PropertyInfo[0]))!=wDataSize) return false;

			//�б���Ϣ
			BYTE cbPropertyCount=0;
			tagPropertyInfo PropertyInfo[MAX_PROPERTY];

			//�����б�
			for (BYTE i=0;i<pPropertyInfo->cbPropertyCount;i++)
			{
				//����ж�
				if (cbPropertyCount>=CountArray(PropertyInfo)) 
				{
					ASSERT(FALSE);
					break;
				}

				//�����б�
				PropertyInfo[cbPropertyCount++]=pPropertyInfo->PropertyInfo[i];
			}

			//�����б�
			ASSERT(m_pIClientKernelSink!=NULL);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->SchemePropertyInfo(PropertyInfo,cbPropertyCount);

			return true;
		}
	case IPC_SUB_GF_CONFIG_FINISH:	//�������
		{
			//�¼�֪ͨ
			ASSERT(m_pIClientKernelSink!=NULL);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->OnSchemeFinish();

			//��ȡ����
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

			//���ñ���
			m_bService=true;

			//���ñ���
			SetGameFrameTitle();

			//��������
			CMD_GF_GameOption GameOption;
			ZeroMemory(&GameOption,sizeof(GameOption));

			//��������
			GameOption.dwFrameVersion=VERSION_FRAME;
			GameOption.cbAllowLookon=pGlobalUnits->m_bAllowLookon;
			GameOption.dwClientVersion=m_GameAttribute.dwClientVersion;
			//AfxMessageBox(TEXT("��ʼ��Ϸ"));
			//��������
			SendSocketData(MDM_GF_FRAME,SUB_GF_GAME_OPTION,&GameOption,sizeof(GameOption));

			return true;
		}
	case IPC_SUB_GF_USER_RIGHT:	//���Ȩ��
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(IPC_GF_UserRight));
			if (wDataSize<sizeof(IPC_GF_UserRight)) return false;

			//��������
			IPC_GF_UserRight *pUserRight = (IPC_GF_UserRight *)pData;

			m_UserAttribute.dwUserRight = pUserRight->dwUserRight;

			return true;
		}
	case IPC_SUB_GF_LAYER_INFO:
		{
			IPC_GF_LayerWndInfo* pLayerWndInfo = (IPC_GF_LayerWndInfo*)pData;

			ASSERT(g_GlobalUnits.QueryGlobalModule(MODULE_GAME_FRAME_WND,IID_IGameFrameWnd,VER_IGameFrameWnd));
			IGameFrameWnd * pIGameFrameWnd=(IGameFrameWnd *)g_GlobalUnits.QueryGlobalModule(MODULE_GAME_FRAME_WND,IID_IGameFrameWnd,VER_IGameFrameWnd);

			pIGameFrameWnd->SetGameLayerHandle(*pLayerWndInfo);

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�û���Ϣ
bool CClientKernel::OnProcessMainUserInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case IPC_SUB_GF_USER_ENTER:		//�û�����
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(IPC_GF_UserInfo));
			if (wDataSize<sizeof(IPC_GF_UserInfo)) return false;

			//��Ϣ����
			IPC_GF_UserInfo * pIPCUserInfo=(IPC_GF_UserInfo *)pData;
			tagUserInfoHead * pUserInfoHead=&pIPCUserInfo->UserInfoHead;

			//��������
			TCHAR szUserNote[LEN_USERNOTE]=TEXT("");

			//��������
			tagUserInfo UserInfo;
			tagCustomFaceInfo CustomFaceInfo;
			ZeroMemory(&UserInfo,sizeof(UserInfo));
			ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

			//�û�����
			UserInfo.wFaceID=pUserInfoHead->wFaceID;
			UserInfo.dwGameID=pUserInfoHead->dwGameID;
			UserInfo.dwUserID=pUserInfoHead->dwUserID;
			UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
			UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

			//�û�����
			UserInfo.cbGender=pUserInfoHead->cbGender;
			UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
			UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

			//�û�״̬
			UserInfo.wTableID=pUserInfoHead->wTableID;
			UserInfo.wChairID=pUserInfoHead->wChairID;
			UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

			//�û�����
			UserInfo.lScore=pUserInfoHead->lScore;
			UserInfo.lGrade=pUserInfoHead->lGrade;
			UserInfo.lInsure=pUserInfoHead->lInsure;
			UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
			UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
			UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
			UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;
			UserInfo.dwUserMedal=pUserInfoHead->dwUserMedal;
			UserInfo.dwExperience=pUserInfoHead->dwExperience;
			UserInfo.lLoveLiness=pUserInfoHead->lLoveLiness;

			//��������
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pIPCUserInfo+1,wDataSize-sizeof(IPC_GF_UserInfo));

			//��չ��Ϣ
			while (true)
			{
				//��ȡ����
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;

				//��������
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GR_NICK_NAME:		//�û��ǳ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
						if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
						{
							CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
							UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
						}
						break;
					}
				case DTP_GR_GROUP_NAME:		//�û�����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
						if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
						{
							CopyMemory(&UserInfo.szGroupName,pDataBuffer,DataDescribe.wDataSize);
							UserInfo.szGroupName[CountArray(UserInfo.szGroupName)-1]=0;
						}
						break;
					}
				case DTP_GR_UNDER_WRITE:	//����ǩ��
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
						if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
						{
							CopyMemory(UserInfo.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
							UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite)-1]=0;
						}
						break;
					}
				case DTP_GR_USER_NOTE:			//�û���ע
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(szUserNote));
						if (DataDescribe.wDataSize<=sizeof(szUserNote))
						{
							CopyMemory(szUserNote,pDataBuffer,DataDescribe.wDataSize);
							szUserNote[CountArray(szUserNote)-1]=0;
						}
						break;
					}
				case DTP_GR_CUSTOM_FACE:		//�Զ�ͷ��
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize==sizeof(tagCustomFaceInfo));
						if (DataDescribe.wDataSize==sizeof(tagCustomFaceInfo))
						{
							CopyMemory(&CustomFaceInfo,pDataBuffer,DataDescribe.wDataSize);
						}
						break;
					}
				}
			}

			//�����û�
			m_GameUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo,pIPCUserInfo->cbCompanion,szUserNote);

			return true;
		}
	case IPC_SUB_GF_USER_SCORE:		//�û�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GF_UserScore));
			if (wDataSize!=sizeof(IPC_GF_UserScore)) return false;

			//Ѱ���û�
			IPC_GF_UserScore * pUserScore=(IPC_GF_UserScore *)pData;
			IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

			//�û��ж�
			ASSERT(pIClientUserItem!=NULL);
			if (pIClientUserItem==NULL) return true;

			//���û���
			m_GameUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);

			return true;
		}
	case IPC_SUB_GF_USER_STATUS:	//�û�״̬
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GF_UserStatus));
			if (wDataSize!=sizeof(IPC_GF_UserStatus)) return false;

			//Ѱ���û�
			IPC_GF_UserStatus * pUserStatus=(IPC_GF_UserStatus *)pData;
			IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

			//�û��ж�
			ASSERT(pIClientUserItem!=NULL);
			if (pIClientUserItem==NULL) return true;

			//״̬����
			BYTE cbUserStatus=pUserStatus->UserStatus.cbUserStatus;

			//�뿪�ж�
			if ((cbUserStatus==US_NULL)||(cbUserStatus==US_FREE))
			{
				if (m_pIMySelfUserItem==pIClientUserItem)
				{
					//���ñ���
					m_bService=false;
					m_bAllowLookon=false;
					m_pIMySelfUserItem=NULL;
					m_cbGameStatus=GAME_STATUS_FREE;
					ZeroMemory(&m_UserAttribute,sizeof(m_UserAttribute));

					//ɾ��ʱ��
					KillGameClock(0L);

					//���ñ���
					SetGameFrameTitle();

					//������Ϸ
					m_pIClientKernelSink->ResetGameClient();

					//ɾ���û�
					m_GameUserManagerModule->ResetUserItem();
				}
 				else
 				{
					//�û��뿪
					if (m_ServerAttribute.wServerType != GAME_GENRE_MATCH)
					{
						//ɾ��ʱ��
						if (pIClientUserItem->GetUserStatus()!=US_LOOKON)
						{
							WORD wChairID=pIClientUserItem->GetChairID();
							if (m_wClockChairID==wChairID) KillGameClock(m_wClockID);
						}

						//ɾ���û�
						m_GameUserManagerModule->DeleteUserItem(pIClientUserItem);
					}
 				}

				return true;
			}

			//����״̬
			ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
			m_GameUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);

			return true;
		}
	case IPC_SUB_GF_USER_ATTRIB:	//�û�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GF_UserAttrib));
			if (wDataSize!=sizeof(IPC_GF_UserAttrib)) return false;

			//Ѱ���û�
			IPC_GF_UserAttrib * pUserAttrib=(IPC_GF_UserAttrib *)pData;
			IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserAttrib->dwUserID);

			//�û��ж�
			ASSERT(pIClientUserItem!=NULL);
			if (pIClientUserItem==NULL) return true;

			//���ù�ϵ
			ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
			m_GameUserManagerModule->UpdateUserItemAttrib(pIClientUserItem,&pUserAttrib->UserAttrib);

			return true;
		}
	case IPC_SUB_GF_CUSTOM_FACE:	//�Զ�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GF_CustomFace));
			if (wDataSize!=sizeof(IPC_GF_CustomFace)) return false;

			//Ѱ���û�
			IPC_GF_CustomFace * pCustomFace=(IPC_GF_CustomFace *)pData;
			IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(pCustomFace->dwUserID);

			//�û��ж�
			ASSERT(pIClientUserItem!=NULL);
			if (pIClientUserItem==NULL) return true;

			//���û���
			ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
			m_GameUserManagerModule->UpdateUserCustomFace(pIClientUserItem,pCustomFace->dwCustomID,pCustomFace->CustomFaceInfo);

			return true;
		}
	case IPC_SUB_GR_USER_ITEM:
		{
			IPC_GF_UserItemList* pItemList=(IPC_GF_UserItemList*)pData;

			WORD wHeadSize=sizeof(IPC_GF_UserItemList)-sizeof(pItemList->GameItemInfo);
			WORD wSize =pItemList->wCount*sizeof(pItemList->GameItemInfo[0]);

			//Ч�����
			ASSERT((wHeadSize+wSize)==wDataSize);
			if ((wHeadSize+wSize)!=wDataSize) return false;

			//IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(pItemList->dwUserID);

			//�û��ж�
			//ASSERT(pIClientUserItem!=NULL);
			//if (pIClientUserItem==NULL) return true;

			ZeroMemory(&m_GameItemList,sizeof(m_GameItemList));
			m_GameItemList.dwUserID		= pItemList->dwUserID;
			m_GameItemList.wCount		= pItemList->wCount;
			m_GameItemList.wKindID		= pItemList->wKindID;
			m_GameItemList.wServerID	= pItemList->wServerID;

			OutputDebugString(pItemList->GameItemInfo[0].szItemName);

			CopyMemory(m_GameItemList.GameItemInfo,pItemList->GameItemInfo,pItemList->wCount*sizeof(pItemList->GameItemInfo[0]));

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//������Ϣ
bool CClientKernel::OnProcessMainPropertyInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case IPC_SUB_GF_PROPERTY_SUCCESS:	//���߳ɹ�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GR_PropertySuccess));
			if (wDataSize!=sizeof(IPC_GR_PropertySuccess)) return false;

			//��ȡ����
			IPC_GR_PropertySuccess * pPropertySuccess=(IPC_GR_PropertySuccess *)pData;
			ASSERT(m_pIClientPropertySink!=NULL);
			if(m_pIClientPropertySink==NULL) return true;

			//���߳ɹ�
			m_pIClientPropertySink->OnPropertySuccess(pPropertySuccess->szSourceNickName,pPropertySuccess->dwSourceUserID,pPropertySuccess->dwTargetUserID, pPropertySuccess->wPropertyIndex,pPropertySuccess->wPropertyCount);
				
			return true;
		}
	case IPC_SUB_GF_PROPERTY_FAILURE:	//����ʧ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GR_PropertyFailure));
			if (wDataSize!=sizeof(IPC_GR_PropertyFailure)) return false;

			//��ȡ����
			IPC_GR_PropertyFailure * pPropertyFailure=(IPC_GR_PropertyFailure *)pData;
			ASSERT(m_pIClientPropertySink!=NULL);
			if(m_pIClientPropertySink==NULL) return true;

			//���߳ɹ�
			m_pIClientPropertySink->OnPropertyFailure(pPropertyFailure->lErrorCode,pPropertyFailure->szDescribeString);
				
			return true;
		}
	case IPC_SUB_GR_PROPERTY_MESSAGE:   //������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GR_PropertyMessage));
			if (wDataSize!=sizeof(IPC_GR_PropertyMessage)) return false;

			//��ȡ����
			IPC_GR_PropertyMessage * pPropertyMessage=(IPC_GR_PropertyMessage *)pData;
			ASSERT(pPropertyMessage!=NULL);
			if(pPropertyMessage==NULL) return true;

			//��ȡ����
			ASSERT(CGamePropertyManager::GetInstance()!=NULL);
			CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

			//Ѱ�ҵ���
			ASSERT(pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex)!=NULL);
			CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex);
			if(pGamePropertyItem==NULL) return true;

			//��Ϣ����
			TCHAR szNotifyMessage[128]=TEXT("");			
			pGamePropertyItem->GetNotifyMessage(pPropertyMessage->szSourceNickName,pPropertyMessage->szTargerNickName,pPropertyMessage->wPropertyCount,szNotifyMessage);

			//������Ϣ
			m_pIStringMessage->InsertPromptString(szNotifyMessage);

			return true;
		}
	case IPC_SUB_GR_PROPERTY_TRUMPET:  //��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(IPC_GR_PropertyTrumpet));
			if (wDataSize!=sizeof(IPC_GR_PropertyTrumpet)) return false;

			//��ȡ����
			IPC_GR_PropertyTrumpet * pPropertyTrumpet=(IPC_GR_PropertyTrumpet *)pData;
			ASSERT(pPropertyTrumpet!=NULL);
			if(pPropertyTrumpet==NULL) return true;

			//������Ϣ
			if(pPropertyTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)
				m_pIStringMessage->InsertUserTrumpet(pPropertyTrumpet->szSendNickName,pPropertyTrumpet->szTrumpetContent,pPropertyTrumpet->TrumpetColor);
            
			if(pPropertyTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON)
				m_pIStringMessage->InsertUserTyphon(pPropertyTrumpet->szSendNickName,pPropertyTrumpet->szTrumpetContent,pPropertyTrumpet->TrumpetColor);

            return true;
		}
	case IPC_SUB_GR_PROPERTY_GLAD_MSG: // ������Ϣ
		{
			IPC_GR_PropertyNoticeMsg* pNoticeMsg = (IPC_GR_PropertyNoticeMsg*)pData;

			//Ч�����
			ASSERT(wDataSize == sizeof(IPC_GR_PropertyNoticeMsg));
			if (wDataSize != sizeof(IPC_GR_PropertyNoticeMsg)) return false;

			//������Ϣ����
			g_GlobalUnits.m_notifyMsg.push_back(*pNoticeMsg);

			//��֪ͨ����
			g_GlobalUnits.m_notify = true;

			return true;
		}
	case IPC_SUB_GF_PROPERTY_CHAT_MSG:
		{
			IPC_GF_PropertyChatMsg* pChatMsg = (IPC_GF_PropertyChatMsg*)pData;
			OutputDebugString(pChatMsg->szChatMsg);
			//Ч�����
			ASSERT(wDataSize == sizeof(IPC_GF_PropertyChatMsg));
			if (wDataSize != sizeof(IPC_GF_PropertyChatMsg)) return false;

			if ( pChatMsg->wChannel == 3 ) //������
			{
				m_pIStringMessage->InsertUserTyphon(pChatMsg->szNickName,pChatMsg->szChatMsg,RGB(0,255,0));
			}
			else if ( pChatMsg->wChannel == 4 ) //С����
			{
				m_pIStringMessage->InsertUserTrumpet(pChatMsg->szNickName,pChatMsg->szChatMsg,RGB(125,70,30));
			}
			else if ( pChatMsg->wChannel == 1 ) //ϵͳ��Ϣ
			{
				m_pIStringMessage->InsertSystemString(pChatMsg->szChatMsg);
			}
			if(CGameGladMessage::GetInstance() != NULL)
			CGameGladMessage::GetInstance()->InsertGladMessage(pChatMsg->szChatMsg,pChatMsg->szNickName,TEXT("1,000,000"),0xFFFFFF,0xFFB300,0xFFB300);
			return true;
		}
	case IPC_SUB_GF_PROPERTY_PING_MSG:
		{
			WORD* pSpeed = (WORD*)pData;
			if (wDataSize != sizeof(WORD)) return false;

			TCHAR sztmp[32]={0};
			_stprintf(sztmp,_T("�����ٶ�:%d ms"),*pSpeed);
			m_pIStringMessage->InsertSystemString(sztmp);
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�û�����
bool CClientKernel::OnSocketSubUserChat(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GF_S_UserChat * pUserChat=(CMD_GF_S_UserChat *)pData;

	//Ч�����
	ASSERT(wDataSize>=(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//Ч�����
	if (wDataSize<(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//��ʾ��Ϣ
	if (m_pIStringMessage!=NULL)
	{
		//��ȡ�û�
		IClientUserItem * pISendUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserChat->dwSendUserID);
		if (pISendUserItem==NULL) return true;

		//��Ϣ����
		if (pISendUserItem!=m_pIMySelfUserItem)
		{
			//���ű���
			DWORD dwUserGroupID=pISendUserItem->GetGroupID();
			DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

			//�����ж�
			bool bSameGroup=false;
			if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

			//��������
			BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

			//����ģʽ
			if ((pGlobalUnits->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
			{
				return true;
			}

			//���ģʽ
			if ((pGlobalUnits->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
			{
				return true;
			}
		}

		//������Ϣ
		if (pUserChat->dwTargetUserID!=0L)
		{
			//��ȡ�û�
			IClientUserItem * pIRecvUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserChat->dwTargetUserID);

			if (pIRecvUserItem!=NULL)
			{
				//������Ϣ
				LPCTSTR pszSendUser=pISendUserItem->GetNickName();
				LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
				m_pIStringMessage->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,RGB(255, 255, 255));
			}
		}
		else 
		{
			//������Ϣ
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			m_pIStringMessage->InsertUserChat(pszSendUser, pUserChat->szChatString, RGB(255, 255, 255));
		}

#ifdef PLAYCHATSHORTSOUND
		//��ȡ����
		BYTE cbIndex = GetChatShortIndex(pUserChat->szChatString);
		if(cbIndex > 0)
		{
			//��������
			PlayChatShortSound(pISendUserItem->GetGender(), cbIndex);
		}
#endif
	}

	return true;
}

//�û�����
bool CClientKernel::OnSocketSubExpression(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GF_S_UserExpression));
	if (wDataSize!=sizeof(CMD_GF_S_UserExpression)) return false;

	//��������
	CMD_GF_S_UserExpression * pUserExpression=(CMD_GF_S_UserExpression *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//��������
	CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
	CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

		//����ģʽ
		if ((pGlobalUnits->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pGlobalUnits->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//��ʾ��Ϣ
	if ((m_pIStringMessage!=NULL)&&(pExpressionItem!=NULL))
	{
		//��ȡ·��
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

		//������Ϣ
		if (pUserExpression->dwTargetUserID!=0L)
		{
			//��ȡ�û�
			IClientUserItem * pIRecvUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserExpression->dwTargetUserID);

			//������Ϣ
			if (pIRecvUserItem!=NULL)
			{
				LPCTSTR pszSendUser=pISendUserItem->GetNickName();
				LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
				m_pIStringMessage->InsertExpression(pszSendUser,pszRecvUser,szImagePath);
			}
		}
		else
		{
			//������Ϣ
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			m_pIStringMessage->InsertExpression(pszSendUser,szImagePath);
		}
	}

	return true;
}

//��Ϸ״̬
bool CClientKernel::OnSocketSubGameStatus(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GF_GameStatus));
	if (wDataSize!=sizeof(CMD_GF_GameStatus)) return false;

	//��Ϣ����
	CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)pData;

	//���ñ���
	m_cbGameStatus=pGameStatus->cbGameStatus;
	m_bAllowLookon=pGameStatus->cbAllowLookon?true:false;

	return true;
}

//��Ϸ����
bool CClientKernel::OnSocketSubGameScene(VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return true;

	//��������
	bool bLookonUser=(m_pIMySelfUserItem->GetUserStatus()==US_LOOKON);
	return m_pIClientKernelSink->OnEventSceneMessage(m_cbGameStatus,bLookonUser,pData,wDataSize);
}

//�Թ�״̬
bool CClientKernel::OnSocketSubLookonStatus(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GF_LookonStatus));
	if (wDataSize!=sizeof(CMD_GF_LookonStatus)) return false;

	//��Ϣ����
	if (IsLookonMode()==true)
	{
		//��������
		CMD_GF_LookonStatus * pLookonStatus=(CMD_GF_LookonStatus *)pData;

		//�¼�����
		m_bAllowLookon=(pLookonStatus->cbAllowLookon==TRUE)?true:false;

		//�¼�֪ͨ
		ASSERT(m_pIMySelfUserItem!=NULL);
		m_pIClientKernelSink->OnEventLookonMode(pData,wDataSize);

		//��ʾ��Ϣ
		if (m_bAllowLookon==true)
		{
			if (m_pIStringMessage!=NULL)
			{
				m_pIStringMessage->InsertNormalString(TEXT("��������ۿ������Ϸ"));
			}
		}
		else
		{
			if (m_pIStringMessage!=NULL)
			{
				m_pIStringMessage->InsertNormalString(TEXT("������ֹ�ۿ������Ϸ"));
			}
		}
	}

	return true;
}

//ϵͳ��Ϣ
bool CClientKernel::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	//�رմ���
 	if ((pSystemMessage->wType&SMT_CLOSE_GAME)!=0)
 	{
 		//���ñ���
 		m_bService=false;
 
 		//ɾ��ʱ��
 		KillGameClock(0);
 
 		//�ж�����
 		//IntermitConnect();
		//g_Log.LogToFile(g_szLogRoot,TEXT("ϵͳ��Ϣ֪ͨ��Ϸ�ر�"));
 	}

	//��ʾ��Ϣ
	if ((pSystemMessage->wType&SMT_CHAT)&&(m_pIStringMessage!=NULL))
	{
		m_pIStringMessage->InsertSystemString(pSystemMessage->szString);
	}

	//������Ϣ
	if (pSystemMessage->wType&SMT_EJECT)
	{
		CDlgGameClose info(pSystemMessage->szString,AfxGetMainWnd());
		if (info.DoModal() != IDOK)
		{
			//return TRUE;
		}
		//CGameFrameView* pGameFrameView=CGameFrameView::GetInstance();
		//if (pGameFrameView) pGameFrameView->ShowInsureGetZJG(this,CPoint(0,0));
	}

	//�رշ���
 	if (pSystemMessage->wType&SMT_CLOSE_GAME)
 	{
		//g_Log.LogToFile(g_szLogRoot,TEXT("OnSocketSubSystemMessage"));
 		m_pIClientKernelSink->CloseGameClient();
 	}

	return true;
}

//������Ϣ
bool CClientKernel::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(TCHAR)))) return false;

	//�رմ���
	if ((pActionMessage->wType&SMT_CLOSE_GAME)!=0)
	{
		//���ñ���
		m_bService=false;

		//ɾ��ʱ��
		KillGameClock(0L);

		//�ж�����
		//IntermitConnect();
		//g_Log.LogToFile(g_szLogRoot,TEXT("������Ϣ֪ͨ��Ϸ�ر�"));
	}

	//������Ϣ
	INT nResultCode=ShowInformation(pActionMessage->szString,pActionMessage->nButtonType,0);

	//��������
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//��ȡ����
	while (wExcursion<wDataSize)
	{
		//��������
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//Ч�����
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//Ч�����
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//��������
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//�������
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E ���
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("open"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					//�������
					if (pActionBrowse->cbBrowseType&BRT_PLAZA)
					{
//						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true,true);
					}

					break;
				}
			case ACT_DOWN_LOAD:	//���ض���
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E ����
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("open"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//����ģ��
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("open"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//����ƫ��
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//�رշ���
	if (pActionMessage->wType&SMT_CLOSE_GAME) 
	{
		//g_Log.LogToFile(g_szLogRoot,TEXT("OnSocketSubSystemMessage"));
		m_pIClientKernelSink->CloseGameClient();
	}

	return true;
}

//���ñ���
VOID CClientKernel::SetGameFrameTitle()
{
	//��������
	TCHAR szTitle[128]=TEXT("");

	//�������
	//if ((m_ServerAttribute.szServerName[0]==0)||(m_UserAttribute.dwUserID==0L))
	{
		if (VERSION_EFFICACY==FALSE)
		{
			//��ȡ����
			//TCHAR szModuleName[MAX_PATH]=TEXT("");
			//GetModuleFileName(AfxGetInstanceHandle(),szModuleName,CountArray(szModuleName));

			//��ȡ�汾
			//DWORD dwClientVersion=0L;
			//CWHService::GetModuleVersion(szModuleName,dwClientVersion);

			//�������
			//_sntprintf(szTitle,CountArray(szTitle),TEXT("%s �� Build %ld.%ld.%ld.%ld"),m_GameAttribute.szGameName,GetProductVer(dwClientVersion),
			//	GetMainVer(dwClientVersion),GetSubVer(dwClientVersion),GetBuildVer(dwClientVersion));
			TCHAR szServerName[128]={0};

			if (m_ServerAttribute.wServerType&GAME_GENRE_MATCH)
			{		
				_sntprintf(szTitle,CountArray(szTitle),TEXT("%s - %s"),m_GameAttribute.szGameName,m_ServerAttribute.szServerName);
			}
			else
			{
				switch ( m_ServerTypeEx )
				{
				case 1:
					_stprintf(szServerName,_T("���鳡"));
					break;
				case 2:
					_stprintf(szServerName,_T("���ֳ�"));
					break;
				case 3:
					_stprintf(szServerName,_T("������"));
					break;
				case 4:
					_stprintf(szServerName,_T("�м���"));
					break;
				case 5:
					_stprintf(szServerName,_T("�߼���"));
					break;
				case 6:
					_stprintf(szServerName,_T("������"));
					break;
				}

				_sntprintf(szTitle,CountArray(szTitle),TEXT("%s->%s->�׷�%d"),m_GameAttribute.szGameName,szServerName,m_lCellScore);
			}
			
		}
		else
		{
			//�������
			DWORD dwClientVersion=m_GameAttribute.dwClientVersion;
			_sntprintf(szTitle,CountArray(szTitle),TEXT("%s �� Build %ld.%ld.%ld.%ld"),m_GameAttribute.szGameName,GetProductVer(dwClientVersion),
				GetMainVer(dwClientVersion),GetSubVer(dwClientVersion),GetBuildVer(dwClientVersion));
		}
	}
// 	else
// 	{
// 		if (CServerRule::IsAllowAvertCheatMode(m_ServerAttribute.dwServerRule)==false)
// 		{
// 			_sntprintf(szTitle,CountArray(szTitle),TEXT("%s �� %s - �� %d �� ��"),m_GameAttribute.szGameName,m_ServerAttribute.szServerName,m_UserAttribute.wTableID+1);
// 		}
// 		else
// 		{
// 			_sntprintf(szTitle,CountArray(szTitle),TEXT("%s �� %s ��"),m_GameAttribute.szGameName,m_ServerAttribute.szServerName);
// 			
// 		}
// 	}

	//���ñ���
	CWnd * pMainWnd=AfxGetMainWnd();
	if ((pMainWnd!=NULL)&&(pMainWnd->m_hWnd!=NULL)) pMainWnd->SetWindowText(szTitle);
}

//ʱ����Ϣ
VOID CClientKernel::OnTimer(UINT nIDEvent)
{
	//ʱ�䴦��
	if ((m_wClockID==nIDEvent)&&(m_wClockChairID!=INVALID_CHAIR))
	{
		//�¼�����
		if (m_nElapseCount>0) m_nElapseCount--;
		bool bSuccess=m_pIClientKernelSink->OnEventGameClockInfo(m_wClockChairID,m_nElapseCount,m_wClockID);

		//ɾ������
		if ((m_nElapseCount==0)||(bSuccess==false)) KillGameClock(m_wClockID);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//���Ŷ�������
bool CClientKernel::PlayChatShortSound(BYTE cbGender, BYTE cbIndex)
{
	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();

	//����
	if((!pGlobalUnits->m_bMuteStatuts) && pGlobalUnits->m_bAllowSound)
	{
		//����Ŀ¼
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//����·��
		TCHAR szFileName2[MAX_PATH]=TEXT("");
		_sntprintf(szFileName2,CountArray(szFileName2),TEXT("%s\\%s\\PhraseInfo.ini"),szDirectory,AfxGetApp()->m_pszExeName);

		//�ж��ļ�
		bool bGameConfig = false;
		CFile file;
		if(file.Open(szFileName2,CFile::modeRead)) bGameConfig = true;
		file.Close();

		//����·��
		TCHAR szFileName[MAX_PATH]=TEXT("");
		if(bGameConfig)
		{
			if(cbGender==GENDER_FEMALE)
				_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\%s\\PhraseSound\\GAME\\FEMALE\\w%d.wav"),szDirectory,AfxGetApp()->m_pszExeName,cbIndex);
			else
				_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\%s\\PhraseSound\\GAME\\MANKIND\\m%d.wav"),szDirectory,AfxGetApp()->m_pszExeName,cbIndex);
		}
		else
		{
			if(cbGender==GENDER_FEMALE)
				_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PhraseSound\\GAME\\FEMALE\\w%d.wav"),szDirectory,cbIndex);
			else
				_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PhraseSound\\GAME\\MANKIND\\m%d.wav"),szDirectory,cbIndex);
		}

		//��ȡ����
		CD3DSound * pD3DSound=CD3DSound::GetInstance();

		//��������
		if(pD3DSound)
		{
			return pD3DSound->PlaySound(szFileName,false);
		}
	}

	return false;
}

//��ö����±�
BYTE CClientKernel::GetChatShortIndex(LPCTSTR pChat)
{
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	TCHAR szFileName2[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\%s\\PhraseInfo.ini"),szDirectory,AfxGetApp()->m_pszExeName);
	_sntprintf(szFileName2,CountArray(szFileName2),TEXT("%s\\PhraseInfo.ini"),szDirectory);

	//�ж��ļ�
	TCHAR *pFileName = szFileName;
	CFile file;
	if(!file.Open(szFileName,CFile::modeRead)) pFileName = szFileName2;
	file.Close();
	ASSERT(pFileName);

	//��������
	BYTE cbIndex=1;
	TCHAR szItemName[16]=TEXT(""),szShortcut[LEN_USER_CHAT]=TEXT("");

	//��ʽ��Ϣ
	CString strChat = pChat;
	strChat.TrimLeft();
	strChat.TrimRight();

	//��ȡ��Ϣ
	while (true)
	{
		//��ȡ��Ϣ
		_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),cbIndex);
		GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),pFileName);

		//����ж�
		if (szShortcut[0]!=0)
		{
			//��ʽ��Ϣ
			CString strTempChat = szShortcut;
			strTempChat.TrimLeft();
			strTempChat.TrimRight();

			//�Ƚ�
			if(strTempChat == strChat) return cbIndex;

			//���ñ���
			cbIndex++;
		}

		//�����ж�
		if ((szShortcut[0]==0)||(cbIndex>=MAX_SHORT_COUNT))
		{
			break;
		}
	}

	return 0;
}

bool CClientKernel::IsWebPlazaReady()
{
	if ( !m_hWebPlaza )	m_hWebPlaza = ::FindWindow(_T("CefBrowserWindow"),NULL);
	if ( !m_hWebPlaza ) m_hWebPlaza = ::FindWindow(_T("WebViewHost"),NULL);
	if ( m_hWebPlaza ) return true;

	return false;
}

tagGameItemInfo* CClientKernel::GetUserItemInfo(enUserItem enUI)
{
	OutputDebugString(L"��ȡ����");
	if ( !m_GameItemList.wCount ) return NULL;
	for (INT_PTR i=0;i<m_GameItemList.wCount;++i)
	{
		if (m_GameItemList.GameItemInfo[i].wItemID == enUI)
		{
			OutputDebugString(m_GameItemList.GameItemInfo[i].szItemName);
			return &m_GameItemList.GameItemInfo[i];
		}
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(ClientKernel);

//////////////////////////////////////////////////////////////////////////////////
