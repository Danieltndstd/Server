#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "DlgGameClose.h"
#include "DlgGameOption.h"
#include "GameFrameWnd.h"
#include "DlgVolumnControl.h"

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_MIN						100									//��С��ť
#define IDC_CLOSE					101									//�رհ�ť
#define IDC_OPTION				102									//���ð�ť
#define IDC_RULE						103									//����ť
#define IDC_CHAT					104									//���찴ť

//�ؼ���ʶ
#define IDC_SKIN_SPLITTER			200									//��ֿؼ�
#define IDC_GAME_CLIENT_VIEW		201									//��ͼ��ʶ

//��Ļλ��
#define BORAD_SIZE					3									//�߿��С
#define CAPTION_SIZE				32									//�����С

//�ؼ���С
#define SPLITTER_CX					0									//��ֿ��
#define CAPTION_SIZE				32									//�����С
#define GLAD_MSG_CY					46									//ϲ���߶�

//������С
#define FALSH_MOVIE_CX				300									//�������
#define FALSH_MOVIE_CY				300									//�����߶�

//��Ļ����
#define LESS_SCREEN_CY				738									//��С�߶�
#define LESS_SCREEN_CX				1274								//��С���

//��ʱ��
#define IDI_NOTIFY_TEXT			1001								//��ѯ��ʱ��
#define IDI_SCROLL_TEXT			1002								//������ʱ��
#define	IDI_GAMECONTROL			99

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameWnd, CFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
	ON_WM_PAINT()

	//�Զ���Ϣ
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

	ON_WM_DESTROY()
	ON_WM_MOVE()
//	ON_WM_SHOWWINDOW()
ON_WM_MOVING()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//ϵͳ
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

//���캯��
CFrameEncircle::CFrameEncircle()
{
}

//��������
CFrameEncircle::~CFrameEncircle()
{
}

//�滭����
bool CFrameEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//����ͼ��
	CBitImage ImageLogo;
	ImageLogo.LoadFromResource(AfxGetInstanceHandle(),TEXT("GAME_LOGO"));

	//�滭ͼ��
	if (ImageLogo.IsNull()==false)
	{
		//��������
		CDC * pDC=CDC::FromHandle(EncircleImage.ImageTL.GetDC());

		//�滭ͼ��
		ImageLogo.TransDrawImage(pDC,10,4,RGB(255,0,255));

		//�ͷ���Դ
		EncircleImage.ImageTL.ReleaseDC();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameWnd::CGameFrameWnd()
{
	//��������
	m_nScrollXPos=0L;
	m_nScrollYPos=0L;
	m_nScrollXMax=0L;
	m_nScrollYMax=0L;

	//״̬����
	m_bMaxShow=false;
	m_bShowControl=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	//����ӿ�
	m_pIClientKernel=NULL;
	m_pIGameFrameView=NULL;

	//�ؼ�ָ��
	m_pDlgGameRule=NULL;

	//��ܻ���
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

	//ע�����
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_WND,QUERY_ME_INTERFACE(IUnknownEx));

	//������������Ϣ
	m_strLen = 0;
	m_strX = 1200;

	return;
}

//��������
CGameFrameWnd::~CGameFrameWnd()
{
	//���ٿؼ�
	if(m_pDlgGameRule)
	{
		if(m_pDlgGameRule->m_hWnd != NULL)m_pDlgGameRule->DestroyWindow();
		delete m_pDlgGameRule;
		m_pDlgGameRule=NULL;
	}
}

//�ӿڲ�ѯ
VOID * CGameFrameWnd::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameWnd,Guid,dwQueryVer);
	QUERYINTERFACE(IFlashControlSink,Guid,dwQueryVer);
	QUERYINTERFACE(IClientPropertySink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameWnd,Guid,dwQueryVer);
	return NULL;
}

//��Ϣ����
BOOL CGameFrameWnd::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CGameFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_MIN:				//��С��ť
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_CLOSE:				//�رհ�ť
		{
			//��ȡ�û�
			ASSERT(m_pIClientKernel!=NULL);
			IClientUserItem * pIClientUserItem=m_pIClientKernel->GetMeUserItem();

			//ǿ����ʾ
 			if ((pIClientUserItem!=NULL)&&(pIClientUserItem->GetUserStatus()==US_PLAYING))
 			{
 				CString str;
 				//��ʾ��Ϣ
 				if ( m_MatchFrameControl.m_hWnd !=NULL )
 				{
 					str.Format(TEXT("�����ڱ�����Ϸ�У����ڲ������˳��������ı����ɣ�"));
 					CDlgGameClose info(str.GetBuffer(0), this);
 					info.DoModal();
 					return TRUE;
 				}
 				else str.Format(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"));
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
			//Ͷ�ݹر�
			PostMessage(WM_CLOSE,0,0);

			return TRUE;
		}
	case IDC_OPTION:		//ѡ�ť
		{
			//m_pIClientKernel->SendProcessData(IPC_CMD_GR_CONTROL,909);
			if (m_pDlgVolumn == nullptr)
			{
				m_pDlgVolumn = new CDlgVolumnControl;
				m_pDlgVolumn->Create(CDlgVolumnControl::IDD, this);
			}

			//���öԻ���������������Ч
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
			//����·��
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

			//����·��
			CString strFilePath=TEXT("");
			strFilePath.Format(TEXT("%s\\%s.jpg"),szDirectory,strTime);

			BOOL bRes=Snapshot(rcRect,strFilePath);
			if (bRes == TRUE)
			{
				//��ʾ��Ϣ
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("ͼƬ�ѳɹ����Ƶ������壬�����������棡"),MB_ICONINFORMATION,30L);
			}
			return TRUE;

			try
			{
				TCHAR szRuleUrl[256]=TEXT("");
				CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
				tagGameAttribute * pGameAttribute=pGlobalUnits->m_ClientKernelModule->GetGameAttribute();
				_sntprintf(szRuleUrl,CountArray(szRuleUrl),TEXT("http://www.8099.com/Download/GameDetails?KindID=%ld"),pGameAttribute->wKindID);
				ShellExecute(NULL,TEXT("OPEN"),szRuleUrl,NULL,NULL,SW_NORMAL);

// 				//��������
// 				if (m_pDlgGameRule==NULL) m_pDlgGameRule=new CDlgGameRule;
// 
// 				//��������
// 				if (m_pDlgGameRule->m_hWnd==NULL)
// 				{
// 					AfxSetResourceHandle(GetModuleHandle(GAME_FRAME_DLL_NAME));
// 					m_pDlgGameRule->Create(IDD_GAME_RULE,GetDesktopWindow());
// 					AfxSetResourceHandle(GetModuleHandle(NULL));
// 				}
// 
// 				//��ʾ����
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

//�����ؼ�
VOID CGameFrameWnd::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;
	if ((m_nScrollXMax==0)||(m_nScrollYMax==0)) return;

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//������Ϣ
	tagEncircleInfo EncircleInfoFrame;
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);

	//��������
	CRect rcSplitter;
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=m_nScrollYMax-EncircleInfoFrame.nBBorder;

	//��ֿؼ�
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

	//��ѯ��Ϸ
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	IGameFrameView * pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//��Ϸ��ͼ
	if (pIGameFrameView!=NULL)
	{
		HWND hWndView=pIGameFrameView->GetGameViewHwnd();
		DeferWindowPos(hDwp,hWndView, NULL, 0, EncircleInfoFrame.nTBorder, 1024, 702, uFlags);
	}

	//���ư�ť
	DeferWindowPos(hDwp, m_btClose, NULL, m_nScrollXMax -15-290, 0, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btOption,NULL,m_nScrollXMax-65-290,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btMin, NULL, m_nScrollXMax -115-290, 0, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btRule, NULL, m_nScrollXMax -178-290, 0, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChat,NULL,m_nScrollXMax -265-290, 0, 0, 0, uFlags | SWP_NOSIZE);
	
	//�ƶ��ؼ�
	//DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
 	if ( m_GameFrameControl.m_hWnd != NULL )
 	{
 		//OutputDebugString(L"������Ϸ����");
 		DeferWindowPos(hDwp,m_GameFrameControl, NULL, rcSplitter.right, 0, 250,738,uFlags);
 	}
	
	if ( m_MatchFrameControl.m_hWnd != NULL )
	{
		//OutputDebugString(L"���Ʊ�������");
		DeferWindowPos(hDwp,m_MatchFrameControl, NULL, rcSplitter.right, 0, 250,738,uFlags);
	}

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	UpdateGladControll();
}

//�滭����
BOOL CGameFrameWnd::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//��������
LRESULT CGameFrameWnd::OnNcHitTest(CPoint Point)
{
	//��ť����
	if (m_bMaxShow==false)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//ת��λ��
		CPoint ClientPoint=Point;
		ScreenToClient(&ClientPoint);
	}

	return __super::OnNcHitTest(Point);
}

//λ����Ϣ
VOID CGameFrameWnd::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//��������
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
		
	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
INT CGameFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION, 0, 0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//���ô���
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_CLIPCHILDREN);
	ModifyStyle(0, WS_CLIPSIBLINGS);

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ѯ�ӿ�
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	m_btOption.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_OPTION);
	m_btRule.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_RULE);
	m_btChat.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHAT);

	//���ð�ť
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,false,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,hInstance,false,false);
	m_btOption.SetButtonImage(IDB_BT_OPTION,hInstance,false,false);
	m_btRule.SetButtonImage(IDB_BT_RULE,hInstance,false,false);
	m_btChat.SetButtonImage(m_bShowControl?IDB_BT_CLOSE_CHAT:IDB_BT_OPEN_CHAT,hInstance,false,false);

	//�ָ���
	m_splitPng.LoadImage(hInstance, TEXT("SPLITSTRIP"));

	m_processTitle.LoadImage(hInstance, TEXT("PROGRESS_TITLE"));

	//���ƴ���
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


	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("��С��"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("�ر���Ϸ"));

	SetWindowPos(NULL,0,0,1024,LESS_SCREEN_CY,SWP_NOZORDER);
	CenterWindow();
	SetTimer(IDI_GAMECONTROL,1000,NULL);

	//������ͼ
	ASSERT(m_pIGameFrameView!=NULL);
	if (m_pIGameFrameView!=NULL) m_pIGameFrameView->CreateGameViewWnd(this,IDC_GAME_CLIENT_VIEW);

	//�����ؼ�
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IFlashControlSink);
	m_FlashControl.CreateControl(this,CRect(0,0,FALSH_MOVIE_CX,FALSH_MOVIE_CY),pIUnknownEx);
	m_FlashControl.ShowControl(SW_HIDE);

	//ע��ӿ�
	m_pIClientKernel->SetClientPropertySink(QUERY_ME_INTERFACE(IClientPropertySink));

	//��ʾ����
	//MaxSizeWindow();
	//��������

	//���ù���
	m_SkinScrollBar.InitScroolBar(this);

	//���ù�����ʱ��
	SetTimer(IDI_NOTIFY_TEXT, 8000, nullptr);

	m_GameGladMessage.Create(IDD_GAME_GLAD_MESSAGE,this);

	//���ɶ�����Ϣ��
	/*m_pDlgTop = new CDlgTopMsg;
	m_pDlgTop->Create(CDlgTopMsg::IDD, this);

	m_pDlgTop->ShowWindow(SW_SHOW);*/

	return 0L;
}

//�����Ϣ
VOID CGameFrameWnd::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//���øı�
VOID CGameFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//��ȡ��С
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);
	ClientToScreen(&rcClient);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,0);

	//����λ��
	rcArce.top-=(rcClient.top-rcWindow.top);
	rcArce.left-=(rcClient.left-rcWindow.left);
	rcArce.right+=(rcWindow.right-rcClient.right);
	rcArce.bottom+=(rcWindow.bottom-rcClient.bottom);

	//�ƶ�����
	SetWindowPos(NULL,rcArce.left,rcArce.top,rcArce.Width(),rcArce.Height(),SWP_NOZORDER|SWP_NOCOPYBITS);

	return;
}

//������Ϣ
LRESULT	CGameFrameWnd::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�ϵ���
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//���±���
	Invalidate(TRUE);

	return lResult;
}


//��Ϸ����
bool CGameFrameWnd::ShowGameRule()
{

	return false;
}

//���ƽӿ�
bool CGameFrameWnd::OnGameOptionChange()
{
	//�������
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();

	//֪ͨ��Ϸ
	if(m_pIClientKernel)
	{
		AllowGameSound(pGlobalUnits->m_bAllowSound);
		AllowGameLookon(0L, pGlobalUnits->m_bAllowLookon);
		m_pIClientKernel->OnGameOptionChange();
	}

	return true;
}

//�������
VOID CGameFrameWnd::OnEventFlashFinish()
{
	m_FlashControl.ShowControl(SW_HIDE);
}

//���߳ɹ�
VOID  CGameFrameWnd::OnPropertySuccess(LPCTSTR szSourceNickName,DWORD dwSourceUserID,DWORD dwTargerUserID,WORD wPropertyIndex,WORD wPropertyCount)
{
	//��ȡ����
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//Ѱ�ҵ���
	ASSERT(pGamePropertyManager->GetPropertyItem(wPropertyIndex)!=NULL);
	CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(wPropertyIndex);

	//���ߴ���
	if (pGamePropertyItem!=NULL && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	{
		//ͼƬ��Ϣ
		tagPropertyImage PropertyImage;
		pGamePropertyItem->GetImageInfo(PropertyImage);

		//���Ŷ���
		if ((PropertyImage.pszFlash!=NULL)&&(PropertyImage.pszFlash[0]!=0))
		{
			//����·��
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFlashPath[MAX_PATH]=TEXT("");
			_sntprintf(szFlashPath,CountArray(szFlashPath),TEXT("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

			try
			{
				//���Ŷ���
				if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
				{
					throw TEXT("�����ļ�����ʧ��");
				}

				//�����ؼ�
				CRect rcWindow;
				HWND hGameViewWnd = m_pIGameFrameView->GetGameViewHwnd();
				::GetWindowRect(hGameViewWnd,rcWindow);
				m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

				//���Ŷ���
				m_FlashControl.PlayMovie();
				m_FlashControl.ShowControl(SW_SHOW);
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
		}
	}

	//������ʾ
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	{
		//��������
		TCHAR  szMessage[256]=TEXT("");
		IClientUserItem * pMeUserItem = m_pIClientKernel->GetMeUserItem();

		//��ȡ��ʾ
		if(dwSourceUserID==pMeUserItem->GetUserID() && dwSourceUserID!=dwTargerUserID)
            _sntprintf(szMessage,CountArray(szMessage),TEXT("��ϲ��,���߹���ɹ�!"));
		else
		{
			if(dwSourceUserID==dwTargerUserID)
				pGamePropertyItem->GetNotifyMessage(szSourceNickName,szSourceNickName,wPropertyCount,szMessage);
			else
				pGamePropertyItem->GetNotifyMessage(szSourceNickName,pMeUserItem->GetNickName(),wPropertyCount,szMessage);
		}

		//������Ϣ
		CInformation Information;
		Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);
	}

	return;
}

//����ʧ��
VOID CGameFrameWnd::OnPropertyFailure( LONG lErrorCode,LPCTSTR szDescribeString)
{
	//������Ϣ
	CInformation Information;
	Information.ShowMessageBox(szDescribeString,MB_ICONINFORMATION);

	return;
}

//�Թۿ���
bool CGameFrameWnd::AllowGameLookon(DWORD dwUserID, bool bAllowLookon)
{
	//������Ϣ
	m_pIClientKernel->SendUserLookon(dwUserID,bAllowLookon);

	return true;
}

//��������
void CGameFrameWnd::OnDestroy()
{
	__super::OnDestroy();

	if (m_pDlgVolumn)
	{
		delete m_pDlgVolumn;
		m_pDlgVolumn = nullptr;
	}

	//ɾ��D2D����
	CD2DEngine::Delete();
}

//��ʱ����Ϣ
void CGameFrameWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_NOTIFY_TEXT:
	{
		//�Ѿ���⵽
		if (g_GlobalUnits.m_notify && m_strX <= 270)
		{
			g_GlobalUnits.m_notify = false;
			m_strLen = 0;
		}	

		//�������е���Ϣ
		if (!g_GlobalUnits.m_notifyMsg.empty() && m_strLen == 0)
		{
			//ȡ��ͷ������Ϣ
			m_noticeMsg = g_GlobalUnits.m_notifyMsg.front();
			g_GlobalUnits.m_notifyMsg.pop_front();

			//����
			m_strLen = _tcslen(m_noticeMsg.szAccount) + _tcslen(m_noticeMsg.szKindName) + _tcslen(m_noticeMsg.szScore);
			m_strX = 1200;

			//��ʼ����
			SetTimer(IDI_SCROLL_TEXT, 20, nullptr);
		}
		break;
	}
	case IDI_SCROLL_TEXT:
	{
		//�Ѿ�����ͷ��
		if (m_strX <= 270)
		{
			//ֹͣ����
			KillTimer(IDI_SCROLL_TEXT);	
		}

		//λ�Ʊ���
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

	//��ȡ����
	CString strTitle;
	GetWindowText(strTitle);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC buffer_dc;
	buffer_dc.CreateCompatibleDC(nullptr);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	buffer_dc.SelectObject(&bmp);

	//���λ��
	tagEncircleInfo FrameEncircleInfo;
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);

	//�滭���
	m_FrameEncircle.DrawEncircleFrame(&buffer_dc, rcClient, true, false, false, false);

	//���Ʒָ���
	/*if (m_bShowControl)
		m_splitPng.DrawImage(&buffer_dc, 1024, 0);*/

	//�滭����
	if (strTitle.IsEmpty() == false)
	{
		//��������
		CRect rcTitle;
		rcTitle.SetRect(10, 8, rcClient.Width() - 150, FrameEncircleInfo.nTBorder - 8);

		//��������
		INT nXMove[6] = { 1, 1, 1, 0, -1, -1 };
		INT nYMove[6] = { -1, 0, 1, 1, 1, 0 };

		//���û���
		buffer_dc.SetBkMode(TRANSPARENT);
		buffer_dc.SetTextColor(RGB(219, 182, 97));
		//buffer_dc.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		CFont font;
		font.CreateFont(14, 7, 0, 0, FW_BOLD, 0, 0, 0, CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
			FF_MODERN, TEXT("����"));

		CFont* pOldFont = buffer_dc.SelectObject(&font);

		//�滭�߿�
// 		for (INT i = 0; i < CountArray(nXMove); i++)
// 		{
// 			//����λ��
// 			CRect rcTitleFrame;
// 			rcTitleFrame.top = rcTitle.top + nYMove[i];
// 			rcTitleFrame.left = rcTitle.left + nXMove[i];
// 			rcTitleFrame.right = rcTitle.right + nXMove[i];
// 			rcTitleFrame.bottom = rcTitle.bottom + nYMove[i];
// 
// 			//�滭�ַ�
// 			buffer_dc.DrawText(strTitle, &rcTitleFrame, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
// 		}

		//�滭����
		//buffer_dc.SetTextColor(RGB(255, 255, 255));
		buffer_dc.DrawText(strTitle, &rcTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	}

	//�滭������Ϣ
	if (m_strLen != 0)
	{
		//�������岢����
		CFont font;
		font.CreateFont(20, 8, 0, 0, FW_BOLD, 0, 0, 0, CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
			FF_MODERN, TEXT("����"));

		CFont* pOldFont = buffer_dc.SelectObject(&font);

		auto goWidth = m_processTitle.GetWidth();
		auto goHeight = m_processTitle.GetHeight();

		m_processTitle.DrawImage(&buffer_dc, m_strX-30, 1, goWidth, goHeight, 0, 0);

		//��������
		CRect rcNotify;
		CSize accountSize = buffer_dc.GetTextExtent(m_noticeMsg.szAccount);
		CSize roomSize = buffer_dc.GetTextExtent(m_noticeMsg.szKindName);
		CSize scoreSize = buffer_dc.GetTextExtent(m_noticeMsg.szScore);

		//��������
		rcNotify.SetRect(m_strX, 8, min(690, m_strX + accountSize.cx), 28);
		buffer_dc.SetTextColor(RGB(0, 255, 0));
		buffer_dc.DrawText(m_noticeMsg.szAccount, &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcNotify.SetRect(min(690, m_strX + accountSize.cx), 8, min(690, m_strX + accountSize.cx + 50), 28);
		buffer_dc.SetTextColor(RGB(255, 255, 255));
		buffer_dc.DrawText(TEXT("��"), &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		//���Ʒ�������
		rcNotify.SetRect(min(690, m_strX + accountSize.cx + 25), 8, min(690, m_strX + accountSize.cx + 25 + roomSize.cx), 28);
		buffer_dc.SetTextColor(RGB(180, 220, 130));
		buffer_dc.DrawText(m_noticeMsg.szKindName, &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcNotify.SetRect(min(690, m_strX + accountSize.cx + 25 + roomSize.cx), 8, min(690, m_strX + accountSize.cx + 125 + roomSize.cx), 28);
		buffer_dc.SetTextColor(RGB(255, 255, 255));
		buffer_dc.DrawText(TEXT("��ʤ��Ӯ��"), &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		//���ƽ��
		rcNotify.SetRect(min(690, m_strX + accountSize.cx + 125 + roomSize.cx), 8, min(690, m_strX + accountSize.cx + 125 + roomSize.cx + scoreSize.cx), 28);
		buffer_dc.SetTextColor(RGB(255, 255, 0));
		buffer_dc.DrawText(m_noticeMsg.szScore, &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcNotify.SetRect(min(690, m_strX + accountSize.cx + 125 + roomSize.cx + scoreSize.cx), 8, min(690, m_strX + accountSize.cx + 180 + roomSize.cx + scoreSize.cx), 28);
		buffer_dc.SetTextColor(RGB(255, 255, 255));
		buffer_dc.DrawText(TEXT("�ڱң�"), &rcNotify, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		buffer_dc.SelectObject(pOldFont);


	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &buffer_dc, 0, 0, SRCCOPY);
}


bool CGameFrameWnd::SetGameLayerHandle(IPC_GF_LayerWndInfo& LayerWndInfo)
{
	//����
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
		//�������Ļ������-��Ϸ��������==�����Ϸ��������,���򲻸���
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

	// TODO: �ڴ˴������Ϣ����������

	UpdateGladControll();
// 	TCHAR sztmp[128]={0};
// 
// 	POSITION pos = g_GlobalUnits.m_LayerWndMap.GetStartPosition();
// 
// 	//ֻ���ƶ��̶�λ�õķֲ㴰��
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
//	// TODO: �ڴ˴������Ϣ����������
//}


void CGameFrameWnd::OnMoving(UINT fwSide, LPRECT pRect)
{
	__super::OnMoving(fwSide, pRect);
	UpdateGladControll();
	// TODO: �ڴ˴������Ϣ����������
}

void CGameFrameWnd::UpdateGladControll()
{
	if ( m_GameGladMessage.m_hWnd == NULL ) return;
	CRect rcClient;
	GetClientRect(&rcClient);
	//ת��λ��
	CPoint pt(BORAD_SIZE, CAPTION_SIZE);
	ClientToScreen(&pt);
	m_GameGladMessage.SetWindowPos(NULL, pt.x, pt.y, 1024 - BORAD_SIZE * 2, GLAD_MSG_CY, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS /*| SWP_HIDEWINDOW*/);
} 

//����
BOOL CGameFrameWnd::Snapshot(CRect rcRect,CString strFileName)
{
	if(strFileName ==TEXT(""))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CDC *pDC;//������ĻDC
	pDC=CDC::FromHandle(::GetDC(NULL));//��õ�ǰ������ĻDC
	//int Width = pDC->GetDeviceCaps(HORZRES);
	//int Height = pDC->GetDeviceCaps(VERTRES);
	int Width = rcRect.Width();
	int Height = rcRect.Height();

	CDC memDC;//�ڴ�DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, Width, Height, pDC, rcRect.left, rcRect.top, SRCCOPY);//������Ļͼ���ڴ�DC

	//�Ѽ���λͼ�ŵ�������
	if (OpenClipboard())
	{
		EmptyClipboard();
		SetClipboardData(CF_BITMAP,memBitmap.m_hObject);
		CloseClipboard();
	}

	//�����ļ�
	Bitmap mbitmap(HBITMAP(memBitmap),NULL);//��CBitmap�еõ�HBitmap
	CLSID jpegClsid;
	GetEncoderClsid(TEXT("image/jpeg"),&jpegClsid);//ѡ�����
	mbitmap.Save(strFileName,&jpegClsid);//����

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