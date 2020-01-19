#pragma once

#include "Singleton.h"

#import "../内核引擎/MSADO15.dll"   no_namespace rename("EOF","adoEOF")   

class CAdoAdapter
{
public:
	CAdoAdapter(void);
	~CAdoAdapter(void);

	void OpenConnection();
	
	void CloseConnection();

	void SetCommandType(CommandTypeEnum type);

	bool  TryConnectAgain(bool bFocusConnect, CComError * pComError);


	bool SetConnectionInfo(TCHAR szDBAddr[], WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);

	void AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, _variant_t & DBVarValue);

	void ClearParemeter();

	void CloseRecordset();

	bool IsRecordsetOpened();

	bool IsConnectError();

public:
	bool ExecuteProcess(_bstr_t pszSPName, bool bRecordset);

private:
	DWORD			m_dwResumeConnectTime;
	DWORD			m_dwConnectErrorTime;
	DWORD			m_dwConnectCount;
	DWORD			m_dwResumeConnectCount;


protected:
	CString			m_strConnect;						//连接字符串

private:
	_CommandPtr		m_ptrCommand;
	
	_RecordsetPtr	m_ptrRecordSet;

	_ConnectionPtr  m_ptrConnection; 
};
