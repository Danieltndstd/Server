#pragma once

class CSkinVerticleScrollbar : public CStatic
{
	friend class CSkinListCtrlEx;

	//��������
public:
	//���캯��
	CSkinVerticleScrollbar();
	//��������
	virtual ~CSkinVerticleScrollbar();

	//λ�ƺ���
public:
	//���Ϲ���
	void ScrollUp();
	//���¹���
	void ScrollDown();
	//��ͷ
	void	PageUp();
	//��β
	void	PageDown();
	//����λ��
	void UpdateThumbPosition();

	//��Ϣ���庯��
protected:
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
	CListCtrl*	m_list;		//������List

	bool			m_bMouseDownArrowUp, m_bMouseDownArrowDown;
	bool			m_bDragging;
	bool			m_bMouseDown;

	int			m_nThumbTop;		//�Ӻδ���ʼ��
	double		m_dbThumbInterval;
};