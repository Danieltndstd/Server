#pragma once
#include "..\..\���������\�ں�����\traceservice.h"

class CTraceServiceControlEx :
	public CRichEditCtrl,public ITraceService
{
	//��������
protected:
	LONG							m_lMaxLineCount;					//�������
	LONG							m_lReserveLineCount;				//��������
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];	//��ʱ����

	//�������
protected:
	CWHDataQueue					m_DataQueue;						//���ݶ���
	CCriticalSection				m_CriticalSection;					//ͬ������

	//��������
public:
	//���캯��
	CTraceServiceControlEx();
	//��������
	virtual ~CTraceServiceControlEx();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//��Ϣ�ӿ�
public:
	//׷����Ϣ
	virtual bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel);
	//���غ���
protected:
	//�󶨺���
	//virtual VOID PreSubclassWindow();

	//���ܺ���
public:
	//������Ϣ
	bool LoadMessage(LPCTSTR pszFileName);
	//������Ϣ
	bool SaveMessage(LPCTSTR pszFileName);
	//���ò���
	bool SetParameter(LONG lMaxLineCount, LONG lReserveLineCount);

	//��������
private:
	//���÷���
	//VOID InitializeService();
	//��ȡ��ɫ
	//COLORREF GetTraceColor(enTraceLevel TraceLevel);
	//�����ִ�
	//bool InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat);

	//�ص�����
private:
	//���ػص�
	//static DWORD CALLBACK LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);
	//����ص�
	//static DWORD CALLBACK SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);

	//�˵�����
protected:
	//ɾ����Ϣ
	//VOID OnClearAll();
	//ȫ��ѡ��
	//VOID OnSelectAll();
	//�����ַ�
	//VOID OnCopyString();
	//������Ϣ
	//VOID OnSaveString();
	//ɾ���ַ�
	//VOID OnDeleteString();

	//��Ϣӳ��
protected:
	//������Ϣ
	//INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�Ҽ���Ϣ
	//VOID OnRButtonDown(UINT nFlags, CPoint point);
	//׷����Ϣ
	//LRESULT OnTraceServiceMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
