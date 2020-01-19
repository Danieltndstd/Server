#pragma once
#include "afxwin.h"

// CDlgGameInsureZJG �Ի���

#include "GameFrameHead.h"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////
void MakeStringZJG(CString &strNum,SCORE lNumber);
void MakeStringToNumZJG(CString str,SCORE &Num);
////////////////////////////////////////////////////////////////////////

class CDlgGameInsureZJG : public CDialog
{
	DECLARE_DYNAMIC(CDlgGameInsureZJG)

public:
	CDlgGameInsureZJG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGameInsureZJG();

// �Ի�������
	enum { IDD = IDD_ZJG_BAKER_STORAGE };

public:
	//��ʾ�ؼ�
	void ShowItem();
	//���½���
	void UpdateView();
	//�滭����
	void DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos);
	//ˢ����Ϣ
	void FreshBank();

public:
	IClientUserItem					*m_pMeUserData;					//�û���Ϣ
	IClientKernel					*m_pIClientKernel;				//�ں˽ӿ�

	bool							m_blUsingPassWord;				//�����Ǽ���
	WORD							m_wRevenueTake;					//˰�ձ���
	WORD							m_wRevenueTransfer;				//˰�ձ���
	LONGLONG						m_bankGold;						//������Ŀ

protected:
	CSkinButton						m_btClose;
	CSkinButton						m_btLink;
	CSkinButton						m_btAll;
	CSkinButton						m_btOkay;

	CPngImage						m_pngNumber;


	LONGLONG						m_OrInCount;  	//����ֵ
	LONGLONG						m_helpCount;
	

protected:
	virtual BOOL OnInitDialog();

	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//�رհ�ť
	afx_msg void OnBnClickedClose();
	//���Ӱ�ť
	afx_msg void OnBnClickedLink();
	//ȫ����ť
	afx_msg void OnBnClickedAll();
	//ȷ��ȡ�ť
	afx_msg void OnBnClickedMakeSure();
	//���ƶԻ���
	afx_msg void OnPaint();
	//������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void OnLButtonDown(UINT nFlags, CPoint point);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnEnChangeEditMoney();

public:
	//��������
	void SetBankerActionType(bool bStorage);
	//����λ��
	void SetPostPoint(CPoint Point);
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserDatas);
	//������Ϣ
	void SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
	//��������	
	void UpdateBanker();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_money;
	CEdit m_password;
};
