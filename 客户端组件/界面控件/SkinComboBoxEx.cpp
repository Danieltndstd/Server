#include "Stdafx.h"
#include "memdc.h"
#include "Resource.h"
#include "SkinComboBoxEx.h"

CSkinComboBoxEx::CSkinComboBoxEx()
{
	m_bReadOnly = FALSE;
	m_clrEditText = RGB(255, 255, 255);
	m_clrListitText = RGB(0, 0, 0);
	m_strEditFontName = TEXT("����");
	m_strListFontName = TEXT("����");
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
	//��ȡ��Դ
	auto hResInstance = GetModuleHandle(SKIN_CONTROL_DLL_NAME);
	m_pngComboBack.LoadImage(hResInstance, TEXT("SKIN_COMBO_BACK"));
	m_bmpButton.LoadBitmap(IDB_COMBO_BUTTON);

	//Ĭ��ʹ��ϵͳһ��������
	LOGFONT lg;
	auto pFont = GetFont();
	pFont->GetLogFont(&lg);

	//m_nEditFontPtSize = lg.lfHeight * 12;
	m_strEditFontName = lg.lfFaceName;

	//m_nListFontPtSize = m_nEditFontPtSize;
	m_strListFontName = m_strListFontName;

	//��������
	//_CreateEditFont();
	_CreateListFont();

	//����Ӧ���
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

	//���������б�
	_DrawContent(pDC, uItemID, uItemState, rcItem);
}

LRESULT CSkinComboBoxEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_CTLCOLORLISTBOX == message)
	{
		auto hWndListBox = (HWND)lParam;
		auto pListBox = (CListBox*)FromHandle(hWndListBox);
		auto nItemCount = pListBox->GetCount();

		//����ָ��
		m_pListBox = pListBox;

		if (CB_ERR != nItemCount)
		{
			//ת����Ļ���λ�õ��ͻ���λ��
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

						//ˢ��֮ǰ����
						pListBox->GetItemRect(m_nPreSelItem, &rcPreItem);
						pListBox->InvalidateRect(&rcPreItem);

						//ˢ�µ�ǰ����
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
		//��ȡ�༭��
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
		OutputDebugString(TEXT("��ť��Ϣ"));
	}

	return hbr;
}

void CSkinComboBoxEx::OnCbnSelChange()
{

}

void CSkinComboBoxEx::_DrawBack(CDC* pDC, CRect& rc)
{
	//���Ʊ�����߿�
	CBrush brushInner, brushOuter;
	brushInner.CreateSolidBrush(RGB(2, 21, 27));
	brushOuter.CreateSolidBrush(RGB(149, 99, 18));

	//��ʱ���ã�ʹ�ñ���ͼ
// pDC->FillRect(rc, &brushInner);
// pDC->FrameRect(rc, &brushOuter); 

	m_pngComboBack.DrawImage(pDC, 0, 0);

	COMBOBOXINFO comboInfo;
	comboInfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboInfo);

	//���ư�ť
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

	//��������
	m_pEditFont = new CFont;
	m_pEditFont->CreatePointFontIndirect(&lg);

	//��������
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

	//��������
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

	//��������и���
	if (uItemID == m_nSelItem)
	{
		//�������ʲ�ѡ��
		bgItemPen.CreatePen(PS_SOLID, 1, RGB(222, 234, 198));
		auto oldPen = pDC->SelectObject(&bgItemPen);

		//������ˢ��ѡ��
		bgItemBrush.CreateSolidBrush(RGB(222, 234, 198));
		auto oldBrush = pDC->SelectObject(&bgItemBrush);

		pDC->RoundRect(&rcItem, CPoint(0, 0));

		//�ָ���ɾ���豸
		pDC->SelectObject(oldBrush);
		pDC->SelectObject(oldPen);
		bgItemBrush.DeleteObject();
		bgItemPen.DeleteObject();

		//��ʾ�����ı�
		GetLBText(uItemID, strShowText);
		SetWindowText(strShowText);
	}

	//�����б��
	listBoxBrush.CreateSolidBrush(RGB(254, 230, 202));
	m_pListBox->GetClientRect(&rcListBox);
	pDC->FrameRect(rcListBox, &listBoxBrush);

	//�������ı�
	pDC->SetTextColor(m_clrListitText);
	auto oldFpnt = pDC->SelectObject(m_pListFont);
	GetLBText(uItemID, strShowText);
	rcItem.left += 3;
	pDC->DrawText(strShowText, &rcItem, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
}

void CSkinComboBoxEx::_DrawButton(CDC* pDC, CRect rcButton)
{
	//��ȡλͼ��Ϣ
	BITMAP bitmapBtn;
	m_bmpButton.GetBitmap(&bitmapBtn); 

	//�ڴ�DC
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&m_bmpButton);

	// ���ݲ�ͬ��״̬��ȡͼƬ�Ĳ�ͬ���֣�������ֻ������
	/*if (m_bBtnDown || m_bBtnHover)
		pDC->BitBlt(rcButton.left, rcButton.top + nTop, rcButton.Width() / 2, rcButton.Height(), pMemDC, rcButton.Width() / 2 + 1, 0, SRCCOPY);
	else*/
		pDC->BitBlt(rcButton.left - 2, rcButton.top , rcButton.Width() + 1, rcButton.Height(), &memDC, 0, 0, SRCCOPY);
}

void CSkinComboBoxEx::_AdjustDroppedWidth()
{
	//���������б�����
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

	//������������
	SetDroppedWidth(nWidth);

	//�ָ��豸
	pDC->SelectObject(oldFont);
	ReleaseDC(pDC);
}


BOOL CSkinComboBoxEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message  == WM_KEYDOWN)
	{
		return TRUE;
	}

	return CComboBox::PreTranslateMessage(pMsg);
}
