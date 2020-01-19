#pragma once

// CDlgRecordPlayZJG �Ի���
#include "Resource.h"
#include "GameFrameHead.h"
#include "afxcmn.h"

#define WM_OPRATION		WM_USER + 1		//��������
#define WM_OPRATIONEX	WM_USER + 2		//�������

class CDlgRecordPlayZJG : public CDialog
{
	DECLARE_DYNAMIC(CDlgRecordPlayZJG)

public:
	virtual ~CDlgRecordPlayZJG();

// �Ի�������
	enum { IDD = IDD_ZJG_RECORD_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//��Ϣ����
public:
	//���Ʊ���
	afx_msg void OnPaint();
	//�������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//���ſ�
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��С������
	afx_msg void OnBnClickedMin(){ ::ShowWindow(m_hWnd, SW_MINIMIZE); }
	//�رմ���
	afx_msg void OnBnClickedClose(){ AfxGetMainWnd()->PostMessage(WM_CLOSE); DestroyWindow(); }
	//ʱ��������Ӧ
	afx_msg LRESULT OnOperationDo(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//���÷�Χ
	void SetRange(vector<UINT>& vElapsed);
	//����λ��
	void SetPos(int nPos) { if(m_dRatio == 0) return; m_CurrentPos += (UINT)(nPos / m_dRatio); }
	//���ù�������
	void SetBuddyWnd(CWnd* pWnd){ m_pParent = pWnd; }

	//���캯��˽��
private:
	CDlgRecordPlayZJG(CWnd* pParent = NULL);   // ��׼���캯��
	CDlgRecordPlayZJG(const CDlgRecordPlayZJG&);
	CDlgRecordPlayZJG& operator=(const CDlgRecordPlayZJG&);

	//��̬����
public:
	//��ȡʵ��
	static CDlgRecordPlayZJG& GetInstance();

private:
	//��Դ
	CDC								m_dcMem;					//����DC
	CWnd*							m_pParent;					//������
	CPngImage						m_ImageBack;				//����ͼƬ
	CPngImage						m_ImageSliderBack;			//����������
	CPngImage						m_ImageSlider;				//������
	CPngImage						m_ImageShoe;				//������

	//��ť
	CSkinButton						m_btRecordMin;				//��С����ť
	CSkinButton						m_btRecordClose;			//�رհ�ť
	CSkinButton						m_btRecordPlay;				//���Ű�ť
	CSkinButton						m_btRecordPause;			//��ͣ��ť
	CSkinButton						m_btRecordOpen;				//�򿪰�ť

	//��������
	vector<UINT>					m_vElapsed;					//��ʱ��
	UINT							m_SliderLength;				//�������ܳ���
	UINT							m_SliderHeight;				//�������߶�
	UINT							m_CurrentPos;				//��ǰλ��(������λ��)
	UINT							m_AlreadyCount;				//�����в���
	BOOL							m_IsSelect;					//�Ƿ�ѡ�л���
	CPoint							m_DownPt;					//���µĵ�
	double							m_dRatio;					//���ʣ���ֵ��Χ/�ܳ��ȣ�
};
