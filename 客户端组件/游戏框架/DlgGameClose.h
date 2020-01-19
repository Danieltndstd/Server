#pragma once

// CDlgGameClose 对话框
#include "Resource.h"
#include "GameFrameHead.h"

class CDlgGameClose : public CDialog
{
	DECLARE_DYNAMIC(CDlgGameClose)

public:
	CDlgGameClose(LPTSTR msg = nullptr, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGameClose();

// 对话框数据
	enum { IDD = IDD_GAME_CLOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//消息函数
public:
	//绘制背景
	afx_msg void OnPaint();

private:
	//消息
	CString				m_strInfo;				//信息

	//资源
	CPngImage		m_ImageBack;		//背景

	//按钮
	CSkinButton		m_btOk;
	CSkinButton		m_btCancel;
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
