#include "Stdafx.h"
#include "LogonServer.h"
#include "LogonServerDlg.h"
#include "..\..\全局定义\Dumpfile.h"
//////////////////////////////////////////////////////////////////////////////////

//程序对象
CLogonServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CLogonServerApp, CWinApp)
END_MESSAGE_MAP()

//构造函数
CLogonServerApp::CLogonServerApp()
{
}

//启动函数
BOOL CLogonServerApp::InitInstance()
{
	__super::InitInstance();

	//设置组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szProduct);
	// 生成 mini dump 文件。
	DeclareDumpFile();

	//显示窗口
	CLogonServerDlg LogonServerDlg;
	m_pMainWnd=&LogonServerDlg;
	LogonServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
