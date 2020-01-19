#include "StdAfx.h"
#include "CAdoAdapter.h"


CAdoAdapter::CAdoAdapter(void) : m_dwResumeConnectCount(30), m_dwResumeConnectTime(30)
{
	m_dwConnectCount = 0;
	m_dwConnectErrorTime = 0L;

	m_ptrCommand.CreateInstance(__uuidof(Command));
	m_ptrRecordSet.CreateInstance(__uuidof(Recordset));
	m_ptrConnection.CreateInstance(__uuidof(Connection));

	if (m_ptrCommand == NULL)		throw TEXT("数据库命令对象创建失败");
	if (m_ptrConnection == NULL)	throw TEXT("数据库连接对象创建失败");
	if (m_ptrRecordSet == NULL)		throw TEXT("数据库记录集对象创建失败");

	SetCommandType(adCmdText);
}



CAdoAdapter::~CAdoAdapter(void)
{
	CloseConnection();
	m_ptrCommand.Release();
	m_ptrConnection.Release();
	m_ptrRecordSet.Release();
}


void CAdoAdapter::CloseRecordset()
{
	if (IsRecordsetOpened()) m_ptrRecordSet->Close();
}

bool CAdoAdapter::IsRecordsetOpened()
{
	if (m_ptrRecordSet == nullptr) return false;

	if (m_ptrRecordSet->GetState() == adStateClosed) return false;

	return true;
}

void CAdoAdapter::SetCommandType(CommandTypeEnum type)
{
	m_ptrCommand->CommandType = type;
}


void CAdoAdapter::CloseConnection()
{
	CloseRecordset();

	try 
	{
		if ((m_ptrRecordSet!=nullptr) && (m_ptrConnection->GetState()!=adStateClosed)) m_ptrConnection->Close();
	}
	catch (CComError& error)
	{
		TRACE_LOG(error.Description(), TraceLevel_Exception);
	}
}

void CAdoAdapter::OpenConnection()
{
	try 
	{
		
		CloseConnection();
		m_ptrConnection->Open(_bstr_t(m_strConnect), L"", L"", adConnectUnspecified);
		m_ptrConnection->CursorLocation = adUseClient;
		m_ptrCommand->ActiveConnection = m_ptrConnection;
		m_dwConnectErrorTime = 0l;
		m_dwConnectCount = 0;
	}
	catch (CComError& error)
	{
		TRACE_LOG(error.Description(), TraceLevel_Exception);
	}
}

bool CAdoAdapter::SetConnectionInfo(TCHAR szDBAddr[], WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		szPassword, szUser, szDBName, szDBAddr, wPort);

	return true;
}

void CAdoAdapter::AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, _variant_t & DBVarValue)
{
	try 
	{

		_ParameterPtr Paremeter = m_ptrCommand->CreateParameter(pszName, Type, Direction, lSize, DBVarValue);

		m_ptrCommand->Parameters->Append(Paremeter);
	}
	catch (CComError& error)
	{
		TRACE_LOG(error.Description(), TraceLevel_Exception);
	}

}

void CAdoAdapter::ClearParemeter()
{
	try 
	{
		long count = m_ptrCommand->Parameters->Count;

		if (count > 0l)
		{
			for (long i = count; i > 0; i--)
			{
				m_ptrCommand->Parameters->Delete(i-1);
			}
		}
	}
	catch (CComError& error)
	{
		TRACE_LOG(error.Description(), TraceLevel_Exception);
	}

}

bool CAdoAdapter::ExecuteProcess(_bstr_t pszSPName, bool bRecordset)
{

	try 
	{
		CloseRecordset();

		m_ptrCommand->CommandText = pszSPName;
	
		if (bRecordset == true)
		{
			m_ptrRecordSet->PutRefSource(m_ptrCommand);
			m_ptrRecordSet->CursorLocation = adUseClient;

			m_ptrRecordSet->Open((IDispatch*)m_ptrCommand, vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified);
		
			return true;
		}
		else
		{
			m_ptrRecordSet->CursorLocation = adUseClient;
			m_ptrCommand->Execute(NULL, NULL, adExecuteNoRecords);
			return true;
		}
	}
	catch (CComError& error)
	{
		if (IsConnectError() == true)	TryConnectAgain(false, &error);
		else TRACE_LOG(error.Description(), TraceLevel_Exception);
	}

	return false;
}

bool CAdoAdapter::IsConnectError()
{
	try
	{
		if (m_ptrConnection == NULL) return true;

		long e = m_ptrConnection->GetState();

		if (e == adStateClosed) return true;

		long lErrorCount = m_ptrConnection->Errors->Count;

		if (lErrorCount > 0L)
		{
			ErrorPtr pError = NULL;

			for (long i = 0; i < lErrorCount; ++i)
			{
				pError = m_ptrConnection->Errors->GetItem(i);
				if (pError->Number == 0x80004005) return true;
			}
		}
	}
	catch (CComError& error)
	{
		TRACE_LOG(error.Description(), TraceLevel_Exception);
	}

	return false;
}

bool CAdoAdapter::TryConnectAgain(bool bFocusConnect, CComError * pComError)
{
	try
	{
		//判断重连
		bool bReConnect = bFocusConnect;
		if (bReConnect == false)
		{
			DWORD dwNowTime = (DWORD)time(NULL);
			if ((m_dwConnectErrorTime + m_dwResumeConnectTime) > dwNowTime) bReConnect = true;
		}
		if ((bReConnect == false) && (m_dwConnectCount > m_dwResumeConnectCount)) bReConnect = true;

		//设置变量
		m_dwConnectCount++;
		m_dwConnectErrorTime = (DWORD)time(NULL);
		if (bReConnect == false)
		{
			if (pComError != NULL) TRACE_LOG(pComError->Description(), TraceLevel_Exception);
			return false;
		}

		//重新连接
		OpenConnection();
		return true;
	}
	catch (CComError& comError)
	{
		//重新连接错误
		TRACE_LOG(comError.Description(), TraceLevel_Exception);
	}

	return false;

}