#include "Stdafx.h"
#include "memdc.h"
#include "SkinListCtrlEx.h"

CSkinListCtrlEx::CSkinListCtrlEx()
{
	m_clrFgHi = RGB(115, 123, 165);
	m_clrBgHi = RGB(2, 27, 36);

	m_bAscendSort = false;
}

CSkinListCtrlEx::~CSkinListCtrlEx()
{

}

void CSkinListCtrlEx::Init()
{
	InitializeFlatSB(m_hWnd);
	FlatSB_EnableScrollBar(m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);

	auto parent = GetParent();

	m_SkinVerticleScrollbar.Create(nullptr, WS_CHILD | SS_LEFT | SS_NOTIFY | WS_VISIBLE | WS_GROUP, CRect(0, 0, 0, 0), parent);
	m_SkinHorizontalScrollbar.Create(nullptr, WS_CHILD | SS_LEFT | SS_NOTIFY | WS_VISIBLE | WS_GROUP, CRect(0, 0, 0, 0), parent);
	m_SkinVerticleScrollbar.m_list = this;
	m_SkinHorizontalScrollbar.m_list = this;

	PositionScrollBars();

	ListView_SetExtendedListViewStyle(m_hWnd, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
}

void CSkinListCtrlEx::PositionScrollBars()
{
	auto parent = GetParent();

	CRect windowRect;
	GetWindowRect(&windowRect);

	auto ntitleBarHeight = 0;

	if (parent->GetStyle() & WS_CAPTION)
		ntitleBarHeight = GetSystemMetrics(SM_CYSIZE);

	auto nDialogFrameHeight = 0;
	auto nDialogFrameWidth = 0;
	if (parent->GetStyle() & WS_BORDER)
	{
		nDialogFrameHeight = GetSystemMetrics(SM_CYDLGFRAME);
		nDialogFrameWidth = GetSystemMetrics(SM_CYDLGFRAME);
	}

	if (parent->GetStyle() & WS_THICKFRAME)
	{
		nDialogFrameHeight += 1;
		nDialogFrameWidth += 1;
	}

	parent->ScreenToClient(&windowRect);

	windowRect.top += ntitleBarHeight + nDialogFrameHeight;
	windowRect.bottom += ntitleBarHeight + nDialogFrameHeight;
	windowRect.left += nDialogFrameWidth;
	windowRect.right += nDialogFrameWidth;

	CRect vBar(windowRect.right - nDialogFrameWidth, windowRect.top - ntitleBarHeight - nDialogFrameHeight, windowRect.right + 12 - nDialogFrameWidth, windowRect.bottom + 12 - ntitleBarHeight - nDialogFrameHeight);
	CRect hBar(windowRect.left - nDialogFrameWidth, windowRect.bottom - ntitleBarHeight - nDialogFrameHeight, windowRect.right + 1 - nDialogFrameWidth, windowRect.bottom + 16 - ntitleBarHeight - nDialogFrameHeight);

	if (GetScrollLimit(SB_VERT) != 0)
	{
		m_SkinVerticleScrollbar.SetWindowPos(nullptr, vBar.left, vBar.top, vBar.Width(), vBar.Height(), SWP_NOZORDER);
	}

	m_SkinHorizontalScrollbar.SetWindowPos(nullptr, hBar.left, hBar.top, hBar.Width(), hBar.Height(), SWP_NOZORDER);
	
	m_SkinVerticleScrollbar.UpdateThumbPosition();
	m_SkinHorizontalScrollbar.UpdateThumbPosition();
}

BEGIN_MESSAGE_MAP(CSkinListCtrlEx, CListCtrl)
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDrawList)
END_MESSAGE_MAP()

//��������
void	CSkinListCtrlEx::InsertDataItem(void* pItemData)
{
	//Ч��״̬
	ASSERT((pItemData != NULL) && (m_hWnd != NULL));
	if ((pItemData == NULL) || (m_hWnd == NULL)) return;

	//��������
	INT nItemInsert = 0;
	INT nColumnCount = m_SkinHeaderCtrl.GetItemCount();

	//��������
	for (INT i = 0; i < nColumnCount; i++)
	{
		//��ȡ����
		TCHAR szString[256] = TEXT("");
		ConstructString(pItemData, i, szString, CountArray(szString));

		//�����б�
		if (i == 0)
		{
			nItemInsert = InsertItem(LVIF_TEXT | LVIF_PARAM, GetInsertIndex(pItemData), szString, 0, 0, 0, (LPARAM)pItemData);
		}
		else
		{
			SetItem(nItemInsert, i, LVIF_TEXT, szString, 0, 0, 0, 0);
		}
	}
}

//��������
void	CSkinListCtrlEx::UpdateDataItem(void* pItemData)
{
	//Ч��״̬
	ASSERT((pItemData != NULL) && (m_hWnd != NULL));
	if ((pItemData == NULL) || (m_hWnd == NULL)) return;

	//��������
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo, sizeof(FindInfo));

	//���ñ���
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)pItemData;

	//��������
	INT nItem = FindItem(&FindInfo);
	INT nColumnCount = m_SkinHeaderCtrl.GetItemCount();

	//��������
	if (nItem != -1L)
	{
		//��������
		for (INT i = 0; i < nColumnCount; i++)
		{
			//��ȡ����
			TCHAR szString[256] = TEXT("");
			ConstructString(pItemData, i, szString, CountArray(szString));

			//�����б�
			SetItem(nItem, i, LVIF_TEXT, szString, 0, 0, 0, 0);
		}

		//��������
		RedrawItems(nItem, nItem);
	}
}

//ɾ������
void CSkinListCtrlEx::DeleteDataItem(void* pItemData)
{
	//Ч��״̬
	ASSERT((pItemData != NULL) && (m_hWnd != NULL));
	if ((pItemData == NULL) || (m_hWnd == NULL)) return;

	//��������
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo, sizeof(FindInfo));

	//�������
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)pItemData;

	//ɾ������
	INT nItem = FindItem(&FindInfo);
	if (nItem != -1L) DeleteItem(nItem);
}

//��ȡλ��
int CSkinListCtrlEx::GetInsertIndex(void* pItemData)
{
	return GetItemCount();
}

//��������
int CSkinListCtrlEx::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort)
{
	//��ȡ����
	TCHAR szBuffer1[256] = TEXT("");
	TCHAR szBuffer2[256] = TEXT("");
	GetItemText((INT)lParam1, wColumnIndex, szBuffer1, CountArray(szBuffer1));
	GetItemText((INT)lParam2, wColumnIndex, szBuffer2, CountArray(szBuffer2));

	//�Ա�����
	INT nResult = lstrcmp(szBuffer1, szBuffer2);
	return (bAscendSort == true) ? nResult : -nResult;
}

//�����ַ�
void	CSkinListCtrlEx::ConstructString(void* pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//����Ч��
	ASSERT(pszString != NULL);
	if (pszString == NULL) return;

	//���ñ���
	pszString[0] = 0;
}

void CSkinListCtrlEx::DrawCustomItem(CDC* pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect& rcSubItem, INT nColumnIndex)
{
	//��ȡ����
	TCHAR szString[256] = TEXT("");
	GetItemText(lpDrawItemStruct->itemID, nColumnIndex, szString, CountArray(szString));

	//�滭����
	rcSubItem.left += 5;
	pDC->DrawText(szString, lstrlen(szString), &rcSubItem, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

void CSkinListCtrlEx::GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF& crColorText, COLORREF& crColorBack)
{
	//������ɫ
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		//ѡ����ɫ
		crColorText = RGB(217, 206, 142);
		crColorBack = RGB(212, 208, 200);
	}
	else
	{
		//ѡ����ɫ
		crColorText = RGB(10, 10, 10);
		crColorBack = RGB(229, 249, 255);
	}
}

void	CSkinListCtrlEx::PreSubclassWindow()
{
	if (GetHeaderCtrl())
	{
		m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
	}

	__super::PreSubclassWindow();
}

void CSkinListCtrlEx::DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//��������
	auto rcItem = lpDrawItemStruct->rcItem;
	auto pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//��ȡ����
	auto nItemID = lpDrawItemStruct->itemID;
	auto nColumnCount = m_SkinHeaderCtrl.GetItemCount();

	//������ɫ
	auto rcTextColor = m_clrFgHi;
	auto rcBackColor = m_clrBgHi;
	GetItemColor(lpDrawItemStruct, rcTextColor, rcBackColor);

	//�滭����
	CRect rcClipBox;
	pDC->GetClipBox(&rcClipBox);

	//���û���
	pDC->SetBkColor(rcBackColor);
	pDC->SetTextColor(rcTextColor);

	//���ƽ���
	/*if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		CDC bitmapDC;
		bitmapDC.CreateCompatibleDC(pDC);

		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, rcItem.right, rcItem.bottom);

		bitmapDC.SelectObject(&bitmap);

		bitmapDC.FillSolidRect(&rcItem, RGB(128, 128, 128));

		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = 100;

		pDC->AlphaBlend(0, 0, rcItem.right, rcItem.bottom, &bitmapDC, 0, 0, rcItem.right, rcItem.bottom, bf);
	}*/

	//��������
	for (auto i = 0; i < nColumnCount; i++)
	{
		//��ȡλ��
		CRect rcSubItem;
		GetSubItemRect(nItemID, i, LVIR_BOUNDS, rcSubItem);

		//�����ж�
		if (rcSubItem.left > rcClipBox.right) break;
		if (rcSubItem.right < rcClipBox.left) continue;

		//���Ʊ���
		pDC->FillSolidRect(&rcSubItem, rcBackColor);

		//��������
		DrawCustomItem(pDC, lpDrawItemStruct, rcSubItem, i);
	}
}

void	CSkinListCtrlEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	UpdateWindow();
	
	__super::OnNcCalcSize(bCalcValidRects, lpncsp);
}

BOOL CSkinListCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_SkinVerticleScrollbar.UpdateThumbPosition();
	m_SkinHorizontalScrollbar.UpdateThumbPosition();

	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void	CSkinListCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_SkinVerticleScrollbar.UpdateThumbPosition();
	m_SkinHorizontalScrollbar.UpdateThumbPosition();

	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

void	CSkinListCtrlEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_SkinVerticleScrollbar.UpdateThumbPosition();
	m_SkinHorizontalScrollbar.UpdateThumbPosition();

	__super::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CSkinListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	m_SkinVerticleScrollbar.UpdateThumbPosition();
	m_SkinHorizontalScrollbar.UpdateThumbPosition();

	return FALSE;
}

void	CSkinListCtrlEx::OnPaint()
{
	CPaintDC	dc(this);
	CRect	rect;
	GetClientRect(&rect);
	CMemDCEx	memDC(&dc, rect);

	CRect headRect;
	GetDlgItem(0)->GetWindowRect(&headRect);
	ScreenToClient(&headRect);
	dc.ExcludeClipRect(&headRect);

	CRect clip;
	memDC->GetClipBox(&clip);
	memDC->FillSolidRect(clip, RGB(2, 27, 36));

	SetTextBkColor(RGB(255, 255, 255));

	m_SkinHorizontalScrollbar.UpdateThumbPosition();
	m_SkinVerticleScrollbar.UpdateThumbPosition();

	DefWindowProc(WM_PAINT, (WPARAM)memDC->m_hDC, (LPARAM)0);
}

void	CSkinListCtrlEx::OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	static bool bHighlighted = false;

	auto pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = CDRF_DODEFAULT;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		auto iRow = (int)pLVCD->nmcd.dwItemSpec;

		bHighlighted = _IsRowHighlighted(m_hWnd, iRow);
		if (bHighlighted)
		{
			pLVCD->clrText = m_clrFgHi;
			pLVCD->clrTextBk = m_clrBgHi;

			_EnableRowHighlighted(m_hWnd, iRow, false);
		}

		*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
	}
	else if (CDDS_ITEMPOSTPAINT == pLVCD->nmcd.dwDrawStage)
	{
		if (bHighlighted)
		{
			auto iRow = (int)pLVCD->nmcd.dwItemSpec;

			_EnableRowHighlighted(m_hWnd, iRow, true);
		}

		*pResult = CDRF_DODEFAULT;
	}
}

bool	CSkinListCtrlEx::_IsRowSelected(HWND hwnd, int row)
{
	return ListView_GetItemState(hwnd, row, LVIS_SELECTED) != 0;
}

bool	CSkinListCtrlEx::_IsRowHighlighted(HWND hwnd, int row)
{
	return _IsRowSelected(hwnd, row);
}

void	CSkinListCtrlEx::_EnableRowHighlighted(HWND hwnd, int row, bool bHighlight /* = true */)
{
	ListView_SetItemState(hwnd, row, bHighlight ? 0xff : 0, LVIS_SELECTED);
}