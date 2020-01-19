#pragma once
#include "..\..\服务器组件\内核引擎\traceservice.h"

class CTraceServiceControlEx :
	public CRichEditCtrl,public ITraceService
{
	//辅助变量
protected:
	LONG							m_lMaxLineCount;					//最大行数
	LONG							m_lReserveLineCount;				//保留行数
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];	//临时对象

	//组件变量
protected:
	CWHDataQueue					m_DataQueue;						//数据队列
	CCriticalSection				m_CriticalSection;					//同步对象

	//函数定义
public:
	//构造函数
	CTraceServiceControlEx();
	//析构函数
	virtual ~CTraceServiceControlEx();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//信息接口
public:
	//追踪信息
	virtual bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel);
	//重载函数
protected:
	//绑定函数
	//virtual VOID PreSubclassWindow();

	//功能函数
public:
	//加载消息
	bool LoadMessage(LPCTSTR pszFileName);
	//保存信息
	bool SaveMessage(LPCTSTR pszFileName);
	//设置参数
	bool SetParameter(LONG lMaxLineCount, LONG lReserveLineCount);

	//辅助函数
private:
	//配置服务
	//VOID InitializeService();
	//获取颜色
	//COLORREF GetTraceColor(enTraceLevel TraceLevel);
	//插入字串
	//bool InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat);

	//回调函数
private:
	//加载回调
	//static DWORD CALLBACK LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);
	//保存回调
	//static DWORD CALLBACK SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);

	//菜单命令
protected:
	//删除信息
	//VOID OnClearAll();
	//全部选择
	//VOID OnSelectAll();
	//拷贝字符
	//VOID OnCopyString();
	//保存信息
	//VOID OnSaveString();
	//删除字符
	//VOID OnDeleteString();

	//消息映射
protected:
	//建立消息
	//INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//右键消息
	//VOID OnRButtonDown(UINT nFlags, CPoint point);
	//追踪消息
	//LRESULT OnTraceServiceMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

