#pragma once

#include "SkinControlHead.h"

class CSkinHorizontalScrollbar : public CStatic
{
	friend class CSkinListCtrlEx;

	//函数定义
public:
	//构造函数
	CSkinHorizontalScrollbar();
	//析构函数
	virtual ~CSkinHorizontalScrollbar();

	//位移函数
public:
	//往左滚动
	void ScrollLeft();
	//往右滚动
	void ScrollRight();
	//到头
	void	PageLeft();
	//到尾
	void	PageRight();
	//更新位置
	void UpdateThumbPosition();

	//消息定义函数
protected:
	//创建函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//重绘函数
	afx_msg void OnPaint();
	//左键点击
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//左键放开
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//定时器
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

	//私有函数
private:
	//自绘函数
	void	_Draw();
	//限制位置
	void	_LimitThumbPosition();

private:
	CListCtrl*		m_list;		//隶属的List

	bool				m_bMouseDownArrowRight, m_bMouseDownArrowLeft;
	bool				m_bDragging;
	bool				m_bMouseDown;

	int				m_nThumbLeft;	//从何处开始画
	double			m_dbThumbRemainder;
	double			m_dbThumbInterval;

	CPngImage	m_pngBack;				//背景
	CPngImage	m_pngLeftArrow;		//左箭头
	CPngImage	m_pngThumb;			//滚动滑块
	CPngImage	m_pngRightArrow;	//右箭头
};