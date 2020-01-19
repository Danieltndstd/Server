#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameControl.h"

//////////////////////////////////////////////////////////////////////////////////

//���涨��
//#define USER_VIEW_CY				223									//�û���ͼ
//#define USER_LIST_CY				100									//�û��б�
//#define PROPERTY_CY				89									//��Ϸ����
//#define MEDAL_VIEW_CY				63									//������ͼ

#define TITLE_VIEW_CY				0
#define USER_VIEW_CY				0									//�û���ͼ
#define USER_LIST_CY				150									//�û��б�
#define PROPERTY_CY					0									//��Ϸ����
#define MEDAL_VIEW_CY				0									//������ͼ



//�ָ����
//#define INCISE_SCALE				50/100								//����ָ�

//�ؼ���ʶ
#define IDC_PROPERTY				300									//��Ϸ����
#define IDC_USER_VIEW				301									//�û���Ϣ
#define IDC_MEDAL_VIEW				302									//������Ϣ
#define IDC_CHAT_CONTROL			303									//�������
#define IDC_CHAT_BUTTON			304								//�ջ������

//////////////////////////////////////////////////////////////////////////////////
//�˵�����

//��ɫ�˵�
#define MAX_CHAT_COLOR				16									//�����Ŀ
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//ѡ����ɫ

//��ݶ���
#define MAX_SHORT_COUNT				16									//�����Ŀ
#define IDM_SELECT_CHAT_SHORT		(WM_USER+300)						//ѡ�����

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameControl, CDialog)

	//ϵͳ��Ϣ
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()

	//�ؼ���Ϣ
	//ON_NOTIFY(NM_RCLICK, IDC_USER_LIST_CONTROL, OnNMRclickUserList)
	//ON_NOTIFY(NM_CLICK, IDC_USER_LIST_CONTROL, OnNMClickUserList)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_USER_LIST_CONTROL, OnLvnItemchangedUserList)

	//��������
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_SHORT, (IDM_SELECT_CHAT_SHORT+MAX_SHORT_COUNT), OnSelectChatShort)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameControl::CGameFrameControl() : CDialog(IDD_GAME_CONTROL)
{
	//�������
	m_bCreateFlag=false;

	//�������
	m_dwChatTime=0L;

	//�ӿڱ���
	m_pIClientKernel=NULL;
	m_pIMySelfUserItem=NULL;

	//�ؼ�����
	m_pExpressionControl=NULL;
	m_pTrumpetItem=NULL;

	//������Դ
	tagEncircleResource	EncircleChat;
	EncircleChat.pszImageTL=MAKEINTRESOURCE(IDB_CHAT_TL);
	EncircleChat.pszImageTM=MAKEINTRESOURCE(IDB_CHAT_TM);
	EncircleChat.pszImageTR=MAKEINTRESOURCE(IDB_CHAT_TR);
	EncircleChat.pszImageML=MAKEINTRESOURCE(IDB_CHAT_ML);
	EncircleChat.pszImageMR=MAKEINTRESOURCE(IDB_CHAT_MR);
	EncircleChat.pszImageBL=MAKEINTRESOURCE(IDB_CHAT_BL);
	EncircleChat.pszImageBM=MAKEINTRESOURCE(IDB_CHAT_BM);
	EncircleChat.pszImageBR=MAKEINTRESOURCE(IDB_CHAT_BR);
	m_ChatEncircle.InitEncircleResource(EncircleChat,GetModuleHandle(GAME_FRAME_DLL_NAME));

	//�û���Դ
	tagEncircleResource	EncircleUserList;
	EncircleUserList.pszImageTL=MAKEINTRESOURCE(IDB_USER_LIST_TL);
	EncircleUserList.pszImageTM=MAKEINTRESOURCE(IDB_USER_LIST_TM);
	EncircleUserList.pszImageTR=MAKEINTRESOURCE(IDB_USER_LIST_TR);
	EncircleUserList.pszImageML=MAKEINTRESOURCE(IDB_USER_LIST_ML);
	EncircleUserList.pszImageMR=MAKEINTRESOURCE(IDB_USER_LIST_MR);
	EncircleUserList.pszImageBL=MAKEINTRESOURCE(IDB_USER_LIST_BL);
	EncircleUserList.pszImageBM=MAKEINTRESOURCE(IDB_USER_LIST_BM);
	EncircleUserList.pszImageBR=MAKEINTRESOURCE(IDB_USER_LIST_BR);
	m_UserListEncircle.InitEncircleResource(EncircleUserList,GetModuleHandle(GAME_FRAME_DLL_NAME));

	//
	tagEncircleResource	EncircleBG;
	EncircleBG.pszImageTL=MAKEINTRESOURCE(IDB_CONTROL_TL);
	EncircleBG.pszImageTM=MAKEINTRESOURCE(IDB_CONTROL_TM);
	EncircleBG.pszImageTR=MAKEINTRESOURCE(IDB_CONTROL_TR);
	EncircleBG.pszImageML=MAKEINTRESOURCE(IDB_CONTROL_ML);
	EncircleBG.pszImageMR=MAKEINTRESOURCE(IDB_CONTROL_MR);
	EncircleBG.pszImageBL=MAKEINTRESOURCE(IDB_CONTROL_BL);
	EncircleBG.pszImageBM=MAKEINTRESOURCE(IDB_CONTROL_BM);
	EncircleBG.pszImageBR=MAKEINTRESOURCE(IDB_CONTROL_BR);
	EncircleBG.pszImageMM=MAKEINTRESOURCE(IDB_CONTROL_MM);
	m_EncircleBG.InitEncircleResource(EncircleBG,GetModuleHandle(GAME_FRAME_DLL_NAME));

	//ע�����
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_CONTROL,QUERY_ME_INTERFACE(IUnknownEx));
	pGlobalUnits->RegisterGlobalModule(MODULE_STRING_MESSAGE,QUERY_OBJECT_INTERFACE(m_ChatMessage,IUnknownEx));

	return;
}

//��������
CGameFrameControl::~CGameFrameControl()
{
	//ɾ������
	SafeDelete(m_pExpressionControl);
	SafeDelete(m_pTrumpetItem);

	return;
}

//�ӿڲ�ѯ
VOID * CGameFrameControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameFrameControl,Guid,dwQueryVer);
	QUERYINTERFACE(IGamePropertySink,Guid,dwQueryVer);
	QUERYINTERFACE(IGamePropertyUseSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameControl,Guid,dwQueryVer);
	return NULL;
}

//���ÿؼ�
VOID CGameFrameControl::ResetControl()
{
	//���ñ���
	m_pIMySelfUserItem=NULL;
	m_MenuUserItemArray.RemoveAll();

	//��������
	m_ChatControl.SetChatTargetUser(NULL);

	//���ÿؼ�
	m_UserListControl.DeleteAllItems();
	m_GameFrameUserView.SetUserViewInfo(NULL);

	return;
}

//���ù��
VOID CGameFrameControl::SetGameBillInfo(WORD wKindID, WORD wServerID)
{
	//���ù��
	m_GameFrameUserView.SetGameBillInfo(wKindID,wServerID);

	return;
}

//�����û�
VOID CGameFrameControl::SetUserViewInfo(IClientUserItem * pIClientUserItem)
{
	//�����û�
	m_GameFrameUserView.SetUserViewInfo(pIClientUserItem);

	return;
}

//�б�����
VOID CGameFrameControl::SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount)
{
	//�����б�
	m_UserListControl.SetColumnDescribe(ColumnItem,cbColumnCount);

	return;
}

//��ʾ����
VOID CGameFrameControl::SetUserListControl(bool bShow)
{
	if ( bShow )
	{
		m_UserListControl.ShowWindow(SW_SHOW);
	}
	else
	{
		m_UserListControl.ShowWindow(SW_HIDE);
	}
}

//�������
bool CGameFrameControl::OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//���ñ���
	IPC_GF_BuyProperty  BuyProperty;
	BuyProperty.wItemCount=wItemCount;
	BuyProperty.wPropertyIndex=wPropertyIndex;
	CopyMemory(BuyProperty.szNickName,pszNickName,sizeof(BuyProperty.szNickName));

	//��������
	return m_pIClientKernel->SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_BUY_PROPERTY,&BuyProperty,sizeof(BuyProperty));
}

bool CGameFrameControl::OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage)
{
	//ָ���ж�
	if(m_pIClientKernel==NULL) return false;

	//�����׳�
	tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();
	if (CServerRule::IsAllowAvertCheatMode(pServerAttribute->dwServerRule))
	{
		TCHAR szMessage[128]=TEXT("�����׳��������������");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	//����û�
	IClientUserItem * pIClientUserItem=m_pIClientKernel->SearchUserByNickName(pszNickName);
	if(pIClientUserItem==NULL)
	{
		TCHAR szMessage[128]=TEXT("��ָ����ʹ�ö����ڱ����л򲻴��ڣ�");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	return true;
}

//ʹ�õ���
bool CGameFrameControl::OnEventUseProperty(WORD wPropertyIndex)
{
	//��������
	if(wPropertyIndex==PROPERTY_ID_TRUMPET || wPropertyIndex==PROPERTY_ID_TYPHON)
	{
		//������
		IPC_GR_PropertyTrumpet PropertyTrumpet;
		PropertyTrumpet.wPropertyIndex=wPropertyIndex;
		PropertyTrumpet.TrumpetColor=m_pTrumpetItem->GetTrumpetColor();
		m_pTrumpetItem->GetTrumpetContent(PropertyTrumpet.szTrumpetContent);
		CopyMemory(PropertyTrumpet.szSendNickName,m_pIMySelfUserItem->GetNickName(),sizeof(PropertyTrumpet.szSendNickName));

		//��������
		return m_pIClientKernel->SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET,&PropertyTrumpet,sizeof(PropertyTrumpet));

	}

	return true;
}

//��ʾ�˵�
VOID CGameFrameControl::TrackUserItemMenu(IClientUserItem * pIClientUserItem)
{
	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//���ñ���
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.Add(pIClientUserItem);

	//����˵�
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//��������
	TCHAR szMenuString[256]=TEXT("");
	bool bMeUserItem=(pIClientUserItem==m_pIMySelfUserItem);

	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//��������
	LPCTSTR pszUserNote=pIClientUserItem->GetUserNoteInfo();
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	IGameLevelParser * pGameLevelParser=CGlobalUnits::GetInstance()->m_GameLevelParserModule.GetInterface();

	//������Ƭ
	CImage ImageUserCard;
	pUserItemElement->ConstructNameCard(pIClientUserItem,pGameLevelParser,ImageUserCard);
	if (ImageUserCard.IsNull()==false) UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,ImageUserCard,MF_GRAYED);

	//����ָ�
	UserInfoMenu.AppendSeparator();

	//����˵�
	UserInfoMenu.AppendMenu(IDM_COPY_USER_NICKNAME,TEXT("�����ǳ�"));

	//���ý�̸
	if ((bMeUserItem==false)&&(pIChatTargetItem!=m_pIMySelfUserItem)&&(pIChatTargetItem!=pIClientUserItem))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�� [ %s ] ��̸"),pIClientUserItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_SET_CHAT_USER,szMenuString);
	}

	//ȡ����̸
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("ȡ���� [ %s ] ��̸"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//�����˵�
/*	if ((bMeUserItem==false)&&(cbMeUserStatus!=US_PLAYING)&&(cbUserStatus==US_LOOKON))
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//������Ϸ
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("���� [ %s ] һ������Ϸ"),(LPCTSTR)strAccounts);
		UserInfoMenu.AppendMenu(IDM_INVITE_USER,szMenuString);
	}*/

	//���ѹ���
	if (bMeUserItem==false&&m_pIClientKernel->GetServerAttribute()->wServerType!=GAME_GENRE_MATCH)
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//����˵�
		BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
		UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("����Ϊ����"),(cbCompanion==CP_FRIEND)?MF_CHECKED:0);
		UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("����Ϊ���"),(cbCompanion==CP_DETEST)?MF_CHECKED:0);
	}

	//��Ա�˵�
	if( !bMeUserItem && CUserRight::CanKillOutUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_pIMySelfUserItem->GetMasterOrder() == 0
		&& m_pIClientKernel->GetServerAttribute()->wServerType!=GAME_GENRE_MATCH)
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();
        
		//��������
		BYTE  cbMeUserStatus = m_pIMySelfUserItem->GetUserStatus();
		BYTE  cbUserStatus = pIClientUserItem->GetUserStatus();
		bool bEnableKick = true;
		if( pMeUserInfo->cbMemberOrder<=pUserInfo->cbMemberOrder || (cbMeUserStatus!=US_SIT&&cbMeUserStatus!=US_READY) ||
			(cbUserStatus!=US_SIT&&cbUserStatus!=US_READY) )
			bEnableKick = false;

		//�Թ۲˵�
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�� [ %s ] �뿪"),pIClientUserItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_KICK_USER,szMenuString,bEnableKick?MF_ENABLED:MF_GRAYED);
	}

	/*
	tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();
	if(!(CServerRule::IsHideUserInfo(pServerAttribute->dwServerRule)) || pUserInfo->dwUserID==m_pIMySelfUserItem->GetUserID())
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//������Ϣ
		CW2CT strArea((pUserInfo->szArea[0]==0)?L"--":pUserInfo->szArea);
		CW2CT strCity((pUserInfo->szCity[0]==0)?L"--":pUserInfo->szCity);
		CW2CT strProvince((pUserInfo->szProvince[0]==0)?L"--":pUserInfo->szProvince);
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("ʡ�ݣ�%s ���У�%s ������%s"),(LPCTSTR)strProvince,(LPCTSTR)strCity,(LPCTSTR)strArea);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

		//������Ϣ
		LONG lPlayCount=pIClientUserItem->GetUserPlayCount();
		float fUserFleeRate=pIClientUserItem->GetUserFleeRate();
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�ɼ���%I64d  ����ֵ��%ld �ܾ�����%ld  �����ʣ�%5.2f%%"),
			pUserInfo->lScore,pUserInfo->lExperience,lPlayCount,fUserFleeRate);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

		//������Ϣ
		if (pUserInfo->szGroupName[0]!=0)
		{
			CW2CT strGroupName(pUserInfo->szGroupName);
			_sntprintf(szMenuString,CountArray(szMenuString),TEXT("��Ϸ���ţ�%s"),(LPCTSTR)(strGroupName));
			UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
		}

		//��ע��Ϣ
		LPCWSTR pszUserNote=pIClientUserItem->GetUserNoteInfo();
		if ((pszUserNote!=NULL)&&(pszUserNote[0]!=0))
		{
			CW2CT strUserNote(pIClientUserItem->GetUserNoteInfo());
			_sntprintf(szMenuString,CountArray(szMenuString),TEXT("��ע��Ϣ��%s"),(LPCTSTR)strUserNote);
			UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
		}
	}*/

	//������Ϣ
	DWORD dwPlayCount=pIClientUserItem->GetUserPlayCount();
	FLOAT fUserFleeRate=pIClientUserItem->GetUserFleeRate();
	_sntprintf(szMenuString,CountArray(szMenuString),TEXT("�ɼ���%I64d  ����ֵ��%ld �ܾ�����%ld  �����ʣ�%5.2f%%"),
		pUserInfo->lScore,pUserInfo->dwExperience,dwPlayCount,fUserFleeRate);
	UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

	//������Ϣ
	if (pUserInfo->szGroupName[0]!=0)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("��Ϸ���ţ�%s"),pUserInfo->szGroupName);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//��ע��Ϣ
	if ((pszUserNote!=NULL)&&(pszUserNote[0]!=0))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("��ע��Ϣ��%s"),pszUserNote);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//�Թ۲˵�
	/*if ((cbMeUserStatus!=US_LOOKON)&&(cbUserStatus==US_LOOKON))
	{
		//����ָ�
		UserInfoMenu.AppendSeparator();

		//�Թ۲˵�
		UserInfoMenu.AppendMenu(IDM_ALLOW_LOOKON,TEXT("�������Թ�����Ϸ"));
		UserInfoMenu.AppendMenu(IDM_FORBID_LOOKON,TEXT("��ֹ���Թ�����Ϸ"));
	}*/

	//�����˵�
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//��ʾ�˵�
VOID CGameFrameControl::TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount)
{
	//Ч�����
	ASSERT((pIClientUserItem!=NULL)&&(wItemCount>1));
	if ((pIClientUserItem==NULL)||(wItemCount<=1)) return;

	return;
}

//�û�����
VOID CGameFrameControl::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//�����Լ�
	if (m_pIMySelfUserItem==NULL)
	{
		//��������
		ASSERT(m_pIClientKernel->GetUserAttribute()!=NULL);
		tagUserAttribute * pUserAttribute=m_pIClientKernel->GetUserAttribute();
		tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();

		//�Լ��ж�
		if (pUserAttribute->dwUserID==pIClientUserItem->GetUserID())
		{
			//���ñ���
			m_pIMySelfUserItem=pIClientUserItem;

			//���ý���
			SetUserViewInfo(m_pIMySelfUserItem);
			m_UserListControl.SetMySelfUserID(m_pIMySelfUserItem->GetUserID());
			m_UserListControl.SetServerRule(pServerAttribute->dwServerRule);
			m_GameFrameMedalView.SetUserMedal(m_pIMySelfUserItem->GetUserMedal());
		}
	}

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//�����û�
	m_UserListControl.InsertDataItem(pIClientUserItem);

	//��ʾ��Ϣ
	if (pGlobalUnits && (pGlobalUnits->m_bNotifyUserInOut==true)&&(m_pIClientKernel->IsServiceStatus()==true))
	{
		m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
	}

	return;
}

//�û��뿪
VOID CGameFrameControl::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��������
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();

	//�������
	m_ChatControl.DeleteUserItem(pIClientUserItem);

	//ɾ���û�
	m_UserListControl.DeleteDataItem(pIClientUserItem);

	//�˵�����
	for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
	{
		//��ȡ�û�
		IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
		if (pIChatUserItem->GetUserID()==dwLeaveUserID) m_MenuUserItemArray.RemoveAt(i);
	}

	//��ʾ��Ϣ
	if (pGlobalUnits->m_bNotifyUserInOut==true)
	{
		m_ChatMessage.InsertUserLeave(pIClientUserItem->GetNickName());
	}

	return;
}

//�û�����
VOID CGameFrameControl::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���»���
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	//������Ϣ
	if (pIClientUserItem==m_pIMySelfUserItem)
	{
		m_GameFrameMedalView.SetUserMedal(pIClientUserItem->GetUserMedal());
		m_GameFrameUserView.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//�û�״̬
VOID CGameFrameControl::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//����״̬
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	return;
}

//�û�����
VOID CGameFrameControl::OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//����״̬
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	return;
}

//�û�ͷ��
VOID CGameFrameControl::OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//
VOID CGameFrameControl::OnUserTaskUpdate(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return ;
}

//��������
VOID CGameFrameControl::OnEventPropertyTrumpet()
{
	//���½���
	if(m_pTrumpetItem!=NULL) 
		m_pTrumpetItem->UpdateControlSurface();

	return;
}

//�û�ѡ��
VOID CGameFrameControl::OnChangeChatTarget(IClientUserItem * pIClientUserItem)
{
	//���ñ���
	if (pIClientUserItem!=NULL)
	{
		//���ý���
		if (m_ChatControl.m_hWnd==NULL)
		{
			//��������
			CRect rcCreate(0,0,0,0);
			m_ChatControl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHAT_CONTROL);

			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}
	else
	{
		//���ý���
		if (m_ChatControl.m_hWnd!=NULL)
		{
			//���ٴ���
			m_ChatControl.DestroyWindow();

			//��������
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	//���ý���
	m_ChatEdit.SetFocus();

	return;
}

//�����¼�
VOID CGameFrameControl::OnExpressionSelect(CExpression * pUserExpression, tagExpressionInfo * pExpressionInfo)
{
	bool hasChatControl = false;

	if (m_pIClientKernel && m_pIClientKernel->GetServerAttribute())
	{
		tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();
		hasChatControl = (pServerAttribute->wServerType & GAME_GENRE_MEDAL)!=0;
	}

	if (!hasChatControl)
	{
		//���ͱ���
		ASSERT(m_pIClientKernel!=NULL);
		m_pIClientKernel->SendUserExpression(m_ChatControl.GetChatTargetUserID(),pUserExpression->GetIndex());
		return;
	}

	//���ý���
	m_ChatEdit.SetFocus();

	if (!EfficacyUserChat(NULL,pUserExpression->GetIndex()))
	{
		return;
	}
		
	CString strChatMsg;
	int nStart = 0;
	int nEnd   = 0;
	// ��ȡ��ǰ���λ��
	//m_ChatEdit.GetSel(nStart, nEnd);
	// ��ȡ��ǰ����
	m_ChatEdit.GetWindowText(strChatMsg);

	// �������
	CString strInsert;
	strInsert	+= "/";
	strInsert	+= pExpressionInfo->szExpression[1];
	strChatMsg = strChatMsg.Mid(0, nStart) + strInsert + strChatMsg.Mid(nEnd, strChatMsg.GetLength() - nEnd);

	// ��������
	m_ChatEdit.SetWindowText(strChatMsg);
	// ������λ��
	nStart = nStart + strInsert.GetLength();
	//m_ChatEdit.SetSel(nStart,nStart);
	return;
}

//�ؼ���
VOID CGameFrameControl::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//����ؼ�
	DDX_Control(pDX, IDC_CHAT_EDIT, m_ChatEdit);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);

	//���찴ť
	DDX_Control(pDX, IDC_BT_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_BT_CHAT_SHORT, m_btChatShort);
	DDX_Control(pDX, IDC_REGAIN_BUTTON, m_btCloseChat);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_1, m_btChatButton1);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_2, m_btChatButton2);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_3, m_btChatButton3);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_4, m_btChatButton4);

	//����ؼ�
	DDX_Control(pDX, IDC_USER_LIST_CONTROL, m_UserListControl);

	return;
}

//���ú���
BOOL CGameFrameControl::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	m_bCreateFlag=true;

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��������
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	m_ImageBackground.LoadImage(hInstance, TEXT("BACKGROUND"));

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_GameFrameUserView.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_USER_VIEW);
	m_GameFrameMedalView.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_MEDAL_VIEW);

	//m_UserListControl.ShowWindow(SW_HIDE);
	//��ʼ���б�
	m_UserListControl.Init();

	//��������
	tagPropertyResource PropertyResource;
	ZeroMemory(&PropertyResource,sizeof(PropertyResource));

	//���õ���
	PropertyResource.uImageL=IDB_GAME_PROPERTY_L;
	PropertyResource.uImageM=IDB_GAME_PROPERTY_M;
	PropertyResource.uImageR=IDB_GAME_PROPERTY_R;
	PropertyResource.uImageItem=IDB_GAME_PROPERTY_ITEM;
	PropertyResource.hResInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	//����λ��
	PropertyResource.PointExcursion.x=4;
	PropertyResource.PointExcursion.y=10;
	PropertyResource.SizeExcursion.cx=38;
	PropertyResource.SizeExcursion.cy=39;

	//����ؼ�
	//m_ChatEdit.SetLimitText(LEN_USER_CHAT-1);
	//m_ChatEdit.SetEnableColor(RGB(0,0,0),RGB(255,255,255),RGB(255,255,255));

	//����ؼ�
	m_ChatControl.SetChatControlSink(this);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(2,27,36));

	//���찴ť
	m_btSendChat.SetButtonImage(IDB_BT_SEND_CHAT,hInstance,false,false);
	m_btChatShort.SetButtonImage(IDB_BT_CHAT_SHORT,hInstance,false,false);

	//���찴ť
	m_btCloseChat.SetButtonImage(IDB_BT_CHAT_BUTTON, hInstance, false, false);
	m_btChatButton1.SetButtonImage(IDB_BT_CHAT_BUTTON_1,TEXT("BT_CHAT_BUTTON_1"),hInstance,false,false);
	m_btChatButton2.SetButtonImage(IDB_BT_CHAT_BUTTON_2,TEXT("BT_CHAT_BUTTON_2"),hInstance,false,false);
	m_btChatButton3.SetButtonImage(IDB_BT_CHAT_BUTTON_3,TEXT("BT_CHAT_BUTTON_3"),hInstance,false,false);
	m_btChatButton4.SetButtonImage(IDB_BT_CHAT_BUTTON_4,TEXT("BT_CHAT_BUTTON_4"),hInstance,false,false);

	//��ѯ�ӿ�
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);

	//��ѯ�ӿ�
	IGameUserManager * pIGameUserManager=(IGameUserManager *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_USER_MANAGER,IID_IGameUserManager,VER_IGameUserManager);
	IGameLevelParser * pIGameLevelParser=(IGameLevelParser *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_LEVEL_PARSER,IID_IGameLevelParser,VER_IGameLevelParser);

	//���ýӿ�
	m_UserListControl.SetGameLevelParser(pIGameLevelParser);
	m_UserListControl.SetGameUserManager(pIGameUserManager);
		
	//ע���¼�
	m_pIClientKernel->SetUserEventSink(QUERY_ME_INTERFACE(IUserEventSink));
	m_pIClientKernel->SetStringMessage(QUERY_OBJECT_INTERFACE(m_ChatMessage,IStringMessage));

	//���ý���
	RectifyControl(CSkinRenderManager::GetInstance());

	return FALSE;
}

//��Ϣ����
BOOL CGameFrameControl::PreTranslateMessage(MSG * pMsg)
{
	//������Ϣ
	if (pMsg->message==WM_KEYDOWN)
	{
		//ȡ����ť
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}

		//�س���ť
		if (pMsg->wParam==VK_RETURN)
		{
			PostMessage(WM_COMMAND,IDC_BT_SEND_CHAT,0);
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CGameFrameControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//��ť����
	switch (nCommandID)
	{
	case IDC_BT_SEND_CHAT:			//��������
		{
			//��ȡ��Ϣ
			CString strMessage;
			m_ChatEdit.GetWindowText(strMessage);

			//��ʽ��Ϣ
			strMessage.TrimLeft();
			strMessage.TrimRight();

			//������Ϣ
			if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
			{
				CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
				m_pIClientKernel->SendUserChatMessage(m_ChatControl.GetChatTargetUserID(),strMessage,pGlobalUnits->m_crChatTX);
			}

			//���ý���
			m_ChatEdit.SetFocus();
			m_ChatEdit.SetWindowText(TEXT(""));

			return TRUE;
		}
	case IDC_BT_CHAT_SHORT:			//�������
		{
			//����˵�
			CSkinMenu ChatShortMenu;
			ChatShortMenu.CreateMenu();

			//����Ŀ¼
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFileName[MAX_PATH]=TEXT("");
			TCHAR szFileName2[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Table\\PhraseInfo.ini"), szDirectory);
			_sntprintf(szFileName2,CountArray(szFileName2),TEXT("%s\\..\\..\\Res\\PhraseInfo.ini"), szDirectory);

			//�ж��ļ�
			TCHAR *pFileName = szFileName;
			CFile file;
			if(!file.Open(szFileName,CFile::modeRead)) pFileName = szFileName2;
			file.Close();
			ASSERT(pFileName);

			//��������
			BYTE cbIndex=1;
			bool bSuccess=false;
			TCHAR szItemName[16]=TEXT(""),szShortcut[LEN_USER_CHAT]=TEXT("");

			//��ȡ��Ϣ
			while (true)
			{
				//��ȡ��Ϣ
				_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),cbIndex);
				GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),pFileName);

				//����ж�
				if (szShortcut[0]!=0)
				{
					//���ñ���
					cbIndex++;
					bSuccess=true;

					//����˵�
					ChatShortMenu.AppendMenu(IDM_SELECT_CHAT_SHORT+cbIndex-1,szShortcut);
				}

				//�����ж�
				if ((szShortcut[0]==0)||(cbIndex>=MAX_SHORT_COUNT))
				{
					break;
				}
			}

			//�����˵�
			if (bSuccess==true)
			{
				CRect rcButton;
				m_btChatShort.GetWindowRect(&rcButton);
				ChatShortMenu.TrackPopupMenu(rcButton.right,rcButton.top,this);
			}

			return TRUE;
		}
	case IDC_REGAIN_BUTTON:					//�ջ�����
		{
			::SendMessage(GetParent()->m_hWnd, WM_COMMAND, MAKEWPARAM(104, BN_CLICKED), 0);
			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_1:		//���鰴ť
		{
			//��������
			CRect rcButton;
			m_btChatButton1.GetWindowRect(&rcButton);

			//��������
			if (m_pExpressionControl==NULL)
			{
				m_pExpressionControl=new CExpressionControl;
			}

			//��ʾ����
			m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_2:		//ѡ����ɫ
		{
			////��������
			//ASSERT(CGlobalUnits::GetInstance()!=NULL);
			//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

			////��������
			//INT nWidth=72,nHeight=16;
			//COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
			//	RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

			////������ɫ
			//CImage ImageColor[CountArray(crColor)];
			//for (INT i=0;i<CountArray(ImageColor);i++)
			//{
			//	//����λͼ
			//	ImageColor[i].Create(nWidth,nHeight,16);

			//	//��ȡ�豸
			//	CImageDC ImageDC(ImageColor[i]);
			//	CDC * pDC=CDC::FromHandle(ImageDC);

			//	//�滭λͼ
			//	pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
			//	pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
			//	pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
			//}

			////�����˵�
			//CSkinMenu ColorMenu;
			//ColorMenu.CreateMenu();

			////����˵�
			//ColorMenu.AppendMenu(IDM_MORE_COLOR,TEXT("ѡ��������ɫ"));

			////��ɫѡ��
			//ColorMenu.AppendSeparator();
			//for (INT i=0;i<CountArray(ImageColor);i++) 
			//{
			//	ColorMenu.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
			//	if (pGlobalUnits->m_crChatTX==crColor[i]) ColorMenu.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
			//}

			////�����˵�
			//CRect rcButton;
			//m_btChatButton2.GetWindowRect(&rcButton);
			//ColorMenu.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_3:		//���ΰ�ť
		{
			//�����˵�
			CSkinMenu MenuMessage;
			MenuMessage.CreateMenu();

			//����˵�
			MenuMessage.AppendMenu(IDM_MESSAGE_SHOW_ALL,TEXT("��ʾ������Ϣ"));
			MenuMessage.AppendMenu(IDM_MESSAGE_HIDE_DETEST,TEXT("���������Ϣ"));
			MenuMessage.AppendMenu(IDM_MESSAGE_ONLY_FRIEND,TEXT("ֻ��ʾ������Ϣ"));

			//��������
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

			//���ò˵�
			BYTE cbMessageMode=pGlobalUnits->m_cbMessageMode;
			if (cbMessageMode==MESSAGE_MODE_ALL) MenuMessage.CheckMenuItem(IDM_MESSAGE_SHOW_ALL,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_DETEST) MenuMessage.CheckMenuItem(IDM_MESSAGE_HIDE_DETEST,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_FRIEND) MenuMessage.CheckMenuItem(IDM_MESSAGE_ONLY_FRIEND,MF_BYCOMMAND|MF_CHECKED);

			//�����˵�
			CRect rcButton;
			m_btChatButton3.GetWindowRect(&rcButton);
			MenuMessage.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_4:		//���Ȱ�ť
		{
			if ( !m_pIClientKernel->IsWebPlazaReady() )
			{
				//��������
				CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
				CGamePropertyItem * pGamePropertyTrumpet = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET);
				CGamePropertyItem * pGamePropertyTyphon  = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON);

				//��Ч�ж�
				if(pGamePropertyTrumpet==NULL && pGamePropertyTyphon==NULL)
				{
					CInformation Information(this);
					Information.ShowMessageBox(TEXT("��ǰ���ȹ���δ���ã�"),MB_ICONINFORMATION,30L);
					return TRUE;
				}

				//��������
				if (m_pTrumpetItem==NULL)
				{
					m_pTrumpetItem=new CDlgTrumpetItem;
					m_pTrumpetItem->CenterWindow(NULL);
					m_pTrumpetItem->SetGameTrumpetSink(QUERY_ME_INTERFACE(IUnknownEx));
				}

				//��ʾ����
				m_pTrumpetItem->ShowTrumpetWindow(AfxGetMainWnd(),m_pIMySelfUserItem);
			}
			else
			{
				m_pIClientKernel->SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET);
			}

			return TRUE;
		}
	}

	//�˵�����
	switch (nCommandID)
	{
	case IDM_SET_CHAT_USER:			//��̸����
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem!=NULL) m_ChatControl.SetChatTargetUser(pIClientUserItem);

			return TRUE;
		}
	case IDM_CANCEL_CHAT_USER:		//ȡ����̸
		{
			//��������
			m_ChatControl.SetChatTargetUser(NULL);

			return TRUE;
		}
	case IDM_COPY_USER_NICKNAME:	//�����ǳ�
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//�����ַ�
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			CWHService::SetClipboardString(pIClientUserItem->GetNickName());

			//�����ַ�
			m_ChatEdit.Paste();
			m_ChatEdit.SetFocus();

			return TRUE;
		}
	case IDM_SET_FRIEND_USER:		//��Ϊ����
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_FRIEND)?CP_NORMAL:CP_FRIEND;

			//�û�״̬
			IPC_GF_UserAttrib UserAttribData;
			ZeroMemory(&UserAttribData,sizeof(UserAttribData));

			//������Ϣ
			UserAttribData.dwUserID=pIClientUserItem->GetUserID();
			UserAttribData.UserAttrib.cbCompanion=cbDestCompanion;

			//��������
			m_pIClientKernel->SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_ATTRIB,&UserAttribData,sizeof(UserAttribData));

			return TRUE;
		}
	case IDM_SET_DETEST_USER:		//��Ϊ���
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_DETEST)?CP_NORMAL:CP_DETEST;

			//�û�״̬
			IPC_GF_UserAttrib UserAttribData;
			ZeroMemory(&UserAttribData,sizeof(UserAttribData));

			//������Ϣ
			UserAttribData.dwUserID=pIClientUserItem->GetUserID();
			UserAttribData.UserAttrib.cbCompanion=cbDestCompanion;

			//��������
			m_pIClientKernel->SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_ATTRIB,&UserAttribData,sizeof(UserAttribData));

			return TRUE;
		}
	case IDM_KICK_USER:             //�߳��û�
		{
			//Ч��״̬
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//��������
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

			//������Ϣ
			bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_pIClientKernel->GetServerAttribute()->dwServerRule);
			if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMemberOrder()<=pIClientUserItem->GetMemberOrder())) return FALSE;

			//��ȡ����
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
			if(cbUserStatus==US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("������ѿ�ʼ��Ϸ�������ߣ�"),MB_ICONINFORMATION);
				return TRUE;
			}
			
			//��������
			IPC_GF_KickUser KickUser;
			KickUser.dwTargetUserID=pIClientUserItem->GetUserID();

			//��������
			m_pIClientKernel->SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GR_KICK_USER,&KickUser,sizeof(KickUser));
		}
	case IDM_MESSAGE_SHOW_ALL:		//��ʾ����
		{
			//��������
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->m_cbMessageMode=MESSAGE_MODE_ALL;

			return TRUE;
		}
	case IDM_MESSAGE_HIDE_DETEST:	//�������
		{
			//��������
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->m_cbMessageMode=MESSAGE_MODE_DETEST;

			return TRUE;
		}
	case IDM_MESSAGE_ONLY_FRIEND:	//��ʾ����
		{
			//��������
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->m_cbMessageMode=MESSAGE_MODE_FRIEND;

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//�����ؼ�
VOID CGameFrameControl::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	bool hasChatControl = false;

	if (m_pIClientKernel && m_pIClientKernel->GetServerAttribute())
	{
		tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();
		hasChatControl = (pServerAttribute->wServerType & GAME_GENRE_MEDAL)!=0;
	}

	// �û���ͼ����
	m_rcUserView.left	= 0;
	m_rcUserView.top	= TITLE_VIEW_CY;
	m_rcUserView.right	= nWidth;
	m_rcUserView.bottom	= m_rcUserView.top + USER_VIEW_CY;
	// �û��б�����
	m_rcUserList.left	= 0;
	m_rcUserList.top	= USER_VIEW_CY + TITLE_VIEW_CY;
	m_rcUserList.right	= nWidth;
	m_rcUserList.bottom	= m_rcUserList.top + USER_LIST_CY+140;
	// ��������
	m_rcChat.left		= 0;
	m_rcChat.top		= USER_VIEW_CY + USER_LIST_CY + MEDAL_VIEW_CY + PROPERTY_CY + TITLE_VIEW_CY;
	m_rcChat.right		= nWidth;
	m_rcChat.bottom		= nHeight;

	CRect rcChat;
	rcChat.left		= 0;
	rcChat.top		= USER_VIEW_CY + USER_LIST_CY + MEDAL_VIEW_CY + PROPERTY_CY + TITLE_VIEW_CY+165;
	rcChat.right		= nWidth;
	rcChat.bottom		= nHeight+10;

	//������Ϣ
	tagEncircleInfo EncircleInfoChat;
	tagEncircleInfo EncircleInfoUserList;
	m_ChatEncircle.GetEncircleInfo(EncircleInfoChat);
	m_UserListEncircle.GetEncircleInfo(EncircleInfoUserList);

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER | SWP_NOREDRAW;

	//���찴ť
	CSkinButton * ChatButtonArray[]={&m_btChatButton1,&m_btChatButton4,&m_btChatButton2,&m_btChatButton3};

	m_btChatShort.EnableWindow(hasChatControl ? TRUE : FALSE);
	m_btChatShort.ShowWindow(hasChatControl ? SW_NORMAL : SW_HIDE);

	int btnLen = CountArray(ChatButtonArray);
	INT nChatButtonSpace=3;
	INT nXPosChat = m_rcChat.left   + 17;
	INT nYPosChat = m_rcChat.bottom - (hasChatControl ? 28 : 4);
	INT nChatButtonWidths = 0;
	
	//���찴ť
	for (INT i=0;i<btnLen;i++)
	{
		//��ȡλ��
		CRect rcChatButton;
		ChatButtonArray[i]->GetWindowRect(&rcChatButton);
		nChatButtonWidths += rcChatButton.Width();

		if (i == 0)
		{
			nYPosChat -= rcChatButton.Height();
		}
	}

	nChatButtonSpace = max(0, (nWidth - nXPosChat * 2 - nChatButtonWidths) /  (btnLen - 1));
	nXPosChat += 1;

	//���찴ť
	DeferWindowPos(hDwp, m_btChatButton1, nullptr, 13, 693, 0, 0, uFlags | SWP_NOSIZE );
	DeferWindowPos(hDwp, m_btChatButton4, nullptr, 48, 693, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_ChatEdit, nullptr, 83, 699, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btSendChat, nullptr, 178, 693, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btCloseChat, nullptr, 5, 6, 0, 0, uFlags | SWP_NOSIZE);

 	if (m_UserListControl)
 	{
 		CRect rcControl;
 		m_UserListEncircle.GetEncircleRect(m_rcUserList, rcControl);
 
 		//::DeferWindowPos(hDwp, m_UserListControl, nullptr, 5, 34, rcControl.Width(), rcControl.Height(), uFlags | SWP_NOSIZE);
 
 		::DeferWindowPos(hDwp, m_UserListControl, nullptr, 5, 34, rcControl.Width(), rcControl.Height(), SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER);
 	}

	m_rcChat.bottom = nYPosChat;

	//�������
 	if ((m_ChatControl.m_hWnd!=NULL)&&(m_ChatControl.GetChatTargetUserItem()!=NULL))
 	{
 		//��ȡλ��
 		CRect rcChatControl;
 		m_ChatControl.GetWindowRect(&rcChatControl);
 
 		//λ����Ϣ
 		tagEncircleInfo EncircleInfo;
 		m_ChatEncircle.GetEncircleInfo(EncircleInfo);
 
 		//����ؼ�
 		m_rcChat.bottom -= rcChatControl.Height();
 		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,m_rcChat);
 
 		//�������
 		DeferWindowPos(hDwp,m_ChatControl,NULL,m_rcChat.left+EncircleInfo.nLBorder,m_rcChat.bottom-EncircleInfo.nBBorder,
 			m_rcChat.Width()-EncircleInfo.nLBorder-EncircleInfo.nRBorder,rcChatControl.Height(),uFlags);
 	}
 	else
 	{
		//����ؼ�
		::DeferWindowPos(hDwp, m_ChatMessage, nullptr, 15, 360, 222, 327, SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER);
	}

	//�ƶ�����
	EndDeferWindowPos(hDwp);

 	if (m_UserListControl)
 	{
 		CRect rect;
 		GetClientRect(&rect);
 		rect.DeflateRect(0, 0, 12, 465);
 
 		if (rect.Width() > 100 && rect.Height() > 100)
 		{
 			m_UserListControl.SetWindowPos(nullptr, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE);
 			m_UserListControl.PositionScrollBars();
 		}
 	}

	if (m_ChatEdit)
	{
		CRect rect;
		GetClientRect(&rect);
		rect.DeflateRect(0, 0, 160, 10);

		m_ChatEdit.SetWindowPos(nullptr, 0, 0, 96, rect.Height(), SWP_NOMOVE);

		m_ChatEdit.SetItemHeight(-1, 21);
		m_ChatEdit.SetItemHeight(1, 20);
	}

	return;
}

//��������
VOID CGameFrameControl::RectifyControl(CSkinRenderManager * pSkinRenderManager)
{
	//�������
	tagColorUserList ColorUserList;
	ZeroMemory(&ColorUserList,sizeof(ColorUserList));

	//��ɫ����
	ColorUserList.crSelectTX=COLOR_SELECT_TX;
	ColorUserList.crSelectBK=COLOR_SELECT_BK;
	ColorUserList.crNormalTX=COLOR_NORMAL_TX;
	ColorUserList.crNormalBK=COLOR_NORMAL_BK;

	//��ɫ����
	ColorUserList.crMyselfTX=COLOR_MYSELF_TX;
	ColorUserList.crMyselfBK=COLOR_MYSELF_BK;
	ColorUserList.crMasterTX=COLOR_MASTER_TX;
	ColorUserList.crMasterBK=COLOR_MASTER_BK;
	ColorUserList.crMemberTX=COLOR_MEMBER_TX;
	ColorUserList.crMemberBK=COLOR_MEMBER_BK;

	//���ÿؼ�
	m_UserListControl.SetColorUserList(ColorUserList);

	return;
}

//����Ч��
bool CGameFrameControl::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	//״̬�ж�
	if (m_pIClientKernel==NULL) return false;
	if (m_pIMySelfUserItem==NULL) return false;

	//���ȱȽ�
	if(pszChatString != NULL)
	{
		CString strChat=pszChatString;
		if(strChat.GetLength() >= (LEN_USER_CHAT/2))
		{
			m_ChatMessage.InsertString(TEXT("��Ǹ����������������ݹ����������̺��ٷ��ͣ�\r\n"),COLOR_WARN);
			return false;
		}
	}

	//��������
	BYTE cbMemberOrder=m_pIMySelfUserItem->GetMemberOrder();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//���Զ���
	tagUserAttribute * pUserAttribute=m_pIClientKernel->GetUserAttribute();
	tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();

	//��������
	if (CServerRule::IsForfendGameChat(pServerAttribute->dwServerRule)&&(cbMasterOrder==0))
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("��Ǹ����ǰ����Ϸ�����ֹ�û��������죡"));

		return false;
	}

	//Ȩ���ж�
	if (CUserRight::CanGameChat(pUserAttribute->dwUserRight)==false)
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("��Ǹ����û����Ϸ���Ե�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"));

		return false;
	}

	//�ٶ��ж�
	DWORD dwCurrentTime=(DWORD)time(NULL);
	if ((cbMasterOrder==0)&&((dwCurrentTime-m_dwChatTime)<=TIME_USER_CHAT))
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n��%s������ʧ��"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//������Ϣ
		m_ChatMessage.InsertSystemString(TEXT("����˵���ٶ�̫���ˣ����������ȱ�����Ϣ�°ɣ�"));

		return false;
	}

	//���ñ���
	m_dwChatTime=dwCurrentTime;
	m_strChatString=pszChatString;

	return true;
}

//������ɫ
VOID CGameFrameControl::OnSelectMoreColor()
{
	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//������ɫ
	CColorDialog ColorDialog(pGlobalUnits->m_crChatTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pGlobalUnits->m_crChatTX=ColorDialog.GetColor();

	//���ý���
	m_ChatEdit.SetFocus();

	return;
}

//ѡ����ɫ
VOID CGameFrameControl::OnSelectChatColor(UINT nCommandID)
{
	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��������
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//ѡ����ɫ
	ASSERT(nIndex<CountArray(crColor));
	if (nIndex<CountArray(crColor)) pGlobalUnits->m_crChatTX=crColor[nIndex];

	//���ý���
	m_ChatEdit.SetFocus();

	return;
}

//ѡ�����
VOID CGameFrameControl::OnSelectChatShort(UINT nCommandID)
{
	//��������
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_SHORT;

	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]=TEXT("");
	TCHAR szFileName2[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Table\\PhraseInfo.ini"), szDirectory);
	_sntprintf(szFileName2,CountArray(szFileName2),TEXT("%s\\..\\..\\Res\\PhraseInfo.ini"), szDirectory);

	//�ж��ļ�
	TCHAR *pFileName = szFileName;
	CFile file;
	if(!file.Open(szFileName,CFile::modeRead)) pFileName = szFileName2;
	file.Close();
	ASSERT(pFileName);

	//��������
	TCHAR szItemName[16]=TEXT("");
	TCHAR szShortcut[LEN_USER_CHAT]=TEXT("");

	//��ȡ��Ϣ
	_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),nIndex);
	GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),pFileName);

	//������Ϣ
	if (szShortcut[0]!=0)
	{
		//��ȡ��Ϣ
		CString strMessage=szShortcut;
		strMessage.TrimLeft();strMessage.TrimRight();

		//������Ϣ
		if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
		{
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
			m_pIClientKernel->SendUserChatMessage(m_ChatControl.GetChatTargetUserID(),strMessage,pGlobalUnits->m_crChatTX);
		}
	}

	//���ý���
	m_ChatEdit.SetFocus();

	return;
}

LRESULT CGameFrameControl::OnNcHitTest(CPoint Point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UINT lResult = CDialog::OnNcHitTest(Point);

	switch(lResult)
	{
	case HTCAPTION :  return HTNOWHERE;
	}
	return lResult;
}
//�滭����
BOOL CGameFrameControl::OnEraseBkgnd(CDC * pDC)
{

	return TRUE;
}

VOID CGameFrameControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����DC
	CMemDCEx memDC(&dc, rcClient);

	m_ImageBackground.DrawImage(&memDC, 0, 0, 250, 738, 0, 0);
}

//λ����Ϣ
VOID CGameFrameControl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//�Ҽ��б�
VOID CGameFrameControl::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	////��������
	//NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	//
	////�����˵�
	//if (pListNotify->iItem!=-1)
	//{
	//	//ѡ����Ŀ
	//	UINT nSelectCount=m_UserListControl.GetSelectedCount();

	//	//ѡ����
	//	if (nSelectCount>1L)
	//	{
	//		//�û�����
	//		IClientUserItem * pIClientUserItem[32];
	//		ZeroMemory(pIClientUserItem,sizeof(pIClientUserItem));

	//		//��������
	//		WORD wUserCount=0;
	//		POSITION nSelectPos=m_UserListControl.GetFirstSelectedItemPosition();

	//		//ѡ���ռ�
	//		while ((nSelectPos!=NULL)&&(wUserCount<CountArray(pIClientUserItem)))
	//		{
	//			//��ȡѡ��
	//			INT nSelectItem=m_UserListControl.GetNextSelectedItem(nSelectPos);

	//			//�����û�
	//			DWORD_PTR lItemData=m_UserListControl.GetItemData(nSelectItem);
	//			if (lItemData!=NULL) pIClientUserItem[wUserCount++]=((IClientUserItem *)(lItemData));
	//		};

	//		//�����˵�
	//		if (wUserCount>0) TrackUserItemMenu(pIClientUserItem,wUserCount);
	//	}
	//	else
	//	{
	//		//�����˵�
	//		DWORD_PTR lItemData=m_UserListControl.GetItemData(pListNotify->iItem);
	//		if (lItemData!=NULL) TrackUserItemMenu((IClientUserItem *)(lItemData));
	//	}
	//}

	return;
}

//�����б�
VOID  CGameFrameControl::OnNMClickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	////��������
	//NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	//
	////��Ϣ����
	//if ((pListNotify->iItem!=-1)&&(pListNotify->lParam!=NULL))
	//{
	//	//��ȡ����
	//	DWORD_PTR dwItemData=m_UserListControl.GetItemData(pListNotify->iItem);
	//	IClientUserItem * pIClientUserItem=(IClientUserItem *)dwItemData;
	//	if(pIClientUserItem==NULL) return;

	//	//�����û�
	//	m_GamePropertyCtrl.OnSelectedUserItemChange(pIClientUserItem);
	//}
}

//ѡ��ı�
VOID CGameFrameControl::OnLvnItemchangedUserList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//��������
	LPNMLISTVIEW pNmListView=reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	//�����û�
	if ((pNmListView->lParam!=NULL)&&(pNmListView->uNewState&(ODS_SELECTED|ODS_FOCUS)))
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=(IClientUserItem *)pNmListView->lParam;

		//�����û�
		SetUserViewInfo(pIClientUserItem);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
