#pragma once

// CDlgVolumnControl 对话框

#include "GameFrameHead.h"
#include "Resource.h"

class CDSoundManager;

class CDlgVolumnControl : public CDialog
{
	DECLARE_DYNAMIC(CDlgVolumnControl)

public:
	CDlgVolumnControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVolumnControl();

// 对话框数据
	enum { IDD = IDD_VOLUMN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//消息函数
private:
	//重绘背景
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//绘制背景
	afx_msg void OnPaint();
	//左键单击
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//左键放开
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//创建消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//确定
	afx_msg void OnBnClickedOk();
	//取消
	afx_msg void OnBnClickedCancel();

private:
	//资源
	CDC			m_dcMem;					//缓冲DC
	CPngImage	m_ImageBack;			//背景图片
	CPngImage	m_ImageSliderBack;	//滑动条背景
	CPngImage	m_ImageText1;			//文字1
	CPngImage	m_ImageText2;			//文字2
	CPngImage	m_ImageText3;			//文字3
	CPngImage	m_ImageSlider;			//滑动条
	CPngImage	m_ImageShoe;			//滑动块
	CPngImage	m_ImageCheck1;		//复选框1
	CPngImage	m_ImageCheck2;		//复选框2

	//按钮
	CSkinButton	m_btOk;				//确定
	CSkinButton	m_btCancel;			//取消
	CSkinButton	m_btClose;			//关闭

	//音量
	int			m_CurVolume;		//当前音量
	int			m_DefVolume;		//默认音量
	bool		m_bEffect;			//当前音效开关
	bool		m_bDefEffet;		//默认音效开关
	bool		m_bMusic;			//当前音乐开关
	bool		m_bDefMusic;		//默认音乐开关

	//位置信息
	Point	m_CurPos;		//当前位置
	Point	m_DefPos;		//默认位置
	BOOL	m_isSelect;		//选中滑块

	//音乐管理指针
	CDSoundManager*	m_pSoundMgr;		
};
