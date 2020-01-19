#pragma once

#include "SkinControlHead.h"

class CSkinHorizontalScrollbar : public CStatic
{
	friend class CSkinListCtrlEx;

	//��������
public:
	//���캯��
	CSkinHorizontalScrollbar();
	//��������
	virtual ~CSkinHorizontalScrollbar();

	//λ�ƺ���
public:
	//�������
	void ScrollLeft();
	//���ҹ���
	void ScrollRight();
	//��ͷ
	void	PageLeft();
	//��β
	void	PageRight();
	//����λ��
	void UpdateThumbPosition();

	//��Ϣ���庯��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ػ溯��
	afx_msg void OnPaint();
	//������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//����ſ�
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//��ʱ��
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

	//˽�к���
private:
	//�Ի溯��
	void	_Draw();
	//����λ��
	void	_LimitThumbPosition();

private:
	CListCtrl*		m_list;		//������List

	bool				m_bMouseDownArrowRight, m_bMouseDownArrowLeft;
	bool				m_bDragging;
	bool				m_bMouseDown;

	int				m_nThumbLeft;	//�Ӻδ���ʼ��
	double			m_dbThumbRemainder;
	double			m_dbThumbInterval;

	CPngImage	m_pngBack;				//����
	CPngImage	m_pngLeftArrow;		//���ͷ
	CPngImage	m_pngThumb;			//��������
	CPngImage	m_pngRightArrow;	//�Ҽ�ͷ
};