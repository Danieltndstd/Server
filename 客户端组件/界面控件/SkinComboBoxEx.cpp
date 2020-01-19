#include "Stdafx.h"
#include "memdc.h"
#include "Resource.h"
#include "SkinComboBoxEx.h"

CSkinComboBoxEx::CSkinComboBoxEx()
{
	m_bReadOnly = FALSE;
	m_clrEditText = RGB(255, 255, 255);
	m_clrListitText = RGB(0, 0, 0);
	m_strEditFontName = TEXT("宋体");
	m_strListFontName = TEXT("宋体");
	m_nEditFontPtSize = 100;
	m_nListFontPtSize = 100;

	m_pListBox = nullptr;
	m_pEditFont = nullptr;
	m_pListFont = nullptr;

	m_nPreSelItem = -1;
	m_nSelItem = -1;
}

CSkinComboBoxEx::~CSkinComboBoxEx()
{
	if (m_pEditFont)
	{
		delete m_pEditFont;
		m_pEditFont = nullptr;
	}

	if (m_pListFont)
	{
		delete m_pListFont;
		m_pListFont = nullptr;
	}
}

void CSkinComboBoxEx::PreSubclassWindow()
{
	//读取资源
	auto hResInstance = GetModuleHandle(SKIN_CONTROL_DLL_NAME);
	m_pngComboBack.LoadImage(hResInstance, TEXT("SKIN_COMBO_BACK"));
	m_bmpButton.LoadBitmap(IDB_COMBO_BUTTON);

	//默认使用系统一样的字体
	LOGFONT lg;
	auto pFont = GetFont();
	pFont->GetLogFont(&lg);

	//m_nEditFontPtSize = lg.lfHeight * 12;
	m_strEditFontName = lg.lfFaceName;

	//m_nListFontPtSize = m_nEditFontPtSize;
	m_strListFontName = m_strListFontName;

	//创建字体
	//_CreateEditFont();
	_CreateListFont();

	//自适应宽度
	_AdjustDroppedWidth();

	__super::PreSubclassWindow();
}

void CSkinComboBoxEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 50;
	lpMeasureItemStruct->itemWidth = 400;
}

void CSkinComboBoxEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType != ODT_COMBOBOX)
		return;

	CRect rcItem = lpDrawItemStruct->rcItem;
	auto uItemID = lpDrawItemStruct->itemID;
	auto uItemState = lpDrawItemStruct->itemState;
	auto pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//绘制下拉列表
	_DrawContent(pDC, uItemID, uItemState, rcItem);
}

LRESULT CSkinComboBoxEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_CTLCOLORLISTBOX == message)
	{
		auto hWndListBox = (HWND)lParam;
		auto pListBox = (CListBox*)FromHandle(hWndListBox);
		auto nItemCount = pListBox->GetCount();

		//保存指针
		m_pListBox = pListBox;

		if (CB_ERR != nItemCount)
		{
			//转换屏幕鼠标位置到客户区位置
			CPoint pt;
			GetCursorPos(&pt);
			pListBox->ScreenToClient(&pt);

			CRect rcItem;
			for (auto i = 0; i < nItemCount; i++)
			{
				pListBox->GetItemRect(i, &rcItem);
				if (rcItem.PtInRect(pt))
				{
					m_nPreSelItem = m_nSelItem;
					m_nSelItem = i;

					if (m_nSelItem != m_nPreSelItem)
					{
						CRect rcPreItem;
						CRect rcFocus(rcItem);

						//刷新之前区域
						pListBox->GetItemRect(m_nPreSelItem, &rcPreItem);
						pListBox->InvalidateRect(&rcPreItem);

						//刷新当前区域
						pListBox->InvalidateRect(&rcItem);

						break;
					}
				}
			}
		}
	}

	return __super::WindowProc(message, wParam, lParam);
}

BEGIN_MESSAGE_MAP(CSkinComboBoxEx, CComboBox)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &CSkinComboBoxEx::OnCbnSelChange)
END_MESSAGE_MAP()

void CSkinComboBoxEx::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CMemDCEx memDC(&dc, rect);

	_DrawBack(&memDC, rect);
}

HBRUSH CSkinComboBoxEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	auto hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CFont font;
		font.CreatePointFont(m_nEditFontPtSize, m_strEditFontName);

		pDC->SelectObject(font);
		pDC->SetTextColor(m_clrEditText);
		pDC->SetBkMode(TRANSPARENT);

		return CreateSolidBrush(RGB(255, 255, 255));
	}
	else if (nCtlColor == CTLCOLOR_EDIT)
	{
		//获取编辑框
		auto edit = reinterpret_cast<CEdit*>(pWnd);
		edit->SetWindowPos(nullptr, 2, 4, 70, 20, SWP_NOSIZE);
		edit->SetLimitText(256);  

		CFont font;
		font.CreatePointFont(m_nEditFontPtSize, m_strEditFontName);

		pDC->SelectObject(font);
		pDC->SetTextColor(m_clrEditText);
		pDC->SetBkMode(TRANSPARENT);

		return CreateSolidBrush(RGB(2, 21, 27));
	}
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		CFont font;
		font.CreatePointFont(m_nListFontPtSize, m_strListFontName);

		pDC->SelectObject(font);
		pDC->SetTextColor(m_clrListitText);
		pDC->SetBkMode(TRANSPARENT);

		return CreateSolidBrush(RGB(255, 255, 255));
	}
	else if (nCtlColor == CTLCOLOR_BTN)
	{
		OutputDebugString(TEXT("按钮消息"));
	}

	return hbr;
}

void CSkinComboBoxEx::OnCbnSelChange()
{

}

void CSkinComboBoxEx::_DrawBack(CDC* pDC, CRect& rc)
{
	//绘制背景与边框
	CBrush brushInner, brushOuter;
	brushInner.CreateSolidBrush(RGB(2, 21, 27));
	brushOuter.CreateSolidBrush(RGB(149, 99, 18));

	//暂时不用，使用背景图
// pDC->FillRect(rc, &brushInner);
// pDC->FrameRect(rc, &brushOuter); 

	m_pngComboBack.DrawImage(pDC, 0, 0);

	COMBOBOXINFO comboInfo;
	comboInfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboInfo);

	//绘制按钮
	_DrawButton(pDC, comboInfo.rcButton);
}

void CSkinComboBoxEx::_CreateEditFont()
{
	LOGFONT lg = { 0 };
	lg.lfCharSet = DEFAULT_CHARSET;
	lg.lfHeight = m_nEditFontPtSize;
	lg.lfPitchAndFamily = 2;
	lg.lfWeight = FW_NORMAL;
	_tcscpy_s(lg.lfFaceName, m_strEditFontName);

	//创建字体
	m_pEditFont = new CFont;
	m_pEditFont->CreatePointFontIndirect(&lg);

	//设置字体
	SetFont(m_pEditFont);
}

void CSkinComboBoxEx::_CreateListFont()
{
	LOGFONT lg = { 0 };
	lg.lfCharSet = DEFAULT_CHARSET;
	lg.lfHeight = m_nListFontPtSize;
	lg.lfPitchAndFamily = 2;
	lg.lfWeight = FW_NORMAL;
	_tcscpy_s(lg.lfFaceName, m_strListFontName);

	//创建字体
	m_pListFont = new CFont;
	m_pListFont->CreatePointFontIndirect(&lg);
}

void CSkinComboBoxEx::_DrawContent(CDC* pDC, UINT uItemID, UINT uItemState, CRect& rcItem)
{
	CString	strShowText;
	CBrush bgItemBrush, listBoxBrush;
	CRect rcListBox;
	CPen itemPen, bgItemPen;
	pDC->SetBkMode(TRANSPARENT);

	CBrush bt;
	bt.CreateSolidBrush(RGB(10, 110, 110));

	CRect rcc = rcItem;
	rcc.DeflateRect(1, 0, 1, 0);
	pDC->FillRect(rcc, &bt);

	//鼠标所在行高亮
	if (uItemID == m_nSelItem)
	{
		//创建画笔并选入
		bgItemPen.CreatePen(PS_SOLID, 1, RGB(222, 234, 198));
		auto oldPen = pDC->SelectObject(&bgItemPen);

		//创建画刷并选入
		bgItemBrush.CreateSolidBrush(RGB(222, 234, 198));
		auto oldBrush = pDC->SelectObject(&bgItemBrush);

		pDC->RoundRect(&rcItem, CPoint(0, 0));

		//恢复并删除设备
		pDC->SelectObject(oldBrush);
		pDC->SelectObject(oldPen);
		bgItemBrush.DeleteObject();
		bgItemPen.DeleteObject();

		//显示高亮文本
		GetLBText(uItemID, strShowText);
		SetWindowText(strShowText);
	}

	//绘制列表框
	listBoxBrush.CreateSolidBrush(RGB(254, 230, 202));
	m_pListBox->GetClientRect(&rcListBox);
	pDC->FrameRect(rcListBox, &listBoxBrush);

	//绘制项文本
	pDC->SetTextColor(m_clrListitText);
	auto oldFpnt = pDC->SelectObject(m_pListFont);
	GetLBText(uItemID, strShowText);
	rcItem.left += 3;
	pDC->DrawText(strShowText, &rcItem, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
}

void CSkinComboBoxEx::_DrawButton(CDC* pDC, CRect rcButton)
{
	//获取位图信息
	BITMAP bitmapBtn;
	m_bmpButton.GetBitmap(&bitmapBtn); 

	//内存DC
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&m_bmpButton);

	// 根据不同的状态读取图片的不同部分，本例中只有两种
	/*if (m_bBtnDown || m_bBtnHover)
		pDC->BitBlt(rcButton.left, rcButton.top + nTop, rcButton.Width() / 2, rcButton.Height(), pMemDC, rcButton.Width() / 2 + 1, 0, SRCCOPY);
	else*/
		pDC->BitBlt(rcButton.left - 2, rcButton.top , rcButton.Width() + 1, rcButton.Height(), &memDC, 0, 0, SRCCOPY);
}

void CSkinComboBoxEx::_AdjustDroppedWidth()
{
	//计算下拉列表最长宽度
	CString strItem;
	CSize sz;

	auto nWidth = GetDroppedWidth();
	auto pDC = GetDC();
	auto oldFont = pDC->SelectObject(m_pListFont);

	for (auto i = 0; i < GetCount(); i++)
	{
		GetLBText(i, strItem);
		sz = pDC->GetTextExtent(strItem);
		if (sz.cx > nWidth)
			nWidth = sz.cx;
	}

	//设置下拉框宽度
	SetDroppedWidth(nWidth);

	//恢复设备
	pDC->SelectObject(oldFont);
	ReleaseDC(pDC);
}


BOOL CSkinComboBoxEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message  == WM_KEYDOWN)
	{
		return TRUE;
	}

	return CComboBox::PreTranslateMessage(pMsg);
}
