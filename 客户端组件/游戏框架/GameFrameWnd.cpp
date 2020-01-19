#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "DlgGameClose.h"
#include "DlgGameOption.h"
#include "GameFrameWnd.h"
#include "DlgVolumnControl.h"

//////////////////////////////////////////////////////////////////////////////////

//控制按钮
#define IDC_MIN						100									//最小按钮
#define IDC_CLOSE					101									//关闭按钮
#define IDC_OPTION				102									//设置按钮
#define IDC_RULE						103									//规则按钮
#define IDC_CHAT					104									//聊天按钮

//控件标识
#define IDC_SKIN_SPLITTER			200									//拆分控件
#define IDC_GAME_CLIENT_VIEW		201									//视图标识

//屏幕位置
#define BORAD_SIZE					3									//边框大小
#define CAPTION_SIZE				32									//标题大小

//控件大小
#define SPLITTER_CX					0									//拆分宽度
#define CAPTION_SIZE				32									//标题大小
#define GLAD_MSG_CY					46									//喜报高度

//动画大小
#define FALSH_MOVIE_CX				300									//动画宽度
#define FALSH_MOVIE_CY				300									//动画高度

//屏幕限制
#define LESS_SCREEN_CY				738									//最小高度
#define LESS_SCREEN_CX				1274								//最小宽度

//定时器
#define IDI_NOTIFY_TEXT			1001								//轮询定时器
#define IDI_SCROLL_TEXT			1002								//滚动定时器
#define	IDI_GAMECONTROL			99

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameWnd, CFrameWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
	ON_WM_PAINT()

	//自定消息
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

	ON_WM_DESTROY()
	ON_WM_MOVE()
//	ON_WM_SHOWWINDOW()
ON_WM_MOVING()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//系统
int GetOSVer()
{
	OSVERSIONINFO   osver;
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osver);
	if (osver.dwPlatformId == 2)
	{
		if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
		{
			printf("xp\n");
			return(2);
		}
		if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
		{
			printf("windows 2003\n");
			return(3);
		}
		if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
		{
			printf("vista and 2008\n");
			return(4);
		}
		if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
		{
			printf("2008 R2 and Windows 7\n");
			return(5);
		}
	}
	return 0;
}

//构造函数
CFrameEncircle::CFrameEncircle()
{
}

//析构函数
CFrameEncircle::~CFrameEncircle()
{
}

//绘画处理
bool CFrameEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//加载图标
	CBitImage ImageLogo;
	ImageLogo.LoadFromResource(AfxGetInstanceHandle(),TEXT("GAME_LOGO"));

	//绘画图标
	if (ImageLogo.IsNull()==false)
	{
		//变量定义
		CDC * pDC=CDC::FromHandle(EncircleImage.ImageTL.GetDC());

		//绘画图标
		ImageLogo.TransDrawImage(pDC,10,4,RGB(255,0,255));

		//释放资源
		EncircleImage.ImageTL.ReleaseDC();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameWnd::CGameFrameWnd()
{
	//滚动变量
	m_nScrollXPos=0L;
	m_nScrollYPos=0L;
	m_nScrollXMax=0L;
	m_nScrollYMax=0L;

	//状态变量
	m_bMaxShow=false;
	m_bShowControl=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	//组件接口
	m_pIClientKernel=NULL;
	m_pIGameFrameView=NULL;

	//控件指针
	m_pDlgGameRule=NULL;

	//框架环绕
	tagEncircleResource	EncircleFrame;
	EncircleFrame.pszImageTL=MAKEINTRESOURCE(IDB_FRAME_TL);
	EncircleFrame.pszImageTM=MAKEINTRESOURCE(IDB_FRAME_TM);
	EncircleFrame.pszImageTR=MAKEINTRESOURCE(IDB_FRAME_TR);
	EncircleFrame.pszImageML=MAKEINTRESOURCE(IDB_FRAME_ML);
	EncircleFrame.pszImageMR=MAKEINTRESOURCE(IDB_FRAME_MR);
	EncircleFrame.pszImageBL=MAKEINTRESOURCE(IDB_FRAME_BL);
	EncircleFrame.pszImageBM=MAKEINTRESOURCE(IDB_FRAME_BM);
	EncircleFrame.pszImageBR=MAKEINTRESOURCE(IDB_FRAME_BR);
	m_FrameEncircle.InitEncircleResource(EncircleFrame,GetModuleHandle(GAME_FRAME_DLL_NAME));

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_WND,QUERY_ME_INTERFACE(IUnknownEx));

	//滚动标题栏消息
	m_strLen = 0;
	m_strX = 1200;

	return;
}

//析构函数
CGameFrameWnd::~CGameFrameWnd()
{
	//销毁控件
	if(m_pDlgGameRule)
	{
		if(m_pDlgGameRule->m_hWnd != NULL)m_pDlgGameRule->DestroyWindow();
		delete m_pDlgGameRule;
		m_pDlgGameRule=NULL;
	}
}

//接口查询
VOID * CGameFrameWnd::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameWnd,Guid,dwQueryVer);
	QUERYINTERFACE(IFlashControlSink,Guid,dwQueryVer);
	QUERYINTERFACE(IClientPropertySink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameWnd,Guid,dwQueryVer);
	return NULL;
}

//消息解释
BOOL CGameFrameWnd::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CGameFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//功能按钮
	switch (nCommandID)
	{
	case IDC_MIN:				//最小按钮
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_CLOSE:				//关闭按钮
		{
			//获取用户
			ASSERT(m_pIClientKernel!=NULL);
			IClientUserItem * pIClientUserItem=m_pIClientKernel->GetMeUserItem();

			//强退提示
 			if ((pIClientUserItem!=NULL)&&(pIClientUserItem->GetUserStatus()==US_PLAYING))
 			{
 				CString str;
 				//提示消息
 				if ( m_MatchFrameControl.m_hWnd !=NULL )
 				{
 					str.Format(TEXT("您正在比赛游戏中，现在不允许退出，请耐心比赛吧！"));
 					CDlgGameClose info(str.GetBuffer(0), this);
 					info.DoModal();
 					return TRUE;
 				}
 				else str.Format(TEXT("您正在游戏中，强行退出将被扣分，确实要强退吗？"));
 				CDlgGameClose info(str.GetBuffer(0), this);
 				if (info.DoModal() != IDOK)
 				{
 					return TRUE;
 				}
 			}
			
			if ( m_MatchFrameControl.m_hWnd )
			{
				int ty=3;
				CRect   m_rect;   
				GetWindowRect(&m_rect);  
				int recordy=m_rect.left;
				int recordx=m_rect.top;

				for(int i=0;i<5;i++)
				{
					m_rect.left=recordy;
					m_rect.top=recordx;
					m_rect.top = m_rect.top + ty;
					m_rect.left = m_rect.left - ty;
					SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
					Sleep(20);
					m_rect.top = m_rect.top -ty;
					SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
					Sleep(20);
					m_rect.top = m_rect.top -2*ty;
					SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
					Sleep(20);
					m_rect.left=m_rect.left+ty;
					SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
					Sleep(20);
					m_rect.left=m_rect.left+2*ty;
					SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
					Sleep(20);
					m_rect.top = m_rect.top + ty;  
					SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE );
					Sleep(20);
					m_rect.top=m_rect.top+2*ty;
					SetWindowPos( NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE ); 
					SetWindowPos( NULL,recordy,recordx,0,0,SWP_NOSIZE );
					Sleep(20);
				}
			}
			//投递关闭
			PostMessage(WM_CLOSE,0,0);

			return TRUE;
		}
	case IDC_OPTION:		//选项按钮
		{
			//m_pIClientKernel->SendProcessData(IPC_CMD_GR_CONTROL,909);
			if (m_pDlgVolumn == nullptr)
			{
				m_pDlgVolumn = new CDlgVolumnControl;
				m_pDlgVolumn->Create(CDlgVolumnControl::IDD, this);
			}

			//设置对话框，设置音量，音效
			m_pDlgVolumn->ShowWindow(m_pDlgVolumn->IsWindowVisible() ? SW_HIDE : SW_SHOW);

			return true;
		}
	case IDC_CHAT:
		{
			CGlobalUnits* pGlobalUnits = CGlobalUnits::GetInstance();
			pGlobalUnits->m_bChatStatus = !pGlobalUnits->m_bChatStatus;

			HINSTANCE hInstance = GetModuleHandle(GAME_FRAME_DLL_NAME);
			
			m_bShowControl = !m_bShowControl;
			m_btChat.SetButtonImage(m_bShowControl?IDB_BT_CLOSE_CHAT:IDB_BT_OPEN_CHAT,hInstance,false,false);
				
				CRect rc;
				ClientToScreen(&rc);

				if ( m_bShowControl )
					SetWindowPos(NULL, rc.left, rc.top, LESS_SCREEN_CX , LESS_SCREEN_CY, SWP_NOZORDER |SWP_NOREDRAW);
				else SetWindowPos(NULL, rc.left, rc.top, 1024 , LESS_SCREEN_CY, SWP_NOZORDER );

			return true;
		}
	case IDC_RULE:
		{
			HWND hwnd =this->GetSafeHwnd();
			CRect rcRect;
			::GetWindowRect(hwnd,&rcRect);
			//桌面路径
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			SHGetSpecialFolderPath(0,szDirectory,CSIDL_DESKTOPDIRECTORY,TRUE);

			SYSTEMTIME st;
			GetLocalTime(&st);
			CString strTime=TEXT("");
			strTime.Format( TEXT( "%d%02d%02d%02d%02d%02d" ) ,
				st.wYear,
				st.wMonth,
				st.wDay ,
				st.wHour,
				st.wMinute ,
				st.wSecond
				);

			//构造路径
			CString strFilePath=TEXT("");
			strFilePath.Format(TEXT("%s\\%s.jpg"),szDirectory,strTime);

			BOOL bRes=Snapshot(rcRect,strFilePath);
			if (bRes == TRUE)
			{
				//提示消息
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("图片已成功复制到剪贴板，并保存至桌面！"),MB_ICONINFORMATION,30L);
			}
			return TRUE;

			try
			{
				TCHAR szRuleUrl[256]=TEXT("");
				CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
				tagGameAttribute * pGameAttribute=pGlobalUnits->m_ClientKernelModule->GetGameAttribute();
				_sntprintf(szRuleUrl,CountArray(szRuleUrl),TEXT("http://www.8099.com/Download/GameDetails?KindID=%ld"),pGameAttribute->wKindID);
				ShellExecute(NULL,TEXT("OPEN"),szRuleUrl,NULL,NULL,SW_NORMAL);

// 				//创建对象
// 				if (m_pDlgGameRule==NULL) m_pDlgGameRule=new CDlgGameRule;
// 
// 				//创建窗口
// 				if (m_pDlgGameRule->m_hWnd==NULL)
// 				{
// 					AfxSetResourceHandle(GetModuleHandle(GAME_FRAME_DLL_NAME));
// 					m_pDlgGameRule->Create(IDD_GAME_RULE,GetDesktopWindow());
// 					AfxSetResourceHandle(GetModuleHandle(NULL));
// 				}
// 
// 				//显示窗口
// 				m_pDlgGameRule->ShowWindow(SW_RESTORE);
// 				m_pDlgGameRule->SetForegroundWindow();

				return true;
			}
			catch (...)
			{ 
				ASSERT(FALSE); 
				return false; 
			}

			return true;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//调整控件
VOID CGameFrameWnd::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if ((nWidth==0)||(nHeight==0)) return;
	if ((m_nScrollXMax==0)||(m_nScrollYMax==0)) return;

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//环绕信息
	tagEncircleInfo EncircleInfoFrame;
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);

	//变量定义
	CRect rcSplitter;
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=m_nScrollYMax-EncircleInfoFrame.nBBorder;

	//拆分控件
	if (m_bShowControl==true)
	{
		rcSplitter.left=1024;
		rcSplitter.right=rcSplitter.left+SPLITTER_CX;
	}
 	else
 	{
 		rcSplitter.left=m_nScrollXMax-EncircleInfoFrame.nRBorder-SPLITTER_CX;
 		rcSplitter.right=m_nScrollXMax-EncircleInfoFrame.nRBorder;
 	}

	//查询游戏
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	IGameFrameView * pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//游戏视图
	if (pIGameFrameView!=NULL)
	{
		HWND hWndView=pIGameFrameView->GetGameViewHwnd();
		DeferWindowPos(hDwp,hWndView, NULL, 0, EncircleInfoFrame.nTBorder, 1024, 702, uFlags);
	}

	//控制按钮
	DeferWindowPos(hDwp, m_btClose, NULL, m_nScrollXMax -15-290, 0, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btOption,NULL,m_nScrollXMax-65-290,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btMin, NULL, m_nScrollXMax -115-290, 0, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btRule, NULL, m_nScrollXMax -178-290, 0, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChat,NULL,m_nScrollXMax -265-290, 0, 0, 0, uFlags | SWP_NOSIZE);
	
	//移动控件
	//DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
 	if ( m_GameFrameControl.m_hWnd != NULL )
 	{
 		//OutputDebugString(L"绘制游戏控制");
 		DeferWindowPos(hDwp,m_GameFrameControl, NULL, rcSplitter.right, 0, 250,738,uFlags);
 	}
	
	if ( m_MatchFrameControl.m_hWnd != NULL )
	{
		//OutputDebugString(L"绘制比赛控制");
		DeferWindowPos(hDwp,m_MatchFrameControl, NULL, rcSplitter.right, 0, 250,738,uFlags);
	}

	//移动结束
	EndDeferWindowPos(hDwp);

	UpdateGladControll();
}

//绘画背景
BOOL CGameFrameWnd::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//按键测试
LRESULT CGameFrameWnd::OnNcHitTest(CPoint Point)
{
	//按钮测试
	if (m_bMaxShow==false)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//转换位置
		CPoint ClientPoint=Point;
		ScreenToClient(&ClientPoint);
	}

	return __super::OnNcHitTest(Point);
}

//位置消息
VOID CGameFrameWnd::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//滚动数据
	m_nScrollXMax=__max(LESS_SCREEN_CX,cx);
	m_nScrollYMax=__max(LESS_SCREEN_CY,cy);
	m_nScrollXPos=__min(0L,m_nScrollXMax-cy);
	m_nScrollYPos=__min(0L,m_nScrollYMax-cy);

	if ( cx > LESS_SCREEN_CX || cy > LESS_SCREEN_CY)
	{
		if ( m_bShowControl )
			SetWindowPos(NULL, 0, 0, LESS_SCREEN_CX , LESS_SCREEN_CY, SWP_NOZORDER );
		else SetWindowPos(NULL, 0, 0, 1024 , LESS_SCREEN_CY, SWP_NOZORDER );

		CenterWindow();
	}
		
	//调整控件
	RectifyControl(cx,cy);

	return;
}

//建立消息
INT CGameFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置窗口
	ModifyStyle(WS_CAPTION, 0, 0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//设置窗口
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_CLIPCHILDREN);
	ModifyStyle(0, WS_CLIPSIBLINGS);

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//查询接口
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	m_btOption.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_OPTION);
	m_btRule.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_RULE);
	m_btChat.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHAT);

	//设置按钮
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,false,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,hInstance,false,false);
	m_btOption.SetButtonImage(IDB_BT_OPTION,hInstance,false,false);
	m_btRule.SetButtonImage(IDB_BT_RULE,hInstance,false,false);
	m_btChat.SetButtonImage(m_bShowControl?IDB_BT_CLOSE_CHAT:IDB_BT_OPEN_CHAT,hInstance,false,false);

	//分隔条
	m_splitPng.LoadImage(hInstance, TEXT("SPLITSTRIP"));

	m_processTitle.LoadImage(hInstance, TEXT("PROGRESS_TITLE"));

	//控制窗口
	AfxSetResourceHandle(hInstance);
	tagServerAttribute* pServerAttribute = m_pIClientKernel->GetServerAttribute();

 	if (pServerAttribute->wServerType&GAME_GENRE_MATCH)
 	{
 		m_MatchFrameControl.Create(IDD_MATCH_CONTROL,this);
 		m_MatchFrameControl.ShowWindow(SW_SHOW);
 	}
 	else
 	{
 		m_GameFrameControl.Create(IDD_GAME_CONTROL,this);
 		m_GameFrameControl.ShowWindow(SW_SHOW);
 	}

	//m_MatchFrameControl.Create(IDD_MATCH_CONTROL,this);
	//m_MatchFrameControl.ShowWindow(SW_SHOW);

	//m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);


	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("最小化"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("关闭游戏"));

	SetWindowPos(NULL,0,0,1024,LESS_SCREEN_CY,SWP_NOZORDER);
	CenterWindow();
	SetTimer(IDI_GAMECONTROL,1000,NULL);

	//创建视图
	ASSERT(m_pIGameFrameView!=NULL);
	if (m_pIGameFrameView!=NULL) m_pIGameFrameView->CreateGameViewWnd(this,IDC_GAME_CLIENT_VIEW);

	//动画控件
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IFlashControlSink);
	m_FlashControl.CreateControl(this,CRect(0,0,FALSH_MOVIE_CX,FALSH_MOVIE_CY),pIUnknownEx);
	m_FlashControl.ShowControl(SW_HIDE);

	//注册接口
	m_pIClientKernel->SetClientPropertySink(QUERY_ME_INTERFACE(IClientPropertySink));

	//显示窗口
	//MaxSizeWindow();
	//音量窗口

	//设置滚动
	m_SkinScrollBar.InitScroolBar(this);

	//设置滚动定时器
	SetTimer(IDI_NOTIFY_TEXT, 8000, nullptr);

	m_GameGladMessage.Create(IDD_GAME_GLAD_MESSAGE,this);

	//生成顶部信息栏
	/*m_pDlgTop = new CDlgTopMsg;
	m_pDlgTop->Create(CDlgTopMsg::IDD, this);

	m_pDlgTop->ShowWindow(SW_SHOW);*/

	return 0L;
}

//鼠标消息
VOID CGameFrameWnd::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//设置改变
VOID CGameFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//获取大小
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);
	ClientToScreen(&rcClient);

	//计算位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,0);

	//调整位置
	rcArce.top-=(rcClient.top-rcWindow.top);
	rcArce.left-=(rcClient.left-rcWindow.left);
	rcArce.right+=(rcWindow.right-rcClient.right);
	rcArce.bottom+=(rcWindow.bottom-rcClient.bottom);

	//移动窗口
	SetWindowPos(NULL,rcArce.left,rcArce.top,rcArce.Width(),rcArce.Height(),SWP_NOZORDER|SWP_NOCOPYBITS);

	return;
}

//标题消息
LRESULT	CGameFrameWnd::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//默认调用
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//更新标题
	Invalidate(TRUE);

	return lResult;
}


//游戏规则
bool CGameFrameWnd::ShowGameRule()
{

	return false;
}

//控制接口
bool CGameFrameWnd::OnGameOptionChange()
{
	//定义变量
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();

	//通知游戏
	if(m_pIClientKernel)
	{
		AllowGameSound(pGlobalUnits->m_bAllowSound);
		AllowGameLookon(0L, pGlobalUnits->m_bAllowLookon);
		m_pIClientKernel->OnGameOptionChange();
	}

	return true;
}

//播放完成
VOID CGameFrameWnd::OnEventFlashFinish()
{
	m_FlashControl.ShowControl(SW_HIDE);
}

//道具成功
VOID  CGameFrameWnd::OnPropertySuccess(LPCTSTR szSourceNickName,DWORD dwSourceUserID,DWORD dwTargerUserID,WORD wPropertyIndex,WORD wPropertyCount)
{
	//获取对象
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//寻找道具
	ASSERT(pGamePropertyManager->GetPropertyItem(wPropertyIndex)!=NULL);
	CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(wPropertyIndex);

	//道具处理
	if (pGamePropertyItem!=NULL && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	{
		//图片信息
		tagPropertyImage PropertyImage;
		pGamePropertyItem->GetImageInfo(PropertyImage);

		//播放动画
		if ((PropertyImage.pszFlash!=NULL)&&(PropertyImage.pszFlash[0]!=0))
		{
			//工作路径
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//构造路径
			TCHAR szFlashPath[MAX_PATH]=TEXT("");
			_sntprintf(szFlashPath,CountArray(szFlashPath),TEXT("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

			try
			{
				//播放动画
				if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
				{
					throw TEXT("动画文件加载失败");
				}

				//动画控件
				CRect rcWindow;
				HWND hGameViewWnd = m_pIGameFrameView->GetGameViewHwnd();
				::GetWindowRect(hGameViewWnd,rcWindow);
				m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

				//播放动画
				m_FlashControl.PlayMovie();
				m_FlashControl.ShowControl(SW_SHOW);
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
		}
	}

	//道具提示
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	{
		//变量定义
		TCHAR  szMessage[256]=TEXT("");
		IClientUserItem * pMeUserItem = m_pIClientKernel->GetMeUserItem();

		//获取提示
		if(dwSourceUserID==pMeUserItem->GetUserID() && dwSourceUserID!=dwTargerUserID)
            _sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您,道具购买成功!"));
		else
		{
			if(dwSourceUserID==dwTargerUserID)
				pGamePropertyItem->GetNotifyMessage(szSourceNickName,szSourceNickName,wPropertyCount,szMessage);
			else
				pGamePropertyItem->GetNotifyMessage(szSourceNickName,pMeUserItem->GetNickName(),wPropertyCount,szMessage);
		}

		//弹出消息
		CInformation Information;
		Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);
	}

	return;
}

//道具失败
VOID CGameFrameWnd::OnPropertyFailure( LONG lErrorCode,LPCTSTR szDescribeString)
{
	//弹出消息
	CInformation Information;
	Information.ShowMessageBox(szDescribeString,MB_ICONINFORMATION);

	return;
}

//旁观控制
bool CGameFrameWnd::AllowGameLookon(DWORD dwUserID, bool bAllowLookon)
{
	//发送消息
	m_pIClientKernel->SendUserLookon(dwUserID,bAllowLookon);

	return true;
}

//窗口销毁
void CGameFrameWnd::OnDestroy()
{
	__super::OnDestroy();

	if (m_pDlgVolumn)
	{
		delete m_pDlgVolumn;
		m_pDlgVolumn = nullptr;
	}

	//删除D2D程序
	CD2DEngine::Delete();
}

//定时器消息
void CGameFrameWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_NOTIFY_TEXT:
	{
		//已经检测到
		if (g_GlobalUnits.m_notify && m_strX <= 270)
		{
			g_GlobalUnits.m_notify = false;
			m_strLen = 0;
		}	

		//检测队列中的消息
		if (!g_GlobalUnits.m_notifyMsg.empty() && m_strLen == 0)
		{
			//取出头部的信息
			m_noticeMsg = g_GlobalUnits.m_notifyMsg.front();
			g_GlobalUnits.m_notifyMsg.pop_front();

			//长度
			m_strLen = _tcslen(m_noticeMsg.szAccount) + _tcslen(m_noticeMsg.szKindName) + _tcslen(m_noticeMsg.szScore);
			m_strX = 1200;

			//开始滚动
			SetTimer(IDI_SCROLL_TEXT, 20, nullptr);
		}
		break;
	}
	case IDI_SCROLL_TEXT:
	{
		//已经到达头部
		if (m_strX <= 270)
		{
			//停止滚动
			KillTimer(IDI_SCROLL_TEXT);	
		}

		//位移变量
		m_strX -= 6;

		Invalidate(TRUE);
		break;
	}
	case IDI_GAMECONTROL:
		{
			KillTimer(IDI_GAMECONTROL);
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CHAT, BN_CLICKED), 0);
			break;
		}
	}
}

void CGameFrameWnd::OnPaint()
{
	CPaintDC dc(this);

	//获取标题
	CString strTitle;
	GetWindowText(strTitle);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC buffer_dc;
	buffer_dc.CreateCompatibleDC(nullptr);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	buffer_dc.SelectObject(&bmp);

	//框架位置
	tagEncircleInfo FrameEncircleInfo;
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);

	//绘画框架
	m_FrameEncircle.DrawEncircleFrame(&buffer_dc, rcClient, true, false, false, false);

	//绘制分隔条
	/*if (m_bShowControl)
		m_splitPng.DrawImage(&buffer_dc, 1024, 0);*/

	//绘画标题
	if (strTitle.IsEmpty() == false)
	{
		//计算区域
		CRect rcTitle;
		rcTitle.SetRect(10, 8, rcClient.Width() - 150, FrameEncircleInfo.nTBorder - 8);

		//变量定义
		INT nXMove[6] = { 1, 1, 1, 0, -1, -1 };
		INT nYMove[6] = { -1, 0, 1, 1, 1, 0 };

		//设置环境
		buffer_dc.SetBkMode(TRANSPARENT);
		buffer_dc.SetTextColor(RGB(219, 182, 97));
		//buffer_dc.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		CFont font;
		font.CreateFont(14, 7, 0, 0, FW_BOLD, 0, 0, 0, CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
			FF_MODERN, TEXT("宋体"));

		CFont* pOldFont = buffer_dc.SelectObject(&font);

		//绘画边框
// 		for (INT i = 0; i < CountArray(nXMove); i++)
// 		{
// 			//计算位置
// 			CRect rcTitleFrame;
// 			rcTitleFrame.top = rcTitle.top + nYMove[i];
// 			rcTitleFrame.left = rcTitle.left + nXMove[i];
// 			rcTitleFrame.right = rcTitle.right + nXMove[i];
// 			rcTitleFrame.bottom = rcTitle.bottom + nYMove[i];
// 
// 			//绘画字符
// 			buffer_dc.DrawText(strTitle, &rcTitleFrame, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
// 		}

		//绘画文字
		//buffer_dc.SetTextColor(RGB(255, 255, 255));
		buffer_dc.DrawText(strTitle, &rcTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	}

	//绘画滚动消息
	if (m_strLen != 0)
	{
		//创建字体并加载
		CFont font;
		font.CreateFont(20, 8, 0, 0, FW_BOLD, 0, 0, 0, CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
			FF_MODERN, TEXT("宋体"));

		CFont* pOldFont = buffer_dc.SelectObject(&font);

		auto goWidth = m_processTitle.GetWidth();
		auto goHeight = m_processTitle.GetHeight();

		m_processTitle.DrawImage(&buffer_dc, m_strX-30, 1, goWidth, goHeight, 0, 0);

		//计算区域
		CRect rcNotify;
		CSize accountSize = buffer_dc.GetTextExtent(m_noticeMsg.szAccount);
		CSize roomSize = buffer_dc.GetTextExtent(m_noticeMsg.szKindName);
		CSize scoreSize = buffer_dc.GetTextExtent(m_noticeMsg.szScore);

		//绘制名字
		rcNotify.SetRect(m_strX, 8, min(690, m_strX + accountSize.cx), 28);
		buffer_dc.SetTextColor(RGB(0, 255, 0));
		buffer_dc.DrawText(m_noticeMsg.szAccount, &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcNotify.SetRect(min(690, m_strX + accountSize.cx), 8, min(690, m_strX + accountSize.cx + 50), 28);
		buffer_dc.SetTextColor(RGB(255, 255, 255));
		buffer_dc.DrawText(TEXT("在"), &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		//绘制房间名字
		rcNotify.SetRect(min(690, m_strX + accountSize.cx + 25), 8, min(690, m_strX + accountSize.cx + 25 + roomSize.cx), 28);
		buffer_dc.SetTextColor(RGB(180, 220, 130));
		buffer_dc.DrawText(m_noticeMsg.szKindName, &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcNotify.SetRect(min(690, m_strX + accountSize.cx + 25 + roomSize.cx), 8, min(690, m_strX + accountSize.cx + 125 + roomSize.cx), 28);
		buffer_dc.SetTextColor(RGB(255, 255, 255));
		buffer_dc.DrawText(TEXT("大胜，赢了"), &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		//绘制金币
		rcNotify.SetRect(min(690, m_strX + accountSize.cx + 125 + roomSize.cx), 8, min(690, m_strX + accountSize.cx + 125 + roomSize.cx + scoreSize.cx), 28);
		buffer_dc.SetTextColor(RGB(255, 255, 0));
		buffer_dc.DrawText(m_noticeMsg.szScore, &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcNotify.SetRect(min(690, m_strX + accountSize.cx + 125 + roomSize.cx + scoreSize.cx), 8, min(690, m_strX + accountSize.cx + 180 + roomSize.cx + scoreSize.cx), 28);
		buffer_dc.SetTextColor(RGB(255, 255, 255));
		buffer_dc.DrawText(TEXT("亿币！"), &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		buffer_dc.SelectObject(pOldFont);


	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &buffer_dc, 0, 0, SRCCOPY);
}


bool CGameFrameWnd::SetGameLayerHandle(IPC_GF_LayerWndInfo& LayerWndInfo)
{
	//作废
/*
 	IPC_GF_LayerWndInfo LayerWndInfo2;
 	ZeroMemory(&LayerWndInfo2,sizeof(LayerWndInfo2));

	RECT rtGameWnd;
	DWORD dwXPos,dwYPos;
	GetWindowRect(&rtGameWnd);
	IPC_GF_LayerWndInfo LayerWnd = g_GlobalUnits.m_LayerWndMap[LayerWndInfo.wLayerType];
	dwXPos = LayerWndInfo.dwXPos - rtGameWnd.left;
	dwYPos = LayerWndInfo.dwYPos - rtGameWnd.top;
	if ( g_GlobalUnits.m_LayerWndMap.GetCount() )
	{
		//相对于屏幕的坐标-游戏窗口坐标==相对游戏窗口坐标,，则不更新
		if ( dwXPos == LayerWnd.dwXPos && 
			 dwYPos ==  LayerWnd.dwYPos	)
		return true;
		
	}
	CopyMemory(&LayerWndInfo2,&LayerWndInfo,sizeof(LayerWndInfo));
	LayerWndInfo2.dwXPos = dwXPos;
	LayerWndInfo2.dwYPos = dwYPos;

	g_GlobalUnits.m_LayerWndMap.SetAt(LayerWndInfo2.wLayerType,LayerWndInfo2);
*/
	g_GlobalUnits.m_LayerWndMap.SetAt(LayerWndInfo.wLayerType,LayerWndInfo);

	return true;
}

void CGameFrameWnd::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码

	UpdateGladControll();
// 	TCHAR sztmp[128]={0};
// 
// 	POSITION pos = g_GlobalUnits.m_LayerWndMap.GetStartPosition();
// 
// 	//只能移动固定位置的分层窗口
// 	while(pos)
// 	{
// 		g_GlobalUnits.m_LayerWndMap.GetNextAssoc(pos,m_LayerWndMapIndex,m_LayerWndInfo);
// 
// 		::MoveWindow(HWND(m_LayerWndInfo.dwLayerHand),m_LayerWndInfo.dwXPos+x,m_LayerWndInfo.dwYPos+y,m_LayerWndInfo.dwSrcWidth,m_LayerWndInfo.dwSrcHeight,FALSE);
// 
// 	}
}



//void CGameFrameWnd::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	__super::OnShowWindow(bShow, nStatus);
//
//	// TODO: 在此处添加消息处理程序代码
//}


void CGameFrameWnd::OnMoving(UINT fwSide, LPRECT pRect)
{
	__super::OnMoving(fwSide, pRect);
	UpdateGladControll();
	// TODO: 在此处添加消息处理程序代码
}

void CGameFrameWnd::UpdateGladControll()
{
	if ( m_GameGladMessage.m_hWnd == NULL ) return;
	CRect rcClient;
	GetClientRect(&rcClient);
	//转换位置
	CPoint pt(BORAD_SIZE, CAPTION_SIZE);
	ClientToScreen(&pt);
	m_GameGladMessage.SetWindowPos(NULL, pt.x, pt.y, 1024 - BORAD_SIZE * 2, GLAD_MSG_CY, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS /*| SWP_HIDEWINDOW*/);
} 

//截屏
BOOL CGameFrameWnd::Snapshot(CRect rcRect,CString strFileName)
{
	if(strFileName ==TEXT(""))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CDC *pDC;//保存屏幕DC
	pDC=CDC::FromHandle(::GetDC(NULL));//获得当前整个屏幕DC
	//int Width = pDC->GetDeviceCaps(HORZRES);
	//int Height = pDC->GetDeviceCaps(VERTRES);
	int Width = rcRect.Width();
	int Height = rcRect.Height();

	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, Width, Height, pDC, rcRect.left, rcRect.top, SRCCOPY);//复制屏幕图像到内存DC

	//把兼容位图放到剪贴板
	if (OpenClipboard())
	{
		EmptyClipboard();
		SetClipboardData(CF_BITMAP,memBitmap.m_hObject);
		CloseClipboard();
	}

	//保存文件
	Bitmap mbitmap(HBITMAP(memBitmap),NULL);//从CBitmap中得到HBitmap
	CLSID jpegClsid;
	GetEncoderClsid(TEXT("image/jpeg"),&jpegClsid);//选择编码
	mbitmap.Save(strFileName,&jpegClsid);//保存

	memDC.SelectObject(oldmemBitmap);

	return TRUE;
}

int CGameFrameWnd::GetEncoderClsid(const WCHAR* format,CLSID* pClsid)
{
	UINT num=0;    //number of image encoders
	UINT size=0;	//size of the image encoder array in bytes
	ImageCodecInfo* pImageCodecInfo=NULL;
	GetImageEncodersSize(&num,&size);
	if (size == 0)
		return -1;  //Failure

	pImageCodecInfo=(ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  //Failure

	GetImageEncoders(num,size,pImageCodecInfo);
	for (UINT j=0;j<num;++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType,format)==0)
		{
			*pClsid=pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;   //Success
		}
	}
	free(pImageCodecInfo);
	return -1;  //Failure
}