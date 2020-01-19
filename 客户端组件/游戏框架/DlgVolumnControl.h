#pragma once

// CDlgVolumnControl �Ի���

#include "GameFrameHead.h"
#include "Resource.h"

class CDSoundManager;

class CDlgVolumnControl : public CDialog
{
	DECLARE_DYNAMIC(CDlgVolumnControl)

public:
	CDlgVolumnControl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVolumnControl();

// �Ի�������
	enum { IDD = IDD_VOLUMN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//��Ϣ����
private:
	//�ػ汳��
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//���Ʊ���
	afx_msg void OnPaint();
	//�������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//����ſ�
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//ȷ��
	afx_msg void OnBnClickedOk();
	//ȡ��
	afx_msg void OnBnClickedCancel();

private:
	//��Դ
	CDC			m_dcMem;					//����DC
	CPngImage	m_ImageBack;			//����ͼƬ
	CPngImage	m_ImageSliderBack;	//����������
	CPngImage	m_ImageText1;			//����1
	CPngImage	m_ImageText2;			//����2
	CPngImage	m_ImageText3;			//����3
	CPngImage	m_ImageSlider;			//������
	CPngImage	m_ImageShoe;			//������
	CPngImage	m_ImageCheck1;		//��ѡ��1
	CPngImage	m_ImageCheck2;		//��ѡ��2

	//��ť
	CSkinButton	m_btOk;				//ȷ��
	CSkinButton	m_btCancel;			//ȡ��
	CSkinButton	m_btClose;			//�ر�

	//����
	int			m_CurVolume;		//��ǰ����
	int			m_DefVolume;		//Ĭ������
	bool		m_bEffect;			//��ǰ��Ч����
	bool		m_bDefEffet;		//Ĭ����Ч����
	bool		m_bMusic;			//��ǰ���ֿ���
	bool		m_bDefMusic;		//Ĭ�����ֿ���

	//λ����Ϣ
	Point	m_CurPos;		//��ǰλ��
	Point	m_DefPos;		//Ĭ��λ��
	BOOL	m_isSelect;		//ѡ�л���

	//���ֹ���ָ��
	CDSoundManager*	m_pSoundMgr;		
};
