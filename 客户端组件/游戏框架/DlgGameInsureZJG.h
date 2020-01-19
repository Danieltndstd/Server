#pragma once
#include "afxwin.h"

// CDlgGameInsureZJG 对话框

#include "GameFrameHead.h"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////
void MakeStringZJG(CString &strNum,SCORE lNumber);
void MakeStringToNumZJG(CString str,SCORE &Num);
////////////////////////////////////////////////////////////////////////

class CDlgGameInsureZJG : public CDialog
{
	DECLARE_DYNAMIC(CDlgGameInsureZJG)

public:
	CDlgGameInsureZJG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGameInsureZJG();

// 对话框数据
	enum { IDD = IDD_ZJG_BAKER_STORAGE };

public:
	//显示控件
	void ShowItem();
	//更新界面
	void UpdateView();
	//绘画数字
	void DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos);
	//刷新信息
	void FreshBank();

public:
	IClientUserItem					*m_pMeUserData;					//用户信息
	IClientKernel					*m_pIClientKernel;				//内核接口

	bool							m_blUsingPassWord;				//密码是加密
	WORD							m_wRevenueTake;					//税收比例
	WORD							m_wRevenueTransfer;				//税收比例
	LONGLONG						m_bankGold;						//银行数目

protected:
	CSkinButton						m_btClose;
	CSkinButton						m_btLink;
	CSkinButton						m_btAll;
	CSkinButton						m_btOkay;

	CPngImage						m_pngNumber;


	LONGLONG						m_OrInCount;  	//输入值
	LONGLONG						m_helpCount;
	

protected:
	virtual BOOL OnInitDialog();

	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//关闭按钮
	afx_msg void OnBnClickedClose();
	//链接按钮
	afx_msg void OnBnClickedLink();
	//全部按钮
	afx_msg void OnBnClickedAll();
	//确认取款按钮
	afx_msg void OnBnClickedMakeSure();
	//绘制对话框
	afx_msg void OnPaint();
	//创建消息
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//命令信息
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void OnLButtonDown(UINT nFlags, CPoint point);
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);

	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnEnChangeEditMoney();

public:
	//设置类型
	void SetBankerActionType(bool bStorage);
	//设置位置
	void SetPostPoint(CPoint Point);
	//设置组件
	void SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserDatas);
	//发送信息
	void SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
	//更新银行	
	void UpdateBanker();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_money;
	CEdit m_password;
};
