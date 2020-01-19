#pragma once

#include "SkinHeaderCtrl.h"
#include "SkinControlHead.h"
#include "SkinVerticleScrollbar.h"
#include "SkinHorizontalScrollbar.h"

class SKIN_CONTROL_CLASS CSkinListCtrlEx : public CListCtrl
{
	//函数定义
public:
	//构造函数
	CSkinListCtrlEx();
	//析构函数
	virtual ~CSkinListCtrlEx();

	//初始化函数
	void	Init();
	//放置滚动条
	void PositionScrollBars();

	//插入函数
public:
	//插入数据
	void InsertDataItem(void* pItemData);
	//更新数据
	void UpdateDataItem(void* pItemData);
	//删除数据
	void DeleteDataItem(void* pItemData);

	//数据控制（子类可重写）
protected:
	//获取位置
	virtual int GetInsertIndex(void* pItemData);
	//排列数据
	virtual int SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	//描述字符
	virtual void ConstructString(void* pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);
	//子类绘制
	virtual void DrawCustomItem(CDC* pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect& rcSubItem, INT nColumnIndex);
	//子类重载颜色
	virtual void GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF& crColorText, COLORREF& crColorBack);

	//函数重载
protected:
	//子类化
	void	PreSubclassWindow();
	//绘画函数
	void	DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct);

	//消息定义函数
protected:
	//计算尺寸时调用
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	//滚动调用
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//按键响应
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//按键响应
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//重绘背景
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//重绘函数
	afx_msg void OnPaint();

	//自绘函数
	afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	//私有函数
private:
	//行选中？
	bool	_IsRowSelected(HWND hwnd, int row);
	//行高亮？
	bool	_IsRowHighlighted(HWND hwnd, int row);
	//高亮
	void	_EnableRowHighlighted(HWND hwnd, int row, bool bHighlight = true);

protected:
	CSkinHeaderCtrlEx			m_SkinHeaderCtrl;
	CSkinVerticleScrollbar		m_SkinVerticleScrollbar;
	CSkinHorizontalScrollbar	m_SkinHorizontalScrollbar;

	COLORREF						m_clrFgHi;
	COLORREF						m_clrBgHi;

	bool									m_bAscendSort;			//升序标志
};