#pragma once

#include "SkinHeaderCtrl.h"
#include "SkinControlHead.h"
#include "SkinVerticleScrollbar.h"
#include "SkinHorizontalScrollbar.h"

class SKIN_CONTROL_CLASS CSkinListCtrlEx : public CListCtrl
{
	//��������
public:
	//���캯��
	CSkinListCtrlEx();
	//��������
	virtual ~CSkinListCtrlEx();

	//��ʼ������
	void	Init();
	//���ù�����
	void PositionScrollBars();

	//���뺯��
public:
	//��������
	void InsertDataItem(void* pItemData);
	//��������
	void UpdateDataItem(void* pItemData);
	//ɾ������
	void DeleteDataItem(void* pItemData);

	//���ݿ��ƣ��������д��
protected:
	//��ȡλ��
	virtual int GetInsertIndex(void* pItemData);
	//��������
	virtual int SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	//�����ַ�
	virtual void ConstructString(void* pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);
	//�������
	virtual void DrawCustomItem(CDC* pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect& rcSubItem, INT nColumnIndex);
	//����������ɫ
	virtual void GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF& crColorText, COLORREF& crColorBack);

	//��������
protected:
	//���໯
	void	PreSubclassWindow();
	//�滭����
	void	DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct);

	//��Ϣ���庯��
protected:
	//����ߴ�ʱ����
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	//��������
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//������Ӧ
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//������Ӧ
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//�ػ汳��
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//�ػ溯��
	afx_msg void OnPaint();

	//�Ի溯��
	afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	//˽�к���
private:
	//��ѡ�У�
	bool	_IsRowSelected(HWND hwnd, int row);
	//�и�����
	bool	_IsRowHighlighted(HWND hwnd, int row);
	//����
	void	_EnableRowHighlighted(HWND hwnd, int row, bool bHighlight = true);

protected:
	CSkinHeaderCtrlEx			m_SkinHeaderCtrl;
	CSkinVerticleScrollbar		m_SkinVerticleScrollbar;
	CSkinHorizontalScrollbar	m_SkinHorizontalScrollbar;

	COLORREF						m_clrFgHi;
	COLORREF						m_clrBgHi;

	bool									m_bAscendSort;			//�����־
};