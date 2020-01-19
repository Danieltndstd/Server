#include "Stdafx.h"
#include "memdc.h"
#include "Resource.h"
#include "SkinVerticleScrollbar.h"

#define IDI_VERTICLE_ARROW	102	//点击箭头
#define IDI_VERTICLE_SCROLL	103	//开始滚动

#define VERTICLE_WIDTH			15	//宽度
#define VERTICLE_HEIGHT		32	//宽度

CSkinVerticleScrollbar::CSkinVerticleScrollbar()
{
	m_nThumbTop = 11 + VERTICLE_HEIGHT - 1;
	m_dbThumbInterval = 0.0;

	m_bMouseDown = false;
	m_bMouseDownArrowUp = false;
	m_bMouseDownArrowDown= false;
	m_bDragging = false;
	m_list = nullptr;
}

CSkinVerticleScrollbar::~CSkinVerticleScrollbar()
{

}

void CSkinVerticleScrollbar::ScrollUp()
{
	m_list->SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP, 0), 0);

	UpdateThumbPosition();
}

void CSkinVerticleScrollbar::ScrollDown()
{
	m_list->SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN, 0), 0);

	UpdateThumbPosition();
}

void CSkinVerticleScrollbar::PageUp()
{
	m_list->SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP, 0), 0);

	UpdateThumbPosition();
}

void CSkinVerticleScrollbar::PageDown()
{
	m_list->SendMessage(WM_HSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0);

	UpdateThumbPosition();
}

void CSkinVerticleScrollbar::UpdateThumbPosition()
{
	CRect rect;
	GetClientRect(&rect);

	auto dbpos = (double)m_list->GetScrollPos(SB_VERT);
	auto dbmax = (double)m_list->GetScrollLimit(SB_VERT);
	auto nheight = rect.Height() - (VERTICLE_HEIGHT * 3 + 20);
	auto dbvar = dbmax;

	m_dbThumbInterval = nheight / dbvar;

	auto newdbvalue = m_dbThumbInterval * dbpos;
	auto nnewvalue = (int)newdbvalue;

	m_nThumbTop = nnewvalue + 11 + VERTICLE_HEIGHT - 1;

	_LimitThumbPosition();

	_Draw();
}

BEGIN_MESSAGE_MAP(CSkinVerticleScrollbar, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void	CSkinVerticleScrollbar::OnPaint()
{
	CPaintDC dc(this);

	_Draw();
}

void	CSkinVerticleScrollbar::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	CRect rect;
	GetClientRect(&rect);

	auto height = rect.Height() - (11 + VERTICLE_HEIGHT - 1);	//标志滚动条高度

	CRect rectUpArrow(0, 11, 12, 11 + VERTICLE_HEIGHT);											//上箭头尺寸
	CRect rectDownArrow(0, height, VERTICLE_WIDTH, height + VERTICLE_HEIGHT);		//下箭头尺寸
	CRect rectThumb(0, m_nThumbTop, VERTICLE_WIDTH, m_nThumbTop + VERTICLE_HEIGHT);		//滚动条尺寸

	if (rectThumb.PtInRect(point))
		m_bMouseDown = true;

	if (rectDownArrow.PtInRect(point))
	{
		m_bMouseDownArrowDown = true;
		SetTimer(IDI_VERTICLE_ARROW, 250, nullptr);
	}

	if (rectUpArrow.PtInRect(point))
	{
		m_bMouseDownArrowUp = true;
		SetTimer(IDI_VERTICLE_ARROW, 250, nullptr);
	}

	__super::OnLButtonDown(nFlags, point);
}

void	CSkinVerticleScrollbar::OnLButtonUp(UINT nFlags, CPoint point)
{
	UpdateThumbPosition();
	KillTimer(IDI_VERTICLE_SCROLL);
	ReleaseCapture();

	CRect rect;
	GetClientRect(&rect);

	auto bInChannel = true;
	auto height = rect.Height() - (11 + VERTICLE_HEIGHT - 1);

	CRect rectUpArrow(0, 0, VERTICLE_WIDTH, VERTICLE_HEIGHT);								//上箭头尺寸
	CRect rectDownArrow(0, height, VERTICLE_WIDTH, height + VERTICLE_HEIGHT);		//下箭头尺寸
	CRect rectThumb(0, m_nThumbTop, VERTICLE_WIDTH, m_nThumbTop + VERTICLE_HEIGHT);		//滚动条尺寸

	if (rectUpArrow.PtInRect(point) && m_bMouseDownArrowUp)
	{
		ScrollUp();
		bInChannel = false;
	}

	if (rectDownArrow.PtInRect(point) && m_bMouseDownArrowDown)
	{
		ScrollDown();
		bInChannel = false;
	}

	if (rectThumb.PtInRect(point))
		bInChannel = false;

	if (bInChannel && !m_bMouseDown)
	{
		if (point.y > m_nThumbTop)
			PageDown();
		else
			PageUp();
	}

	//重置所有变量
	m_bMouseDown = false;
	m_bDragging = false;
	m_bMouseDownArrowUp = false;
	m_bMouseDownArrowDown = false;

	__super::OnLButtonUp(nFlags, point);
}

void	CSkinVerticleScrollbar::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	if (m_bMouseDown)
	{
		m_nThumbTop = point.y - VERTICLE_HEIGHT / 2;

		auto dbmax = (double)m_list->GetScrollLimit(SB_VERT);
		auto nPos = m_list->GetScrollPos(SB_VERT);
		auto nheight = rect.Height() - (VERTICLE_HEIGHT * 3 + 20);
		auto dbvar = dbmax;

		m_dbThumbInterval = nheight / dbvar;

		auto nScrollTimes = (int)((m_nThumbTop - (11 + VERTICLE_HEIGHT - 1)) / m_dbThumbInterval) - nPos;

		CRect itemrect;
		m_list->GetItemRect(0, &itemrect, LVIR_BOUNDS);

		CSize size;
		size.cx = 0;
		size.cy = nScrollTimes * itemrect.Height();

		m_list->Scroll(size);

		_LimitThumbPosition();

		_Draw();
	}

	__super::OnMouseMove(nFlags, point);
}

void	CSkinVerticleScrollbar::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_VERTICLE_ARROW:
	{
		if (m_bMouseDownArrowUp || m_bMouseDownArrowDown)
		{
			KillTimer(IDI_VERTICLE_ARROW);
			SetTimer(IDI_VERTICLE_SCROLL, 50, nullptr);
		}

		break;
	}
	case IDI_VERTICLE_SCROLL:
	{
		if (m_bMouseDownArrowDown)
			ScrollDown();

		if (m_bMouseDownArrowUp)
			ScrollUp();
	}
	}

	__super::OnTimer(nIDEvent);
}

void	CSkinVerticleScrollbar::_Draw()
{
	CClientDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CMemDCEx memDC(&dc, rect);
	memDC->FillSolidRect(&rect, RGB(1, 15, 26));

	CDC	bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);

	//上面的装饰块
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_VERTICLE_SCROLLBAR_TOP);
	auto oldBitmap = bitmapDC.SelectObject(&bitmap);

	memDC->BitBlt(rect.left, rect.top, 11, 11, &bitmapDC, 0, 0, SRCCOPY);

	bitmapDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	oldBitmap = nullptr;

	//上箭头
	bitmap.LoadBitmap(IDB_VERTICLE_SCROLLBAR_UPARROW);
	oldBitmap = bitmapDC.SelectObject(&bitmap);

	memDC->BitBlt(rect.left , rect.top + 11, VERTICLE_WIDTH, VERTICLE_HEIGHT, &bitmapDC, 0, 0, SRCCOPY);

	bitmapDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	oldBitmap = nullptr;

	//滚动条背景
	bitmap.LoadBitmap(IDB_VERTICLE_SCROLLBAR_SPAN);
	oldBitmap = bitmapDC.SelectObject(&bitmap);

	auto height = rect.Height() - (11 + VERTICLE_HEIGHT);

	memDC->StretchBlt(rect.left, rect.top + (11 + VERTICLE_HEIGHT), VERTICLE_WIDTH, height, &bitmapDC, 0, 0, 12, 1, SRCCOPY);

	bitmapDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	oldBitmap = nullptr;

	//下箭头
	bitmap.LoadBitmap(IDB_VERTICLE_SCROLLBAR_DOWNARROW);
	oldBitmap = bitmapDC.SelectObject(&bitmap);

	memDC->BitBlt(rect.left, height, VERTICLE_WIDTH, VERTICLE_HEIGHT, &bitmapDC, 0, 0, SRCCOPY);

	bitmapDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	oldBitmap = nullptr;

	//下面的装饰块
	bitmap.LoadBitmap(IDB_VERTICLE_SCROLLBAR_BOTTOM);
	oldBitmap = bitmapDC.SelectObject(&bitmap);

	memDC->BitBlt(rect.left + 1, height + VERTICLE_HEIGHT, 11, 11, &bitmapDC, 0, 0, SRCCOPY);

	bitmapDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	oldBitmap = nullptr;

	//滚动滑块
	if (m_list->GetScrollLimit(SB_VERT) != 0)
	{
		bitmap.LoadBitmap(IDB_VERTICLE_SCROLLBAR_THUMB);
		oldBitmap = bitmapDC.SelectObject(&bitmap);

		memDC->BitBlt(rect.left, rect.top + m_nThumbTop, VERTICLE_WIDTH, VERTICLE_HEIGHT, &bitmapDC, 0, 0, SRCCOPY);

		bitmapDC.SelectObject(oldBitmap);
		bitmap.DeleteObject();
		oldBitmap = nullptr;
	}
}

void	CSkinVerticleScrollbar::_LimitThumbPosition()
{
	CRect rect;
	GetClientRect(&rect);

	if (m_nThumbTop + VERTICLE_HEIGHT > (rect.Height() - (11 + VERTICLE_HEIGHT)))
		m_nThumbTop = rect.Height() - (11 + VERTICLE_HEIGHT * 2 - 1);

	if (m_nThumbTop < (rect.top + VERTICLE_HEIGHT - 1 + 11))
		m_nThumbTop = rect.top + VERTICLE_HEIGHT - 1 + 11;
}