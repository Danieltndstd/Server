#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "MatchFrameControl.h"

//////////////////////////////////////////////////////////////////////////////////

//界面定义
//#define USER_VIEW_CY				223									//用户视图
//#define USER_LIST_CY				100									//用户列表
//#define PROPERTY_CY				89									//游戏道具
//#define MEDAL_VIEW_CY				63									//奖牌视图

#define TITLE_VIEW_CY				0
#define USER_VIEW_CY				0									//用户视图
#define USER_LIST_CY				150									//用户列表
#define PROPERTY_CY					0									//游戏道具
#define MEDAL_VIEW_CY				0									//奖牌视图



//分割参数
//#define INCISE_SCALE				50/100								//界面分割

//控件标识
#define IDC_PROPERTY				300									//游戏道具
#define IDC_USER_VIEW				301									//用户信息
#define IDC_MEDAL_VIEW				302									//奖牌信息
#define IDC_CHAT_CONTROL			303									//聊天控制
#define IDC_CHAT_BUTTON			304								//收回聊天框

//////////////////////////////////////////////////////////////////////////////////
//菜单命令

//颜色菜单
#define MAX_CHAT_COLOR				16									//最大数目
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//选择颜色

//快捷短语
#define MAX_SHORT_COUNT				16									//最大数目
#define IDM_SELECT_CHAT_SHORT		(WM_USER+300)						//选择短语

//定时器
#define IDI_MATCH_TIMER				1234								//轮询定时器

CMatchFrameControl* CMatchFrameControl::m_pMatchFrameControl = NULL;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMatchFrameControl, CDialog)

	//系统消息
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()

	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMatchFrameControl::CMatchFrameControl() : CDialog(IDD_MATCH_CONTROL)
{
	//接口变量
	m_pIClientKernel=NULL;
	m_pIMySelfUserItem=NULL;
	ZeroMemory(&m_MatchViewInfo,sizeof(m_MatchViewInfo));

	m_HourTime = 0;
	m_MinuteTime = 0;
	m_SecondTime = 0;

	m_pMatchFrameControl = this;

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_MATCH_FRAME_CONTROL,QUERY_ME_INTERFACE(IUnknownEx));
	pGlobalUnits->RegisterGlobalModule(MODULE_STRING_MESSAGE,QUERY_OBJECT_INTERFACE(m_ChatMessage,IUnknownEx));

	return;
}

//析构函数
CMatchFrameControl::~CMatchFrameControl()
{
	m_pMatchFrameControl = NULL;
	return;
}

//接口查询
VOID * CMatchFrameControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IMatchFrameControl,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IMatchFrameControl,Guid,dwQueryVer);
	return NULL;
}

//用户进入
VOID CMatchFrameControl::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//设置自己
	if (m_pIMySelfUserItem==NULL)
	{
		//变量定义
		ASSERT(m_pIClientKernel->GetUserAttribute()!=NULL);
		tagUserAttribute * pUserAttribute=m_pIClientKernel->GetUserAttribute();
		tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();

		//自己判断
		if (pUserAttribute->dwUserID==pIClientUserItem->GetUserID())
		{
			//设置变量
			m_pIMySelfUserItem=pIClientUserItem;

			//设置界面
// 			SetUserViewInfo(m_pIMySelfUserItem);
// 			m_UserListControl.SetMySelfUserID(m_pIMySelfUserItem->GetUserID());
// 			m_UserListControl.SetServerRule(pServerAttribute->dwServerRule);
// 			m_GameFrameMedalView.SetUserMedal(m_pIMySelfUserItem->GetUserMedal());
		}
	}

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//插入用户
//	m_UserListControl.InsertDataItem(pIClientUserItem);

	//提示信息
// 	if (pGlobalUnits && (pGlobalUnits->m_bNotifyUserInOut==true)&&(m_pIClientKernel->IsServiceStatus()==true))
// 	{
// 		m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
// 	}

	return;
}

//用户离开
VOID CMatchFrameControl::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//变量定义
	//DWORD dwLeaveUserID=pIClientUserItem->GetUserID();

	//聊天对象
	//m_ChatControl.DeleteUserItem(pIClientUserItem);

	//删除用户
	//m_UserListControl.DeleteDataItem(pIClientUserItem);

	//菜单对象
// 	for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
// 	{
// 		//获取用户
// 		IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
// 		if (pIChatUserItem->GetUserID()==dwLeaveUserID) m_MenuUserItemArray.RemoveAt(i);
// 	}

	//提示信息
// 	if (pGlobalUnits->m_bNotifyUserInOut==true)
// 	{
// 		m_ChatMessage.InsertUserLeave(pIClientUserItem->GetNickName());
// 	}

	return;
}

//用户积分
VOID CMatchFrameControl::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//用户状态
VOID CMatchFrameControl::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//用户属性
VOID CMatchFrameControl::OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//用户头像
VOID CMatchFrameControl::OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//
VOID CMatchFrameControl::OnUserTaskUpdate(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return ;
}

//控件绑定
VOID CMatchFrameControl::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_MESSAGE2, m_ChatMessage);
	return;
}

//配置函数
BOOL CMatchFrameControl::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_bCreateFlag=true;

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(25,20,16));

	//变量定义
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

	//查询接口
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
		
	//注册事件
	m_pIClientKernel->SetUserEventSink(QUERY_ME_INTERFACE(IUserEventSink));
	m_pIClientKernel->SetStringMessage(QUERY_OBJECT_INTERFACE(m_ChatMessage,IStringMessage));

	//设置界面
	//RectifyControl(CSkinRenderManager::GetInstance());

	m_bMatchOver = TRUE;

	SetTimer(IDI_MATCH_TIMER, 1000, nullptr);

	return FALSE;
}

//消息过虑
BOOL CMatchFrameControl::PreTranslateMessage(MSG * pMsg)
{
	//按键消息
	if (pMsg->message==WM_KEYDOWN)
	{
		//取消按钮
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}

		//回车按钮
		if (pMsg->wParam==VK_RETURN)
		{
			PostMessage(WM_COMMAND,IDC_BT_SEND_CHAT,0);
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CMatchFrameControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	return __super::OnCommand(wParam,lParam);
}

//调整控件
VOID CMatchFrameControl::RectifyControl(INT nWidth, INT nHeight)
{
	//移动准备
  	HDWP hDwp=BeginDeferWindowPos(32);
  	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER | SWP_NOREDRAW;
 
 	::DeferWindowPos(hDwp, m_ChatMessage, nullptr, 15, 325, 222, 400, SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER);
  
  	//移动结束
  	EndDeferWindowPos(hDwp);
	return;
}


//调整界面
VOID CMatchFrameControl::RectifyControl(CSkinRenderManager * pSkinRenderManager)
{
	return;
}

LRESULT CMatchFrameControl::OnNcHitTest(CPoint Point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT lResult = CDialog::OnNcHitTest(Point);

	switch(lResult)
	{
	case HTCAPTION :  return HTNOWHERE;
	}
	return lResult;
}
//绘画背景
BOOL CMatchFrameControl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

VOID CMatchFrameControl::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	INT nXPos=0;
	INT nYPos=0;

	//缓冲DC
	CMemDCEx memDC(&dc, rcClient);
	memDC.SetBkMode(TRANSPARENT);

	CFont DrawFont;
	DrawFont.CreateFont(16,0,0,0,150,0,0,0,134,3,2,1,2,TEXT("宋体"));
	DrawFont.CreatePointFont(110,TEXT("宋体")); //刚刚好
	CFont * pOldFont=memDC.SelectObject(&DrawFont);

	m_ImageBackground.DrawImage(&memDC, 0, 0, m_ImageBackground.GetWidth(), m_ImageBackground.GetHeight(), 0, 0);

 	INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE;
 
  	CString str;
 	
 	//比赛名称
 	nXPos=43;
 	nYPos=40;
 	//str.Format(TEXT("免费赢1万金币赛"),m_MatchViewInfo.szMatchTitle);
 	//str.Format(TEXT("%s"),m_MatchViewInfo.szMatchTitle);
 	//memDC.SetTextColor(RGB(0,228,241));
 	//memDC.DrawText(str,CRect(0,nYPos,rcClient.Width(),rcClient.Height()),nDrawFormat|DT_CENTER);
 
 	//比赛阶段
 	//str.Format(TEXT("预赛-打立出局"),m_MatchViewInfo.szMatchStatus);
 	str.Format(TEXT("%s"),m_MatchViewInfo.szMatchStatus);
 	memDC.SetTextColor(RGB(0,228,241));
 	memDC.DrawText(str,CRect(0,nYPos+10,rcClient.Width(),rcClient.Height()),nDrawFormat|DT_CENTER);
 
	
		//比赛计时
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
	//当前基数
 	if ( m_MatchViewInfo.cbMatchStatus < 5 )  //预赛阶段
 	{
 		m_ImageBase.DrawImage(&memDC,nXPos+30,nYPos+60,m_ImageBase.GetWidth(),m_ImageBase.GetHeight(),0,0);
 
 		DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.wCurBase, nXPos+150,nYPos+68); 
 		//str.Format(TEXT("%d"),m_MatchViewInfo.wCurBase);
 		//memDC.DrawText(str,CRect(nXPos+107,nYPos+53,rcClient.Width(),rcClient.Height()),nDrawFormat);
 
 		str.Format(TEXT("低于%d分出局"),m_MatchViewInfo.wOutScore);
 		memDC.SetTextColor(RGB(0,228,241));
 		memDC.DrawText(str,CRect(20,nYPos+98,rcClient.Width(),rcClient.Height()),nDrawFormat|DT_LEFT);
 
 	}
 	else  //决赛阶段
	{
		m_ImageTurnDraw.DrawImage(&memDC,nXPos+40,nYPos+68,m_ImageTurnDraw.GetWidth(),m_ImageTurnDraw.GetHeight(),0,0);

		DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.cbCurRound, nXPos+63,nYPos+80); 
		//DrawNumberString(&memDC, m_ImageNumberGreen, 3, nXPos+63,nYPos+80); 

		DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.cbCurCount, nXPos+143,nYPos+80); 
		//DrawNumberString(&memDC, m_ImageNumberGreen, 3, nXPos+143,nYPos+80); 

		DrawFont.CreatePointFont(100,TEXT("宋体")); 
		memDC.SelectObject(&DrawFont);
		memDC.SetTextColor(RGB(255,255,255));
		str.Format(TEXT("共%d轮，每轮%d局"),m_MatchViewInfo.cbRoundCount,m_MatchViewInfo.cbGameCount);
		memDC.DrawText(str,CRect(nXPos+10,nYPos+45,rcClient.Width(),rcClient.Height()),nDrawFormat);

		str.Format(TEXT("当前基数:"),m_MatchViewInfo.wCurBase);
		
		memDC.DrawText(str,CRect(nXPos+10,nYPos+103,rcClient.Width(),rcClient.Height()),nDrawFormat);
	}
 	
	
 
 	//比赛排名
 	nXPos=10;
 	nYPos=208;
 	DrawFont.CreatePointFont(100,TEXT("宋体")); 
 	memDC.SelectObject(&DrawFont);
 	m_ImageRank.DrawImage(&memDC,nXPos,nYPos,m_ImageRank.GetWidth(),m_ImageRank.GetHeight(),0,0);
	if ( m_MatchViewInfo.cbMatchStatus < 5 ) 
	{
		str.Format(TEXT("%d人截止,%d人晋级"),m_MatchViewInfo.wRemainCount,m_MatchViewInfo.wPromotionCount);
		memDC.SetTextColor(RGB(151,140,102));
		memDC.DrawText(str,CRect(nXPos+10,nYPos+78,rcClient.Width(),rcClient.Height()),nDrawFormat);
	}
	else
	{
		if ( m_MatchViewInfo.cbCurRound == m_MatchViewInfo.cbRoundCount )
			str.Format(TEXT("%d局竟出冠、亚、季军"),m_MatchViewInfo.cbGameCount);
		else str.Format(TEXT("末位淘汰%d人"),m_MatchViewInfo.wMatchUser);

		memDC.SetTextColor(RGB(151,140,102));
		memDC.DrawText(str,CRect(nXPos+10,nYPos+78,rcClient.Width(),rcClient.Height()),nDrawFormat);
	}

 	//当前排名
	//TCHAR sz[32]={0};
	//_stprintf(sz,L"当前排名:%d",m_MatchViewInfo.wCurRank);
	//OutputDebugString(sz);
	DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.wCurRank, nXPos+66,nYPos+49); 
 	//DrawNumberString(&memDC, m_ImageNumberGreen, 119, nXPos+66,nYPos+49); 

 	//参赛人数
	DrawNumberString(&memDC, m_ImageNumberGreen, m_MatchViewInfo.wMatchUser, nXPos+133,nYPos+49,false,TA_RIGHT); 
 	//DrawNumberString(&memDC, m_ImageNumberGreen, 8, nXPos+133,nYPos+49,false,TA_RIGHT); 
 
 	memDC.SelectObject(pOldFont);
 	DrawFont.DeleteObject();

}

//位置消息
VOID CMatchFrameControl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
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

//绘画数字
VOID CMatchFrameControl::DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,bool bZero,BYTE cbAlign)
{
	//加载资源
	INT nNumberHeight=PngNumber.GetHeight();
	INT nNumberWidth=PngNumber.GetWidth()/10;

	//if ( lNumber==0 ) lNumber=99;

	//计算数目
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos;
	if(cbAlign==TA_RIGHT)
		nXDrawPos+=(lNumberCount-1)*nNumberWidth;

	//绘画桌号
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		//if ( lNumber == 99 ) lNumber=0;
		LONG lCellNumber=(LONG)(lNumber%10L);
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0);

		//设置变量
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	if ( !bMatchOver ) // 比赛重新开始
	{
		m_HourTime = 0;
		m_MinuteTime = 0;
		m_SecondTime = 0;
	}
	m_bMatchOver = bMatchOver; 
}