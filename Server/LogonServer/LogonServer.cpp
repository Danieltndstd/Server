#include "Stdafx.h"
#include "LogonServer.h"
#include "LogonServerDlg.h"
#include "..\..\ȫ�ֶ���\Dumpfile.h"
//////////////////////////////////////////////////////////////////////////////////

//�������
CLogonServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CLogonServerApp, CWinApp)
END_MESSAGE_MAP()

//���캯��
CLogonServerApp::CLogonServerApp()
{
}

//��������
BOOL CLogonServerApp::InitInstance()
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
	CLogonServerDlg LogonServerDlg;
	m_pMainWnd=&LogonServerDlg;
	LogonServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
