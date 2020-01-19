#pragma once

// CDlgRecordPlayZJG 对话框
#include "Resource.h"
#include "GameFrameHead.h"
#include "afxcmn.h"

#define WM_OPRATION		WM_USER + 1		//操作流逝
#define WM_OPRATIONEX	WM_USER + 2		//快进操作

class CDlgRecordPlayZJG : public CDialog
{
	DECLARE_DYNAMIC(CDlgRecordPlayZJG)

public:
	virtual ~CDlgRecordPlayZJG();

// 对话框数据
	enum { IDD = IDD_ZJG_RECORD_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//消息函数
public:
	//绘制背景
	afx_msg void OnPaint();
	//左键单击
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//鼠标放开
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//创建消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//最小化窗口
	afx_msg void OnBnClickedMin(){ ::ShowWindow(m_hWnd, SW_MINIMIZE); }
	//关闭窗口
	afx_msg void OnBnClickedClose(){ AfxGetMainWnd()->PostMessage(WM_CLOSE); DestroyWindow(); }
	//时间流逝响应
	afx_msg LRESULT OnOperationDo(WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//设置范围
	void SetRange(vector<UINT>& vElapsed);
	//设置位置
	void SetPos(int nPos) { if(m_dRatio == 0) return; m_CurrentPos += (UINT)(nPos / m_dRatio); }
	//设置关联窗口
	void SetBuddyWnd(CWnd* pWnd){ m_pParent = pWnd; }

	//构造函数私有
private:
	CDlgRecordPlayZJG(CWnd* pParent = NULL);   // 标准构造函数
	CDlgRecordPlayZJG(const CDlgRecordPlayZJG&);
	CDlgRecordPlayZJG& operator=(const CDlgRecordPlayZJG&);

	//静态函数
public:
	//获取实例
	static CDlgRecordPlayZJG& GetInstance();

private:
	//资源
	CDC								m_dcMem;					//缓冲DC
	CWnd*							m_pParent;					//父窗口
	CPngImage						m_ImageBack;				//背景图片
	CPngImage						m_ImageSliderBack;			//滑动条背景
	CPngImage						m_ImageSlider;				//滑动条
	CPngImage						m_ImageShoe;				//滑动块

	//按钮
	CSkinButton						m_btRecordMin;				//最小化按钮
	CSkinButton						m_btRecordClose;			//关闭按钮
	CSkinButton						m_btRecordPlay;				//播放按钮
	CSkinButton						m_btRecordPause;			//暂停按钮
	CSkinButton						m_btRecordOpen;				//打开按钮

	//滑动数据
	vector<UINT>					m_vElapsed;					//总时间
	UINT							m_SliderLength;				//滑动条总长度
	UINT							m_SliderHeight;				//滑动条高度
	UINT							m_CurrentPos;				//当前位置(滑动块位置)
	UINT							m_AlreadyCount;				//已运行操作
	BOOL							m_IsSelect;					//是否选中滑块
	CPoint							m_DownPt;					//按下的点
	double							m_dRatio;					//比率（数值范围/总长度）
};
