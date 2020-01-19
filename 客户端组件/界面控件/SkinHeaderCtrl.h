#pragma once

#include "SkinControlHead.h"

class SKIN_CONTROL_CLASS CSkinHeaderCtrlEx : public CHeaderCtrl
{
	//��������
public:
	//���캯��
	CSkinHeaderCtrlEx();
	//��������
	~CSkinHeaderCtrlEx();

	//���ܺ���
public:
	//��������
	void	SetLockCount(UINT uLockCount);
	//�����и�
	void	SetItemHeight(UINT uItemHeight);

	//��Ϣ���庯��
protected:
	//�ػ溯��
	afx_msg void OnPaint();
	//�ػ汳��
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//LayOut��Ϣ
	afx_msg LRESULT OnLayout(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	//�����б�
protected:
	UINT			m_uLockCount;		//��������
	UINT			m_uItemHeight;		//����߶�

	CPngImage	m_pngHeadBack;		//����
};