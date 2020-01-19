#pragma once

#include "SkinControlHead.h"

class SKIN_CONTROL_CLASS CSkinHeaderCtrlEx : public CHeaderCtrl
{
	//函数定义
public:
	//构造函数
	CSkinHeaderCtrlEx();
	//析构函数
	~CSkinHeaderCtrlEx();

	//功能函数
public:
	//设置锁定
	void	SetLockCount(UINT uLockCount);
	//设置列高
	void	SetItemHeight(UINT uItemHeight);

	//消息定义函数
protected:
	//重绘函数
	afx_msg void OnPaint();
	//重绘背景
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//LayOut消息
	afx_msg LRESULT OnLayout(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	//锁定列表
protected:
	UINT			m_uLockCount;		//锁定列数
	UINT			m_uItemHeight;		//子项高度

	CPngImage	m_pngHeadBack;		//背景
};