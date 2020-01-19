#include "Stdafx.h"
#include "..\..\服务器组件\内核引擎\TraceServiceControlEx.h"


//IMPLEMENT_DYNCREATE(CTraceServiceControlEx, CTraceServiceControl)

	BEGIN_MESSAGE_MAP(CTraceServiceControlEx, CRichEditCtrl)
	END_MESSAGE_MAP()


CTraceServiceControlEx::CTraceServiceControlEx(void)
	: CRichEditCtrl()
{
	CTraceService::SetTraceService(this);
	CTraceService::EnableTrace(TraceLevel_Debug,true);
	CTraceService::EnableTrace(TraceLevel_Exception,true);
	CTraceService::EnableTrace(TraceLevel_Info,true);
	CTraceService::EnableTrace(TraceLevel_Normal,true);
	CTraceService::EnableTrace(TraceLevel_Warning,true);
}

CTraceServiceControlEx::~CTraceServiceControlEx(void)
{
}
bool CTraceServiceControlEx::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	CString enter=pszString;
	enter+="\r\n";
	ReplaceSel(enter);
	return true;
}
//接口查询
VOID * CTraceServiceControlEx::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITraceService,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITraceService,Guid,dwQueryVer);
	return NULL;
}
//追踪消息
//LRESULT CTraceServiceControlEx:: OnTraceServiceMessage(WPARAM wParam, LPARAM lParam)
//{
//
//	return 0;
//}
//加载消息
bool CTraceServiceControlEx::LoadMessage(LPCTSTR pszFileName)
{
	return true;
}
//保存信息
bool CTraceServiceControlEx::SaveMessage(LPCTSTR pszFileName)
{
	return true;
}
//设置参数
bool CTraceServiceControlEx::SetParameter(LONG lMaxLineCount, LONG lReserveLineCount)
{
	return true;
}