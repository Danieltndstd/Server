#include "StdAfx.h"
#include "GlobalUnits.h"
#include "GameFrameEngine.h"
#include "DSoundManager.h"
#include <io.h>

//////////////////////////////////////////////////////////////////////////////////

//标识定义
#define IDC_GAME_FRAME_ENGINE		100									//窗口标识

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CGameFrameEngine *	CGameFrameEngine::m_pGameFrameEngine=NULL;			//引擎接口
bool CGameFrameEngine::m_bDXSound = true;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameEngine, CWnd)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameEngine::CGameFrameEngine()
{
	//界面接口
	m_pIClientKernel=NULL;
	m_pIStringMessage=NULL;

	//设置接口
	ASSERT(m_pGameFrameEngine==NULL);
	if (m_pGameFrameEngine==NULL) m_pGameFrameEngine=this;

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_ENGINE,QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//析构函数
CGameFrameEngine::~CGameFrameEngine()
{
	//设置接口
	ASSERT(m_pGameFrameEngine==this);
	if (m_pGameFrameEngine==this) m_pGameFrameEngine=NULL;

	return;
}

VOID CGameFrameEngine::Release()
{
	CDSoundManager::GetHinstance()->ReleaseFmod(); 
	delete this;
}

//接口查询
VOID * CGameFrameEngine::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameEngine,Guid,dwQueryVer);
	return NULL;
}

//单机模式
bool CGameFrameEngine::IsSingleMode()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return true;

	//获取模式
	return m_pIClientKernel->IsSingleMode();
}

//旁观用户
bool CGameFrameEngine::IsLookonMode()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return true;

	//获取设置
	return m_pIClientKernel->IsLookonMode();
}

//允许旁观
bool CGameFrameEngine::IsAllowLookon()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//获取设置
	return m_pIClientKernel->IsAllowLookon();
}

//获取状态
BYTE CGameFrameEngine::GetGameStatus()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return GAME_STATUS_FREE;

	//获取状态
	return m_pIClientKernel->GetGameStatus();
}

//设置状态
VOID CGameFrameEngine::SetGameStatus(BYTE cbGameStatus)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return;

	//设置状态
	m_pIClientKernel->SetGameStatus(cbGameStatus);

	return;
}

//获取自己
WORD CGameFrameEngine::GetMeChairID()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return NULL;

	//获取自己
	return m_pIClientKernel->GetMeChairID();
}

//获取自己
IClientUserItem * CGameFrameEngine::GetMeUserItem()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return NULL;

	//获取自己
	return m_pIClientKernel->GetMeUserItem();
}

//游戏用户
IClientUserItem * CGameFrameEngine::GetTableUserItem(WORD wChariID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return NULL;

	//获取自己
	return m_pIClientKernel->GetTableUserItem(wChariID);
}

//游戏用户
IClientUserItem * CGameFrameEngine::SearchTableUserItem(DWORD dwUserID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return NULL;

	//搜索用户
	return m_pIClientKernel->SearchUserByUserID(dwUserID);
}

//存入元宝
bool CGameFrameEngine::PerformSaveScore(SCORE lScore)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//构造数据
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	SaveScoreRequest.cbActivityGame=TRUE;
	SaveScoreRequest.lSaveScore=lScore;

	//发送数据
	return m_pIClientKernel->SendSocketData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));
}

//提取元宝
bool CGameFrameEngine::PerformTakeScore(SCORE lScore, TCHAR *pszInsurePass)
{
	//效验参数
	ASSERT(pszInsurePass!=NULL);
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//构造数据
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));
	TakeScoreRequest.cbActivityGame=TRUE;
	TakeScoreRequest.lTakeScore=lScore;
	CopyMemory(TakeScoreRequest.szInsurePass,pszInsurePass,sizeof(TakeScoreRequest.szInsurePass));

	//发送数据
	return m_pIClientKernel->SendSocketData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));
}

//发送函数
bool CGameFrameEngine::SendSocketData(WORD wSubCmdID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendSocketData(MDM_GF_GAME,wSubCmdID);
}

//发送函数
bool CGameFrameEngine::SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendSocketData(MDM_GF_GAME,wSubCmdID,pData,wDataSize);
}

//发送准备
bool CGameFrameEngine::SendUserReady(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendUserReady(pData,wDataSize);
}

//发送表情
bool CGameFrameEngine::SendUserExpression(DWORD dwTargetUserID, WORD wItemIndex)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendUserExpression(dwTargetUserID,wItemIndex);
}

//发送聊天
bool CGameFrameEngine::SendUserChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendUserChatMessage(dwTargetUserID,pszChatMessage,crFontColor);
}

//时钟标识
UINT CGameFrameEngine::GetClockID()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//删除时钟
	return m_pIClientKernel->GetClockID();
}

//时钟位置
WORD CGameFrameEngine::GetClockChairID()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//删除时钟
	return m_pIClientKernel->GetClockChairID();
}

//删除时钟
VOID CGameFrameEngine::KillGameClock(WORD wClockID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return;

	//删除时钟
	m_pIClientKernel->KillGameClock(wClockID);

	return;
}

//设置时钟
VOID CGameFrameEngine::SetGameClock(WORD wChairID, WORD wClockID, UINT nElapse)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return;

	//设置时钟
	m_pIClientKernel->SetGameClock(wChairID,wClockID,nElapse);

	return;
}

//激活框架
VOID CGameFrameEngine::ActiveGameFrame()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return;

	//激活框架
	m_pIClientKernel->ActiveGameFrame();

	return;
}

//切换椅子
WORD CGameFrameEngine::SwitchViewChairID(WORD wChairID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return INVALID_CHAIR;

	//转换椅子
	return m_pIClientKernel->SwitchViewChairID(wChairID);
}

//播放声音
bool CGameFrameEngine::PlayGameSound(LPCTSTR pszSoundName)
{
	if ( !m_bDXSound ) return true;
	//先通过DSound播放
	CDSoundManager* pSoundManager = CDSoundManager::GetHinstance();
	if(pSoundManager && pSoundManager->PlaySound(const_cast<LPTSTR>(pszSoundName)))
		return true;

	//播放声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayGameSound(pszSoundName);

	return true;
}

//播放声音
bool CGameFrameEngine::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	if ( !m_bDXSound ) return true;
	//先通过DSound播放
	CDSoundManager* pSoundManager = CDSoundManager::GetHinstance();
	if(pSoundManager && pSoundManager->PlaySound(const_cast<LPTSTR>(pszSoundName)))
		return true;

	//播放声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayGameSound(hInstance,pszSoundName);

	return true;
}

//播放声音
bool CGameFrameEngine::PlayBackGroundSound(LPCTSTR pszSoundName)
{
	if ( !m_bDXSound ) return true;
	//先通过DSound播放
	CDSoundManager* pSoundManager = CDSoundManager::GetHinstance();
	if(pSoundManager && pSoundManager->PlayMusic(const_cast<LPTSTR>(pszSoundName)))
		return true;

	//播放声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayBackGroundSound(pszSoundName);

	return true;
}

//播放声音
bool CGameFrameEngine::PlayBackGroundSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	if ( !m_bDXSound ) return true;
	//先通过DSound播放
	CDSoundManager* pSoundManager = CDSoundManager::GetHinstance();
	if(pSoundManager && pSoundManager->PlayMusic(const_cast<LPTSTR>(pszSoundName)))
		return true;

	//播放声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayBackGroundSound(hInstance,pszSoundName);

	return true;
}

//停止声音
bool CGameFrameEngine::StopSound()
{
	if ( !m_bDXSound ) return true;
	//先通过DSound停止播放
	CDSoundManager* pSoundManager = CDSoundManager::GetHinstance();
	if(pSoundManager && pSoundManager->Stop())
		return true;

	//停止声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->StopSound();

	return true;
}

bool CGameFrameEngine::StopSound(LPCTSTR pszSoundName)
{
	if (!m_bDXSound) return true;
	//先通过DSound停止播放
	CDSoundManager* pSoundManager = CDSoundManager::GetHinstance();
	if (pSoundManager && pSoundManager->Stop(const_cast<LPTSTR>(pszSoundName)))
		return true;

	return true;
}

//重置引擎
bool CGameFrameEngine::ResetGameFrameEngine()
{
	//重置通知
	if (OnResetGameEngine()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//创建引擎
bool CGameFrameEngine::CreateGameFrameEngine()
{
	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//查询接口
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);

	//用户接口
	IUserEventSink * pIUserEventSink=QUERY_ME_INTERFACE(IUserEventSink);
	if (pIUserEventSink!=NULL) m_pIClientKernel->SetUserEventSink(pIUserEventSink);

	//信息接口
	m_pIStringMessage=m_pIClientKernel->GetIStringMessage();

	//创建窗口
	if (Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),AfxGetMainWnd(),IDC_GAME_FRAME_ENGINE)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建通知
	if (OnInitGameEngine()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//银行消息
bool CGameFrameEngine::OnEventInsureMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//显示消息
INT CGameFrameEngine::ShowMessageBox(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	CInformation Information(this);
	return Information.ShowMessageBox(pszString, uType, nElapse);
}

//显示消息
INT CGameFrameEngine::ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	CInformation Information(this);
	return Information.ShowMessageBox(pszCaption, pszString, uType, nElapse);
}

//设置事件
bool CGameFrameEngine::OnGameOptionChange()
{
	//定义变量
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();

	//事件处理
	AllowBackGroundSound(pGlobalUnits->m_bAllowBackGroundSound);

	return true;
}

//创建或者显示视频控制对话框
void CGameFrameEngine::ShowRecordDlg(vector<UINT>& vElapsed, CWnd* pParent)
{
	m_pGlobalRecordDlg = &CDlgRecordPlayZJG::GetInstance();
	if(m_pGlobalRecordDlg->m_hWnd == nullptr)
		m_pGlobalRecordDlg->Create(IDD_ZJG_RECORD_PLAY);

	//任务栏显示图标
	m_pGlobalRecordDlg->SetBuddyWnd(pParent);
	m_pGlobalRecordDlg->SetRange(vElapsed);
	m_pGlobalRecordDlg->ModifyStyleEx(0, WS_EX_APPWINDOW);
	m_pGlobalRecordDlg->ShowWindow(SW_SHOW);
}

//初始化DSound
BOOL CGameFrameEngine::InitializeDSound()
{
	/*寻找Games目录*/
	if (!PathIsDirectory(TEXT("./Games")))
	{
		if (!CreateDirectory(TEXT("./Games"), nullptr))
			return FALSE;
	}

	/*创建Ini文件*/
	if (_access("./Games/SoundSetting.ini", 0) == -1)
	{
		CreateFile(TEXT("./Games/SoundSetting.ini"), GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	};

	/*初始化DSound*/
	return CDSoundManager::GetHinstance()->InitializeDSound(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////////////
