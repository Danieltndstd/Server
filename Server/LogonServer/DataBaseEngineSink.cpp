#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineBegin(IUnknownEx * pIUnknownEx)
{
	//�󶨺���
	{
		// ��¼�ӿ�
		{
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_GAMEID, CDataBaseEngineSink::OnRequestLogonGameID, this);	// ��ʶ��¼
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_ACCOUNTS, CDataBaseEngineSink::OnRequestLogonAccounts, this);	// �˺ŵ�¼
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_REGISTER_ACCOUNTS, CDataBaseEngineSink::OnRequestRegisterAccounts, this);	// ע���˺�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_WEB_REGISTER_ACCOUNTS, CDataBaseEngineSink::OnRequestWebRegisterAccounts, this);	// Web ע���˺�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_ThirdParty, CDataBaseEngineSink::OnRequesLogonThirdParty, this);	// ������¼
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_REGISTER_THIRD, CDataBaseEngineSink::OnRequestRegisterThirdParty, this);	// ����ע��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_Third, CDataBaseEngineSink::OnRequestLogonThird, this);
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_OUT, CDataBaseEngineSink::OnRequestLogonOut, this);
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_BIND_VISITOR_ACCOUNT, CDataBaseEngineSink::OnRequestBindVisitorAccount, this);	// �ο�ת��

			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_LOGON_GAMEID, CDataBaseEngineSink::OnMobileLogonGameID, this);	// ��ʶ��¼
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_LOGON_ACCOUNTS, CDataBaseEngineSink::OnMobileLogonAccounts, this);	// �ʺŵ�¼
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_REGISTER_ACCOUNTS, CDataBaseEngineSink::OnMobileRegisterAccounts, this);	// ע���ʺ�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_REGISTER_MOBILE, CDataBaseEngineSink::OnMobileRegisterMobile, this);	// �ֻ�ע��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_VISITOR_ACCOUNTS, CDataBaseEngineSink::OnMobileVisitorAccounts, this);	// �ο͵�¼
		}
		
		// �û�����
		{
			//�˺�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_MACHINE, CDataBaseEngineSink::OnRequestModifyMachine, this);	// �޸Ļ���
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_LOGON_PASS, CDataBaseEngineSink::OnRequestModifyLogonPass, this);	// �޸ĵ�¼����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_INSURE_PASS, CDataBaseEngineSink::OnRequestModifyInsurePass, this);	// �޸�����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_UNDER_WRITE, CDataBaseEngineSink::OnRequestModifyUnderWrite, this);	// �޸�ǩ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_NICKNAME, CDataBaseEngineSink::OnRequestModifyNickName, this);	// �޸��ǳ�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_INDIVIDUAL, CDataBaseEngineSink::OnRequestModifyIndividual, this);	// �޸�����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_PW_RECOVERY, CDataBaseEngineSink::OnRequestRecoveryPass, this);	// �һ�����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_BIND_MOBILE, CDataBaseEngineSink::OnRequestBindMobilePhone, this);	// ���ֻ�

			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_RETRIEVE_PASSWORD, CDataBaseEngineSink::OnRequestRetrievePassword, this);	// ���һ����루������֤�˺ţ���֤���빦�ܣ�
			//ͷ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GET_AVATAR_INFO, CDataBaseEngineSink::OnRequestGetAvatarFaceInfo, this);	// ��ȡͷ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GET_IMAGE_INFO, CDataBaseEngineSink::OnRequestGetAvatarFaceInfo, this);	// ��ȡ����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_AVATAR, CDataBaseEngineSink::OnRequestModifyAvatar, this);	// �޸�ͷ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_IMAGE, CDataBaseEngineSink::OnRequestModifyImage, this);	// �޸�����
			//����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_USER_SAVE_SCORE, CDataBaseEngineSink::OnRequestUserSaveScore, this);	// ����Ԫ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_USER_TAKE_SCORE, CDataBaseEngineSink::OnRequestUserTakeScore, this);	// ��ȡԪ��
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_USER_TRANSFER_SCORE, CDataBaseEngineSink::OnRequestUserTransferScore, this);// ת��Ԫ��


			//��ѯ
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_QUERY_INDIVIDUAL, CDataBaseEngineSink::OnRequestQueryIndividual, this);	// ��ѯ����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_QUERY_INSURE_INFO, CDataBaseEngineSink::OnRequestQueryInsureInfo, this);	// ��ѯ����
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_QUERY_USER_INFO, CDataBaseEngineSink::OnRequestQueryTransferUserInfo, this);	// ��ѯ�û�

			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_RELOAD_SCORE, CDataBaseEngineSink::OnReloadScore, this);	//���»���

		}
		// �����¼�
		{
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_COLLECTION, CDataBaseEngineSink::OnRequestCollection, this);	// �ղ��б�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOAD_GAME_LIST, CDataBaseEngineSink::OnRequestLoadGameList, this);	// �����б�
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_ONLINE_COUNT_INFO, CDataBaseEngineSink::OnRequestOnLineCountInfo, this);	// ������Ϣ
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_CONTROL_ROOM_STATUS, CDataBaseEngineSink::OnRequestOpenRoomStatus, this);  //�򿪷���
		}
	}

	return true;
}

//�����¼�
// bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
// {
// 	bool bResult = false;
// 
// 	do 
// 	{
// 		// Ѱ�Ҵ�����
// 		auto it = m_mFunctions.find(wRequestID);
// 		if(it == m_mFunctions.end())
// 			break;
// 
// 		// ����
// 		bResult = (it->second)(dwContextID, pData, wDataSize);
// 
// 	} while (0);
// 
// 	return bResult;
// }

//I D ��¼
bool CDataBaseEngineSink::OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_LogonGameID));
		if (wDataSize!=sizeof(DBR_GP_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_GP_LogonGameID * pLogonGameID=(DBR_GP_LogonGameID *)pData;

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
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonGameID->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//������¼
bool CDataBaseEngineSink::OnRequestLogonThird(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_LogonThird));
		if (wDataSize!=sizeof(DBR_GP_LogonThird)) return false;

		//������
		DBR_GP_LogonThird * pLogonAccounts=(DBR_GP_LogonThird *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pLogonAccounts->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyThird"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}


//�ʺŵ�¼
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_LogonAccounts));
		if (wDataSize!=sizeof(DBR_GP_LogonAccounts)) return false;

		//������
		DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

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
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//������¼
bool CDataBaseEngineSink::OnRequesLogonThirdParty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//Ч�����
		ASSERT_ERR(wDataSize == sizeof(DBR_GP_LogonThirdParty));
		if (wDataSize != sizeof(DBR_GP_LogonThirdParty)) return false;

		//������
		DBR_GP_LogonThirdParty * pThirdParty = (DBR_GP_LogonThirdParty *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@Accounts"), pThirdParty->szAccounts);

		//ִ�в�ѯ
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ThirdPartyAccounts"), true) == DB_SUCCESS)
		{
			unsigned short recordCount = m_AccountsDBModule->GetRecordCount();

			if (recordCount == 0) //��ע��
			{
				time_t rawtime;
				struct tm * timeinfo;

				time(&rawtime);
				timeinfo = localtime(&rawtime);

				TCHAR Accounts[LEN_ACCOUNTS];
				wsprintf(Accounts, L"%s%d%d%d", L"fhly", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				TCHAR PassWord[LEN_MD5] = { L"119781010" };
				TCHAR md5PassWord[LEN_MD5] = { 0 };
				CWHEncrypt::MD5Encrypt(PassWord, md5PassWord);

				CMD_GP_RegisterThird registerAccounts;
				memset(&registerAccounts, 0, sizeof(CMD_GP_RegisterThird));
				registerAccounts.dwPlazaVersion = VERSION_PLAZA;
				lstrcpyn(registerAccounts.szAccounts, Accounts, CountArray(registerAccounts.szAccounts));
				lstrcpyn(registerAccounts.szLogonPass, md5PassWord, CountArray(registerAccounts.szLogonPass));
				lstrcpyn(registerAccounts.szInsurePass, md5PassWord, CountArray(registerAccounts.szInsurePass));
				lstrcpyn(registerAccounts.szNickName, Accounts, CountArray(registerAccounts.szNickName));
				lstrcpyn(registerAccounts.szMachineID, md5PassWord, CountArray(registerAccounts.szMachineID));
				registerAccounts.cbValidateFlags = MB_VALIDATE_FLAGS;
				lstrcpyn(registerAccounts.szValidateCode, L"L535", CountArray(registerAccounts.szValidateCode));
				lstrcpyn(registerAccounts.szThirdParty, pThirdParty->szAccounts, CountArray(registerAccounts.szThirdParty));

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_THIRD_REGISTER, dwContextID, &registerAccounts, sizeof(registerAccounts));
			}
			else if (recordCount == 1) //��ע��
			{
				CMD_GP_LogonAccounts logonAccounts;
				memset(&logonAccounts, 0, sizeof(CMD_GP_LogonAccounts));
				logonAccounts.dwPlazaVersion = VERSION_PLAZA;
				logonAccounts.cbValidateFlags = MB_VALIDATE_FLAGS;
				m_AccountsDBAide.GetValue_String(TEXT("Accounts"), logonAccounts.szAccounts, CountArray(logonAccounts.szAccounts));
				m_AccountsDBAide.GetValue_String(TEXT("LogonPass"), logonAccounts.szPassword, CountArray(logonAccounts.szPassword));
				m_AccountsDBAide.GetValue_String(TEXT("RegisterMachine"), logonAccounts.szMachineID, CountArray(logonAccounts.szMachineID));

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_THIRD_LOGON, dwContextID, &logonAccounts, sizeof(logonAccounts));
			}
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID, m_PlatformDBAide.GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_LOGON_THIRDPARTY);
		}

	PROCESS_END()
}

//ע�ᴦ��
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//������
		DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

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
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

// Webע��
bool CDataBaseEngineSink::OnRequestWebRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize == sizeof(DBR_GP_Web_RegisterAccounts));
		if (wDataSize != sizeof(DBR_GP_Web_RegisterAccounts)) return false;

		//������
		DBR_GP_Web_RegisterAccounts * pWebRegisterAccounts = (DBR_GP_Web_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter = (tagBindParameter *)pWebRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID != dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pWebRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"), pWebRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"), pWebRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"), pWebRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"), pWebRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_Web_RegisterAccounts"), true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnLogonDisposeWebResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnLogonDisposeWebResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"), false);

		return false;
	}

	return true;
}

//������ע�ᴦ��
bool CDataBaseEngineSink::OnRequestRegisterThirdParty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_RegisterThird));
		if (wDataSize!=sizeof(DBR_GP_RegisterThird)) return false;

		//������
		DBR_GP_RegisterThird * pRegisterAccounts=(DBR_GP_RegisterThird *)pData;

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
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		DBR_GP_Insert_Accounts insertAccounts;
		lstrcpyn(insertAccounts.szThirdAccounts, pRegisterAccounts->szThirdParty, CountArray(insertAccounts.szThirdAccounts));
		lstrcpyn(insertAccounts.szAccounts, pRegisterAccounts->szAccounts, CountArray(insertAccounts.szAccounts));
		lstrcpyn(insertAccounts.szLogonPass, pRegisterAccounts->szLogonPass, CountArray(insertAccounts.szLogonPass));

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonThirdPartyResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false, insertAccounts);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//I D ��¼
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
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
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
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
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_ACCOUNT);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_MB_EfficacyAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺ�ע��
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_MB_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

		//������
		DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

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
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),"wy1112");  //����ƹ����ֶ�
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_ACCOUNT);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_MB_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ֻ�ע��
bool CDataBaseEngineSink::OnMobileRegisterMobile(DWORD dwContextID, VOID * pData, WORD wDataSize)
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
		OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"), true);

		return false;
	}
}

//�޸Ļ���
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_ModifyMachine));
		if (wDataSize!=sizeof(DBR_GP_ModifyMachine)) return false;

		//������
		DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�󶨲���
		if (pModifyMachine->cbBind==TRUE)
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
		}
		else
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
		}

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, m_AccountsDBModule->GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_MACHINE);

	PROCESS_END()
}

// ��ȡͷ��/����
bool CDataBaseEngineSink::OnRequestGetAvatarFaceInfo(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		DWORD* pUserID = (DWORD*)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("dwUserID"), *pUserID);

		// ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserAvatar"), true);

		// �����
		BYTE cbBuffer[10240] = { 0 };
		WORD wPacketSize = 0;
		DBO_GP_AvatarInfo* pAvatarInfo = NULL;
		while (m_AccountsDBModule->IsRecordsetEnd() == false)
		{
			//������Ϣ
			if ((wPacketSize + sizeof(DBO_GP_AvatarInfo)) > sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_AVATAR_INFO, dwContextID, cbBuffer, wPacketSize);
				wPacketSize = 0;
			}

			//��ȡ��Ϣ
			pAvatarInfo = (DBO_GP_AvatarInfo*)(cbBuffer + wPacketSize);
			pAvatarInfo->dwAvatarID = m_AccountsDBAide.GetValue_DWORD(TEXT("AvatarID"));
			pAvatarInfo->cbAvatarType = m_AccountsDBAide.GetValue_BYTE(TEXT("AvatarType"));
			pAvatarInfo->dwRemainTime = m_AccountsDBAide.GetValue_DWORD(TEXT("dwRemainTime"));

			//����λ��
			wPacketSize += sizeof(DBO_GP_AvatarInfo);

			//�ƶ���¼
			m_AccountsDBModule->MoveToNext();
		}
		if (wPacketSize > 0)
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_AVATAR_INFO, dwContextID, cbBuffer, wPacketSize);

	PROCESS_END()
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifyAvatar(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//������
		CMD_GP_ModifyAvatar* pModifyAvatar = (CMD_GP_ModifyAvatar*)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyAvatar->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwFaceID"), pModifyAvatar->dwAvatarID);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribe, sizeof(szDescribe), adParamOutput);

		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_SetUserFace"), true);

		if (lResultCode == 0)
		{
			DWORD dwFaceID=pModifyAvatar->dwAvatarID;

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MODIFY_AVATAR, dwContextID, &dwFaceID, sizeof(DWORD));
		}
		else
		{

			//�������
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
			OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_AVATAR);
		}



	PROCESS_END()
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyImage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//������
		CMD_GP_ModifyImage* pModifyImage = (CMD_GP_ModifyImage*)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyImage->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwAvatarID"), pModifyImage->dwImageID);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribe, sizeof(szDescribe), adParamOutput);

		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_SetUserAvatar"), true);

		if(lResultCode == 0)
		{
			DWORD dwImageID=pModifyImage->dwImageID;

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MODIFY_IMAGE, dwContextID, &dwImageID, sizeof(DWORD));
		}
		else
		{
			//�������
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
			OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_IMAGE);
		}

	PROCESS_END()
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
		if (wDataSize!=sizeof(DBR_GP_ModifyLogonPass)) return false;

		//������
		DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pModifyLogonPass->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_ModifyLogonPass"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_LOGON_PASS);

	PROCESS_END()
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
		if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

		//������
		DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_INSURE_PASS);

	PROCESS_END()
}



//�޸�ǩ��
bool CDataBaseEngineSink::OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//������
		CMD_GP_ModifyUnderWrite* pModifyUnderWrite = (CMD_GP_ModifyUnderWrite*)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyUnderWrite->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"), pModifyUnderWrite->szUnderWrite);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_ModifyUnderWrite"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_UNDER_WRITE);

	PROCESS_END()
}

//�޸��ǳ�
bool CDataBaseEngineSink::OnRequestModifyNickName(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//������
		CMD_GP_ModifyNickName* pModifyNickName = (CMD_GP_ModifyNickName*)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyNickName->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@szNickName"), pModifyNickName->szNickName);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribe, sizeof(szDescribe), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_ModifyNickName"), false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_NICKNAME);

	PROCESS_END()
}

//���ֻ�
bool CDataBaseEngineSink::OnRequestBindMobilePhone(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//Ч�����
		ASSERT_ERR(wDataSize == sizeof(DBR_GP_BindMobile));
		if (wDataSize != sizeof(DBR_GP_BindMobile)) return false;

		//������
		DBR_GP_BindMobile * pBindMobile = (DBR_GP_BindMobile *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pBindMobile->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobile"), pBindMobile->szMobliePhone);
		m_AccountsDBAide.AddParameter(TEXT("@szPassword"),pBindMobile->szPassword);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@ReturnValue"), szDescribe, sizeof(szDescribe), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_BindMobile"), false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@ReturnValue"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_BIND_MOBILE_PHONE);

	PROCESS_END()
}



// �޸�����
bool CDataBaseEngineSink::OnRequestRecoveryPass(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//������
		CMD_GP_ModifyPass * pModifyPass = (CMD_GP_ModifyPass *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"), pModifyPass->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strIdentity"), TEXT(""));
		m_AccountsDBAide.AddParameter(TEXT("@strMoblie"), TEXT(""));
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"), pModifyPass->szPassword);	
		m_AccountsDBAide.AddParameter(TEXT("@wRecoverType"), 1);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribe, sizeof(szDescribe), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_RecoverPass"), true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_PW_RECOVERY);

	PROCESS_END()
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_ModifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

		//������
		DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"), pModifyIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"), pModifyIndividual->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strPassportID"), pModifyIndividual->szPassportID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"), pModifyIndividual->szMobilePhone);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_SetUserBaseInfo"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_INDIVIDUAL);

	PROCESS_END()
}

//ͨ������ˢ��
bool CDataBaseEngineSink::OnUpdateAttribute(DWORD dwContextID, DBO_UPDATE_ATTRIBUTE::attribute attr,SCORE wScore )
{
	DBO_UPDATE_ATTRIBUTE attrJewel={0};
	attrJewel.wType = attr;
	attrJewel.wScore = wScore;
	return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_UPDATE_ATTRIBUTE, dwContextID, &attrJewel, sizeof(DBO_UPDATE_ATTRIBUTE));
}

//����Ԫ��
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_UserSaveScore));
		if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

		//������
		DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_UserSaveScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ȡԪ��
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_UserTakeScore));
		if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

		//������
		DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_UserTakeScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//ת��Ԫ��
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_UserTransferScore));
		if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

		//������
		DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pUserTransferScore->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szNickName);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//������
		DWORD* dwUserID = (DWORD*)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), *dwUserID);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserBaseInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserIndividual UserIndividual = { 0 };

			//�û���Ϣ
			m_AccountsDBAide.GetValue_String(TEXT("PassPortID"), UserIndividual.szPassportID, CountArray(UserIndividual.szPassportID));
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"), UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("RegisterMobile"), UserIndividual.szMobilePhone, CountArray(UserIndividual.szMobilePhone));
			m_AccountsDBAide.GetValue_String(TEXT("Email"),UserIndividual.szEmail,CountArray(UserIndividual.szEmail));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnOperateDisposeResult(dwContextID, m_AccountsDBAide.GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_QUERY_INDIVIDUAL);
		}

	PROCESS_END()
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//Ч�����
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

		//������
		DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//������Ϣ
			UserInsureInfo.dwUserID = pQueryInsureInfo->dwUserID;
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnOperateDisposeResult(dwContextID, m_TreasureDBAide.GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_QUERY_INSURE_INFO);
		}

	PROCESS_END()
}

//��ѯ�û�
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//������
		CMD_GP_QueryUserInfoRequest * pQueryTransferUserInfo = (CMD_GP_QueryUserInfoRequest *)pData;

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pQueryTransferUserInfo->szNickName);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//������Ϣ
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), TransferUserInfo.szNickName, CountArray(TransferUserInfo.szNickName));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//�����б�
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//��������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));
			

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameType);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));
			m_PlatformDBAide.GetValue_String(TEXT("DownLoadUrl"),pGameKind->szDownLoadUrl,CountArray(pGameKind->szDownLoadUrl));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ�ڵ�
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

		//���ͽڵ�
		wPacketSize=0;
		DBO_GP_GameNode * pGameNode=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameNode))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
			pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameNode);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

		//���Ͷ���
		wPacketSize=0;
		DBO_GP_GamePage * pGamePage=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GamePage))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
			pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
			pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
			m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GamePage);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=TRUE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		TRACE_LOG(pszDescribe,TraceLevel_Exception);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=FALSE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return false;
	}

	return true;
}

//������Ϣ
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//��������
		DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
		WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

		//Ч������
		ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind))));
		if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

		//������ʶ
		TCHAR szMachineID[LEN_MACHINE_ID];
		CWHService::GetMachineID(szMachineID);

		//������Ϣ
		TCHAR szOnLineCountKind[2048]=TEXT("");
		for (WORD i=0;i<pOnLineCountInfo->wKindCount;i++)
		{
			INT nLength=lstrlen(szOnLineCountKind);
			_sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,
				pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
		}

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
		m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);

		//ִ������
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

	PROCESS_END()
}

//�򿪷���
bool CDataBaseEngineSink::OnRequestOpenRoomStatus(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	DBR_GP_ControlRoomStauts *pControl = (DBR_GP_ControlRoomStauts*)pData;
	m_PlatformDBAide.ResetParameter();
	m_PlatformDBAide.AddParameter(TEXT("@wServerID"),pControl->wServerID);
	m_PlatformDBAide.AddParameter(TEXT("@cbIsOpen"),pControl->cbIsOpen);

	m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_UpdateRoomStatus"),true);

	PROCESS_END()
}

//�ղ��б�
bool CDataBaseEngineSink::OnRequestCollection(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		CMD_GP_Collection* pCollection = (CMD_GP_Collection*)pData;
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@Flag"),pCollection->wFlag);
		m_PlatformDBAide.AddParameter(TEXT("@UserID"),pCollection->wUserID);
		if(pCollection->wFlag == COLLECTION_MODIFY)
		{
			CString SortIDStr,DisplayNameStr;
			CHAR buf[256] = {0};
			for(int i = 0; i < pCollection->wCollectionCount; ++i)
			{
				SortIDStr += itoa(pCollection->Collection[i].wSortID,buf,10);
				SortIDStr += L",";
				DisplayNameStr += pCollection->Collection[i].szDisplayName;
				DisplayNameStr += L",";
			}
			m_PlatformDBAide.AddParameter(TEXT("@SortID"),SortIDStr.GetBuffer());
			m_PlatformDBAide.AddParameter(TEXT("@DisplayName"),DisplayNameStr.GetBuffer());
		}
		else
		{
			m_PlatformDBAide.AddParameter(TEXT("@SortID"),pCollection->wSortID);
			m_PlatformDBAide.AddParameter(TEXT("@DisplayName"),pCollection->DisplayName);
		}
		

		//�������
		if (m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_Collection"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_GameCollection CollectionInfo;
			ZeroMemory(&CollectionInfo,sizeof(CollectionInfo));
			CollectionInfo.wFlag = pCollection->wFlag;
			if(pCollection->wFlag != COLLECTION_QUERY) 
			{
				CollectionInfo.wResult = 1;
				lstrcpyn(CollectionInfo.szDisplayName,pCollection->DisplayName,CountArray(CollectionInfo.szDisplayName));
				CollectionInfo.wUserID = pCollection->wUserID;
			}
			else
			{
				lstrcpyn(CollectionInfo.szDisplayName ,pCollection->DisplayName,CountArray(CollectionInfo.szDisplayName));
				//��ȡ��Ϣ
				while(m_PlatformDBModule->IsRecordsetEnd() == false)
				{
					CollectionInfo.Collection[CollectionInfo.wCount].dwUserID = m_PlatformDBAide.GetValue_DWORD(TEXT("UserID"));
					CollectionInfo.Collection[CollectionInfo.wCount].wSortID = m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
					m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),CollectionInfo.Collection[CollectionInfo.wCount].szDisplayName,CountArray(CollectionInfo.Collection[CollectionInfo.wCount].szDisplayName));
					//�ղ���Ϸ���Ϊ128 
					if ( CollectionInfo.wCount == MAX_KIND -1 ) break;
					CollectionInfo.wCount++;
					m_PlatformDBModule->MoveToNext();
				}

				vector<tagGameCollection> collections;
				
				for (int i=0;i<CollectionInfo.wCount;i++)
				{
					m_PlatformDBAide.ResetParameter();
					m_PlatformDBAide.AddParameter(TEXT("@szKindName"), CollectionInfo.Collection[i].szDisplayName);
					
					if (m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_QueryKindName"),true) == DB_SUCCESS && !m_PlatformDBModule->IsRecordsetEnd())//�ҵ�
						collections.push_back(CollectionInfo.Collection[i]);
				}

				CollectionInfo.wCount = collections.size();
				if(CollectionInfo.wCount != 0)
					memcpy(&CollectionInfo.Collection, &collections[0], sizeof(tagGameCollection) * CollectionInfo.wCount);
				else//������ɫ��Ϸ
				{
					CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
					CServerListManager& serverListManager = pServiceUnits->m_AttemperEngineSink.m_ServerListManager;
					POSITION Position = NULL;

					for (DWORD i=0;i<serverListManager.GetGameKindCount();i++)
					{
						CGameKindItem * pGameKindItem = serverListManager.EmunGameKindItem(Position);
						if (pGameKindItem==NULL)
							break;

						const tagGameKind &gameKind = pGameKindItem->m_GameKind;

						//if(gameKind.wTypeID == 1)//��ɫ��Ϸ
						{
							m_PlatformDBAide.ResetParameter();
							m_PlatformDBAide.AddParameter(TEXT("@Flag"), COLLECTION_INSERT);
							m_PlatformDBAide.AddParameter(TEXT("@UserID"), pCollection->wUserID);
							m_PlatformDBAide.AddParameter(TEXT("@SortID"), CollectionInfo.wCount);
							m_PlatformDBAide.AddParameter(TEXT("@DisplayName"), gameKind.szKindName);

							if (m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_Collection"),true) == DB_SUCCESS)
							{
								CollectionInfo.Collection[CollectionInfo.wCount].dwUserID = pCollection->wUserID;
								CollectionInfo.Collection[CollectionInfo.wCount].wSortID = CollectionInfo.wCount;
								memcpy(CollectionInfo.Collection[CollectionInfo.wCount].szDisplayName, gameKind.szKindName, sizeof(gameKind.szKindName));
								
								CollectionInfo.wCount++;
							}
						}
					}
				}
			}
			
			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_COLLECTION,dwContextID,&CollectionInfo,sizeof(CollectionInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID, m_PlatformDBAide.GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_GET_COLLECTION);
		}

	PROCESS_END()
}

// Web ��¼���
VOID CDataBaseEngineSink::OnLogonDisposeWebResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, bool bVisitorClient)
{
	if (bMobileClient == false)
	{
		if (dwErrorCode == DB_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess, sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID = m_AccountsDBAide.GetValue_DWORD(TEXT("FaceID"));
			LogonSuccess.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID = m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal = m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience = m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness = m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"), LogonSuccess.szAccounts, CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"), LogonSuccess.szNickName, CountArray(LogonSuccess.szNickName));

			//��չ����
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"), LogonSuccess.szCompellation, CountArray(LogonSuccess.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("QQ"), LogonSuccess.szQQ, CountArray(LogonSuccess.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"), LogonSuccess.szEMail, CountArray(LogonSuccess.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"), LogonSuccess.szSeatPhone, CountArray(LogonSuccess.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"), LogonSuccess.szMobilePhone, CountArray(LogonSuccess.szMobilePhone));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"), LogonSuccess.szDwellingPlace, CountArray(LogonSuccess.szDwellingPlace));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"), LogonSuccess.szUserNote, CountArray(LogonSuccess.szUserNote));

			//�û��ɼ�
			LogonSuccess.lUserScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender = m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine = m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"), LogonSuccess.szUnderWrite, CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder = m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"), LogonSuccess.MemberOverDate);

			//��ַ��Ϣ
			m_AccountsDBAide.GetValue_SystemTime(TEXT("LastLogonDate"), LogonSuccess.LastLogonDate);
			m_AccountsDBAide.GetValue_String(TEXT("LastLogonIP"), LogonSuccess.szLastClientAddr, CountArray(LogonSuccess.szLastClientAddr));
			m_AccountsDBAide.GetValue_String(TEXT("ClientAddr"), LogonSuccess.szClientAddr, CountArray(LogonSuccess.szClientAddr));

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString, pszErrorString, CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize = CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize = sizeof(LogonSuccess) - sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_WEB_LOGON_SUCCESS, dwContextID, &LogonSuccess, wHeadSize + wDataSize);
		}
		else
		{
			//��������
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure, sizeof(LogonFailure));

			//��������
			LogonFailure.dwResultCode = dwErrorCode;
			LogonFailure.wSubCommandID = SUB_GP_LOGON_ACCOUNTS;
			lstrcpyn(LogonFailure.szDescribeString, pszErrorString, CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize = CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize = sizeof(LogonFailure) - sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE, dwContextID, &LogonFailure, wHeadSize + wDataSize);
		}
	}
	

	return;
}

//��¼�ɹ�
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bVisitorClient)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_DWORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//��չ����
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),LogonSuccess.szCompellation,CountArray(LogonSuccess.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),LogonSuccess.szQQ,CountArray(LogonSuccess.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),LogonSuccess.szEMail,CountArray(LogonSuccess.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),LogonSuccess.szSeatPhone,CountArray(LogonSuccess.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),LogonSuccess.szMobilePhone,CountArray(LogonSuccess.szMobilePhone));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),LogonSuccess.szDwellingPlace,CountArray(LogonSuccess.szDwellingPlace));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),LogonSuccess.szUserNote,CountArray(LogonSuccess.szUserNote));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			//��ַ��Ϣ
			m_AccountsDBAide.GetValue_SystemTime(TEXT("LastLogonDate"),LogonSuccess.LastLogonDate);
			m_AccountsDBAide.GetValue_String(TEXT("LastLogonIP"),LogonSuccess.szLastClientAddr,CountArray(LogonSuccess.szLastClientAddr));
			m_AccountsDBAide.GetValue_String(TEXT("ClientAddr"),LogonSuccess.szClientAddr,CountArray(LogonSuccess.szClientAddr));

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_NEEDMB)
		{
			//��¼�ɹ�
			DBR_GP_ValidateMBCard ValidateMBCard;
			ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

			//��ȡ����
			ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
		}
		else
		{
			//��������
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.dwResultCode = dwErrorCode;
			LogonFailure.wSubCommandID = SUB_GP_LOGON_ACCOUNTS;
			lstrcpyn(LogonFailure.szDescribeString, pszErrorString, CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize = CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize = sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE, dwContextID, &LogonFailure, wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//�û�����
			LogonSuccess.wFaceID = m_AccountsDBAide.GetValue_DWORD(TEXT("FaceID"));
			LogonSuccess.cbGender = m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwVipGrowthValue = m_AccountsDBAide.GetValue_DWORD(TEXT("VipPointValue"));
			LogonSuccess.dwExperience = m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness = m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			LogonSuccess.cbMemberOrder = m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));

			//�û��ɼ�
			LogonSuccess.dwUserMedal = m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.lUserScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));		

			//��չ��Ϣ
			LogonSuccess.dwInoutIndex = m_AccountsDBAide.GetValue_DWORD(TEXT("InoutIndex"));
			LogonSuccess.dwHonorID = m_AccountsDBAide.GetValue_DWORD(TEXT("HonorID"));
			LogonSuccess.wAvatarID = m_AccountsDBAide.GetValue_DWORD(TEXT("AvatarID"));		
			LogonSuccess.wPackGridCount = m_AccountsDBAide.GetValue_WORD(TEXT("wPackGridCount"));
			LogonSuccess.wMarketGridCount = m_AccountsDBAide.GetValue_WORD(TEXT("wMarketGridCount"));

			LogonSuccess.lPower = m_AccountsDBAide.GetValue_LONG(TEXT("Power"));
			LogonSuccess.lTimeDiff = m_AccountsDBAide.GetValue_LONG(TEXT("TimeDiff"));
			LogonSuccess.lJewel = m_AccountsDBAide.GetValue_LONG(TEXT("Jewel"));
			LogonSuccess.lRevenue = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Revenue"));
		
			LogonSuccess.lLobbyLV = 1;

			LogonSuccess.lMasterScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("MasterScore"));
			LogonSuccess.lHonorScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("HonorScore"));
			LogonSuccess.lRaceScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RaceScore"));
			LogonSuccess.lAchieveScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("AchieveScore"));

			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"), LogonSuccess.szAccounts, CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"), LogonSuccess.szUnderWrite, CountArray(LogonSuccess.szUnderWrite));

			//LogonSuccess.cbSecondPass=m_AccountsDBAide.GetValue_BYTE(TEXT("cbSecondPass"));

			LogonSuccess.cbVisitorClient = bVisitorClient?1:0;

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS, dwContextID, &LogonSuccess, sizeof(LogonSuccess));

			//���ͳƺ�
// 			{
// 				m_AccountsDBAide.ResetParameter();
// 				m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), LogonSuccess.dwUserID);
// 
// 				//ִ�в�ѯ
// 				LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserHonor"), true);
// 
// 				//�����
// 				BYTE cbBuffer[1024] = { 0 };
// 				WORD wPacketSize = 0;
// 				DBO_GP_HonorInfo* pHonorInfo = NULL;
// 				while (m_AccountsDBModule->IsRecordsetEnd() == false)
// 				{
// 					//������Ϣ
// 					if ((wPacketSize + sizeof(DBO_GP_HonorInfo)) > sizeof(cbBuffer))
// 					{
// 						m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_HONOR, dwContextID, cbBuffer, wPacketSize);
// 						wPacketSize = 0;
// 					}
// 
// 					//��ȡ��Ϣ
// 					pHonorInfo = (DBO_GP_HonorInfo*)(cbBuffer + wPacketSize);
// 					pHonorInfo->dwHonorID = m_AccountsDBAide.GetValue_DWORD(TEXT("HonorID"));
// 					pHonorInfo->dwHonorType = m_AccountsDBAide.GetValue_DWORD(TEXT("HonorType"));
// 
// 					//����λ��
// 					wPacketSize += sizeof(DBO_GP_HonorInfo);
// 
// 					//�ƶ���¼
// 					m_AccountsDBModule->MoveToNext();
// 				}
// 				if (wPacketSize > 0)
// 					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_HONOR, dwContextID, cbBuffer, wPacketSize);
// 			}

			//����ͷ��/����
			OnRequestGetAvatarFaceInfo(dwContextID, &LogonSuccess.dwUserID, sizeof(DWORD));

		}
		else
		{
			//��������
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.dwResultCode = dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//��������¼�ɹ�����
VOID CDataBaseEngineSink::OnLogonThirdPartyResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, DBR_GP_Insert_Accounts &insertAccounts)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//��չ����
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),LogonSuccess.szCompellation,CountArray(LogonSuccess.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),LogonSuccess.szQQ,CountArray(LogonSuccess.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),LogonSuccess.szEMail,CountArray(LogonSuccess.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),LogonSuccess.szSeatPhone,CountArray(LogonSuccess.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),LogonSuccess.szMobilePhone,CountArray(LogonSuccess.szMobilePhone));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),LogonSuccess.szDwellingPlace,CountArray(LogonSuccess.szDwellingPlace));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),LogonSuccess.szUserNote,CountArray(LogonSuccess.szUserNote));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));



			//��������
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@ThirdAccounts"),insertAccounts.szThirdAccounts);
			m_AccountsDBAide.AddParameter(TEXT("@Accounts"),insertAccounts.szAccounts);
			m_AccountsDBAide.AddParameter(TEXT("@LogonPass"),insertAccounts.szLogonPass);
			m_AccountsDBAide.AddParameter(TEXT("@RegisterMachine"),insertAccounts.szLogonPass);

			//�����ݱ�
			if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_Insert_ThirdParty"), true) != DB_SUCCESS)
			{
				//�������
				OnOperateDisposeResult(dwContextID, DB_ERROR, L"���������ݼ�¼д���ʧ��,�����µ�¼", SUB_GP_OPERATE_FAILURE);
				return;
			}

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_NEEDMB)
		{
			//��¼�ɹ�
			DBR_GP_ValidateMBCard ValidateMBCard;
			ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

			//��ȡ����
			ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
		}
		else
		{
			//��������
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.dwResultCode = dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess, sizeof(LogonSuccess));
		}
		else
		{
			//��������
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.dwResultCode = dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//���н��
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GP_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//�������
		UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
		UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
		UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
		UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
		UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationScore"));
		UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationInsure"));
		lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_OperateResult UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//�������
		UserInsureFailure.dwResultCode = dwErrorCode;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

// �������
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, WORD wSubCommandID)
{
	// ��������
	DBO_GP_OperateResult OperateResult = { 0 };

	// �������
	OperateResult.wSubCommandID = wSubCommandID;
	OperateResult.dwResultCode = dwErrorCode;
	lstrcpyn(OperateResult.szDescribeString, pszErrorString, CountArray(OperateResult.szDescribeString));

	// ���ͽ��
	int wDataSize = CountStringBuffer(OperateResult.szDescribeString);
	int wHeadSize = sizeof(OperateResult) - sizeof(OperateResult.szDescribeString);
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_RESULT, dwContextID, &OperateResult, wHeadSize + wDataSize);
}



//�ο͵�¼
bool CDataBaseEngineSink::OnMobileVisitorAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
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
		m_AccountsDBAide.AddParameter(TEXT("@dwLogonType"), 2);
		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_VisitorLogin"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		if (DBVarValue.vt==VT_NULL || DBVarValue.vt==VT_EMPTY)
			OnLogonDisposeResult(dwContextID, lResultCode, TEXT(""), true,true);
		else
			OnLogonDisposeResult(dwContextID,lResultCode, CW2CT(DBVarValue.bstrVal), true,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}


//�ο�ת��
bool CDataBaseEngineSink::OnRequestBindVisitorAccount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_BindVisitorAccount));
		if (wDataSize!=sizeof(DBR_GP_BindVisitorAccount)) return false;

		//������
		DBR_GP_BindVisitorAccount * pBindVisitorAccount=(DBR_GP_BindVisitorAccount *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pBindVisitorAccount->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pBindVisitorAccount->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pBindVisitorAccount->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pBindVisitorAccount->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickname"),pBindVisitorAccount->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@szPassword"),pBindVisitorAccount->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@szMobilePhone"),pBindVisitorAccount->szMobilePhone);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_BindVisitorInfo"),true);
		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult_BCode(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

	PROCESS_END()
}



bool CDataBaseEngineSink::OnRequestLogonOut(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		DBR_GP_LogonOut* pLogonOut = (DBR_GP_LogonOut*)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pLogonOut->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwInoutIndex"), pLogonOut->dwInOutIndex);

		m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_RecordUserOut"), true);

	PROCESS_END()
}




//���һ����루������֤�˺ţ���֤���빦�ܣ�
bool CDataBaseEngineSink::OnRequestRetrievePassword(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	//Ч�����
	ASSERT_ERR(wDataSize==sizeof(CMD_GP_RetrievePassword));
	if (wDataSize!=sizeof(CMD_GP_RetrievePassword)) return false;

	//������
	CMD_GP_RetrievePassword * pRetrievePassword=(CMD_GP_RetrievePassword *)pData;

	//�������
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@szAccounts"),pRetrievePassword->szAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@szMobilePhone"),pRetrievePassword->szMobilePhone);
	m_AccountsDBAide.AddParameter(TEXT("@szNewLogonPassword"),pRetrievePassword->szNewLogonPassword);
	m_AccountsDBAide.AddParameter(TEXT("@cbOperationType"),pRetrievePassword->cbOperationType);

	//�������
	WCHAR szDescribe[128]=L"";
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

	//�������
	LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_RetrievePassword"),false);

	//�������
	CDBVarValue DBVarValue;
	m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
	
	switch(pRetrievePassword->cbOperationType)
	{
	case 1:
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_VERIFY_ACCOUNT);
		break;
	case 2:
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_VERIFY_MOBILE_PHONE);
		break;
	case 3:
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_VERIFY_VARIFICATE_CODE);
		break;
	default:
		break;
	}
	

	PROCESS_END()
}


//ˢ�»���
bool CDataBaseEngineSink::OnReloadScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()
	// ������
	DWORD* dwUserID = (DWORD*)pData;

	// �������
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), *dwUserID);

	LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_ReloadScore"),true);

	SCORE wUserScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("UserScore"));
	SCORE wUserJewel = m_AccountsDBAide.GetValue_LONGLONG(TEXT("UserJewel"));
	SCORE wRaceScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RaceScore"));
	DWORD wPowerLower = m_AccountsDBAide.GetValue_DWORD(TEXT("PowerLower"));
	SCORE lInsureScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("InsureScore"));
	SCORE lLobbyExp = m_AccountsDBAide.GetValue_LONGLONG(TEXT("LobbyLV"));
	DWORD dwEmail = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Email"));
	DWORD dwUserMedal = m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
	DWORD lLobbyLv = 1;

	//��ʯ����
	//OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Jewel,wUserJewel );
	//��Ǯ����
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_gold,wUserScore );

	//�����ָ���
	//OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_lRaceScore,wRaceScore );
	//��������
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Power,wPowerLower );

	//���и���
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Insure,lInsureScore );

	//����
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_exp,lLobbyExp );
	//�����ȼ�
	//OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_lLobbyLv,lLobbyLv );

	//�ʼ�
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Email,dwEmail );

	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Medal,dwUserMedal );

	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_RELOAD_SCORE, dwContextID, NULL, 0);


	PROCESS_END()
}


//////////////////////////////////////////////////////////////////////////