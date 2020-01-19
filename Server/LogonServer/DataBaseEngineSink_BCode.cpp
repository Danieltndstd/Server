#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineBegin_BCode(IUnknownEx * pIUnknownEx)
{
	//�󶨺���
	{
		// ��¼�ӿ�
		{
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_LOGON_GAMEID_BCode, CDataBaseEngineSink::OnMobileLogonGameID_BCode, this);	// ��ʶ��¼
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_LOGON_ACCOUNTS_BCode, CDataBaseEngineSink::OnMobileLogonAccounts_BCode, this);	// �ʺŵ�¼
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_REGISTER_ACCOUNTS_BCode, CDataBaseEngineSink::OnMobileRegisterAccounts_BCode, this);	// ע���ʺ�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_REGISTER_MOBILE_BCode, CDataBaseEngineSink::OnMobileRegisterMobile_BCode, this);	// �ֻ�ע��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_VISITOR_ACCOUNTS_BCode, CDataBaseEngineSink::OnMobileVisitorAccounts_BCode, this);	// �ο͵�¼
		}
		
		//�û�����
		{
			//ǩ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_TODAY_IS_SIGNIN,CDataBaseEngineSink::OnRequestTotayIsSignIn,this); //��ȡ�����Ƿ�ǩ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GET_SIGNIN,CDataBaseEngineSink::OnRequestGetSignIn,this); //��ȡǩ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_SET_SIGNIN,CDataBaseEngineSink::OnRequestSetSignIn,this); //����ǩ��

			// ת��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GETDAILINFO, CDataBaseEngineSink::OnRequestGetDailInfo, this); //��ȡת��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_ROTATEDAILAWARD, CDataBaseEngineSink::OnRequestRotateDailAward, this); //ת��ת�̻�ý���
			
			// ������
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_CHICKBINDBANK, CDataBaseEngineSink::OnRequestChickBindBank, this); //��ѯ�����п�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_BINDBANK, CDataBaseEngineSink::OnRequestBindBank, this); //�����п�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_CHICKBINDZFB, CDataBaseEngineSink::OnRequestChickBindZFB, this); //��ѯ��֧����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_BINDZFB, CDataBaseEngineSink::OnRequestBindZFB, this); //��֧����

			// �ʼ�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_REQUEST_MAIL, CDataBaseEngineSink::OnRequestRequestMail, this); //�����ʼ�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_READ_DELETE_MAIL, CDataBaseEngineSink::OnRequestReadDeleteMail, this); //��ȡɾ���ʼ�

			//�û�������Ϣ
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GET_USER_PERFECT_INFORMATION, CDataBaseEngineSink::OnRequestGetUserPerFectInformation, this);	//��ȡ�û�������Ϣ
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_SET_USER_PERFECT_INFORMATION, CDataBaseEngineSink::OnRequestSetUserPerFectInformation, this);	//�����û�������Ϣ

			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_IS_SPREAD_REGISTER, CDataBaseEngineSink::OnRequestIsSpreadRegister, this);   //�Ƿ����ƹ�ע��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_BIND_USER_SPREAD, CDataBaseEngineSink::OnRequestBindUserSpread, this);   //���û��ƹ�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_IS_BIND_SPREAD, CDataBaseEngineSink::OnRequestIsBindSpread, this);   //�Ƿ��Ѿ����ƹ�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_IS_ALREADY_MODIFY_NICKNAME, CDataBaseEngineSink::OnRequestIsAlreadyModifyNickName, this);   //�Ƿ��Ѿ��޸Ĺ��ǳ�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_IS_TODAY_FIRST_TIME_LOGIN, CDataBaseEngineSink::OnRequestIsTodayFirstTimeLogin, this);   //�Ƿ��ǽ����һ�ε�¼
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_CLIENT_LOGIN_SUCCESS, CDataBaseEngineSink::OnRequestClientLoginSuccess, this);   //�ͻ��˵�¼���
			
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_KILL_LOCK, CDataBaseEngineSink::OnRequestKillLocker, this);   //�ͻ��˵�¼���
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_LOAD_SCROLL_MESSAGE, CDataBaseEngineSink::OnLoadScrollMessage, this);		//��ȡ������Ϣ
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_CLEAR_SCROLL_MESSAGE, CDataBaseEngineSink::OnClearScrollMessage, this);		//���������Ϣ 
		}

	}

	return true;
}


//I D ��¼
bool CDataBaseEngineSink::OnMobileLogonGameID_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_MB_LogonGameID));
		if (wDataSize!=sizeof(DBR_MB_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult_BCode(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult_BCode(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnMobileLogonAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_MB_LogonAccounts));
		if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

		//������
		DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		//m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),pLogonAccounts->cbDeviceType);
		m_AccountsDBAide.AddParameter(TEXT("@dwInviteID"), pLogonAccounts->dwInviteID);
		m_AccountsDBAide.AddParameter(TEXT("@dwSortID"), pLogonAccounts->dwSortID);
		m_AccountsDBAide.AddParameter(TEXT("@dwLogonType"), pLogonAccounts->dwLogonType);
		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_VisitorLogin"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult_BCode(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult_BCode(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺ�ע��
bool CDataBaseEngineSink::OnMobileRegisterAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_MB_RegisterAccounts_BCode));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts_BCode)) return false;

		//������
		DBR_MB_RegisterAccounts_BCode * pRegisterAccounts=(DBR_MB_RegisterAccounts_BCode *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@dwSpreaderID"),pRegisterAccounts->dwSpreaderID); 
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_ACCOUNT);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_MB_RegisterAccounts_ry"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult_BCode(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult_BCode(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ֻ�ע��
bool CDataBaseEngineSink::OnMobileRegisterMobile_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize == sizeof(DBR_MB_RegisterMobile));
		if (wDataSize != sizeof(DBR_MB_RegisterMobile))
			return false;

		//����
		DBR_MB_RegisterMobile* pRegisterMobile = (DBR_MB_RegisterMobile*)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter = (tagBindParameter *)pRegisterMobile->pBindParameter;
		if (pBindParameter->dwSocketID != dwContextID)
			return true;

		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pRegisterMobile->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strMoblie"), pRegisterMobile->szMobile);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"), pRegisterMobile->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"), pRegisterMobile->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_MOBLIE);

		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_MoblieRegister"), true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnLogonDisposeResult_BCode(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnLogonDisposeResult_BCode(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"), true);

		return false;
	}
}

//�ο͵�¼
bool CDataBaseEngineSink::OnMobileVisitorAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_VisitorAccounts));
		if (wDataSize!=sizeof(DBR_MB_VisitorAccounts)) return false;

		//������
		DBR_MB_VisitorAccounts * pRegisterAccounts=(DBR_MB_VisitorAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);


		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_VISITOR);
		m_AccountsDBAide.AddParameter(TEXT("@dwInviteID"), pRegisterAccounts->dwInviteID);
		m_AccountsDBAide.AddParameter(TEXT("@dwSortID"), pRegisterAccounts->dwSortID);
		m_AccountsDBAide.AddParameter(TEXT("@dwLogonType"), pRegisterAccounts->dwLogonType);
		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_VisitorLogin"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		bool bVisitor = true;
		//���⴦��
		if( lResultCode == 100)
		{
			bVisitor = false;
			lResultCode = 0;
		}

		if (DBVarValue.vt==VT_NULL || DBVarValue.vt==VT_EMPTY)
			OnLogonDisposeResult_BCode(dwContextID, lResultCode, TEXT(""), true,bVisitor);
		else
			OnLogonDisposeResult_BCode(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true,bVisitor);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult_BCode(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//��¼�ɹ�
VOID CDataBaseEngineSink::OnLogonDisposeResult_BCode(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bVisitorClient)
{
	if (bMobileClient==false)
	{
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_MB_LogonSuccess_BCode LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//�û�����
			LogonSuccess.wFaceID = m_AccountsDBAide.GetValue_DWORD(TEXT("FaceID"));
			LogonSuccess.cbGender = m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience = m_AccountsDBAide.GetValue_LONGLONG(TEXT("LobbyLV"));
			LogonSuccess.cbMemberOrder = m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));

			//�û��ɼ�
			LogonSuccess.dwUserMedal = m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.lUserScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));		

			//��չ��Ϣ
			LogonSuccess.dwInoutIndex = m_AccountsDBAide.GetValue_DWORD(TEXT("InoutIndex"));
			LogonSuccess.lRevenue = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Revenue"));
			LogonSuccess.dwServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("dwServerID"));

			LogonSuccess.lLobbyLV = 1;

			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"), LogonSuccess.szAccounts, CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"), LogonSuccess.szUnderWrite, CountArray(LogonSuccess.szUnderWrite));

			LogonSuccess.cbVisitorClient = bVisitorClient?1:0;

			//����Ƿ��������
// 			m_AccountsDBAide.ResetParameter();
// 			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),LogonSuccess.dwUserID);
// 
// 			LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_IsLostConnectionLogin"),true);
// 
// 			if(DB_SUCCESS == lResultCode)
// 			{
// 				if(m_AccountsDBModule->IsRecordsetEnd() == false)
// 				{
// 					LogonSuccess.dwServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("dwServerID"));
// 				}
// 			}

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS_BCode, dwContextID, &LogonSuccess, sizeof(LogonSuccess));

		}
		else
		{
			//��������
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.dwResultCode = dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//��־
			//I_TRACE(TEXT("[SocketID:%d] %s"),dwContextID,LogonFailure.szDescribeString);
			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE_BCode,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//��ȡǩ��
bool CDataBaseEngineSink::OnRequestGetSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(CMD_GP_GetUserSignIn));
	if(wDataSize != sizeof(CMD_GP_GetUserSignIn)) return false;
	if(NULL == pData) return false;

	CMD_GP_GetUserSignIn * pGetUserSignIn = (CMD_GP_GetUserSignIn*)pData;

	DBO_GP_GetSignIn pGetSignIn;
	ZeroMemory(&pGetSignIn, sizeof(pGetSignIn));

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pGetUserSignIn->dwUserID);

	WCHAR szDescribe[128]=L"";
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserSignInBCode"),true);

	if(DB_SUCCESS == lResultCode)
	{
		pGetSignIn.wSignInNum = m_AccountsDBAide.GetValue_WORD(TEXT("wSigInNum"));
		pGetSignIn.cbTodayIsSinIn = m_AccountsDBAide.GetValue_BYTE(TEXT("cbTodayIsSinIn"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_SIGNIN, dwContextID, &pGetSignIn, sizeof(DBO_GP_GetSignIn));
	}
	else
	{
		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_GET_USER_SIGNIN);
	}

	PROCESS_END()
}
//����ǩ��
bool CDataBaseEngineSink::OnRequestSetSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(CMD_GP_SetUserSignIn));
	if(wDataSize != sizeof(CMD_GP_SetUserSignIn)) return false;
	if(NULL == pData) return false;

	CMD_GP_SetUserSignIn * pSetUserSignIn = (CMD_GP_SetUserSignIn*)pData;

	DBO_GP_GetSignIn pGetIgnIn = {0};

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pSetUserSignIn->dwUserID);

	WCHAR szDescribe[128]=L"";
	m_AccountsDBAide.AddParameterOutput(TEXT("@ReturnValue"),szDescribe,sizeof(szDescribe),adParamOutput);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_UserSignInBCode"),true);
	pGetIgnIn.dwResultCode = lResultCode;
	if(DB_SUCCESS == lResultCode)
	{
		pGetIgnIn.wSignInNum = m_AccountsDBAide.GetValue_WORD(TEXT("wSignInNum"));
		pGetIgnIn.cbTodayIsSinIn = 1;

		
	}

	CDBVarValue DBVarValue;
	m_AccountsDBModule->GetParameter(TEXT("@ReturnValue"),DBVarValue);
	lstrcpyn(pGetIgnIn.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pGetIgnIn.szDescribeString));

	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SET_SIGNIN, dwContextID, &pGetIgnIn, sizeof(DBO_GP_GetSignIn));
	//OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_SET_USER_SIGNIN);

	//OnReloadScore(dwContextID,pData,wDataSize);

	PROCESS_END()
}

//��ȡ�����Ƿ��Ѿ�ǩ��
bool CDataBaseEngineSink::OnRequestTotayIsSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pUserID = (DWORD*)pData;

	DBO_GP_GetSignIn pGetSignIn = {0};

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),*pUserID);

	WCHAR szDescribe[128]=L"";
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserSignInBCode"),true);

	if(DB_SUCCESS == lResultCode)
	{
		pGetSignIn.wSignInNum = m_AccountsDBAide.GetValue_WORD(TEXT("wSigInNum"));
		pGetSignIn.cbTodayIsSinIn = m_AccountsDBAide.GetValue_BYTE(TEXT("cbTodayIsSinIn"));

		if(pGetSignIn.wSignInNum <5)
		{
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TODAY_IS_SIGNIN, dwContextID, &pGetSignIn, sizeof(DBO_GP_GetSignIn));
		}	
	}
	else
	{
		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_TODAY_IS_SIGNIN);
	}

	PROCESS_END()
}

//��ȡ�û�������Ϣ
bool CDataBaseEngineSink::OnRequestGetUserPerFectInformation(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pUserID = (DWORD*)pData;

	DBO_GP_GetUserPerfectInformation pGetUserPerfectInformation = {0};

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),*pUserID);

	WCHAR szDescribe[128]=L"";
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserPerfectInformation"),true);

	if(DB_SUCCESS == lResultCode)
	{
		if(m_AccountsDBModule->IsRecordsetEnd() == false)
		{
			m_AccountsDBAide.GetValue_String(TEXT("szCompellation"),pGetUserPerfectInformation.szCompellation,CountArray(pGetUserPerfectInformation.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("szIdentifyCode"),pGetUserPerfectInformation.szIdentifyCode,CountArray(pGetUserPerfectInformation.szIdentifyCode));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_USER_PERFECT_INFORMATION, dwContextID, &pGetUserPerfectInformation, sizeof(DBO_GP_GetUserPerfectInformation));
	}
	else
	{
		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_GET_USER_PERFECT_INFORMATION);
	}

	PROCESS_END()
}

//�����û�������Ϣ
bool CDataBaseEngineSink::OnRequestSetUserPerFectInformation(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(CMD_GP_SetUserPerfectInformation));
	if(wDataSize != sizeof(CMD_GP_SetUserPerfectInformation)) return false;
	if(NULL == pData) return false;

	CMD_GP_SetUserPerfectInformation * pSetUserPerfectInformation = (CMD_GP_SetUserPerfectInformation*)pData;

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pSetUserPerfectInformation->dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@szCompellation"),pSetUserPerfectInformation->szCompellation);
	m_AccountsDBAide.AddParameter(TEXT("@szIdentifyCode"),pSetUserPerfectInformation->szIdentifyCode);

	WCHAR szDescribe[128]=L"";
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_SetUserPerfectInformation"),false);

	CDBVarValue DBVarValue;
	m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
	OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_SET_USER_PERFECT_INFORMATION);

	PROCESS_END()
}

//��ȡת����Ϣ
bool CDataBaseEngineSink::OnRequestGetDailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_GetDailInfo));
		if (wDataSize != sizeof(CMD_GP_GetDailInfo)) return false;
		if (NULL == pData) return false;

		CMD_GP_GetDailInfo * pGetDailInfo = (CMD_GP_GetDailInfo*)pData;


		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pGetDailInfo->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pGetDailInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_DailGetInfo"), true);

		if (DB_SUCCESS == lResultCode)
		{
			DBO_GP_GetDailInfo pGetDailInfo;
			ZeroMemory(&pGetDailInfo, sizeof(pGetDailInfo));
			// ����
			pGetDailInfo.dwSilverConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("SilverConsume"));
			pGetDailInfo.dwGoldConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("GoldConsume"));
			pGetDailInfo.dwJewelConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("JewelConsume"));
			//����
			pGetDailInfo.dwDrawScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("DrawScor"));
			pGetDailInfo.dwAddScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("AddScore"));
			//ת����Ϣ
			CString csDailInfo;
			for (BYTE j = 0; j < 8; j++)
			{
				csDailInfo.Format(TEXT("SilverInfo%d"), j);
				pGetDailInfo.dwDailInfo[0][j] = m_TreasureDBAide.GetValue_LONGLONG(csDailInfo);
			}
			for (BYTE j = 0; j < 8; j++)
			{
				csDailInfo.Format(TEXT("GoldCInfo%d"), j);
				pGetDailInfo.dwDailInfo[0][j] = m_TreasureDBAide.GetValue_LONGLONG(csDailInfo);
			}
			for (BYTE j = 0; j < 8; j++)
			{
				csDailInfo.Format(TEXT("JewelInfo%d"), j);
				pGetDailInfo.dwDailInfo[0][j] = m_TreasureDBAide.GetValue_LONGLONG(csDailInfo);
			}
			
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_DAIL_INFO, dwContextID, &pGetDailInfo, sizeof(pGetDailInfo));
		}
		else
		{
			//�������
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
			OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_GET_USER_SIGNIN);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}

//ת��ת�̻�ý���
bool CDataBaseEngineSink::OnRequestRotateDailAward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_RotateDailAward));
		if (wDataSize != sizeof(CMD_GP_RotateDailAward)) return false;
		if (NULL == pData) return false;

		CMD_GP_RotateDailAward * pRotateDailAward = (CMD_GP_RotateDailAward*)pData;


		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pRotateDailAward->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pRotateDailAward->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@dwDailType"), pRotateDailAward->dwDailType);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_RotateDailAward"), true);

		DBO_GP_RotateDailAwardInfo RotateDailAwardInfo;
		ZeroMemory(&RotateDailAwardInfo, sizeof(RotateDailAwardInfo));
		RotateDailAwardInfo.dwResultCode = lResultCode;
		RotateDailAwardInfo.dwUserID = pRotateDailAward->dwUserID;
		RotateDailAwardInfo.dwDailType = pRotateDailAward->dwDailType;
		if (DB_SUCCESS == lResultCode)
		{
			RotateDailAwardInfo.dwDailAward = m_TreasureDBAide.GetValue_LONGLONG(TEXT("AwardSum"));
			RotateDailAwardInfo.dwAwardID = m_TreasureDBAide.GetValue_LONGLONG(TEXT("AwardID"));
			RotateDailAwardInfo.dwConsumeSum = m_TreasureDBAide.GetValue_LONGLONG(TEXT("ConsumeSum"));
			// ����
			RotateDailAwardInfo.dwSilverConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("SilverConsume"));
			RotateDailAwardInfo.dwGoldConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("GoldConsume"));
			RotateDailAwardInfo.dwJewelConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("JewelConsume"));
			//����
			RotateDailAwardInfo.dwDrawScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("DrawScor"));
			RotateDailAwardInfo.dwAddScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("AddScore"));
			//ת����Ϣ
			CString csDailInfo;
			for (BYTE j = 0; j < 8; j++)
			{
				csDailInfo.Format(TEXT("SilverInfo%d"), j);
				RotateDailAwardInfo.dwDailInfo[0][j] = m_TreasureDBAide.GetValue_LONGLONG(csDailInfo);
			}
			for (BYTE j = 0; j < 8; j++)
			{
				csDailInfo.Format(TEXT("GoldCInfo%d"), j);
				RotateDailAwardInfo.dwDailInfo[0][j] = m_TreasureDBAide.GetValue_LONGLONG(csDailInfo);
			}
			for (BYTE j = 0; j < 8; j++)
			{
				csDailInfo.Format(TEXT("JewelInfo%d"), j);
				RotateDailAwardInfo.dwDailInfo[0][j] = m_TreasureDBAide.GetValue_LONGLONG(csDailInfo);
			}
		}
		
		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(RotateDailAwardInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(RotateDailAwardInfo.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ROTATEDAILAWARD, dwContextID, &RotateDailAwardInfo, sizeof(RotateDailAwardInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}

// ��ѯ�����п�
bool CDataBaseEngineSink::OnRequestChickBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_ChickBindBank));
		if (wDataSize != sizeof(CMD_GP_ChickBindBank)) return false;
		if (NULL == pData) return false;

		CMD_GP_ChickBindBank * pGetDailInfo = (CMD_GP_ChickBindBank*)pData;


		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pGetDailInfo->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pGetDailInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_BindBankChick"), true);

		DBO_GP_BindBankInfo pBindBankInfo;
		ZeroMemory(&pBindBankInfo, sizeof(pBindBankInfo));
		pBindBankInfo.wSubCommandID = pGetDailInfo->wSubCommandID;
		pBindBankInfo.dwResultCode = lResultCode;
		if (DB_SUCCESS == lResultCode)
		{
			m_TreasureDBAide.GetValue_String(TEXT("BankName"), pBindBankInfo.szBankName, CountArray(pBindBankInfo.szBankName));
			m_TreasureDBAide.GetValue_String(TEXT("BankCardNum"), pBindBankInfo.szBankCardNum, CountArray(pBindBankInfo.szBankCardNum));
			m_TreasureDBAide.GetValue_String(TEXT("AccountHolder"), pBindBankInfo.szAccountHolder, CountArray(pBindBankInfo.szAccountHolder));
		}

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(pBindBankInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pBindBankInfo.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHICKBINDBANK, dwContextID, &pBindBankInfo, sizeof(pBindBankInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}
// �����п�
bool CDataBaseEngineSink::OnRequestBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_BindBankCard));
		if (wDataSize != sizeof(CMD_GP_BindBankCard)) return false;
		if (NULL == pData) return false;

		CMD_GP_BindBankCard * pBindBankCard = (CMD_GP_BindBankCard*)pData;


		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pBindBankCard->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pBindBankCard->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@szBankName"), pBindBankCard->szBankName);
		m_TreasureDBAide.AddParameter(TEXT("@szBankCardNum"), pBindBankCard->szBankCardNum);
		m_TreasureDBAide.AddParameter(TEXT("@szAccountHolder"), pBindBankCard->szAccountHolder);

		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_BindBankCard"), true);

		DBO_GP_BindBankInfo pBindBankInfo;
		ZeroMemory(&pBindBankInfo, sizeof(pBindBankInfo));
		pBindBankInfo.wSubCommandID = pBindBankCard->wSubCommandID;
		pBindBankInfo.dwResultCode = lResultCode;
		if (DB_SUCCESS == lResultCode)
		{
			m_TreasureDBAide.GetValue_String(TEXT("szBankName"), pBindBankInfo.szBankName, CountArray(pBindBankInfo.szBankName));
			m_TreasureDBAide.GetValue_String(TEXT("szBankCardNum"), pBindBankInfo.szBankCardNum, CountArray(pBindBankInfo.szBankCardNum));
			m_TreasureDBAide.GetValue_String(TEXT("szAccountHolder"), pBindBankInfo.szAccountHolder, CountArray(pBindBankInfo.szAccountHolder));
		}

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(pBindBankInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pBindBankInfo.szDescribeString));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHICKBINDBANK, dwContextID, &pBindBankInfo, sizeof(pBindBankInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}
// ��ѯ��֧����
bool CDataBaseEngineSink::OnRequestChickBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_ChickBindZFB));
		if (wDataSize != sizeof(CMD_GP_ChickBindZFB)) return false;
		if (NULL == pData) return false;

		CMD_GP_ChickBindZFB * pChickBindZFB = (CMD_GP_ChickBindZFB*)pData;


		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pChickBindZFB->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pChickBindZFB->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_BindZFBChick"), true);

		DBO_GP_BindZFBInfo pBindZFBInfo;
		ZeroMemory(&pBindZFBInfo, sizeof(pBindZFBInfo));
		pBindZFBInfo.wSubCommandID = pChickBindZFB->wSubCommandID;
		pBindZFBInfo.dwResultCode = lResultCode;
		if (DB_SUCCESS == lResultCode)
		{
			m_TreasureDBAide.GetValue_String(TEXT("szZFBName"), pBindZFBInfo.szZFBName, CountArray(pBindZFBInfo.szZFBName));
			m_TreasureDBAide.GetValue_String(TEXT("szNickName"), pBindZFBInfo.szNickName, CountArray(pBindZFBInfo.szNickName));
		}

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(pBindZFBInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pBindZFBInfo.szDescribeString));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHICKBINDZFB, dwContextID, &pBindZFBInfo, sizeof(pBindZFBInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}
// ��֧����
bool CDataBaseEngineSink::OnRequestBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_BindZFB));
		if (wDataSize != sizeof(CMD_GP_BindZFB)) return false;
		if (NULL == pData) return false;

		CMD_GP_BindZFB * pBindZFB = (CMD_GP_BindZFB*)pData;


		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pBindZFB->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pBindZFB->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@szZFBName"), pBindZFB->szZFBName);
		m_TreasureDBAide.AddParameter(TEXT("@szNickName"), pBindZFB->szNickName);
		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_BindZFB"), true);

		DBO_GP_BindZFBInfo pBindZFBInfo;
		ZeroMemory(&pBindZFBInfo, sizeof(pBindZFBInfo));
		pBindZFBInfo.wSubCommandID = pBindZFB->wSubCommandID;
		pBindZFBInfo.dwResultCode = lResultCode;
		if (DB_SUCCESS == lResultCode)
		{
			m_TreasureDBAide.GetValue_String(TEXT("szZFBName"), pBindZFBInfo.szZFBName, CountArray(pBindZFBInfo.szZFBName));
			m_TreasureDBAide.GetValue_String(TEXT("szNickName"), pBindZFBInfo.szNickName, CountArray(pBindZFBInfo.szNickName));
		}

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(pBindZFBInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pBindZFBInfo.szDescribeString));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHICKBINDZFB, dwContextID, &pBindZFBInfo, sizeof(pBindZFBInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}

// �����ʼ�
bool CDataBaseEngineSink::OnRequestRequestMail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_RequestMail));
		if (wDataSize != sizeof(CMD_GP_RequestMail)) return false;
		if (NULL == pData) return false;

		CMD_GP_RequestMail * pRequestMail = (CMD_GP_RequestMail*)pData;


		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pRequestMail->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pRequestMail->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		
		//ִ�в�ѯ
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_RequestMail"), true);

		DBO_GP_RequestMailToal RequestMailToal;
		ZeroMemory(&RequestMailToal, sizeof(RequestMailToal));
		RequestMailToal.dwUserID = pRequestMail->dwUserID;
		if (DB_SUCCESS == lResultCode)
		{
			BYTE MailCount = 0;
			//��ȡ��Ϣ
			while (m_TreasureDBModule->IsRecordsetEnd() == false)
			{
				RequestMailToal.stRequestMail[MailCount].dwMailIndex = m_TreasureDBAide.GetValue_DWORD(TEXT("MailIndex"));
				RequestMailToal.stRequestMail[MailCount].dwStartUsing = m_TreasureDBAide.GetValue_DWORD(TEXT("StartUsing"));
				m_TreasureDBAide.GetValue_String(TEXT("MailContent"), RequestMailToal.stRequestMail[MailCount].szMailContent, 
					CountArray(RequestMailToal.stRequestMail[MailCount].szMailContent));
				MailCount++;
				//�ƶ���¼
				m_TreasureDBModule->MoveToNext();
			};
			RequestMailToal.dwMailCount = MailCount;
		}
		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(RequestMailToal.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(RequestMailToal.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_REQUESTMAIL, dwContextID, &RequestMailToal, sizeof(RequestMailToal));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}
// ��ȡɾ���ʼ�
bool CDataBaseEngineSink::OnRequestReadDeleteMail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_ReadDeleteMail));
		if (wDataSize != sizeof(CMD_GP_ReadDeleteMail)) return false;
		if (NULL == pData) return false;

		CMD_GP_ReadDeleteMail * pReadDeleteMail = (CMD_GP_ReadDeleteMail*)pData;


		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pReadDeleteMail->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pReadDeleteMail->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@dwOpenType"), pReadDeleteMail->dwOpenType);
		m_TreasureDBAide.AddParameter(TEXT("@dwMailIndex"), pReadDeleteMail->dwMailIndex);

		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_ReadDeleteMail"), true);

		DBO_GP_ReadDeleteMailToal ReadDeleteMailToal;
		ZeroMemory(&ReadDeleteMailToal, sizeof(ReadDeleteMailToal));
		ReadDeleteMailToal.dwUserID = pReadDeleteMail->dwUserID;
		ReadDeleteMailToal.dwOpenType = pReadDeleteMail->dwOpenType;
		ReadDeleteMailToal.dwResultCode = lResultCode;
		if (DB_SUCCESS == lResultCode)
		{
			BYTE MailCount = 0;
			//��ȡ��Ϣ
			while (m_TreasureDBModule->IsRecordsetEnd() == false)
			{
				ReadDeleteMailToal.stRequestMail[MailCount].dwMailIndex = m_TreasureDBAide.GetValue_DWORD(TEXT("MailIndex"));
				ReadDeleteMailToal.stRequestMail[MailCount].dwStartUsing = m_TreasureDBAide.GetValue_DWORD(TEXT("StartUsing"));
				m_TreasureDBAide.GetValue_String(TEXT("MailContent"), ReadDeleteMailToal.stRequestMail[MailCount].szMailContent,
					CountArray(ReadDeleteMailToal.stRequestMail[MailCount].szMailContent));
				MailCount++;
				//�ƶ���¼
				m_TreasureDBModule->MoveToNext();
			};
			ReadDeleteMailToal.dwMailCount = MailCount;
		}
		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(ReadDeleteMailToal.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(ReadDeleteMailToal.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_READDELEETETAIL, dwContextID, &ReadDeleteMailToal, sizeof(ReadDeleteMailToal));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}

//�Ƿ����ƹ�ע��
bool CDataBaseEngineSink::OnRequestIsSpreadRegister(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pUserID = (DWORD*)pData;

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), *pUserID);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_IsSpreadRegister"), true);

	if(DB_SUCCESS == lResultCode)
	{
		DBO_GP_IsSpreadRegister pIsSpreadRegister = {0};

		pIsSpreadRegister.dwRewardScore = m_AccountsDBAide.GetValue_DWORD(TEXT("dwRewardScore"));
		pIsSpreadRegister.cbIsSpreadRegister = m_AccountsDBAide.GetValue_BYTE(TEXT("cbIsSpreadRegister"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_IS_SPREAD_REGISTER, dwContextID, &pIsSpreadRegister,sizeof(DBO_GP_IsSpreadRegister));
	}
	else
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_IS_TODAY_FIRST_TIME_LOGIN, dwContextID, NULL,0);
	}

	PROCESS_END()
}

//���û��ƹ�
bool CDataBaseEngineSink::OnRequestBindUserSpread(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(CMD_GP_BindUserSpread));
	if(wDataSize != sizeof(CMD_GP_BindUserSpread)) return false;
	if(NULL == pData) return false;

	CMD_GP_BindUserSpread * pBindUserSpread = (CMD_GP_BindUserSpread*)pData;

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pBindUserSpread->dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwSpreadID"),pBindUserSpread->dwSpreadID);

	WCHAR szDescribe[128]=L"";
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_BindUserSpreadID"),true);

	if (DB_SUCCESS == lResultCode)
	{
		DWORD dwBindRewardScore = m_AccountsDBAide.GetValue_DWORD(TEXT("dwBindRewardScore"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BIND_USER_SPREAD,dwContextID,&dwBindRewardScore,sizeof(DWORD));

		DWORD UserID = pBindUserSpread->dwUserID;

		OnReloadScore(dwContextID,&UserID,sizeof(DWORD));
	}
	else
	{
		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_BIND_USER_SPREAD);
	}

	PROCESS_END()
}

//�Ƿ��Ѿ����ƹ�
bool CDataBaseEngineSink::OnRequestIsBindSpread(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pUserID = (DWORD*)pData;

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),*pUserID);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_IsBindSpread"),false);

	if(DB_SUCCESS == lResultCode)
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_IS_BIND_SPREAD, dwContextID, NULL,0);
	}

	PROCESS_END()
}

//�Ƿ��Ѿ��޸Ĺ��ǳ�
bool CDataBaseEngineSink::OnRequestIsAlreadyModifyNickName(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pUserID = (DWORD*)pData;

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),*pUserID);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_IsAlreadyModifyNickName"),false);

	if(DB_SUCCESS == lResultCode)
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_IS_ALREADY_MODIFY_NICKNAME, dwContextID, NULL,0);
	}

	PROCESS_END()
}

//�Ƿ��ǽ����һ�ε�¼
bool CDataBaseEngineSink::OnRequestIsTodayFirstTimeLogin(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pUserID = (DWORD*)pData;

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),*pUserID);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_IsTodayFirstTimeLogin"),false);

	if(DB_SUCCESS == lResultCode)
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_IS_TODAY_FIRST_TIME_LOGIN, dwContextID, NULL,0);
	}

	PROCESS_END()
}

//�Ƿ��ǵ������µ�¼
bool CDataBaseEngineSink::OnRequestIsLostConnectionLogin(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pUserID = (DWORD*)pData;

	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),*pUserID);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_IsLostConnectionLogin"),true);

	if(DB_SUCCESS == lResultCode)
	{
		if(m_AccountsDBModule->IsRecordsetEnd() == false)
		{
			DWORD dwServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("dwServerID"));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_IS_LOST_CONNECTION_LOGIN, dwContextID, &dwServerID,sizeof(DWORD));
		}
	}

	PROCESS_END()
}

//�ͻ��˵�¼���
bool CDataBaseEngineSink::OnRequestClientLoginSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//ǩ��
	OnRequestTotayIsSignIn(dwContextID,pData,wDataSize);
	//�ƹ�ע��
	//OnRequestIsSpreadRegister(dwContextID,pData,wDataSize);
	//�����һ�ε�¼
	//OnRequestIsTodayFirstTimeLogin(dwContextID,pData,wDataSize);

	return true;
}

//�������
bool CDataBaseEngineSink::OnRequestKillLocker(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(CMD_CS_S_ServerRemove));
	if (wDataSize != sizeof(CMD_CS_S_ServerRemove)) return false;
	if (NULL == pData) return false;

	//��������
	CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

	//�������
	m_TreasureDBAide.ResetParameter();
	m_TreasureDBAide.AddParameter(TEXT("@wServerID"),pServerRemove->wServerID);

	//�������
	LONG lReturnValue=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_KillGameScoreLocker"),false);

	PROCESS_END()

}

//��ȡ������Ϣ
bool CDataBaseEngineSink::OnLoadScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	DWORD dwScrollType;

	srand(time(NULL));
	dwScrollType = rand() % 4;
	if (dwScrollType == 0)
	{
		dwScrollType = 1;
	}

	m_PlatformDBAide.ResetParameter();
	m_PlatformDBAide.AddParameter(TEXT("@dwScrollType"),dwScrollType);
	m_PlatformDBAide.AddParameter(TEXT("@dwServerID"),0);
	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_LoadScrollMessage"),true);

	if(DB_SUCCESS == lResultCode)
	{
		while(m_PlatformDBModule->IsRecordsetEnd() == false)
		{
			DBO_GR_ScrollMessage pScrollMessage = {0};

			m_PlatformDBAide.GetValue_String(TEXT("szScrollMessage"),pScrollMessage.szScrollMessage,CountArray(pScrollMessage.szScrollMessage));
			
			pScrollMessage.wLength = CountStringBuffer(pScrollMessage.szScrollMessage) / 2;

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOAD_SCROLL_MESSAGE,dwContextID,&pScrollMessage,sizeof(DBO_GR_ScrollMessage));

			break;
		}
	}

	PROCESS_END()
}

//���������Ϣ
bool CDataBaseEngineSink::OnClearScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	m_PlatformDBAide.ResetParameter();
	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_ClearScrollMessage"),false);

	PROCESS_END()
}


//////////////////////////////////////////////////////////////////////////