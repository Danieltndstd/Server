#pragma once

#include "SkinControlHead.h"

class SKIN_CONTROL_CLASS CSkinComboBoxEx : public CComboBox
{
	//函数定义
public:
	//构造函数
	CSkinComboBoxEx();
	//析构函数
	virtual ~CSkinComboBoxEx();

	//重载函数
public:
	//子类化
	virtual void PreSubclassWindow();
	//重置尺寸
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//自绘子项
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//消息重载
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//只读属性
	__declspec(property(get = GetReadOnly, put = SetReadOnly)) BOOL bReadOnly;
	BOOL GetReadOnly(void) const { return m_bReadOnly; }
	void	SetReadOnly(BOOL value) { m_bReadOnly = value; }

	DECLARE_MESSAGE_MAP()

	//消息定义函数
protected:
	//重绘
	afx_msg void OnPaint();
	//子控件绘制
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//选中项改变
	afx_msg void OnCbnSelChange();

	//私有函数
private:
	//绘制
	void _DrawBack(CDC* pDC, CRect& rc);
	//创建组合框中编辑框字体
	void	_CreateEditFont();
	//创建组合框中下拉列表字体
	void	_CreateListFont();
	//绘制下拉列表
	void _DrawContent(CDC* pDC, UINT uItemID, UINT uItemState, CRect& rcItem);
	//绘制按钮
	void _DrawButton(CDC* pDC, CRect rcButton);
	//适应下拉宽度（根据字体与最长项）
	void _AdjustDroppedWidth();

private:
	BOOL			m_bReadOnly;				//只读属性

	COLORREF	m_clrEditText;				//编辑框字体颜色
	COLORREF	m_clrListitText;				//列表框字体颜色
	CString			m_strEditFontName;		//编辑框字体名字
	CString			m_strListFontName;		//列表框字体名字
	CFont*			m_pEditFont;				//编辑框字体
	CFont*			m_pListFont;					//列表框字体
	int				m_nEditFontPtSize;		//编辑框字体尺寸
	int				m_nListFontPtSize;		//列表框字体尺寸

	CListBox*		m_pListBox;					//列表框指针
	CPngImage	m_pngComboBack;		//背景
	CBitmap		m_bmpButton;				//按钮图片

	int				m_nPreSelItem;				//前一次选中项
	int				m_nSelItem;					//当前选中项
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};