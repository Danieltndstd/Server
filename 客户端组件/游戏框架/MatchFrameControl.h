#ifndef MATCH_FRAME_CONTROL_HEAD_FILE
#define MATCH_FRAME_CONTROL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameUserView.h"
#include "GameFrameMedalView.h"
#include "UserListControl2.h"

//////////////////////////////////////////////////////////////////////////////////

//用户选择
#define MAX_HISTOY_USER				8									//最大数目

//用户数组
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//控制框架
class GAME_FRAME_CLASS CMatchFrameControl : public CDialog, public IMatchFrameControl, public IUserEventSink
{
	//界面变量
protected:
	bool							m_bCreateFlag;						//创建标志

	//界面区域
protected:
	CRect							m_rcUserView;
	CRect							m_rcUserList;
	CRect							m_rcChat;

	//数组变量
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//用户数组

	//控件变量
protected:
	CUserListControl2				m_UserListControl;					//用户列表
	CGameFrameUserView				m_GameFrameUserView;				//用户信息


	//接口变量
protected:
	IClientKernel *					m_pIClientKernel;					//内核接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

	CPngImage						m_ImageBackground;
	CPngImage						m_ImageBase;
	CPngImage						m_ImageRank;
	CPngImage						m_ImageTime[4];
	CPngImage						m_ImageTurnDraw;
	CPngImage						m_ImageNumberOrange;
	CPngImage						m_ImageNumberGreen;
	CPngImage						m_ImageNumTime;

	tagMatchViewInfo				m_MatchViewInfo;
	CRichEditMessage				m_ChatMessage;						//聊天信息

	WORD							m_HourTime;
	WORD							m_MinuteTime;
	WORD							m_SecondTime;
	BOOL							m_bMatchOver;

	//静态变量
protected:
	static	CMatchFrameControl*		m_pMatchFrameControl;

	//函数定义
public:
	//构造函数
	CMatchFrameControl();
	//析构函数
	virtual ~CMatchFrameControl();

	static CMatchFrameControl* GetInstance() {return m_pMatchFrameControl;}

	void SetMatchOverStatus(BOOL bMatchOver) ;

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//配置函数
	virtual BOOL OnInitDialog();
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
	virtual VOID SetMatchViewInfo(tagMatchViewInfo* pMatchViewInfo);

	//用户事件
public:
	//用户进入
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户离开
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户积分
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户状态
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户属性
	virtual VOID OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户头像
	virtual VOID OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//
	virtual VOID OnUserTaskUpdate(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//调整界面
	VOID RectifyControl(CSkinRenderManager * pSkinRenderManager);
	//绘画数字
	VOID DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,bool bZero=false,BYTE cbAlign=TA_LEFT);

	//消息函数
protected:
	//处理非客户区域点击
	LRESULT OnNcHitTest(CPoint point);
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	VOID OnPaint();
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//////////////////////////////////////////////////////////////////////////////////

#endif