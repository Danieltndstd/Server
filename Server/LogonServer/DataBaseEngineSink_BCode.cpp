#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineBegin_BCode(IUnknownEx * pIUnknownEx)
{
	//绑定函数
	{
		// 登录接口
		{
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_LOGON_GAMEID_BCode, CDataBaseEngineSink::OnMobileLogonGameID_BCode, this);	// 标识登录
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_LOGON_ACCOUNTS_BCode, CDataBaseEngineSink::OnMobileLogonAccounts_BCode, this);	// 帐号登录
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_REGISTER_ACCOUNTS_BCode, CDataBaseEngineSink::OnMobileRegisterAccounts_BCode, this);	// 注册帐号
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_REGISTER_MOBILE_BCode, CDataBaseEngineSink::OnMobileRegisterMobile_BCode, this);	// 手机注册
			REGISTER_FUNCTION_3(m_mFunctions, DBR_MB_VISITOR_ACCOUNTS_BCode, CDataBaseEngineSink::OnMobileVisitorAccounts_BCode, this);	// 游客登录
		}
		
		//用户服务
		{
			//签到
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_TODAY_IS_SIGNIN,CDataBaseEngineSink::OnRequestTotayIsSignIn,this); //获取今天是否签到
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GET_SIGNIN,CDataBaseEngineSink::OnRequestGetSignIn,this); //获取签到
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_SET_SIGNIN,CDataBaseEngineSink::OnRequestSetSignIn,this); //设置签到

			// 转盘
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GETDAILINFO, CDataBaseEngineSink::OnRequestGetDailInfo, this); //获取转盘
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_ROTATEDAILAWARD, CDataBaseEngineSink::OnRequestRotateDailAward, this); //转到转盘获得奖励
			
			// 绑定设置
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_CHICKBINDBANK, CDataBaseEngineSink::OnRequestChickBindBank, this); //查询绑定银行卡
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_BINDBANK, CDataBaseEngineSink::OnRequestBindBank, this); //绑定银行卡
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_CHICKBINDZFB, CDataBaseEngineSink::OnRequestChickBindZFB, this); //查询绑定支付宝
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_BINDZFB, CDataBaseEngineSink::OnRequestBindZFB, this); //绑定支付宝

			// 邮件
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_REQUEST_MAIL, CDataBaseEngineSink::OnRequestRequestMail, this); //请求邮件
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_READ_DELETE_MAIL, CDataBaseEngineSink::OnRequestReadDeleteMail, this); //读取删除邮件

			//用户完善信息
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_GET_USER_PERFECT_INFORMATION, CDataBaseEngineSink::OnRequestGetUserPerFectInformation, this);	//获取用户完善信息
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_SET_USER_PERFECT_INFORMATION, CDataBaseEngineSink::OnRequestSetUserPerFectInformation, this);	//设置用户完善信息

			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_IS_SPREAD_REGISTER, CDataBaseEngineSink::OnRequestIsSpreadRegister, this);   //是否是推广注册
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_BIND_USER_SPREAD, CDataBaseEngineSink::OnRequestBindUserSpread, this);   //绑定用户推广
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_IS_BIND_SPREAD, CDataBaseEngineSink::OnRequestIsBindSpread, this);   //是否已经绑定推广
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_IS_ALREADY_MODIFY_NICKNAME, CDataBaseEngineSink::OnRequestIsAlreadyModifyNickName, this);   //是否已经修改过昵称
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_IS_TODAY_FIRST_TIME_LOGIN, CDataBaseEngineSink::OnRequestIsTodayFirstTimeLogin, this);   //是否是今天第一次登录
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_CLIENT_LOGIN_SUCCESS, CDataBaseEngineSink::OnRequestClientLoginSuccess, this);   //客户端登录完成
			
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GP_KILL_LOCK, CDataBaseEngineSink::OnRequestKillLocker, this);   //客户端登录完成
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_LOAD_SCROLL_MESSAGE, CDataBaseEngineSink::OnLoadScrollMessage, this);		//读取滚动消息
			REGISTER_FUNCTION_3(m_mFunctions, DBR_GR_CLEAR_SCROLL_MESSAGE, CDataBaseEngineSink::OnClearScrollMessage, this);		//清除滚动消息 
		}

	}

	return true;
}


//I D 登录
bool CDataBaseEngineSink::OnMobileLogonGameID_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
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
		OnLogonDisposeResult_BCode(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult_BCode(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}

//帐号登录
bool CDataBaseEngineSink::OnMobileLogonAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
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
		//m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),pLogonAccounts->cbDeviceType);
		m_AccountsDBAide.AddParameter(TEXT("@dwInviteID"), pLogonAccounts->dwInviteID);
		m_AccountsDBAide.AddParameter(TEXT("@dwSortID"), pLogonAccounts->dwSortID);
		m_AccountsDBAide.AddParameter(TEXT("@dwLogonType"), pLogonAccounts->dwLogonType);
		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_VisitorLogin"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult_BCode(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult_BCode(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}

//帐号注册
bool CDataBaseEngineSink::OnMobileRegisterAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_MB_RegisterAccounts_BCode));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts_BCode)) return false;

		//请求处理
		DBR_MB_RegisterAccounts_BCode * pRegisterAccounts=(DBR_MB_RegisterAccounts_BCode *)pData;

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
		m_AccountsDBAide.AddParameter(TEXT("@dwSpreaderID"),pRegisterAccounts->dwSpreaderID); 
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@wRegisterType"),DEVICE_MOBLIE_ACCOUNT);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_MB_RegisterAccounts_ry"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult_BCode(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult_BCode(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}

//手机注册
bool CDataBaseEngineSink::OnMobileRegisterMobile_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
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
		OnLogonDisposeResult_BCode(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult_BCode(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"), true);

		return false;
	}
}

//游客登录
bool CDataBaseEngineSink::OnMobileVisitorAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
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
		m_AccountsDBAide.AddParameter(TEXT("@dwInviteID"), pRegisterAccounts->dwInviteID);
		m_AccountsDBAide.AddParameter(TEXT("@dwSortID"), pRegisterAccounts->dwSortID);
		m_AccountsDBAide.AddParameter(TEXT("@dwLogonType"), pRegisterAccounts->dwLogonType);
		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("QP_P_VisitorLogin"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		bool bVisitor = true;
		//特殊处理
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
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult_BCode(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}

//登录成功
VOID CDataBaseEngineSink::OnLogonDisposeResult_BCode(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bVisitorClient)
{
	if (bMobileClient==false)
	{
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//变量定义
			DBO_MB_LogonSuccess_BCode LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//用户属性
			LogonSuccess.wFaceID = m_AccountsDBAide.GetValue_DWORD(TEXT("FaceID"));
			LogonSuccess.cbGender = m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience = m_AccountsDBAide.GetValue_LONGLONG(TEXT("LobbyLV"));
			LogonSuccess.cbMemberOrder = m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));

			//用户成绩
			LogonSuccess.dwUserMedal = m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.lUserScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));		

			//扩展信息
			LogonSuccess.dwInoutIndex = m_AccountsDBAide.GetValue_DWORD(TEXT("InoutIndex"));
			LogonSuccess.lRevenue = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Revenue"));
			LogonSuccess.dwServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("dwServerID"));

			LogonSuccess.lLobbyLV = 1;

			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"), LogonSuccess.szAccounts, CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"), LogonSuccess.szUnderWrite, CountArray(LogonSuccess.szUnderWrite));

			LogonSuccess.cbVisitorClient = bVisitorClient?1:0;

			//检查是否断线重连
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

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS_BCode, dwContextID, &LogonSuccess, sizeof(LogonSuccess));

		}
		else
		{
			//变量定义
			DBO_GP_OperateResult LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//构造数据
			LogonFailure.dwResultCode = dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//日志
			//I_TRACE(TEXT("[SocketID:%d] %s"),dwContextID,LogonFailure.szDescribeString);
			//发送结果
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE_BCode,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//获取签到
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
		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_GET_USER_SIGNIN);
	}

	PROCESS_END()
}
//设置签到
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

//获取今天是否已经签到
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
		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_TODAY_IS_SIGNIN);
	}

	PROCESS_END()
}

//获取用户完善信息
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
		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_GET_USER_PERFECT_INFORMATION);
	}

	PROCESS_END()
}

//设置用户完善信息
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

//获取转盘信息
bool CDataBaseEngineSink::OnRequestGetDailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_GetDailInfo));
		if (wDataSize != sizeof(CMD_GP_GetDailInfo)) return false;
		if (NULL == pData) return false;

		CMD_GP_GetDailInfo * pGetDailInfo = (CMD_GP_GetDailInfo*)pData;


		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pGetDailInfo->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pGetDailInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_DailGetInfo"), true);

		if (DB_SUCCESS == lResultCode)
		{
			DBO_GP_GetDailInfo pGetDailInfo;
			ZeroMemory(&pGetDailInfo, sizeof(pGetDailInfo));
			// 消耗
			pGetDailInfo.dwSilverConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("SilverConsume"));
			pGetDailInfo.dwGoldConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("GoldConsume"));
			pGetDailInfo.dwJewelConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("JewelConsume"));
			//积分
			pGetDailInfo.dwDrawScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("DrawScor"));
			pGetDailInfo.dwAddScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("AddScore"));
			//转盘信息
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
			//结果处理
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
			OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_GET_USER_SIGNIN);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}

//转动转盘获得奖励
bool CDataBaseEngineSink::OnRequestRotateDailAward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_RotateDailAward));
		if (wDataSize != sizeof(CMD_GP_RotateDailAward)) return false;
		if (NULL == pData) return false;

		CMD_GP_RotateDailAward * pRotateDailAward = (CMD_GP_RotateDailAward*)pData;


		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pRotateDailAward->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pRotateDailAward->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@dwDailType"), pRotateDailAward->dwDailType);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
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
			// 消耗
			RotateDailAwardInfo.dwSilverConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("SilverConsume"));
			RotateDailAwardInfo.dwGoldConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("GoldConsume"));
			RotateDailAwardInfo.dwJewelConsume = m_TreasureDBAide.GetValue_LONGLONG(TEXT("JewelConsume"));
			//积分
			RotateDailAwardInfo.dwDrawScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("DrawScor"));
			RotateDailAwardInfo.dwAddScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("AddScore"));
			//转盘信息
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
		
		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(RotateDailAwardInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(RotateDailAwardInfo.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ROTATEDAILAWARD, dwContextID, &RotateDailAwardInfo, sizeof(RotateDailAwardInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}

// 查询绑定银行卡
bool CDataBaseEngineSink::OnRequestChickBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_ChickBindBank));
		if (wDataSize != sizeof(CMD_GP_ChickBindBank)) return false;
		if (NULL == pData) return false;

		CMD_GP_ChickBindBank * pGetDailInfo = (CMD_GP_ChickBindBank*)pData;


		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pGetDailInfo->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pGetDailInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
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

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(pBindBankInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pBindBankInfo.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHICKBINDBANK, dwContextID, &pBindBankInfo, sizeof(pBindBankInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}
// 绑定银行卡
bool CDataBaseEngineSink::OnRequestBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_BindBankCard));
		if (wDataSize != sizeof(CMD_GP_BindBankCard)) return false;
		if (NULL == pData) return false;

		CMD_GP_BindBankCard * pBindBankCard = (CMD_GP_BindBankCard*)pData;


		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pBindBankCard->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pBindBankCard->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@szBankName"), pBindBankCard->szBankName);
		m_TreasureDBAide.AddParameter(TEXT("@szBankCardNum"), pBindBankCard->szBankCardNum);
		m_TreasureDBAide.AddParameter(TEXT("@szAccountHolder"), pBindBankCard->szAccountHolder);

		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
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

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(pBindBankInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pBindBankInfo.szDescribeString));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHICKBINDBANK, dwContextID, &pBindBankInfo, sizeof(pBindBankInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}
// 查询绑定支付宝
bool CDataBaseEngineSink::OnRequestChickBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_ChickBindZFB));
		if (wDataSize != sizeof(CMD_GP_ChickBindZFB)) return false;
		if (NULL == pData) return false;

		CMD_GP_ChickBindZFB * pChickBindZFB = (CMD_GP_ChickBindZFB*)pData;


		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pChickBindZFB->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pChickBindZFB->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
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

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(pBindZFBInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pBindZFBInfo.szDescribeString));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHICKBINDZFB, dwContextID, &pBindZFBInfo, sizeof(pBindZFBInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}
// 绑定支付宝
bool CDataBaseEngineSink::OnRequestBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_BindZFB));
		if (wDataSize != sizeof(CMD_GP_BindZFB)) return false;
		if (NULL == pData) return false;

		CMD_GP_BindZFB * pBindZFB = (CMD_GP_BindZFB*)pData;


		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pBindZFB->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pBindZFB->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@szZFBName"), pBindZFB->szZFBName);
		m_TreasureDBAide.AddParameter(TEXT("@szNickName"), pBindZFB->szNickName);
		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
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

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(pBindZFBInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(pBindZFBInfo.szDescribeString));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHICKBINDZFB, dwContextID, &pBindZFBInfo, sizeof(pBindZFBInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}

// 请求邮件
bool CDataBaseEngineSink::OnRequestRequestMail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_RequestMail));
		if (wDataSize != sizeof(CMD_GP_RequestMail)) return false;
		if (NULL == pData) return false;

		CMD_GP_RequestMail * pRequestMail = (CMD_GP_RequestMail*)pData;


		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pRequestMail->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pRequestMail->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		
		//执行查询
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_RequestMail"), true);

		DBO_GP_RequestMailToal RequestMailToal;
		ZeroMemory(&RequestMailToal, sizeof(RequestMailToal));
		RequestMailToal.dwUserID = pRequestMail->dwUserID;
		if (DB_SUCCESS == lResultCode)
		{
			BYTE MailCount = 0;
			//读取消息
			while (m_TreasureDBModule->IsRecordsetEnd() == false)
			{
				RequestMailToal.stRequestMail[MailCount].dwMailIndex = m_TreasureDBAide.GetValue_DWORD(TEXT("MailIndex"));
				RequestMailToal.stRequestMail[MailCount].dwStartUsing = m_TreasureDBAide.GetValue_DWORD(TEXT("StartUsing"));
				m_TreasureDBAide.GetValue_String(TEXT("MailContent"), RequestMailToal.stRequestMail[MailCount].szMailContent, 
					CountArray(RequestMailToal.stRequestMail[MailCount].szMailContent));
				MailCount++;
				//移动记录
				m_TreasureDBModule->MoveToNext();
			};
			RequestMailToal.dwMailCount = MailCount;
		}
		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(RequestMailToal.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(RequestMailToal.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_REQUESTMAIL, dwContextID, &RequestMailToal, sizeof(RequestMailToal));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}
// 读取删除邮件
bool CDataBaseEngineSink::OnRequestReadDeleteMail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(CMD_GP_ReadDeleteMail));
		if (wDataSize != sizeof(CMD_GP_ReadDeleteMail)) return false;
		if (NULL == pData) return false;

		CMD_GP_ReadDeleteMail * pReadDeleteMail = (CMD_GP_ReadDeleteMail*)pData;


		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pReadDeleteMail->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pReadDeleteMail->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@dwOpenType"), pReadDeleteMail->dwOpenType);
		m_TreasureDBAide.AddParameter(TEXT("@dwMailIndex"), pReadDeleteMail->dwMailIndex);

		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("QP_P_ReadDeleteMail"), true);

		DBO_GP_ReadDeleteMailToal ReadDeleteMailToal;
		ZeroMemory(&ReadDeleteMailToal, sizeof(ReadDeleteMailToal));
		ReadDeleteMailToal.dwUserID = pReadDeleteMail->dwUserID;
		ReadDeleteMailToal.dwOpenType = pReadDeleteMail->dwOpenType;
		ReadDeleteMailToal.dwResultCode = lResultCode;
		if (DB_SUCCESS == lResultCode)
		{
			BYTE MailCount = 0;
			//读取消息
			while (m_TreasureDBModule->IsRecordsetEnd() == false)
			{
				ReadDeleteMailToal.stRequestMail[MailCount].dwMailIndex = m_TreasureDBAide.GetValue_DWORD(TEXT("MailIndex"));
				ReadDeleteMailToal.stRequestMail[MailCount].dwStartUsing = m_TreasureDBAide.GetValue_DWORD(TEXT("StartUsing"));
				m_TreasureDBAide.GetValue_String(TEXT("MailContent"), ReadDeleteMailToal.stRequestMail[MailCount].szMailContent,
					CountArray(ReadDeleteMailToal.stRequestMail[MailCount].szMailContent));
				MailCount++;
				//移动记录
				m_TreasureDBModule->MoveToNext();
			};
			ReadDeleteMailToal.dwMailCount = MailCount;
		}
		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		lstrcpyn(ReadDeleteMailToal.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(ReadDeleteMailToal.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_READDELEETETAIL, dwContextID, &ReadDeleteMailToal, sizeof(ReadDeleteMailToal));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}

//是否是推广注册
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

//绑定用户推广
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
		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), SUB_GP_BIND_USER_SPREAD);
	}

	PROCESS_END()
}

//是否已经绑定推广
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

//是否已经修改过昵称
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

//是否是今天第一次登录
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

//是否是掉线重新登录
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

//客户端登录完成
bool CDataBaseEngineSink::OnRequestClientLoginSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//签到
	OnRequestTotayIsSignIn(dwContextID,pData,wDataSize);
	//推广注册
	//OnRequestIsSpreadRegister(dwContextID,pData,wDataSize);
	//今天第一次登录
	//OnRequestIsTodayFirstTimeLogin(dwContextID,pData,wDataSize);

	return true;
}

//清除卡线
bool CDataBaseEngineSink::OnRequestKillLocker(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	ASSERT_ERR(wDataSize == sizeof(CMD_CS_S_ServerRemove));
	if (wDataSize != sizeof(CMD_CS_S_ServerRemove)) return false;
	if (NULL == pData) return false;

	//变量定义
	CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

	//构造参数
	m_TreasureDBAide.ResetParameter();
	m_TreasureDBAide.AddParameter(TEXT("@wServerID"),pServerRemove->wServerID);

	//结果处理
	LONG lReturnValue=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_KillGameScoreLocker"),false);

	PROCESS_END()

}

//读取滚动消息
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

//清除滚动消息
bool CDataBaseEngineSink::OnClearScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	PROCESS_BEGIN()

	m_PlatformDBAide.ResetParameter();
	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("QP_P_ClearScrollMessage"),false);

	PROCESS_END()
}


//////////////////////////////////////////////////////////////////////////