#pragma once

class CSkinVerticleScrollbar : public CStatic
{
	friend class CSkinListCtrlEx;

	//函数定义
public:
	//构造函数
	CSkinVerticleScrollbar();
	//析构函数
	virtual ~CSkinVerticleScrollbar();

	//位移函数
public:
	//往上滚动
	void ScrollUp();
	//往下滚动
	void ScrollDown();
	//到头
	void	PageUp();
	//到尾
	void	PageDown();
	//更新位置
	void UpdateThumbPosition();

	//消息定义函数
protected:
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
	CListCtrl*	m_list;		//隶属的List

	bool			m_bMouseDownArrowUp, m_bMouseDownArrowDown;
	bool			m_bDragging;
	bool			m_bMouseDown;

	int			m_nThumbTop;		//从何处开始画
	double		m_dbThumbInterval;
};