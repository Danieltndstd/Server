#pragma once

#include "SkinControlHead.h"

class SKIN_CONTROL_CLASS CSkinComboBoxEx : public CComboBox
{
	//��������
public:
	//���캯��
	CSkinComboBoxEx();
	//��������
	virtual ~CSkinComboBoxEx();

	//���غ���
public:
	//���໯
	virtual void PreSubclassWindow();
	//���óߴ�
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//�Ի�����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//��Ϣ����
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//ֻ������
	__declspec(property(get = GetReadOnly, put = SetReadOnly)) BOOL bReadOnly;
	BOOL GetReadOnly(void) const { return m_bReadOnly; }
	void	SetReadOnly(BOOL value) { m_bReadOnly = value; }

	DECLARE_MESSAGE_MAP()

	//��Ϣ���庯��
protected:
	//�ػ�
	afx_msg void OnPaint();
	//�ӿؼ�����
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ѡ����ı�
	afx_msg void OnCbnSelChange();

	//˽�к���
private:
	//����
	void _DrawBack(CDC* pDC, CRect& rc);
	//������Ͽ��б༭������
	void	_CreateEditFont();
	//������Ͽ��������б�����
	void	_CreateListFont();
	//���������б�
	void _DrawContent(CDC* pDC, UINT uItemID, UINT uItemState, CRect& rcItem);
	//���ư�ť
	void _DrawButton(CDC* pDC, CRect rcButton);
	//��Ӧ������ȣ�������������
	void _AdjustDroppedWidth();

private:
	BOOL			m_bReadOnly;				//ֻ������

	COLORREF	m_clrEditText;				//�༭��������ɫ
	COLORREF	m_clrListitText;				//�б��������ɫ
	CString			m_strEditFontName;		//�༭����������
	CString			m_strListFontName;		//�б����������
	CFont*			m_pEditFont;				//�༭������
	CFont*			m_pListFont;					//�б������
	int				m_nEditFontPtSize;		//�༭������ߴ�
	int				m_nListFontPtSize;		//�б������ߴ�

	CListBox*		m_pListBox;					//�б��ָ��
	CPngImage	m_pngComboBack;		//����
	CBitmap		m_bmpButton;				//��ťͼƬ

	int				m_nPreSelItem;				//ǰһ��ѡ����
	int				m_nSelItem;					//��ǰѡ����
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};