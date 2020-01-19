#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "MatchFrameControl.h"

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

//��ʱ��
#define IDI_MATCH_TIMER				1234								//��ѯ��ʱ��

CMatchFrameControl* CMatchFrameControl::m_pMatchFrameControl = NULL;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMatchFrameControl, CDialog)

	//ϵͳ��Ϣ
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()

	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMatchFrameControl::CMatchFrameControl() : CDialog(IDD_MATCH_CONTROL)
{
	//�ӿڱ���
	m_pIClientKernel=NULL;
	m_pIMySelfUserItem=NULL;
	ZeroMemory(&m_MatchViewInfo,sizeof(m_MatchViewInfo));

	m_HourTime = 0;
	m_MinuteTime = 0;
	m_SecondTime = 0;

	m_pMatchFrameControl = this;

	//ע�����
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_MATCH_FRAME_CONTROL,QUERY_ME_INTERFACE(IUnknownEx));
	pGlobalUnits->RegisterGlobalModule(MODULE_STRING_MESSAGE,QUERY_OBJECT_INTERFACE(m_ChatMessage,IUnknownEx));

	return;
}

//��������
CMatchFrameControl::~CMatchFrameControl()
{
	m_pMatchFrameControl = NULL;
	return;
}

//�ӿڲ�ѯ
VOID * CMatchFrameControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IMatchFrameControl,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IMatchFrameControl,Guid,dwQueryVer);
	return NULL;
}

//�û�����
VOID CMatchFrameControl::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
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
// 			SetUserViewInfo(m_pIMySelfUserItem);
// 			m_UserListControl.SetMySelfUserID(m_pIMySelfUserItem->GetUserID());
// 			m_UserListControl.SetServerRule(pServerAttribute->dwServerRule);
// 			m_GameFrameMedalView.SetUserMedal(m_pIMySelfUserItem->GetUserMedal());
		}
	}

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//�����û�
//	m_UserListControl.InsertDataItem(pIClientUserItem);

	//��ʾ��Ϣ
// 	if (pGlobalUnits && (pGlobalUnits->m_bNotifyUserInOut==true)&&(m_pIClientKernel->IsServiceStatus()==true))
// 	{
// 		m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
// 	}

	return;
}

//�û��뿪
VOID CMatchFrameControl::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��������
	//DWORD dwLeaveUserID=pIClientUserItem->GetUserID();

	//�������
	//m_ChatControl.DeleteUserItem(pIClientUserItem);

	//ɾ���û�
	//m_UserListControl.DeleteDataItem(pIClientUserItem);

	//�˵�����
// 	for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
// 	{
// 		//��ȡ�û�
// 		IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
// 		if (pIChatUserItem->GetUserID()==dwLeaveUserID) m_MenuUserItemArray.RemoveAt(i);
// 	}

	//��ʾ��Ϣ
// 	if (pGlobalUnits->m_bNotifyUserInOut==true)
// 	{
// 		m_ChatMessage.InsertUserLeave(pIClientUserItem->GetNickName());
// 	}

	return;
}

//�û�����
VOID CMatchFrameControl::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID CMatchFrameControl::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID CMatchFrameControl::OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//�û�ͷ��
VOID CMatchFrameControl::OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//
VOID CMatchFrameControl::OnUserTaskUpdate(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return ;
}

//�ؼ���
VOID CMatchFrameControl::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_MESSAGE2, m_ChatMessage);
	return;
}

//���ú���
BOOL CMatchFrameControl::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	m_bCreateFlag=true;

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(25,20,16));

	//��������
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	m_ImageBackground.LoadImage(hInstance, TEXT("MATCH_BG"));
	m_ImageBase.LoadImage(hInstance,TEXT("MATCH_BASE"));
	m_ImageRank.LoadImage(hInstance,TEXT("MATCH_RANK"));
	m_ImageTime[0].LoadImage(hInstance,TEXT("MATCH_TIME"));
	m_ImageTime[1].LoadImage(hInstance,TEXT("MATCH_TIME2"));
	m_ImageTime[2].LoadImage(hInstance,TEXT("MATCH_TIME3"));
	m_ImageTime[3].LoadImage(hInstance,TEXT("MATCH_TIME4"));
	m_ImageNumberOrange.LoadImage(hInstance,TEXT("NUMBER_ORANGE"));
	m_ImageNumberGreen.LoadImage(hInstance, TEXT("NUMBER_GREEN"));
	m_ImageTurnDraw.LoadImage(hInstance, TEXT("MATCH_TURNDRAW"));
	m_ImageNumTime.LoadImage(hInstance, TEXT("MATCH_TIMER_NUM"));

	//��ѯ�ӿ�
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
		
	//ע���¼�
	m_pIClientKernel->SetUserEventSink(QUERY_ME_INTERFACE(IUserEventSink));
	m_pIClientKernel->SetStringMessage(QUERY_OBJECT_INTERFACE(m_ChatMessage,IStringMessage));

	//���ý���
	//RectifyControl(CSkinRenderManager::GetInstance());

	m_bMatchOver = TRUE;

	SetTimer(IDI_MATCH_TIMER, 1000, nullptr);

	return FALSE;
}

//��Ϣ����
BOOL CMatchFrameControl::PreTranslateMessage(MSG * pMsg)
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
BOOL CMatchFrameControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	return __super::OnCommand(wParam,lParam);
}

//�����ؼ�
VOID CMatchFrameControl::RectifyControl(INT nWidth, INT nHeight)
{
	//�ƶ�׼��
  	HDWP hDwp=BeginDeferWindowPos(32);
  	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER | SWP_NOREDRAW;
 
 	::DeferWindowPos(hDwp, m_ChatMessage, nullptr, 15, 325, 222, 400, SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER);
  
  	//�ƶ�����
  	EndDeferWindowPos(hDwp);
	return;
}


//��������
VOID CMatchFrameControl::RectifyControl(CSkinRenderManager * pSkinRenderManager)
{
	return;
}

LRESULT CMatchFrameControl::OnNcHitTest(CPoint Point)
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
BOOL CMatchFrameControl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

VOID CMatchFrameControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	INT nXPos=0;
	INT nYPos=0;

	//����DC
	CMemDCEx memDC(&dc, rcClient);
	memDC.SetBkMode(TRANSPARENT);

	CFont DrawFont;
	DrawFont.CreateFont(16,0,0,0,150,0,0,0,134,3,2,1,2,TEXT("����"));
	DrawFont.CreatePointFont(110,TEXT("����")); //�ոպ�
	CFont * pOldFont=memDC.SelectObject(&DrawFont);

	m_ImageBackground.DrawImage(&memDC, 0, 0, m_ImageBackground.GetWidth(), m_ImageBackground.GetHeight(), 0, 0);

 	INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE;
 
  	CString str;
 	
 	//��������
 	nXPos=43;
 	nYPos=40;
 	//str.Format(TEXT("���Ӯ1������"),m_MatchViewInfo.szMatchTitle);
 	//str.Format(TEXT("%s"),m_MatchViewInfo.szMatchTitle);
 	//memDC.SetTextColor(RGB(0,228,241));
 	//memDC.DrawText(str,CRect(0,nYPos,rcClient.Width(),rcClient.Height()),nDrawFormat|DT_CENTER);
 
 	//�����׶�
 	//str.Format(TEXT("Ԥ��-��������"),m_MatchViewInfo.szMatchStatus);
 	str.Format(TEXT("%s"),m_MatchViewInfo.szMatchStatus);
 	memDC.SetTextColor(RGB(0,228,241));
 	memDC.DrawText(str,CRect(0,nYPos+10,rcClient.Width(),rcClient.Height()),nDrawFormat|DT_CENTER);
 
	
		//������ʱ
	nXPos=10;
	nYPos=84;
	static WORD i=0;
	
// 	if ( !m_bMatchOver )
// 	{
// 		KillTimer(IDI_MATCH_TIMER);
// 	}
	m_ImageTime[i].DrawImage(&memDC,nXPos,nYPos,m_ImageTime[i].GetWidth(),m_ImageTime[i].GetHeight(),0,0);
	++i;
	i = i%4;
	DrawNumberString(&memDC, m_ImageNumTime, m_HourTime, nXPos+115,nYPos+18,true);
	DrawNumberString(&memDC, m_ImageNumTime, m_MinuteTime, nXPos+150,nYPos+18,true); 
	DrawNumberString(&memDC, m_ImageNumTime, m_SecondTime, nXPos+185,nYPos+18,true); 
	//m_MatchViewInfo.cbMatchStatus=6;
	//��ǰ����
 	if ( m_MatchViewInfo.cbMatchStatus < 5 )  //Ԥ���׶�
 	{
 		m_ImageBase.DrawImage(&memDC,nXPos+30,nYPos+60,m_ImageBase.GetWidth(),m_ImageBase.GetHeight(),0,0);
 
 		DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.wCurBase, nXPos+150,nYPos+68); 
 		//str.Format(TEXT("%d"),m_MatchViewInfo.wCurBase);
 		//memDC.DrawText(str,CRect(nXPos+107,nYPos+53,rcClient.Width(),rcClient.Height()),nDrawFormat);
 
 		str.Format(TEXT("����%d�ֳ���"),m_MatchViewInfo.wOutScore);
 		memDC.SetTextColor(RGB(0,228,241));
 		memDC.DrawText(str,CRect(20,nYPos+98,rcClient.Width(),rcClient.Height()),nDrawFormat|DT_LEFT);
 
 	}
 	else  //�����׶�
	{
		m_ImageTurnDraw.DrawImage(&memDC,nXPos+40,nYPos+68,m_ImageTurnDraw.GetWidth(),m_ImageTurnDraw.GetHeight(),0,0);

		DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.cbCurRound, nXPos+63,nYPos+80); 
		//DrawNumberString(&memDC, m_ImageNumberGreen, 3, nXPos+63,nYPos+80); 

		DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.cbCurCount, nXPos+143,nYPos+80); 
		//DrawNumberString(&memDC, m_ImageNumberGreen, 3, nXPos+143,nYPos+80); 

		DrawFont.CreatePointFont(100,TEXT("����")); 
		memDC.SelectObject(&DrawFont);
		memDC.SetTextColor(RGB(255,255,255));
		str.Format(TEXT("��%d�֣�ÿ��%d��"),m_MatchViewInfo.cbRoundCount,m_MatchViewInfo.cbGameCount);
		memDC.DrawText(str,CRect(nXPos+10,nYPos+45,rcClient.Width(),rcClient.Height()),nDrawFormat);

		str.Format(TEXT("��ǰ����:"),m_MatchViewInfo.wCurBase);
		
		memDC.DrawText(str,CRect(nXPos+10,nYPos+103,rcClient.Width(),rcClient.Height()),nDrawFormat);
	}
 	
	
 
 	//��������
 	nXPos=10;
 	nYPos=208;
 	DrawFont.CreatePointFont(100,TEXT("����")); 
 	memDC.SelectObject(&DrawFont);
 	m_ImageRank.DrawImage(&memDC,nXPos,nYPos,m_ImageRank.GetWidth(),m_ImageRank.GetHeight(),0,0);
	if ( m_MatchViewInfo.cbMatchStatus < 5 ) 
	{
		str.Format(TEXT("%d�˽�ֹ,%d�˽���"),m_MatchViewInfo.wRemainCount,m_MatchViewInfo.wPromotionCount);
		memDC.SetTextColor(RGB(151,140,102));
		memDC.DrawText(str,CRect(nXPos+10,nYPos+78,rcClient.Width(),rcClient.Height()),nDrawFormat);
	}
	else
	{
		if ( m_MatchViewInfo.cbCurRound == m_MatchViewInfo.cbRoundCount )
			str.Format(TEXT("%d�־����ڡ��ǡ�����"),m_MatchViewInfo.cbGameCount);
		else str.Format(TEXT("ĩλ��̭%d��"),m_MatchViewInfo.wMatchUser);

		memDC.SetTextColor(RGB(151,140,102));
		memDC.DrawText(str,CRect(nXPos+10,nYPos+78,rcClient.Width(),rcClient.Height()),nDrawFormat);
	}

 	//��ǰ����
	//TCHAR sz[32]={0};
	//_stprintf(sz,L"��ǰ����:%d",m_MatchViewInfo.wCurRank);
	//OutputDebugString(sz);
	DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.wCurRank, nXPos+66,nYPos+49); 
 	//DrawNumberString(&memDC, m_ImageNumberGreen, 119, nXPos+66,nYPos+49); 

 	//��������
	DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.wMatchUser, nXPos+133,nYPos+49,false,TA_RIGHT); 
 	//DrawNumberString(&memDC, m_ImageNumberGreen, 8, nXPos+133,nYPos+49,false,TA_RIGHT); 
 
 	memDC.SelectObject(pOldFont);
 	DrawFont.DeleteObject();

}

//λ����Ϣ
VOID CMatchFrameControl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

VOID CMatchFrameControl::SetMatchViewInfo(tagMatchViewInfo* pMatchViewInfo)
{
	ZeroMemory(&m_MatchViewInfo,sizeof(m_MatchViewInfo));
	CopyMemory(&m_MatchViewInfo,pMatchViewInfo,sizeof(tagMatchViewInfo));

	Invalidate(FALSE);
	UpdateWindow();
}

//�滭����
VOID CMatchFrameControl::DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,bool bZero,BYTE cbAlign)
{
	//������Դ
	INT nNumberHeight=PngNumber.GetHeight();
	INT nNumberWidth=PngNumber.GetWidth()/10;

	//if ( lNumber==0 ) lNumber=99;

	//������Ŀ
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos;
	if(cbAlign==TA_RIGHT)
		nXDrawPos+=(lNumberCount-1)*nNumberWidth;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		//if ( lNumber == 99 ) lNumber=0;
		LONG lCellNumber=(LONG)(lNumber%10L);
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0);

		//���ñ���
		lNumber/=10;
		nXDrawPos-=nNumberWidth;
	};
	if ( lNumberCount <=1 && bZero)
	{
		LONG lCellNumber=(LONG)(lNumber%10L);
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0);

	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////


void CMatchFrameControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if ( nIDEvent == IDI_MATCH_TIMER )
	{
		if ( m_bMatchOver ) return ;
		m_SecondTime+=1;
		if ( m_SecondTime/60 ) 
		{
			m_SecondTime=0;
			m_MinuteTime+=1;
		}
		if ( m_MinuteTime/60 ) 
		{
			m_MinuteTime=0;
			m_HourTime+=1;
		}
		Invalidate(FALSE);
		UpdateWindow();
	}

	__super::OnTimer(nIDEvent);
}

void CMatchFrameControl::SetMatchOverStatus(BOOL bMatchOver)
{ 
	if ( !bMatchOver ) // �������¿�ʼ
	{
		m_HourTime = 0;
		m_MinuteTime = 0;
		m_SecondTime = 0;
	}
	m_bMatchOver = bMatchOver; 
}