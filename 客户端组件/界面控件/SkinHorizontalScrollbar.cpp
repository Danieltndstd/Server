#include "Stdafx.h"
#include "memdc.h"
#include "Resource.h"
#include "SkinHorizontalScrollbar.h"

#define IDI_HORIZONTAL_ARROW			100	//点击箭头
#define IDI_HORIZONTAL_SCROLL			101	//开始滚动

#define HORIZONTAL_THUMB_WIDTH	128	//滚动块宽度
#define HORIZONTAL_WIDTH					32	//宽度
#define HORIZONTAL_HEIGHT					17	//高度

CSkinHorizontalScrollbar::CSkinHorizontalScrollbar()
{
	m_nThumbLeft = HORIZONTAL_WIDTH - 1;
	m_dbThumbRemainder = 0.0f;

	m_bMouseDown = false;
	m_bMouseDownArrowLeft = false;
	m_bMouseDownArrowRight = false;
	m_bDragging = false;
	m_list = nullptr;
}

CSkinHorizontalScrollbar::~CSkinHorizontalScrollbar()
{

}

void CSkinHorizontalScrollbar::ScrollLeft()
{
	m_list->SendMessage(WM_HSCROLL, MAKELONG(SB_LINELEFT, 0), 0);

	UpdateThumbPosition();
}

void CSkinHorizontalScrollbar::ScrollRight()
{
	m_list->SendMessage(WM_HSCROLL, MAKELONG(SB_LINERIGHT, 0), 0);

	UpdateThumbPosition();
}

void CSkinHorizontalScrollbar::PageLeft()
{
	m_list->SendMessage(WM_HSCROLL, MAKELONG(SB_PAGEUP, 0), 0);

	UpdateThumbPosition();
}

void CSkinHorizontalScrollbar::PageRight()
{
	m_list->SendMessage(WM_HSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0);

	UpdateThumbPosition();
}

void CSkinHorizontalScrollbar::UpdateThumbPosition()
{
	CRect rect;
	GetClientRect(&rect);

	auto dbpos = (double)m_list->GetScrollPos(SB_HORZ);
	auto dbmax = (double)m_list->GetScrollLimit(SB_HORZ);
	auto nwidth = rect.Width() - (HORIZONTAL_WIDTH * 2 + HORIZONTAL_THUMB_WIDTH - 3);
	auto dbvar = dbmax;

	m_dbThumbInterval = nwidth / dbvar;

	auto newdbvalue = m_dbThumbInterval * dbpos;
	auto nnewvalue = (int)newdbvalue;
	auto nextra = newdbvalue - nnewvalue;
	m_dbThumbRemainder = nextra;

	m_nThumbLeft = nnewvalue + (HORIZONTAL_WIDTH - 1);

	_LimitThumbPosition();

	_Draw();
}

BEGIN_MESSAGE_MAP(CSkinHorizontalScrollbar, CStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int CSkinHorizontalScrollbar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	auto hResInstance = GetModuleHandle(SKIN_CONTROL_DLL_NAME);

	m_pngBack.LoadImage(hResInstance, TEXT("HORIZONTAL_SPAN"));
	m_pngLeftArrow.LoadImage(hResInstance, TEXT("HORIZONTAL_LEFT_ARROW"));
	m_pngThumb.LoadImage(hResInstance, TEXT("HORIZONTAL_THUMB"));
	m_pngRightArrow.LoadImage(hResInstance, TEXT("HORIZONTAL_RIGHT_ARROW"));

	return __super::OnCreate(lpCreateStruct);
}

void	CSkinHorizontalScrollbar::OnPaint()
{
	CPaintDC dc(this);

	_Draw();
}

void	CSkinHorizontalScrollbar::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	CRect rect;
	GetClientRect(&rect);

	auto width = rect.Width() - HORIZONTAL_WIDTH;

	CRect rectLeftArrow(0, 0, HORIZONTAL_WIDTH, HORIZONTAL_HEIGHT);															//左箭头尺寸
	CRect rectRightArrow(width, 0, width + HORIZONTAL_WIDTH, HORIZONTAL_HEIGHT);										//右箭头尺寸
	CRect rectThumb(m_nThumbLeft, 0, m_nThumbLeft + HORIZONTAL_THUMB_WIDTH, HORIZONTAL_HEIGHT);	//滚动条尺寸

	if (rectThumb.PtInRect(point))
		m_bMouseDown = true;

	if (rectRightArrow.PtInRect(point))
	{
		m_bMouseDownArrowRight = true;
		SetTimer(IDI_HORIZONTAL_ARROW, 250, nullptr);
	}

	if (rectRightArrow.PtInRect(point))
	{
		m_bMouseDownArrowLeft = true;
		SetTimer(IDI_HORIZONTAL_ARROW, 250, nullptr);
	}

	__super::OnLButtonDown(nFlags, point);
}

void	CSkinHorizontalScrollbar::OnLButtonUp(UINT nFlags, CPoint point)
{
	UpdateThumbPosition();
	KillTimer(IDI_HORIZONTAL_SCROLL);
	ReleaseCapture();

	CRect rect;
	GetClientRect(&rect);

	auto bInChannel = true;
	auto width = rect.Width() - HORIZONTAL_WIDTH;

	CRect rectLeftArrow(0, 0, HORIZONTAL_WIDTH, HORIZONTAL_HEIGHT);															//左箭头尺寸
	CRect rectRightArrow(width, 0, width + HORIZONTAL_WIDTH, HORIZONTAL_HEIGHT);										//右箭头尺寸
	CRect rectThumb(m_nThumbLeft, 0, m_nThumbLeft + HORIZONTAL_THUMB_WIDTH, HORIZONTAL_HEIGHT);	//滚动条尺寸

	if (rectLeftArrow.PtInRect(point))
	{
		ScrollLeft();
		bInChannel = false;
	}

	if (rectRightArrow.PtInRect(point))
	{
		ScrollRight();
		bInChannel = false;
	}

	if (rectThumb.PtInRect(point))
		bInChannel = false;

	if (bInChannel && !m_bMouseDown)
	{
		if (point.x > m_nThumbLeft)
			PageRight();
		else
			PageLeft();
	}

	//重置所有变量
	m_bMouseDown = false;
	m_bDragging = false;
	m_bMouseDownArrowLeft = false;
	m_bMouseDownArrowRight = false;

	__super::OnLButtonUp(nFlags, point);
}

void	CSkinHorizontalScrollbar::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	m_bDragging = m_bMouseDown;

	if (m_bDragging)
	{
		m_nThumbLeft = point.x - HORIZONTAL_THUMB_WIDTH / 2;

		auto dbmax = (double)m_list->GetScrollLimit(SB_HORZ);
		auto nPos = m_list->GetScrollPos(SB_HORZ);
		auto nwidth = rect.Width() - (HORIZONTAL_WIDTH * 2 + HORIZONTAL_THUMB_WIDTH - 3);
		auto dbvar = dbmax;

		m_dbThumbInterval = nwidth / dbvar;

		auto nScrollTimes = (int)((m_nThumbLeft - (HORIZONTAL_WIDTH - 1)) / m_dbThumbInterval) - nPos;

		CSize size;
		size.cx = nScrollTimes;
		size.cy = 0;

		m_list->Scroll(size);

		_LimitThumbPosition();

		_Draw();
	}

	__super::OnMouseMove(nFlags, point);
}

void	CSkinHorizontalScrollbar::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_HORIZONTAL_ARROW:
	{
		if (m_bMouseDownArrowRight || m_bMouseDownArrowLeft)
		{
			KillTimer(IDI_HORIZONTAL_ARROW);
			SetTimer(IDI_HORIZONTAL_SCROLL, 50, nullptr);
		}

		break;
	}
	case IDI_HORIZONTAL_SCROLL:
	{
		if (m_bMouseDownArrowRight)
			ScrollRight();

		if (m_bMouseDownArrowLeft)
			ScrollLeft();
	}
	}

	__super::OnTimer(nIDEvent);
}

void	CSkinHorizontalScrollbar::_Draw()
{
	CClientDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CMemDCEx memDC(&dc, rect);
	memDC->FillSolidRect(&rect, RGB(2, 27, 36));

	//滚动条背景
	m_pngBack.DrawImage(&memDC, rect.left, rect.top, rect.Width(), HORIZONTAL_HEIGHT, 0, 0, m_pngBack.GetWidth(), m_pngBack.GetHeight());

	//左箭头
	m_pngLeftArrow.DrawImage(&memDC, rect.left, rect.top);

	//滚动滑块
	m_pngThumb.DrawImage(&memDC, rect.left + m_nThumbLeft, rect.top);
	
	//右箭头
	m_pngRightArrow.DrawImage(&memDC, rect.Width() - HORIZONTAL_WIDTH, rect.top);
}

void	CSkinHorizontalScrollbar::_LimitThumbPosition()
{
	CRect rect;
	GetClientRect(&rect);

 	if (m_nThumbLeft + HORIZONTAL_THUMB_WIDTH > (rect.Width() - HORIZONTAL_WIDTH))
 		m_nThumbLeft = rect.Width() - (HORIZONTAL_WIDTH + HORIZONTAL_THUMB_WIDTH - 1);

 	if (m_nThumbLeft < (rect.left + (HORIZONTAL_WIDTH - 1)))
 		m_nThumbLeft = rect.left + (HORIZONTAL_WIDTH - 1);
}