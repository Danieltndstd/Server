#include "Stdafx.h"
#include "CorrespondServer.h"
#include "CorrespondServerDlg.h"
#include "..\..\ȫ�ֶ���\Dumpfile.h"
//////////////////////////////////////////////////////////////////////////////////

//�������
CCorrespondServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCorrespondServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//���캯��
CCorrespondServerApp::CCorrespondServerApp()
{
}

//��������
BOOL CCorrespondServerApp::InitInstance()
{
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);
	// ���� mini dump �ļ���
	DeclareDumpFile();

	//��ʾ����
	CCorrespondServerDlg CorrespondServerDlg;
	m_pMainWnd=&CorrespondServerDlg;
	CorrespondServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
