#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineBegin(IUnknownEx * pIUnknownEx)
{
	//绑定函数
	{
		// 登录接口
		{
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_GAMEID, CDataBaseEngineSink::OnRequestLogonGameID, this);	// 标识登录
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_ACCOUNTS, CDataBaseEngineSink::OnRequestLogonAccounts, this);	// 账号登录
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_REGISTER_ACCOUNTS, CDataBaseEngineSink::OnRequestRegisterAccounts, this);	// 注册账号
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_WEB_REGISTER_ACCOUNTS, CDataBaseEngineSink::OnRequestWebRegisterAccounts, this);	// Web 注册账号
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_ThirdParty, CDataBaseEngineSink::OnRequesLogonThirdParty, this);	// 三方登录
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_REGISTER_THIRD, CDataBaseEngineSink::OnRequestRegisterThirdParty, this);	// 三方注册
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_Third, CDataBaseEngineSink::OnRequestLogonThird, this);
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOGON_OUT, CDataBaseEngineSink::OnRequestLogonOut, this);
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_BIND_VISITOR_ACCOUNT, CDataBaseEngineSink::OnRequestBindVisitorAccount, this);	// 游客转正

			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_LOGON_GAMEID, CDataBaseEngineSink::OnMobileLogonGameID, this);	// 标识登录
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_LOGON_ACCOUNTS, CDataBaseEngineSink::OnMobileLogonAccounts, this);	// 帐号登录
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_REGISTER_ACCOUNTS, CDataBaseEngineSink::OnMobileRegisterAccounts, this);	// 注册帐号
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_REGISTER_MOBILE, CDataBaseEngineSink::OnMobileRegisterMobile, this);	// 手机注册
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_VISITOR_ACCOUNTS, CDataBaseEngineSink::OnMobileVisitorAccounts, this);	// 游客登录
		}
		
		// 用户服务
		{
			//账号
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_MACHINE, CDataBaseEngineSink::OnRequestModifyMachine, this);	// 修改机器
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_LOGON_PASS, CDataBaseEngineSink::OnRequestModifyLogonPass, this);	// 修改登录密码
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_INSURE_PASS, CDataBaseEngineSink::OnRequestModifyInsurePass, this);	// 修改密码
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_UNDER_WRITE, CDataBaseEngineSink::OnRequestModifyUnderWrite, this);	// 修改签名
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_NICKNAME, CDataBaseEngineSink::OnRequestModifyNickName, this);	// 修改昵称
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_INDIVIDUAL, CDataBaseEngineSink::OnRequestModifyIndividual, this);	// 修改资料
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_PW_RECOVERY, CDataBaseEngineSink::OnRequestRecoveryPass, this);	// 找回密码
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_BIND_MOBILE, CDataBaseEngineSink::OnRequestBindMobilePhone, this);	// 绑定手机

			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_RETRIEVE_PASSWORD, CDataBaseEngineSink::OnRequestRetrievePassword, this);	// 新找回密码（包含验证账号，验证密码功能）
			//头像
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GET_AVATAR_INFO, CDataBaseEngineSink::OnRequestGetAvatarFaceInfo, this);	// 获取头像
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GET_IMAGE_INFO, CDataBaseEngineSink::OnRequestGetAvatarFaceInfo, this);	// 获取形象
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_AVATAR, CDataBaseEngineSink::OnRequestModifyAvatar, this);	// 修改头像
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_MODIFY_IMAGE, CDataBaseEngineSink::OnRequestModifyImage, this);	// 修改形象
			//银行
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_USER_SAVE_SCORE, CDataBaseEngineSink::OnRequestUserSaveScore, this);	// 存入元宝
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_USER_TAKE_SCORE, CDataBaseEngineSink::OnRequestUserTakeScore, this);	// 提取元宝
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_USER_TRANSFER_SCORE, CDataBaseEngineSink::OnRequestUserTransferScore, this);// 转账元宝


			//查询
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_QUERY_INDIVIDUAL, CDataBaseEngineSink::OnRequestQueryIndividual, this);	// 查询资料
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_QUERY_INSURE_INFO, CDataBaseEngineSink::OnRequestQueryInsureInfo, this);	// 查询银行
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_QUERY_USER_INFO, CDataBaseEngineSink::OnRequestQueryTransferUserInfo, this);	// 查询用户

			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_RELOAD_SCORE, CDataBaseEngineSink::OnReloadScore, this);	//更新积分

		}
		// 其他事件
		{
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_COLLECTION, CDataBaseEngineSink::OnRequestCollection, this);	// 收藏列表
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_LOAD_GAME_LIST, CDataBaseEngineSink::OnRequestLoadGameList, this);	// 加载列表
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_ONLINE_COUNT_INFO, CDataBaseEngineSink::OnRequestOnLineCountInfo, this);	// 在线信息
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_CONTROL_ROOM_STATUS, CDataBaseEngineSink::OnRequestOpenRoomStatus, this);  //打开房间
		}
	}

	return true;
}

//请求事件
// bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
// {
// 	bool bResult = false;
// 
// 	do 
// 	{
// 		// 寻找处理函数
// 		auto it = m_mFunctions.find(wRequestID);
// 		if(it == m_mFunctions.end())
// 			break;
// 
// 		// 调用
// 		bResult = (it->second)(dwContextID, pData, wDataSize);
// 
// 	} while (0);
// 
// 	return bResult;
// }

//I D 登录
bool CDataBaseEngineSink::OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_LogonGameID));
		if (wDataSize!=sizeof(DBR_GP_LogonGameID)) return false;

		//执行查询
		DBR_GP_LogonGameID * pLogonGameID=(DBR_GP_LogonGameID *)pData;

		//变量定义
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonGameID->cbNeeValidateMBCard);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

		return false;
	}

	return true;
}

//三方登录
bool CDataBaseEngineSink::OnRequestLogonThird(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_LogonThird));
		if (wDataSize!=sizeof(DBR_GP_LogonThird)) return false;

		//请求处理
		DBR_GP_LogonThird * pLogonAccounts=(DBR_GP_LogonThird *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pLogonAccounts->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyThird"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

		return false;
	}

	return true;
}


//帐号登录
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_LogonAccounts));
		if (wDataSize!=sizeof(DBR_GP_LogonAccounts)) return false;

		//请求处理
		DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

		return false;
	}

	return true;
}

//三方登录
bool CDataBaseEngineSink::OnRequesLogonThirdParty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//效验参数
		ASSERT_ERR(wDataSize == sizeof(DBR_GP_LogonThirdParty));
		if (wDataSize != sizeof(DBR_GP_LogonThirdParty)) return false;

		//请求处理
		DBR_GP_LogonThirdParty * pThirdParty = (DBR_GP_LogonThirdParty *)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@Accounts"), pThirdParty->szAccounts);

		//执行查询
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ThirdPartyAccounts"), true) == DB_SUCCESS)
		{
			unsigned short recordCount = m_AccountsDBModule->GetRecordCount();

			if (recordCount == 0) //需注册
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
			else if (recordCount == 1) //已注册
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
			//获取参数
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);

			//结果处理
			OnOperateDisposeResult(dwContextID, m_PlatformDBAide.GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_LOGON_THIRDPARTY);
		}

	PROCESS_END()
}

//注册处理
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//请求处理
		DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
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

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

		return false;
	}

	return true;
}

// Web注册
bool CDataBaseEngineSink::OnRequestWebRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize == sizeof(DBR_GP_Web_RegisterAccounts));
		if (wDataSize != sizeof(DBR_GP_Web_RegisterAccounts)) return false;

		//请求处理
		DBR_GP_Web_RegisterAccounts * pWebRegisterAccounts = (DBR_GP_Web_RegisterAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter = (tagBindParameter *)pWebRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID != dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pWebRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"), pWebRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"), pWebRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"), pWebRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"), pWebRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_Web_RegisterAccounts"), true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnLogonDisposeWebResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnLogonDisposeWebResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"), false);

		return false;
	}

	return true;
}

//第三方注册处理
bool CDataBaseEngineSink::OnRequestRegisterThirdParty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_RegisterThird));
		if (wDataSize!=sizeof(DBR_GP_RegisterThird)) return false;

		//请求处理
		DBR_GP_RegisterThird * pRegisterAccounts=(DBR_GP_RegisterThird *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
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

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		DBR_GP_Insert_Accounts insertAccounts;
		lstrcpyn(insertAccounts.szThirdAccounts, pRegisterAccounts->szThirdParty, CountArray(insertAccounts.szThirdAccounts));
		lstrcpyn(insertAccounts.szAccounts, pRegisterAccounts->szAccounts, CountArray(insertAccounts.szAccounts));
		lstrcpyn(insertAccounts.szLogonPass, pRegisterAccounts->szLogonPass, CountArray(insertAccounts.szLogonPass));

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonThirdPartyResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false, insertAccounts);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

		return false;
	}

	return true;
}

//I D 登录
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_MB_LogonGameID));
		if (wDataSize!=sizeof(DBR_MB_LogonGameID)) return false;

		//执行查询
		DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

		//变量定义
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}

//帐号登录
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_MB_LogonAccounts));
		if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

		//请求处理
		DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_ACCOUNT);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_MB_EfficacyAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}

//帐号注册
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_MB_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

		//请求处理
		DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),"wy1112");  //添加推广人字段
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_ACCOUNT);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_MB_RegisterAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}

//手机注册
bool CDataBaseEngineSink::OnMobileRegisterMobile(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize == sizeof(DBR_MB_RegisterMobile));
		if (wDataSize != sizeof(DBR_MB_RegisterMobile))
			return false;

		//变量
		DBR_MB_RegisterMobile* pRegisterMobile = (DBR_MB_RegisterMobile*)pData;

		//执行判断
		tagBindParameter * pBindParameter = (tagBindParameter *)pRegisterMobile->pBindParameter;
		if (pBindParameter->dwSocketID != dwContextID)
			return true;

		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pRegisterMobile->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strMoblie"), pRegisterMobile->szMobile);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"), pRegisterMobile->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"), pRegisterMobile->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_MOBLIE);

		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_MoblieRegister"), true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"), true);

		return false;
	}
}

//修改机器
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_ModifyMachine));
		if (wDataSize!=sizeof(DBR_GP_ModifyMachine)) return false;

		//请求处理
		DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//绑定操作
		if (pModifyMachine->cbBind==TRUE)
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
		}
		else
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
		}

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, m_AccountsDBModule->GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_MACHINE);

	PROCESS_END()
}

// 获取头像/形象
bool CDataBaseEngineSink::OnRequestGetAvatarFaceInfo(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		DWORD* pUserID = (DWORD*)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("dwUserID"), *pUserID);

		// 执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserAvatar"), true);

		// 结果集
		BYTE cbBuffer[10240] = { 0 };
		WORD wPacketSize = 0;
		DBO_GP_AvatarInfo* pAvatarInfo = NULL;
		while (m_AccountsDBModule->IsRecordsetEnd() == false)
		{
			//发送信息
			if ((wPacketSize + sizeof(DBO_GP_AvatarInfo)) > sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_AVATAR_INFO, dwContextID, cbBuffer, wPacketSize);
				wPacketSize = 0;
			}

			//读取信息
			pAvatarInfo = (DBO_GP_AvatarInfo*)(cbBuffer + wPacketSize);
			pAvatarInfo->dwAvatarID = m_AccountsDBAide.GetValue_DWORD(TEXT("AvatarID"));
			pAvatarInfo->cbAvatarType = m_AccountsDBAide.GetValue_BYTE(TEXT("AvatarType"));
			pAvatarInfo->dwRemainTime = m_AccountsDBAide.GetValue_DWORD(TEXT("dwRemainTime"));

			//设置位移
			wPacketSize += sizeof(DBO_GP_AvatarInfo);

			//移动记录
			m_AccountsDBModule->MoveToNext();
		}
		if (wPacketSize > 0)
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_AVATAR_INFO, dwContextID, cbBuffer, wPacketSize);

	PROCESS_END()
}

//修改头像
bool CDataBaseEngineSink::OnRequestModifyAvatar(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//请求处理
		CMD_GP_ModifyAvatar* pModifyAvatar = (CMD_GP_ModifyAvatar*)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyAvatar->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwFaceID"), pModifyAvatar->dwAvatarID);

		//输出变量
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

			//结果处理
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
			OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_AVATAR);
		}



	PROCESS_END()
}

//修改形象
bool CDataBaseEngineSink::OnRequestModifyImage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//请求处理
		CMD_GP_ModifyImage* pModifyImage = (CMD_GP_ModifyImage*)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyImage->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwAvatarID"), pModifyImage->dwImageID);

		//输出变量
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
			//结果处理
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
			OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_IMAGE);
		}

	PROCESS_END()
}

//修改密码
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
		if (wDataSize!=sizeof(DBR_GP_ModifyLogonPass)) return false;

		//请求处理
		DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pModifyLogonPass->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_ModifyLogonPass"),false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_LOGON_PASS);

	PROCESS_END()
}

//修改密码
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
		if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

		//请求处理
		DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//结果处理
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_INSURE_PASS);

	PROCESS_END()
}



//修改签名
bool CDataBaseEngineSink::OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//请求处理
		CMD_GP_ModifyUnderWrite* pModifyUnderWrite = (CMD_GP_ModifyUnderWrite*)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyUnderWrite->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"), pModifyUnderWrite->szUnderWrite);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_ModifyUnderWrite"),false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_UNDER_WRITE);

	PROCESS_END()
}

//修改昵称
bool CDataBaseEngineSink::OnRequestModifyNickName(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//请求处理
		CMD_GP_ModifyNickName* pModifyNickName = (CMD_GP_ModifyNickName*)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyNickName->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@szNickName"), pModifyNickName->szNickName);

		//输出变量
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribe, sizeof(szDescribe), adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_ModifyNickName"), false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_NICKNAME);

	PROCESS_END()
}

//绑定手机
bool CDataBaseEngineSink::OnRequestBindMobilePhone(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//效验参数
		ASSERT_ERR(wDataSize == sizeof(DBR_GP_BindMobile));
		if (wDataSize != sizeof(DBR_GP_BindMobile)) return false;

		//请求处理
		DBR_GP_BindMobile * pBindMobile = (DBR_GP_BindMobile *)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pBindMobile->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobile"), pBindMobile->szMobliePhone);
		m_AccountsDBAide.AddParameter(TEXT("@szPassword"),pBindMobile->szPassword);

		//输出变量
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@ReturnValue"), szDescribe, sizeof(szDescribe), adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_BindMobile"), false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@ReturnValue"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_BIND_MOBILE_PHONE);

	PROCESS_END()
}



// 修改密码
bool CDataBaseEngineSink::OnRequestRecoveryPass(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//请求处理
		CMD_GP_ModifyPass * pModifyPass = (CMD_GP_ModifyPass *)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"), pModifyPass->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strIdentity"), TEXT(""));
		m_AccountsDBAide.AddParameter(TEXT("@strMoblie"), TEXT(""));
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"), pModifyPass->szPassword);	
		m_AccountsDBAide.AddParameter(TEXT("@wRecoverType"), 1);

		//输出变量
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribe, sizeof(szDescribe), adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_RecoverPass"), true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_PW_RECOVERY);

	PROCESS_END()
}

//修改资料
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_ModifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

		//请求处理
		DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"), pModifyIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"), pModifyIndividual->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strPassportID"), pModifyIndividual->szPassportID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"), pModifyIndividual->szMobilePhone);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_SetUserBaseInfo"),false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_MODIFY_INDIVIDUAL);

	PROCESS_END()
}

//通用属性刷新
bool CDataBaseEngineSink::OnUpdateAttribute(DWORD dwContextID, DBO_UPDATE_ATTRIBUTE::attribute attr,SCORE wScore )
{
	DBO_UPDATE_ATTRIBUTE attrJewel={0};
	attrJewel.wType = attr;
	attrJewel.wScore = wScore;
	return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_UPDATE_ATTRIBUTE, dwContextID, &attrJewel, sizeof(DBO_UPDATE_ATTRIBUTE));
}

//存入元宝
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_UserSaveScore));
		if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

		//请求处理
		DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_UserSaveScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//提取元宝
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_UserTakeScore));
		if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

		//请求处理
		DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_UserTakeScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//转账元宝
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_UserTransferScore));
		if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

		//请求处理
		DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
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

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//查询资料
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//请求处理
		DWORD* dwUserID = (DWORD*)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), *dwUserID);

		//结果处理
		if (m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserBaseInfo"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_UserIndividual UserIndividual = { 0 };

			//用户信息
			m_AccountsDBAide.GetValue_String(TEXT("PassPortID"), UserIndividual.szPassportID, CountArray(UserIndividual.szPassportID));
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"), UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("RegisterMobile"), UserIndividual.szMobilePhone, CountArray(UserIndividual.szMobilePhone));
			m_AccountsDBAide.GetValue_String(TEXT("Email"),UserIndividual.szEmail,CountArray(UserIndividual.szEmail));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnOperateDisposeResult(dwContextID, m_AccountsDBAide.GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_QUERY_INDIVIDUAL);
		}

	PROCESS_END()
}

//查询银行
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

		//请求处理
		DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//结果处理
		if (m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//银行信息
			UserInsureInfo.dwUserID = pQueryInsureInfo->dwUserID;
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnOperateDisposeResult(dwContextID, m_TreasureDBAide.GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_QUERY_INSURE_INFO);
		}

	PROCESS_END()
}

//查询用户
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//请求处理
		CMD_GP_QueryUserInfoRequest * pQueryTransferUserInfo = (CMD_GP_QueryUserInfoRequest *)pData;

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pQueryTransferUserInfo->szNickName);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//结果处理
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//银行信息
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), TransferUserInfo.szNickName, CountArray(TransferUserInfo.szNickName));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}
}

//加载列表
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//加载类型
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//发送种类
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));
			

			//设置位移
			wPacketSize+=sizeof(DBO_GP_GameType);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//读取类型
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//发送类型
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));
			m_PlatformDBAide.GetValue_String(TEXT("DownLoadUrl"),pGameKind->szDownLoadUrl,CountArray(pGameKind->szDownLoadUrl));

			//设置位移
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//读取节点
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

		//发送节点
		wPacketSize=0;
		DBO_GP_GameNode * pGameNode=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameNode))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
			pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));
			
			//设置位移
			wPacketSize+=sizeof(DBO_GP_GameNode);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//读取定制
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

		//发送定制
		wPacketSize=0;
		DBO_GP_GamePage * pGamePage=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GamePage))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
			pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
			pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
			m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));
			
			//设置位移
			wPacketSize+=sizeof(DBO_GP_GamePage);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//变量定义
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//设置变量
		GameListResult.cbSuccess=TRUE;

		//发送消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		TRACE_LOG(pszDescribe,TraceLevel_Exception);

		//变量定义
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//设置变量
		GameListResult.cbSuccess=FALSE;

		//发送消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return false;
	}

	return true;
}

//在线信息
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//变量定义
		DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
		WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

		//效验数据
		ASSERT_ERR((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind))));
		if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

		//机器标识
		TCHAR szMachineID[LEN_MACHINE_ID];
		CWHService::GetMachineID(szMachineID);

		//构造信息
		TCHAR szOnLineCountKind[2048]=TEXT("");
		for (WORD i=0;i<pOnLineCountInfo->wKindCount;i++)
		{
			INT nLength=lstrlen(szOnLineCountKind);
			_sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,
				pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
		}

		//构造参数
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
		m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);

		//执行命令
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

	PROCESS_END()
}

//打开房间
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

//收藏列表
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
		

		//结果处理
		if (m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_Collection"),true)==DB_SUCCESS)
		{
			//变量定义
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
				//读取信息
				while(m_PlatformDBModule->IsRecordsetEnd() == false)
				{
					CollectionInfo.Collection[CollectionInfo.wCount].dwUserID = m_PlatformDBAide.GetValue_DWORD(TEXT("UserID"));
					CollectionInfo.Collection[CollectionInfo.wCount].wSortID = m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
					m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),CollectionInfo.Collection[CollectionInfo.wCount].szDisplayName,CountArray(CollectionInfo.Collection[CollectionInfo.wCount].szDisplayName));
					//收藏游戏最大为128 
					if ( CollectionInfo.wCount == MAX_KIND -1 ) break;
					CollectionInfo.wCount++;
					m_PlatformDBModule->MoveToNext();
				}

				vector<tagGameCollection> collections;
				
				for (int i=0;i<CollectionInfo.wCount;i++)
				{
					m_PlatformDBAide.ResetParameter();
					m_PlatformDBAide.AddParameter(TEXT("@szKindName"), CollectionInfo.Collection[i].szDisplayName);
					
					if (m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_QueryKindName"),true) == DB_SUCCESS && !m_PlatformDBModule->IsRecordsetEnd())//找到
						collections.push_back(CollectionInfo.Collection[i]);
				}

				CollectionInfo.wCount = collections.size();
				if(CollectionInfo.wCount != 0)
					memcpy(&CollectionInfo.Collection, &collections[0], sizeof(tagGameCollection) * CollectionInfo.wCount);
				else//加入特色游戏
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

						//if(gameKind.wTypeID == 1)//特色游戏
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
			
			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_COLLECTION,dwContextID,&CollectionInfo,sizeof(CollectionInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//结果处理
			OnOperateDisposeResult(dwContextID, m_PlatformDBAide.GetReturnValue(), CW2CT(DBVarValue.bstrVal), SUB_GP_GET_COLLECTION);
		}

	PROCESS_END()
}

// Web 登录结果
VOID CDataBaseEngineSink::OnLogonDisposeWebResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, bool bVisitorClient)
{
	if (bMobileClient == false)
	{
		if (dwErrorCode == DB_SUCCESS)
		{
			//变量定义
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess, sizeof(LogonSuccess));

			//属性资料
			LogonSuccess.wFaceID = m_AccountsDBAide.GetValue_DWORD(TEXT("FaceID"));
			LogonSuccess.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID = m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal = m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience = m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness = m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"), LogonSuccess.szAccounts, CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"), LogonSuccess.szNickName, CountArray(LogonSuccess.szNickName));

			//拓展资料
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"), LogonSuccess.szCompellation, CountArray(LogonSuccess.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("QQ"), LogonSuccess.szQQ, CountArray(LogonSuccess.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"), LogonSuccess.szEMail, CountArray(LogonSuccess.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"), LogonSuccess.szSeatPhone, CountArray(LogonSuccess.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"), LogonSuccess.szMobilePhone, CountArray(LogonSuccess.szMobilePhone));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"), LogonSuccess.szDwellingPlace, CountArray(LogonSuccess.szDwellingPlace));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"), LogonSuccess.szUserNote, CountArray(LogonSuccess.szUserNote));

			//用户成绩
			LogonSuccess.lUserScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//用户资料
			LogonSuccess.cbGender = m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine = m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"), LogonSuccess.szUnderWrite, CountArray(LogonSuccess.szUnderWrite));

			//会员资料
			LogonSuccess.cbMemberOrder = m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"), LogonSuccess.MemberOverDate);

			//地址信息
			m_AccountsDBAide.GetValue_SystemTime(TEXT("LastLogonDate"), LogonSuccess.LastLogonDate);
			m_AccountsDBAide.GetValue_String(TEXT("LastLogonIP"), LogonSuccess.szLastClientAddr, CountArray(LogonSuccess.szLastClientAddr));
			m_AccountsDBAide.GetValue_String(TEXT("ClientAddr"), LogonSuccess.szClientAddr, CountArray(LogonSuccess.szClientAddr));

			//获取信息
			lstrcpyn(LogonSuccess.szDescribeString, pszErrorString, CountArray(LogonSuccess.szDescribeString));

			//发送结果
			WORD wDataSize = CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize = sizeof(LogonSuccess) - sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_WEB_LOGON_SUCCESS, dwContextID, &LogonSuccess, wHeadSize + wDataSize);
		}
		else
		{
			//变量定义
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure, sizeof(LogonFailure));

			//构造数据
			LogonFailure.dwResultCode = dwErrorCode;
			LogonFailure.wSubCommandID = SUB_GP_LOGON_ACCOUNTS;
			lstrcpyn(LogonFailure.szDescribeString, pszErrorString, CountArray(LogonFailure.szDescribeString));

			//发送结果
			WORD wDataSize = CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize = sizeof(LogonFailure) - sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE, dwContextID, &LogonFailure, wHeadSize + wDataSize);
		}
	}
	

	return;
}

//登录成功
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bVisitorClient)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//属性资料
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_DWORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//拓展资料
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),LogonSuccess.szCompellation,CountArray(LogonSuccess.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),LogonSuccess.szQQ,CountArray(LogonSuccess.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),LogonSuccess.szEMail,CountArray(LogonSuccess.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),LogonSuccess.szSeatPhone,CountArray(LogonSuccess.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),LogonSuccess.szMobilePhone,CountArray(LogonSuccess.szMobilePhone));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),LogonSuccess.szDwellingPlace,CountArray(LogonSuccess.szDwellingPlace));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),LogonSuccess.szUserNote,CountArray(LogonSuccess.szUserNote));

			//用户成绩
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//用户资料
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//会员资料
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			//地址信息
			m_AccountsDBAide.GetValue_SystemTime(TEXT("LastLogonDate"),LogonSuccess.LastLogonDate);
			m_AccountsDBAide.GetValue_String(TEXT("LastLogonIP"),LogonSuccess.szLastClientAddr,CountArray(LogonSuccess.szLastClientAddr));
			m_AccountsDBAide.GetValue_String(TEXT("ClientAddr"),LogonSuccess.szClientAddr,CountArray(LogonSuccess.szClientAddr));

			//获取信息
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_NEEDMB)
		{
			//登录成功
			DBR_GP_ValidateMBCard ValidateMBCard;
			ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

			//读取变量
			ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
		}
		else
		{
			//变量定义
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//构造数据
			LogonFailure.dwResultCode = dwErrorCode;
			LogonFailure.wSubCommandID = SUB_GP_LOGON_ACCOUNTS;
			lstrcpyn(LogonFailure.szDescribeString, pszErrorString, CountArray(LogonFailure.szDescribeString));

			//发送结果
			WORD wDataSize = CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize = sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE, dwContextID, &LogonFailure, wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//变量定义
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//用户属性
			LogonSuccess.wFaceID = m_AccountsDBAide.GetValue_DWORD(TEXT("FaceID"));
			LogonSuccess.cbGender = m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwVipGrowthValue = m_AccountsDBAide.GetValue_DWORD(TEXT("VipPointValue"));
			LogonSuccess.dwExperience = m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness = m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			LogonSuccess.cbMemberOrder = m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));

			//用户成绩
			LogonSuccess.dwUserMedal = m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.lUserScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));		

			//扩展信息
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

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS, dwContextID, &LogonSuccess, sizeof(LogonSuccess));

			//发送称号
// 			{
// 				m_AccountsDBAide.ResetParameter();
// 				m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), LogonSuccess.dwUserID);
// 
// 				//执行查询
// 				LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_GetUserHonor"), true);
// 
// 				//结果集
// 				BYTE cbBuffer[1024] = { 0 };
// 				WORD wPacketSize = 0;
// 				DBO_GP_HonorInfo* pHonorInfo = NULL;
// 				while (m_AccountsDBModule->IsRecordsetEnd() == false)
// 				{
// 					//发送信息
// 					if ((wPacketSize + sizeof(DBO_GP_HonorInfo)) > sizeof(cbBuffer))
// 					{
// 						m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_HONOR, dwContextID, cbBuffer, wPacketSize);
// 						wPacketSize = 0;
// 					}
// 
// 					//读取信息
// 					pHonorInfo = (DBO_GP_HonorInfo*)(cbBuffer + wPacketSize);
// 					pHonorInfo->dwHonorID = m_AccountsDBAide.GetValue_DWORD(TEXT("HonorID"));
// 					pHonorInfo->dwHonorType = m_AccountsDBAide.GetValue_DWORD(TEXT("HonorType"));
// 
// 					//设置位移
// 					wPacketSize += sizeof(DBO_GP_HonorInfo);
// 
// 					//移动记录
// 					m_AccountsDBModule->MoveToNext();
// 				}
// 				if (wPacketSize > 0)
// 					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_HONOR, dwContextID, cbBuffer, wPacketSize);
// 			}

			//发送头像/形象
			OnRequestGetAvatarFaceInfo(dwContextID, &LogonSuccess.dwUserID, sizeof(DWORD));

		}
		else
		{
			//变量定义
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//构造数据
			LogonFailure.dwResultCode = dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//第三方登录成功处理
VOID CDataBaseEngineSink::OnLogonThirdPartyResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, DBR_GP_Insert_Accounts &insertAccounts)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//属性资料
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//拓展资料
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),LogonSuccess.szCompellation,CountArray(LogonSuccess.szCompellation));
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),LogonSuccess.szQQ,CountArray(LogonSuccess.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),LogonSuccess.szEMail,CountArray(LogonSuccess.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),LogonSuccess.szSeatPhone,CountArray(LogonSuccess.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),LogonSuccess.szMobilePhone,CountArray(LogonSuccess.szMobilePhone));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),LogonSuccess.szDwellingPlace,CountArray(LogonSuccess.szDwellingPlace));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),LogonSuccess.szUserNote,CountArray(LogonSuccess.szUserNote));

			//用户成绩
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//用户资料
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//会员资料
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			//获取信息
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));



			//构造数据
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@ThirdAccounts"),insertAccounts.szThirdAccounts);
			m_AccountsDBAide.AddParameter(TEXT("@Accounts"),insertAccounts.szAccounts);
			m_AccountsDBAide.AddParameter(TEXT("@LogonPass"),insertAccounts.szLogonPass);
			m_AccountsDBAide.AddParameter(TEXT("@RegisterMachine"),insertAccounts.szLogonPass);

			//插数据表
			if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_Insert_ThirdParty"), true) != DB_SUCCESS)
			{
				//结果处理
				OnOperateDisposeResult(dwContextID, DB_ERROR, L"第三方数据记录写入表失败,请重新登录", SUB_GP_OPERATE_FAILURE);
				return;
			}

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_NEEDMB)
		{
			//登录成功
			DBR_GP_ValidateMBCard ValidateMBCard;
			ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

			//读取变量
			ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
		}
		else
		{
			//变量定义
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//构造数据
			LogonFailure.dwResultCode = dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//变量定义
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//属性资料
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess, sizeof(LogonSuccess));
		}
		else
		{
			//变量定义
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//构造数据
			LogonFailure.dwResultCode = dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//发送结果
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//银行结果
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_GP_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//构造变量
		UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
		UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
		UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
		UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
		UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationScore"));
		UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationInsure"));
		lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GP_OperateResult UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//构造变量
		UserInsureFailure.dwResultCode = dwErrorCode;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

// 操作结果
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, WORD wSubCommandID)
{
	// 变量定义
	DBO_GP_OperateResult OperateResult = { 0 };

	// 构造变量
	OperateResult.wSubCommandID = wSubCommandID;
	OperateResult.dwResultCode = dwErrorCode;
	lstrcpyn(OperateResult.szDescribeString, pszErrorString, CountArray(OperateResult.szDescribeString));

	// 发送结果
	int wDataSize = CountStringBuffer(OperateResult.szDescribeString);
	int wHeadSize = sizeof(OperateResult) - sizeof(OperateResult.szDescribeString);
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_RESULT, dwContextID, &OperateResult, wHeadSize + wDataSize);
}



//游客登录
bool CDataBaseEngineSink::OnMobileVisitorAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_VisitorAccounts));
		if (wDataSize!=sizeof(DBR_MB_VisitorAccounts)) return false;

		//请求处理
		DBR_MB_VisitorAccounts * pRegisterAccounts=(DBR_MB_VisitorAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);


		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_VISITOR);
		m_AccountsDBAide.AddParameter(TEXT("@dwLogonType"), 2);
		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_VisitorLogin"),true);

		//结果处理
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
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}


//游客转正
bool CDataBaseEngineSink::OnRequestBindVisitorAccount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_BindVisitorAccount));
		if (wDataSize!=sizeof(DBR_GP_BindVisitorAccount)) return false;

		//请求处理
		DBR_GP_BindVisitorAccount * pBindVisitorAccount=(DBR_GP_BindVisitorAccount *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pBindVisitorAccount->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pBindVisitorAccount->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pBindVisitorAccount->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pBindVisitorAccount->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickname"),pBindVisitorAccount->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@szPassword"),pBindVisitorAccount->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@szMobilePhone"),pBindVisitorAccount->szMobilePhone);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//结果处理
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_BindVisitorInfo"),true);
		//结果处理
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




//新找回密码（包含验证账号，验证密码功能）
bool CDataBaseEngineSink::OnRequestRetrievePassword(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	//效验参数
	ASSERT_ERR(wDataSize==sizeof(CMD_GP_RetrievePassword));
	if (wDataSize!=sizeof(CMD_GP_RetrievePassword)) return false;

	//请求处理
	CMD_GP_RetrievePassword * pRetrievePassword=(CMD_GP_RetrievePassword *)pData;

	//构造参数
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@szAccounts"),pRetrievePassword->szAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@szMobilePhone"),pRetrievePassword->szMobilePhone);
	m_AccountsDBAide.AddParameter(TEXT("@szNewLogonPassword"),pRetrievePassword->szNewLogonPassword);
	m_AccountsDBAide.AddParameter(TEXT("@cbOperationType"),pRetrievePassword->cbOperationType);

	//输出变量
	WCHAR szDescribe[128]=L"";
	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

	//结果处理
	LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_RetrievePassword"),false);

	//结果处理
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


//刷新积分
bool CDataBaseEngineSink::OnReloadScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()
	// 请求处理
	DWORD* dwUserID = (DWORD*)pData;

	// 构造参数
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

	//钻石更新
	//OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Jewel,wUserJewel );
	//金钱更新
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_gold,wUserScore );

	//竞赛分更新
	//OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_lRaceScore,wRaceScore );
	//体力更新
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Power,wPowerLower );

	//银行更新
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Insure,lInsureScore );

	//经验
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_exp,lLobbyExp );
	//大厅等级
	//OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_lLobbyLv,lLobbyLv );

	//邮件
	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Email,dwEmail );

	OnUpdateAttribute( dwContextID, DBO_UPDATE_ATTRIBUTE::enumAttr_Medal,dwUserMedal );

	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_RELOAD_SCORE, dwContextID, NULL, 0);


	PROCESS_END()
}


//////////////////////////////////////////////////////////////////////////