#pragma once

// CDlgGameClose �Ի���
#include "Resource.h"
#include "GameFrameHead.h"

class CDlgGameClose : public CDialog
{
	DECLARE_DYNAMIC(CDlgGameClose)

public:
	CDlgGameClose(LPTSTR msg = nullptr, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGameClose();

// �Ի�������
	enum { IDD = IDD_GAME_CLOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//��Ϣ����
public:
	//���Ʊ���
	afx_msg void OnPaint();

private:
	//��Ϣ
	CString				m_strInfo;				//��Ϣ

	//��Դ
	CPngImage		m_ImageBack;		//����

	//��ť
	CSkinButton		m_btOk;
	CSkinButton		m_btCancel;
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
