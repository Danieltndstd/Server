#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"


void CDataBaseEngineSink::RegisterRequestEvent()
{
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOGON_USERID, CDataBaseEngineSink::OnRequestLogonUserID, this);						// ID登录
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOGON_MOBILE, CDataBaseEngineSink::OnRequestLogonMobile, this);						// 手机登录
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOGON_ACCOUNTS, CDataBaseEngineSink::OnRequestLogonAccounts, this);					// 帐号登录
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOGON_TOKENID, CDataBaseEngineSink::OnRequestLogonTokenID, this);					// 帐号登录

	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_WRITE_GAME_SCORE, CDataBaseEngineSink::OnRequestWriteGameScore, this);					// 游戏写分


	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LEAVE_GAME_SERVER, CDataBaseEngineSink::OnRequestLeaveGameServer, this);				// 离开房间
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_GAME_SCORE_RECORD, CDataBaseEngineSink::OnRequestGameScoreRecord, this);				// 游戏记录
	
	//捕鱼游戏记录
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GP_GAME_FISH_RECORD, CDataBaseEngineSink::OnRequestGameFishRecord, this);
	// 游戏滚动消息
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GP_GAME_SCROLL_MESSAGE, CDataBaseEngineSink::OnRequestGameScrollMessage, this);

	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_PARAMETER, CDataBaseEngineSink::OnRequestLoadParameter, this);					// 加载参数
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_GAME_COLUMN, CDataBaseEngineSink::OnRequestLoadGameColumn, this);					// 加载列表
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_ANDROID_USER, CDataBaseEngineSink::OnRequestLoadAndroidUser, this);				// 加载机器
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_GAME_PROPERTY, CDataBaseEngineSink::OnRequestLoadGameProperty, this);				// 加载道具
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_USER_SAVE_SCORE, CDataBaseEngineSink::OnRequestUserSaveScore, this);					// 存入元宝
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_USER_TAKE_SCORE, CDataBaseEngineSink::OnRequestUserTakeScore, this);					// 提取元宝
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_USER_TRANSFER_SCORE, CDataBaseEngineSink::OnRequestUserTransferScore, this);			// 转账元宝
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_QUERY_INSURE_INFO, CDataBaseEngineSink::OnRequestQueryInsureInfo, this);				// 查询银行
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_QUERY_TRANSFER_USER_INFO, CDataBaseEngineSink::OnRequestQueryTransferUserInfo, this);	// 查询用户
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_PROPERTY_REQUEST, CDataBaseEngineSink::OnRequestPropertyRequest, this);				// 道具请求
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_MANAGE_USER_RIGHT, CDataBaseEngineSink::OnRequestManageUserRight, this);				// 用户权限
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_SYSTEM_MESSAGE, CDataBaseEngineSink::OnRequestLoadSystemMessage, this);			// 系统消息
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_LOAD_SENSITIVE_WORDS, CDataBaseEngineSink::OnRequestLoadSensitiveWords, this);			// 加载敏感词


	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_KILL_PALAYER_LOCKER, CDataBaseEngineSink::OnRequestKillLocker, this);					// 清除卡线
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_ANDRIOD_APPLY, CDataBaseEngineSink::OnRequestAndriodApply, this);						// 申请机器人
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_ANDRIOD_RELEASE, CDataBaseEngineSink::OnRequestAndriodRelease, this);					// 释放机器人（单个）
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_ANDRIOD_REFRESH, CDataBaseEngineSink::OnRequestAndriodRefresh, this);					// 刷新时间戳
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_ANDRIOD_ALL, CDataBaseEngineSink::OnRequestAndriodALL, this);							// 释放所有机器人
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_ANDRIOD_ARRAY, CDataBaseEngineSink::OnRequestAndriodArray, this);						// 批量释放机器人
	REGISTER_FUNCTION_4(m_mFunctions, DBR_GR_ANDRIOD_RELEASE_DELETE, CDataBaseEngineSink::OnRequestAndriodReleaseAndDelete, this);

	REGISTER_FUNCTION_4(m_mFunctions, DRB_GR_STATUS_USER_OFFLINE, CDataBaseEngineSink::OnRequestWriteUserStatus, this);
				
}


//请求事件
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	bool bResult = false;
	DWORD dwUserID = 0;

	do
	{
		// 寻找处理函数
		std::map<WORD, std::function<bool(DWORD, VOID*, WORD, DWORD&)>>::iterator it = m_mFunctions.find(wRequestID);
		if (it == m_mFunctions.end())
			break;

		// 调用
		bResult = (it->second)(dwContextID, pData, wDataSize, dwUserID);

		// 协调通知
		if (m_pIDBCorrespondManager)
			m_pIDBCorrespondManager->OnPostRequestComplete(dwUserID, bResult);

	} while (0);

	return bResult;
}

//I D 登录
bool CDataBaseEngineSink::OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		DBR_GR_LogonUserID* pLogonUserID = (DBR_GR_LogonUserID*)pData;
		dwUserID = pLogonUserID->dwUserID;
		
		// 转化地址
		TCHAR szClientAddr[16] = { 0 };
		BYTE* pClientAddr = (BYTE*)&pLogonUserID->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"), pLogonUserID->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@strPassword"), pLogonUserID->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"), pLogonUserID->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), m_LogonFailure.szDescribeString, sizeof(m_LogonFailure.szDescribeString), adParamOutput);

		//执行查询
		LONG lResultCode = m_GameDBAide.ExecuteProcess(TEXT("QP_P_EfficacyUserID_java"), true);

		//用户信息
		lstrcpyn(m_LogonSuccess.szPassword, pLogonUserID->szPassword, CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID, pLogonUserID->szMachineID, CountArray(m_LogonSuccess.szMachineID));

		//结果处理
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

	PROCESS_END()
}

bool CDataBaseEngineSink::OnRequestLogonTokenID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

	DBR_GR_LogonTokenID* pLogonTokenID = (DBR_GR_LogonTokenID*)pData;
	dwUserID = pLogonTokenID->dwUserID;

	// 转化地址
	TCHAR szGateWayAddr[16] = { 0 };
	BYTE* pClientAddr = (BYTE*)&pLogonTokenID->dwClientAddr;
	_sntprintf(szGateWayAddr, CountArray(szGateWayAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

	//构造参数
	m_GameDBAide.ResetParameter();
	m_GameDBAide.AddParameter(TEXT("@dwMerchantID"), pLogonTokenID->dwMerchantID);
	m_GameDBAide.AddParameter(TEXT("@dwUserID"), pLogonTokenID->dwUserID);
	m_GameDBAide.AddParameter(TEXT("@strTokenID"), pLogonTokenID->szTokenID);
	m_GameDBAide.AddParameter(TEXT("@strGateWayIP"), szGateWayAddr);
	m_GameDBAide.AddParameter(TEXT("@strClientIP"), pLogonTokenID->szClientIP);
	m_GameDBAide.AddParameter(TEXT("@strMachineID"), pLogonTokenID->szMachineID);
	m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
	m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
	m_GameDBAide.AddParameter(TEXT("@cbDeviceType"), pLogonTokenID->cbDeviceType);
	m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), m_LogonFailure.szDescribeString, sizeof(m_LogonFailure.szDescribeString), adParamOutput);

	//执行查询
	LONG lResultCode = m_GameDBAide.ExecuteProcess(TEXT("QP_P_EfficacyMobile_java"), true);

	//用户信息
	lstrcpyn(m_LogonSuccess.szPassword, pLogonTokenID->szTokenID, CountArray(m_LogonSuccess.szPassword));
	lstrcpyn(m_LogonSuccess.szMachineID, pLogonTokenID->szMachineID, CountArray(m_LogonSuccess.szMachineID));

	//结果处理
	CDBVarValue DBVarValue;
	m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
	OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false, DEVICE_TYPE_H5);

	PROCESS_END()
}

//I D 登录
bool CDataBaseEngineSink::OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		DBR_GR_LogonTokenID* pLogonTokenID = (DBR_GR_LogonTokenID*)pData;
	dwUserID = pLogonTokenID->dwUserID;

	// 转化地址
	TCHAR szGateWayAddr[16] = { 0 };
	BYTE* pClientAddr = (BYTE*)&pLogonTokenID->dwClientAddr;
	_sntprintf(szGateWayAddr, CountArray(szGateWayAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

	//构造参数
	m_GameDBAide.ResetParameter();
	m_GameDBAide.AddParameter(TEXT("@dwMerchantID"), pLogonTokenID->dwMerchantID);
	m_GameDBAide.AddParameter(TEXT("@dwUserID"), pLogonTokenID->dwUserID);
	m_GameDBAide.AddParameter(TEXT("@strTokenID"), pLogonTokenID->szTokenID);
	m_GameDBAide.AddParameter(TEXT("@strGateWayIP"), szGateWayAddr);
	m_GameDBAide.AddParameter(TEXT("@strClientIP"), pLogonTokenID->szClientIP);
	m_GameDBAide.AddParameter(TEXT("@strMachineID"), pLogonTokenID->szMachineID);
	m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
	m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
	m_GameDBAide.AddParameter(TEXT("@cbDeviceType"), pLogonTokenID->cbDeviceType);
	m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), m_LogonFailure.szDescribeString, sizeof(m_LogonFailure.szDescribeString), adParamOutput);

	//执行查询
	LONG lResultCode = m_GameDBAide.ExecuteProcess(TEXT("QP_P_EfficacyMobile_java"), true);

	//用户信息
	lstrcpyn(m_LogonSuccess.szPassword, pLogonTokenID->szTokenID, CountArray(m_LogonSuccess.szPassword));
	lstrcpyn(m_LogonSuccess.szMachineID, pLogonTokenID->szMachineID, CountArray(m_LogonSuccess.szMachineID));

	//结果处理
	CDBVarValue DBVarValue;
	m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
	OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false, DEVICE_TYPE_ANDROID);

	PROCESS_END()
}

//帐号登录
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		DBR_GR_LogonAccounts* pLogonAccounts = (DBR_GR_LogonAccounts*)pData;

		// 转化地址
		TCHAR szClientAddr[16] = { 0 };
		BYTE* pClientAddr = (BYTE*)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@strAccounts"), pLogonAccounts->szAccounts);
		m_GameDBAide.AddParameter(TEXT("@strPassword"), pLogonAccounts->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"), pLogonAccounts->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), m_LogonFailure.szDescribeString, sizeof(m_LogonFailure.szDescribeString), adParamOutput);

		//执行查询
		LONG lResultCode = m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyAccounts"), true);

		//用户信息
		lstrcpyn(m_LogonSuccess.szPassword, pLogonAccounts->szPassword, CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID, pLogonAccounts->szMachineID, CountArray(m_LogonSuccess.szMachineID));

		//结果处理
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnLogonDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

	PROCESS_END()
}

//游戏写分
bool CDataBaseEngineSink::OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		DBR_GR_WriteGameScore* pWriteGameScore = (DBR_GR_WriteGameScore*)pData;
		dwUserID = pWriteGameScore->dwUserID;

		// 转化地址
		TCHAR szClientAddr[16] = { 0 };
		BYTE* pClientAddr = (BYTE*)&pWriteGameScore->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();

		//用户信息
		m_GameDBAide.AddParameter(TEXT("@dwUserID"), pWriteGameScore->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwDBQuestID"), pWriteGameScore->dwDBQuestID);
		m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"), pWriteGameScore->dwInoutIndex);

		//变更成绩
		m_GameDBAide.AddParameter(TEXT("@lScore"), pWriteGameScore->VariationInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lGrade"), pWriteGameScore->VariationInfo.lGrade);
		m_GameDBAide.AddParameter(TEXT("@lInsure"), pWriteGameScore->VariationInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lRevenue"), pWriteGameScore->VariationInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lWinCount"), pWriteGameScore->VariationInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lLostCount"), pWriteGameScore->VariationInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lDrawCount"), pWriteGameScore->VariationInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lFleeCount"), pWriteGameScore->VariationInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lUserMedal"), pWriteGameScore->VariationInfo.dwUserMedal);
		m_GameDBAide.AddParameter(TEXT("@lExperience"), pWriteGameScore->VariationInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@lLoveLiness"), pWriteGameScore->VariationInfo.lLoveLiness);
		m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"), pWriteGameScore->VariationInfo.dwPlayTimeCount);

		//属性信息
		m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@wTableID"), pWriteGameScore->wTableID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		//有效投注
		m_GameDBAide.AddParameter(TEXT("@lBetScore"), 0);

		//执行查询
		m_GameDBAide.ExecuteProcess(TEXT("QP_P_WriteGameScore_java"), false);
		
	PROCESS_END()
}


//离开房间
bool CDataBaseEngineSink::OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		DBR_GR_LeaveGameServer* pLeaveGameServer = (DBR_GR_LeaveGameServer*)pData;
		dwUserID = pLeaveGameServer->dwUserID;

		// 转化地址
		TCHAR szClientAddr[16] = { 0 };
		BYTE* pClientAddr = (BYTE*)&pLeaveGameServer->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();

		//用户信息
		m_GameDBAide.AddParameter(TEXT("@dwUserID"), pLeaveGameServer->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwOnLineTimeCount"), pLeaveGameServer->dwOnLineTimeCount);
		m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"), pLeaveGameServer->dwInoutIndex);
		m_GameDBAide.AddParameter(TEXT("@dwLeaveReason"), pLeaveGameServer->dwLeaveReason);
		m_GameDBAide.AddParameter(TEXT("@lRecordScore"), pLeaveGameServer->RecordInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lRecordGrade"), pLeaveGameServer->RecordInfo.lGrade);
		m_GameDBAide.AddParameter(TEXT("@lRecordInsure"), pLeaveGameServer->RecordInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lRecordRevenue"), pLeaveGameServer->RecordInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lRecordWinCount"), pLeaveGameServer->RecordInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordLostCount"), pLeaveGameServer->RecordInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordDrawCount"), pLeaveGameServer->RecordInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordFleeCount"), pLeaveGameServer->RecordInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordUserMedal"), pLeaveGameServer->RecordInfo.dwUserMedal);
		m_GameDBAide.AddParameter(TEXT("@lRecordExperience"), pLeaveGameServer->RecordInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@lRecordLoveLiness"), pLeaveGameServer->RecordInfo.lLoveLiness);
		m_GameDBAide.AddParameter(TEXT("@dwRecordPlayTimeCount"), pLeaveGameServer->RecordInfo.dwPlayTimeCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationScore"), pLeaveGameServer->VariationInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lVariationGrade"), pLeaveGameServer->VariationInfo.lGrade);
		m_GameDBAide.AddParameter(TEXT("@lVariationInsure"), pLeaveGameServer->VariationInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lVariationRevenue"), pLeaveGameServer->VariationInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lVariationWinCount"), pLeaveGameServer->VariationInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationLostCount"), pLeaveGameServer->VariationInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationDrawCount"), pLeaveGameServer->VariationInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationFleeCount"), pLeaveGameServer->VariationInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationUserMedal"), pLeaveGameServer->VariationInfo.dwUserMedal);
		m_GameDBAide.AddParameter(TEXT("@lVariationExperience"), pLeaveGameServer->VariationInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@lVariationLoveLiness"), pLeaveGameServer->VariationInfo.lLoveLiness);
		m_GameDBAide.AddParameter(TEXT("@dwVariationPlayTimeCount"), pLeaveGameServer->VariationInfo.dwPlayTimeCount);
		m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"), pLeaveGameServer->szMachineID);

		m_GameDBAide.AddParameter(TEXT("@lPower"),pLeaveGameServer->VariationInfo.lPower);
		m_GameDBAide.AddParameter(TEXT("@lJewel"),pLeaveGameServer->VariationInfo.lJewel);

		//执行查询
		//m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LeaveGameServer"), false);
		I_TRACE(TEXT("---------------LeaveGameServer UserID[%d],SocketID[%d]"), dwUserID,dwContextID);
		m_GameDBAide.ExecuteProcess(TEXT("QP_P_LeaveGameServer"), false);

	PROCESS_END()
}

//捕鱼游戏记录
bool CDataBaseEngineSink::OnRequestGameFishRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()


	DBR_GP_GameFishRecord* pGameFishRecord = (DBR_GP_GameFishRecord*)pData;
	dwUserID = INVALID_DWORD;

	
	//写入记录
	for (int i = 0; i < pGameFishRecord->wRecordCount; i++)
	{
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@dwUserID"), pGameFishRecord->FishRecord[i].dwUserID);
		m_GameDBAide.AddParameter(TEXT("@wTableID"), pGameFishRecord->wTableID);

		m_GameDBAide.AddParameter(TEXT("@dwRoomRatio"), pGameFishRecord->FishRecord[i].dwRoomRatio);
		m_GameDBAide.AddParameter(TEXT("@dwBulletID"), pGameFishRecord->FishRecord[i].dwBulletID);
		m_GameDBAide.AddParameter(TEXT("@dwBulletScore"), pGameFishRecord->FishRecord[i].dwBulletScore);
		m_GameDBAide.AddParameter(TEXT("@dwFishID"), pGameFishRecord->FishRecord[i].dwFishID);
		m_GameDBAide.AddParameter(TEXT("@dwFishType"), pGameFishRecord->FishRecord[i].dwFishType);
		m_GameDBAide.AddParameter(TEXT("@dwFishScore"), pGameFishRecord->FishRecord[i].dwFishScore);
		m_GameDBAide.AddParameter(TEXT("@dwRevenueScore"), pGameFishRecord->FishRecord[i].dwRevenueScore);

		m_GameDBAide.AddParameter(TEXT("@dwSceneType"), pGameFishRecord->FishRecord[i].dwSceneType);
		m_GameDBAide.AddParameter(TEXT("@dwScoreType"), pGameFishRecord->FishRecord[i].dwScoreType);
		m_GameDBAide.AddParameter(TEXT("@dwBulletSum"), pGameFishRecord->FishRecord[i].dwBulletSum);
		m_GameDBAide.AddParameter(TEXT("@dwBulletAllScore"), pGameFishRecord->FishRecord[i].dwBulletAllScore);
		m_GameDBAide.AddParameter(TEXT("@dwCheckFishAllScore"), pGameFishRecord->FishRecord[i].dwCheckFishAllScore);
		m_GameDBAide.AddParameter(TEXT("@dwScore"), pGameFishRecord->FishRecord[i].dwScore);
		m_GameDBAide.AddParameter(TEXT("@dwStartScore"), pGameFishRecord->FishRecord[i].dwStartScore);
		m_GameDBAide.AddParameter(TEXT("@dwEndScore"), pGameFishRecord->FishRecord[i].dwEndScore);

		m_GameDBAide.AddParameter(TEXT("@stStartTiem"), pGameFishRecord->FishRecord[i].stStartTiem);
		m_GameDBAide.AddParameter(TEXT("@stEndTiem"), pGameFishRecord->FishRecord[i].stEndTiem);

		m_GameDBAide.AddParameter(TEXT("@szGameDetails"), pGameFishRecord->FishRecord[i].szGameDetails);

		m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_GameFishRecord"), false);

	}
	
	

	PROCESS_END()
}


// 游戏滚动消息
bool CDataBaseEngineSink::OnRequestGameScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()
		
	CMD_S_Scroll_Message* pScrollMessage = (CMD_S_Scroll_Message*)pData;

	TCHAR ScrollMsg[1024] = { 0 };
	_sntprintf(ScrollMsg, CountArray(ScrollMsg), TEXT("%s"), pScrollMessage->szTipMessage);


	m_PlatformDBAide.ResetParameter();
	m_PlatformDBAide.AddParameter(TEXT("@dwKindID"), m_pGameServiceOption->wKindID);
	m_PlatformDBAide.AddParameter(TEXT("@stScrollMsg"), ScrollMsg);

	LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_GameScrollMessage"), true);

	if (lResultCode == DB_SUCCESS)
	{
		//TCHAR szScrollMessage[1024] = { 0 };
		//m_PlatformDBAide.GetValue_String(TEXT("MessageString"), szScrollMessage, CountArray(szScrollMessage));
	}

	
	PROCESS_END()
	return true;
}

//游戏记录
bool CDataBaseEngineSink::OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()


		DBR_GR_GameScoreRecord* pGameScoreRecord = (DBR_GR_GameScoreRecord*)pData;
		dwUserID = INVALID_DWORD;

		if (m_pGameServiceOption->wKindID == 2016)
		{
			//写入记录
			for (int i = 0; i < pGameScoreRecord->wRecordCount; i++)
			{
				m_GameDBAide.ResetParameter();
				m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
				m_GameDBAide.AddParameter(TEXT("@dwUserID"), pGameScoreRecord->GameScoreRecord[i].dwUserID);
				m_GameDBAide.AddParameter(TEXT("@wTableID"), pGameScoreRecord->wTableID);
				m_GameDBAide.AddParameter(TEXT("@dwDBQuestID"), pGameScoreRecord->GameScoreRecord[i].dwDBQuestID);
				m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"), pGameScoreRecord->GameScoreRecord[i].dwInoutIndex);
				m_GameDBAide.AddParameter(TEXT("@lScore"), pGameScoreRecord->GameScoreRecord[i].lScore);
				m_GameDBAide.AddParameter(TEXT("@lGrade"), pGameScoreRecord->GameScoreRecord[i].lGrade);
				m_GameDBAide.AddParameter(TEXT("@lRevenue"), pGameScoreRecord->GameScoreRecord[i].lRevenue);
				m_GameDBAide.AddParameter(TEXT("@dwUserMedal"), pGameScoreRecord->GameScoreRecord[i].dwUserMemal);
				m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"), pGameScoreRecord->GameScoreRecord[i].dwPlayTimeCount);
				//增加记录
				m_GameDBAide.AddParameter(TEXT("@lAddScore"), pGameScoreRecord->GameScoreRecord[i].lAddScore);
				m_GameDBAide.AddParameter(TEXT("@TCardDate"), pGameScoreRecord->GameScoreRecord[i].TCardDate);
				m_GameDBAide.AddParameter(TEXT("@TGameProce"), pGameScoreRecord->GameScoreRecord[i].TGameProce);
				
				m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawScore_Fish"), false);
				
			}
		}
		else
		{

			m_GameDBAide.ResetParameter();
			m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
			m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
			m_GameDBAide.AddParameter(TEXT("@wTableID"), pGameScoreRecord->wTableID);
			m_GameDBAide.AddParameter(TEXT("@wUserCount"), pGameScoreRecord->wUserCount);
			m_GameDBAide.AddParameter(TEXT("@wAndroidCount"), pGameScoreRecord->wAndroidCount);
			m_GameDBAide.AddParameter(TEXT("@lWasteCount"), pGameScoreRecord->lWasteCount);
			m_GameDBAide.AddParameter(TEXT("@lRevenueCount"), pGameScoreRecord->lRevenueCount);
			m_GameDBAide.AddParameter(TEXT("@dwUserMemal"), pGameScoreRecord->dwUserMemal);
			m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"), pGameScoreRecord->dwPlayTimeCount);
			m_GameDBAide.AddParameter(TEXT("@SystemTimeStart"), pGameScoreRecord->SystemTimeStart);
			m_GameDBAide.AddParameter(TEXT("@SystemTimeConclude"), pGameScoreRecord->SystemTimeConclude);

			if (DB_SUCCESS == m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawInfo"), true))
			{
				//获取标识
				DWORD dwDrawID = m_GameDBAide.GetValue_DWORD(TEXT("DrawID"));

				//写入记录
				for (int i = 0; i < pGameScoreRecord->wRecordCount; i++)
				{
					m_GameDBAide.ResetParameter();
					m_GameDBAide.AddParameter(TEXT("@dwDrawID"), dwDrawID);
					m_GameDBAide.AddParameter(TEXT("@dwUserID"), pGameScoreRecord->GameScoreRecord[i].dwUserID);
					m_GameDBAide.AddParameter(TEXT("@wChairID"), pGameScoreRecord->GameScoreRecord[i].wChairID);
					m_GameDBAide.AddParameter(TEXT("@dwDBQuestID"), pGameScoreRecord->GameScoreRecord[i].dwDBQuestID);
					m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"), pGameScoreRecord->GameScoreRecord[i].dwInoutIndex);
					m_GameDBAide.AddParameter(TEXT("@lScore"), pGameScoreRecord->GameScoreRecord[i].lScore);
					m_GameDBAide.AddParameter(TEXT("@lGrade"), pGameScoreRecord->GameScoreRecord[i].lGrade);
					m_GameDBAide.AddParameter(TEXT("@lRevenue"), pGameScoreRecord->GameScoreRecord[i].lRevenue);
					m_GameDBAide.AddParameter(TEXT("@dwUserMedal"), pGameScoreRecord->GameScoreRecord[i].dwUserMemal);
					m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"), pGameScoreRecord->GameScoreRecord[i].dwPlayTimeCount);
					//增加记录
					m_GameDBAide.AddParameter(TEXT("@lAddScore"), pGameScoreRecord->GameScoreRecord[i].lAddScore);
					m_GameDBAide.AddParameter(TEXT("@TCardDate"), pGameScoreRecord->GameScoreRecord[i].TCardDate);
					m_GameDBAide.AddParameter(TEXT("@TGameProce"), pGameScoreRecord->GameScoreRecord[i].TGameProce);
					
					m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawScore"), false);
					
				}
			}
		}

	PROCESS_END()
}

//加载参数
bool CDataBaseEngineSink::OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		DBO_GR_GameParameter GameParameter = { 0 };

		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);

		if (DB_SUCCESS == m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadParameter"), true))
		{
			GameParameter.wMedalRate = m_GameDBAide.GetValue_WORD(TEXT("MedalRate"));
			GameParameter.wRevenueRate = m_GameDBAide.GetValue_WORD(TEXT("RevenueRate"));
			GameParameter.dwClientVersion = m_GameDBAide.GetValue_DWORD(TEXT("ClientVersion"));
			GameParameter.dwServerVersion = m_GameDBAide.GetValue_DWORD(TEXT("ServerVersion"));

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_PARAMETER, dwContextID, &GameParameter, sizeof(GameParameter));
		}


	//读取第一次完成奖励配置
	//OnLoadFinishedFirstGameRreward(dwContextID,NULL,0,dwUserID);
	//清除卡线
	OnRequestKillLocker(dwContextID,NULL,0,dwUserID);
	//读取弹窗
	OnLoadTipMessage(dwContextID,NULL,0,dwUserID);

	PROCESS_END()
}

//加载列表
bool CDataBaseEngineSink::OnRequestLoadGameColumn(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		//变量定义
		DBO_GR_GameColumnInfo GameColumnInfo = { 0 };

		//执行查询
		m_GameDBAide.ResetParameter();
		GameColumnInfo.lResultCode = m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_GameColumnItem"),true);

		//读取信息
		for (int i = 0; i < CountArray(GameColumnInfo.ColumnItemInfo); i++)
		{
			//结束判断
			if (m_GameDBModule->IsRecordsetEnd()) break;

			//溢出效验
			ASSERT_ERR(GameColumnInfo.cbColumnCount < CountArray(GameColumnInfo.ColumnItemInfo));
			if (GameColumnInfo.cbColumnCount >= CountArray(GameColumnInfo.ColumnItemInfo)) break;

			//读取数据
			GameColumnInfo.cbColumnCount++;
			GameColumnInfo.ColumnItemInfo[i].cbColumnWidth = m_GameDBAide.GetValue_BYTE(TEXT("ColumnWidth"));
			GameColumnInfo.ColumnItemInfo[i].cbDataDescribe = m_GameDBAide.GetValue_BYTE(TEXT("DataDescribe"));
			m_GameDBAide.GetValue_String(TEXT("ColumnName"), GameColumnInfo.ColumnItemInfo[i].szColumnName, CountArray(GameColumnInfo.ColumnItemInfo[i].szColumnName));

			//移动记录
			m_GameDBModule->MoveToNext();
		}

		//发送信息
		WORD wHeadSize = sizeof(GameColumnInfo) - sizeof(GameColumnInfo.ColumnItemInfo);
		WORD wPacketSize = wHeadSize+GameColumnInfo.cbColumnCount * sizeof(GameColumnInfo.ColumnItemInfo[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_COLUMN_INFO, dwContextID, &GameColumnInfo, wPacketSize);

	PROCESS_END()
}

//加载机器
bool CDataBaseEngineSink::OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		//变量定义
		DBO_GR_GameAndroidInfo GameAndroidInfo = { 0 };

		//用户帐户
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceAttrib->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);

		//执行查询
		GameAndroidInfo.lResultCode = m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadAndroidUser"), true);

		//读取信息
		for (WORD i = 0; i < CountArray(GameAndroidInfo.AndroidParameter); i++)
		{
			//结束判断
			if (m_GameDBModule->IsRecordsetEnd()) break;

			//溢出效验
			ASSERT_ERR(GameAndroidInfo.wAndroidCount < CountArray(GameAndroidInfo.AndroidParameter));
			if (GameAndroidInfo.wAndroidCount >= CountArray(GameAndroidInfo.AndroidParameter)) break;

			//读取数据
			GameAndroidInfo.wAndroidCount++;
			GameAndroidInfo.AndroidParameter[i].dwUserID = m_GameDBAide.GetValue_DWORD(TEXT("UserID"));
			GameAndroidInfo.AndroidParameter[i].dwServiceTime = m_GameDBAide.GetValue_DWORD(TEXT("ServiceTime"));
			GameAndroidInfo.AndroidParameter[i].dwMinPlayDraw = m_GameDBAide.GetValue_DWORD(TEXT("MinPlayDraw"));
			GameAndroidInfo.AndroidParameter[i].dwMaxPlayDraw = m_GameDBAide.GetValue_DWORD(TEXT("MaxPlayDraw"));
			GameAndroidInfo.AndroidParameter[i].dwMinReposeTime = m_GameDBAide.GetValue_DWORD(TEXT("MinReposeTime"));
			GameAndroidInfo.AndroidParameter[i].dwMaxReposeTime = m_GameDBAide.GetValue_DWORD(TEXT("MaxReposeTime"));
			GameAndroidInfo.AndroidParameter[i].dwServiceGender = m_GameDBAide.GetValue_DWORD(TEXT("ServiceGender"));
			GameAndroidInfo.AndroidParameter[i].lMinTakeScore = m_GameDBAide.GetValue_LONGLONG(TEXT("MinTakeScore"));
			GameAndroidInfo.AndroidParameter[i].lMaxTakeScore = m_GameDBAide.GetValue_LONGLONG(TEXT("MaxTakeScore"));

			//移动记录
			m_GameDBModule->MoveToNext();
		}

		//发送信息
		WORD wHeadSize = sizeof(GameAndroidInfo) - sizeof(GameAndroidInfo.AndroidParameter);
		WORD wDataSize = GameAndroidInfo.wAndroidCount * sizeof(GameAndroidInfo.AndroidParameter[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_GAME_ANDROID_INFO, dwContextID, &GameAndroidInfo, wHeadSize + wDataSize);

	PROCESS_END()
}

//加载道具
bool CDataBaseEngineSink::OnRequestLoadGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	PROCESS_BEGIN()

		//变量定义
		DBO_GR_GamePropertyInfo GamePropertyInfo = { 0 };

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);

		//执行查询
		GamePropertyInfo.lResultCode = m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadGameProperty"), true);

		//读取信息
		for (WORD i=0; i < CountArray(GamePropertyInfo.PropertyInfo); i++)
		{
			//结束判断
			if (m_GameDBModule->IsRecordsetEnd()) break;

			//溢出效验
			ASSERT_ERR(GamePropertyInfo.cbPropertyCount < CountArray(GamePropertyInfo.PropertyInfo));
			if (GamePropertyInfo.cbPropertyCount >= CountArray(GamePropertyInfo.PropertyInfo)) break;

			//读取数据
			GamePropertyInfo.cbPropertyCount++;
			GamePropertyInfo.PropertyInfo[i].wIndex = m_GameDBAide.GetValue_WORD(TEXT("ID"));
			GamePropertyInfo.PropertyInfo[i].wDiscount = m_GameDBAide.GetValue_WORD(TEXT("Discount"));
			GamePropertyInfo.PropertyInfo[i].wIssueArea = m_GameDBAide.GetValue_WORD(TEXT("IssueArea"));
			GamePropertyInfo.PropertyInfo[i].dPropertyCash = m_GameDBAide.GetValue_DOUBLE(TEXT("Cash"));
			GamePropertyInfo.PropertyInfo[i].lPropertyGold = m_GameDBAide.GetValue_LONGLONG(TEXT("Gold"));
			GamePropertyInfo.PropertyInfo[i].lSendLoveLiness = m_GameDBAide.GetValue_LONGLONG(TEXT("SendLoveLiness"));
			GamePropertyInfo.PropertyInfo[i].lRecvLoveLiness = m_GameDBAide.GetValue_LONGLONG(TEXT("RecvLoveLiness"));

			//移动记录
			m_GameDBModule->MoveToNext();
		}

		//发送信息
		WORD wHeadSize = sizeof(GamePropertyInfo) - sizeof(GamePropertyInfo.PropertyInfo);
		WORD wDataSize = GamePropertyInfo.cbPropertyCount * sizeof(GamePropertyInfo.PropertyInfo[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_PROPERTY_INFO, dwContextID, &GamePropertyInfo, wHeadSize + wDataSize);

	PROCESS_END()
}

//存入元宝
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(DBR_GR_UserSaveScore));
	if (wDataSize!=sizeof(DBR_GR_UserSaveScore)) return false;

	//变量定义
	DBR_GR_UserSaveScore * pUserSaveScore=(DBR_GR_UserSaveScore *)pData;
	dwUserID=pUserSaveScore->dwUserID;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,pUserSaveScore->lSaveScore,CW2CT(DBVarValue.bstrVal),false,pUserSaveScore->cbActivityGame);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestUserSaveScore Error"),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,pUserSaveScore->lSaveScore,TEXT("由于数据库操作异常，请您稍后重试！"),false,pUserSaveScore->cbActivityGame);

		return false;
	}

	return true;
}

//提取元宝
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(DBR_GR_UserTakeScore));
	if (wDataSize!=sizeof(DBR_GR_UserTakeScore)) return false;

	//变量定义
	DBR_GR_UserTakeScore * pUserTakeScore=(DBR_GR_UserTakeScore *)pData;
	dwUserID=pUserTakeScore->dwUserID;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,0L,CW2CT(DBVarValue.bstrVal),false,pUserTakeScore->cbActivityGame);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestUserTakeScore Error"),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false,pUserTakeScore->cbActivityGame);

		return false;
	}

	return true;
}

//转账元宝
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(DBR_GR_UserTransferScore));
	if (wDataSize!=sizeof(DBR_GR_UserTransferScore)) return false;

	//变量定义
	DBR_GR_UserTransferScore * pUserTransferScore=(DBR_GR_UserTransferScore *)pData;
	dwUserID=pUserTransferScore->dwUserID;

	//请求处理
	try
	{
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
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
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
		OnInsureDisposeResult(dwContextID,lResultCode,0L,CW2CT(DBVarValue.bstrVal),false,pUserTransferScore->cbActivityGame);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestUserTransferScore Error"),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false,pUserTransferScore->cbActivityGame);

		return false;
	}

	return true;
}

//查询银行
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(DBR_GR_QueryInsureInfo));
	if (wDataSize!=sizeof(DBR_GR_QueryInsureInfo)) return false;

	//请求处理
	DBR_GR_QueryInsureInfo * pQueryInsureInfo=(DBR_GR_QueryInsureInfo *)pData;
	dwUserID=pQueryInsureInfo->dwUserID;

	try
	{
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
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GR_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//银行信息
			UserInsureInfo.cbActivityGame=pQueryInsureInfo->cbActivityGame;
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),0L,CW2CT(DBVarValue.bstrVal),false,pQueryInsureInfo->cbActivityGame);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestQueryInsureInfo Error"),TraceLevel_Exception);

		//结果处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false,pQueryInsureInfo->cbActivityGame);

		return false;
	}

	return true;
}

//查询用户
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT_ERR(wDataSize==sizeof(DBR_GR_QueryTransferUserInfo));
	if (wDataSize!=sizeof(DBR_GR_QueryTransferUserInfo)) return false;

	//请求处理
	DBR_GR_QueryTransferUserInfo * pQueryTransferUserInfo=(DBR_GR_QueryTransferUserInfo *)pData;
	dwUserID=pQueryTransferUserInfo->dwUserID;

	try
	{
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
			DBO_GR_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//银行信息
			TransferUserInfo.cbActivityGame=pQueryTransferUserInfo->cbActivityGame;
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), TransferUserInfo.szNickName, CountArray(TransferUserInfo.szNickName));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),0L,CW2CT(DBVarValue.bstrVal),false,pQueryTransferUserInfo->cbActivityGame);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestQueryTransferUserInfo Error"),TraceLevel_Exception);

		//结果处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false,pQueryTransferUserInfo->cbActivityGame);

		return false;
	}

	return true;
}

//道具请求
bool CDataBaseEngineSink::OnRequestPropertyRequest(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	return true;
}

bool CDataBaseEngineSink::OnRequestKillLocker(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_KillGameScoreLocker"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestKillLocker Error"),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}
}

bool CDataBaseEngineSink::OnRequestWriteUserStatus(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	DWORD *pdwUserID = (DWORD* )pData;
	dwUserID = *pdwUserID;
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),*pdwUserID);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("QP_P_WriteUserStatus"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestWriteUserStatus Error"),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}
}

//用户权限
bool CDataBaseEngineSink::OnRequestManageUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//效验参数
		ASSERT_ERR(wDataSize==sizeof(DBR_GR_ManageUserRight));
		if (wDataSize!=sizeof(DBR_GR_ManageUserRight)) return false;

		//请求处理
		DBR_GR_ManageUserRight * pManageUserRight=(DBR_GR_ManageUserRight *)pData;
		dwUserID=pManageUserRight->dwUserID;

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pManageUserRight->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwAddRight"),pManageUserRight->dwAddRight);
		m_GameDBAide.AddParameter(TEXT("@dwRemoveRight"),pManageUserRight->dwRemoveRight);

		//执行查询
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_ManageUserRight"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestManageUserRight Error"),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//系统消息
bool CDataBaseEngineSink::OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//执行查询
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSystemMessage"),true);

		//结果处理
		if(lReturnValue==0)
		{
			TCHAR szServerID[32]={0};
			_sntprintf(szServerID, CountArray(szServerID), TEXT("%d"), m_pGameServiceOption->wServerID);

			while(true)
			{
				//结束判断
				if (m_GameDBModule->IsRecordsetEnd()==true) break;

				//定义变量
				TCHAR szServerRange[1024]={0};
				CString strServerRange;
				bool bSendMessage=false;
				bool bAllRoom=false;

				//读取范围
				m_GameDBAide.GetValue_String(TEXT("ServerRange"), szServerRange, CountArray(szServerRange));
				szServerRange[1023]=0;
				strServerRange.Format(TEXT("%s"), szServerRange);

				//范围判断
				while(true)
				{
					int nfind=strServerRange.Find(TEXT(','));
					if(nfind!=-1 && nfind>0)
					{
						CString strID=strServerRange.Left(nfind);
						WORD wServerID=StrToInt(strID);
						bSendMessage=(wServerID==0 || wServerID==m_pGameServiceOption->wServerID);
						if(wServerID==0)bAllRoom=true;

						if(bSendMessage) break;

						strServerRange=strServerRange.Right(strServerRange.GetLength()-nfind-1);
					}
					else
					{
						WORD wServerID=StrToInt(szServerRange);
						bSendMessage=(wServerID==0 || wServerID==m_pGameServiceOption->wServerID);
						if(wServerID==0)bAllRoom=true;

						break;
					}
				}

				//发送消息
				if(bSendMessage)
				{
					//定义变量
					DBR_GR_SystemMessage SystemMessage;
					ZeroMemory(&SystemMessage, sizeof(SystemMessage));

					//读取消息
					SystemMessage.cbMessageID=m_GameDBAide.GetValue_BYTE(TEXT("ID"));
					SystemMessage.cbMessageType=m_GameDBAide.GetValue_BYTE(TEXT("MessageType"));
					SystemMessage.dwTimeRate=m_GameDBAide.GetValue_DWORD(TEXT("TimeRate"));
					SystemMessage.cbAllRoom=bAllRoom?TRUE:FALSE;
					m_GameDBAide.GetValue_String(TEXT("MessageString"),SystemMessage.szSystemMessage,CountArray(SystemMessage.szSystemMessage));

					//发送结果
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SYSTEM_MESSAGE_RESULT,dwContextID,&SystemMessage,sizeof(SystemMessage));
				}

				//下一条
				m_GameDBModule->MoveToNext();
			}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestLoadSystemMessage Error"),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//加载敏感词
bool CDataBaseEngineSink::OnRequestLoadSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//构造参数
		m_PlatformDBAide.ResetParameter();

		//执行查询
		LONG lReturnValue=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSensitiveWords"),true);

		//读取信息
		if (lReturnValue==DB_SUCCESS)
		{
			//起始消息
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0xfffe,NULL,0);

			//读取消息
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//变量定义
				TCHAR szSensitiveWords[32]=TEXT("");

				//读取消息
				m_PlatformDBAide.GetValue_String(TEXT("SensitiveWords"),szSensitiveWords,CountArray(szSensitiveWords));				

				//发送消息
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0,szSensitiveWords,sizeof(szSensitiveWords));

				//移动记录
				m_PlatformDBModule->MoveToNext();
			};

			//结束消息
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0xffff,NULL,0);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestLoadSensitiveWords Error"),TraceLevel_Exception);

		return false;
	}

	return true;
}

//登录结果
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	if (dwErrorCode != DB_SUCCESS)
	{
		//变量定义
		DBO_GR_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));

		//构造数据
		LogonFailure.lResultCode=dwErrorCode;
		lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

		I_TRACE(TEXT("SocketID[%d],%s"),dwContextID,pszErrorString);
		//发送结果
		WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);

		return;
	}

	try
	{

		//属性资料
		m_LogonSuccess.wFaceID=m_GameDBAide.GetValue_DWORD(TEXT("FaceID"));
		m_LogonSuccess.dwUserID=m_GameDBAide.GetValue_DWORD(TEXT("UserID"));
		m_LogonSuccess.dwGameID=m_GameDBAide.GetValue_DWORD(TEXT("GameID"));
		m_LogonSuccess.dwGroupID=m_GameDBAide.GetValue_DWORD(TEXT("GroupID"));
		m_LogonSuccess.dwCustomID=m_GameDBAide.GetValue_DWORD(TEXT("CustomID"));
		m_GameDBAide.GetValue_String(TEXT("NickName"),m_LogonSuccess.szNickName,CountArray(m_LogonSuccess.szNickName));
		m_GameDBAide.GetValue_String(TEXT("GroupName"),m_LogonSuccess.szGroupName,CountArray(m_LogonSuccess.szGroupName));

		//用户资料
		m_LogonSuccess.cbGender=m_GameDBAide.GetValue_BYTE(TEXT("Gender"));
		m_LogonSuccess.cbMemberOrder=m_GameDBAide.GetValue_BYTE(TEXT("MemberOrder"));
		m_LogonSuccess.cbMasterOrder=m_GameDBAide.GetValue_BYTE(TEXT("MasterOrder"));
		m_GameDBAide.GetValue_String(TEXT("UnderWrite"),m_LogonSuccess.szUnderWrite,CountArray(m_LogonSuccess.szUnderWrite));

		//积分信息
		m_LogonSuccess.lScore=m_GameDBAide.GetValue_LONGLONG(TEXT("Score"));
		m_LogonSuccess.lInsure=m_GameDBAide.GetValue_LONGLONG(TEXT("Insure"));
		m_LogonSuccess.lRechargeScore = m_GameDBAide.GetValue_LONGLONG(TEXT("RechargeScore"));
		m_LogonSuccess.lExchangeScore = m_GameDBAide.GetValue_LONGLONG(TEXT("ExchangeScore"));
		m_LogonSuccess.lRevenue = m_GameDBAide.GetValue_LONGLONG(TEXT("Revenue"));
		 
		I_TRACE(TEXT("lScore=%I64d %I64d %I64d %I64d %I64d "), 
			m_LogonSuccess.lScore, m_LogonSuccess.lInsure, m_LogonSuccess.lRechargeScore,
			m_LogonSuccess.lExchangeScore, m_LogonSuccess.lRevenue);

		//局数信息
		m_LogonSuccess.dwWinCount=m_GameDBAide.GetValue_LONG(TEXT("WinCount"));
		m_LogonSuccess.dwLostCount=m_GameDBAide.GetValue_LONG(TEXT("LostCount"));
		m_LogonSuccess.dwDrawCount=m_GameDBAide.GetValue_LONG(TEXT("DrawCount"));
		m_LogonSuccess.dwFleeCount=m_GameDBAide.GetValue_LONG(TEXT("FleeCount"));
		m_LogonSuccess.dwUserMedal=m_GameDBAide.GetValue_LONG(TEXT("UserMedal"));
		m_LogonSuccess.dwExperience=m_GameDBAide.GetValue_LONG(TEXT("Experience"));
		m_LogonSuccess.lLoveLiness=m_GameDBAide.GetValue_LONG(TEXT("LoveLiness"));
		m_LogonSuccess.wAvatarID = m_GameDBAide.GetValue_LONG(TEXT("UserAvatar"));

		//附加信息
		m_LogonSuccess.dwUserRight=m_GameDBAide.GetValue_DWORD(TEXT("UserRight"));
		m_LogonSuccess.dwMasterRight=m_GameDBAide.GetValue_DWORD(TEXT("MasterRight"));
		m_LogonSuccess.cbDeviceType=cbDeviceType;
		m_LogonSuccess.wBehaviorFlags=wBehaviorFlags;
		m_LogonSuccess.wPageTableCount=wPageTableCount;

		//索引变量
		m_LogonSuccess.dwInoutIndex=m_GameDBAide.GetValue_DWORD(TEXT("InoutIndex"));

		m_LogonSuccess.cbIsAndroid = m_GameDBAide.GetValue_BYTE(TEXT("IsAndroid"));

		//获取信息
		if(pszErrorString!=NULL)lstrcpyn(m_LogonSuccess.szDescribeString,pszErrorString,CountArray(m_LogonSuccess.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(m_LogonSuccess.szDescribeString);
		WORD wHeadSize=sizeof(m_LogonSuccess)-sizeof(m_LogonSuccess.szDescribeString);
		if ( cbDeviceType == DEVICE_TYPE_H5)
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_SUCCESS_H5, dwContextID, &m_LogonSuccess, wHeadSize + wDataSize);
		else m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_SUCCESS,dwContextID,&m_LogonSuccess,wHeadSize+wDataSize);
		//g_Log.LogToFile(g_szLogRoot,TEXT("\n OnLogonDisposeResult UserID[%d]:NickName[%s]:UserScore[%I64d]"),m_LogonSuccess.dwUserID,m_LogonSuccess.szNickName,m_LogonSuccess.lScore);

	}
	catch (IDataBaseException* pIException)
	{
		//输出错误
		TRACE_LOG(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		TRACE_LOG(TEXT("OnLogonDisposeResult Error"),TraceLevel_Exception);
		
		OnLogonDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试或者选择另一服务器登录"), false);

		return ;
	}

	return;
}

//银行结果
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbActivityGame)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_GR_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//构造变量
		UserInsureSuccess.cbActivityGame=cbActivityGame;
		UserInsureSuccess.lFrozenedScore=lFrozenedScore;
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
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GR_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//构造变量
		UserInsureFailure.cbActivityGame=cbActivityGame;
		UserInsureFailure.lResultCode=dwErrorCode;
		UserInsureFailure.lFrozenedScore=lFrozenedScore;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//申请机器人
bool CDataBaseEngineSink::OnRequestAndriodApply(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(DBR_GR_GameAndroidApply));
		if (wDataSize != sizeof(DBR_GR_GameAndroidApply)) return false;

		DBR_GR_GameAndroidApply* pAndriodApplly = (DBR_GR_GameAndroidApply*)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@room_guid"), pAndriodApplly->guidRoom);
		m_AccountsDBAide.AddParameter(TEXT("@count"), pAndriodApplly->dwCount);
		m_AccountsDBAide.AddParameter(TEXT("@room_id"), pAndriodApplly->dwRoomID);

		DBO_GR_GameAndroidApply tagApplyInfo;
		ZeroMemory(&tagApplyInfo, sizeof(tagApplyInfo));

		tagApplyInfo.lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("Android_Apply"), true);

		for (int i = 0; i < CountArray(tagApplyInfo.AndroidParameter); i++)
		{
			if (m_AccountsDBModule->IsRecordsetEnd()) break;		

			ASSERT_ERR(tagApplyInfo.wAndroidCount < CountArray(tagApplyInfo.AndroidParameter));
			if (tagApplyInfo.wAndroidCount >= CountArray(tagApplyInfo.AndroidParameter)) break;;

			tagApplyInfo.wAndroidCount++;
			tagApplyInfo.AndroidParameter[i].dwUserID			= m_AccountsDBAide.GetValue_DWORD(TEXT("user_id"));
			tagApplyInfo.AndroidParameter[i].dwServiceTime		= m_AccountsDBAide.GetValue_DWORD(TEXT("ServiceTime"));
			tagApplyInfo.AndroidParameter[i].dwMinPlayDraw		= m_AccountsDBAide.GetValue_DWORD(TEXT("MinPlayDraw"));
			tagApplyInfo.AndroidParameter[i].dwMaxPlayDraw		= m_AccountsDBAide.GetValue_DWORD(TEXT("MaxPlayDraw"));
			tagApplyInfo.AndroidParameter[i].dwMinReposeTime	= m_AccountsDBAide.GetValue_DWORD(TEXT("MinReposeTime"));
			tagApplyInfo.AndroidParameter[i].dwMaxReposeTime	= m_AccountsDBAide.GetValue_DWORD(TEXT("MaxReposeTime"));
			tagApplyInfo.AndroidParameter[i].dwServiceGender	= m_AccountsDBAide.GetValue_DWORD(TEXT("ServiceGender"));
			tagApplyInfo.AndroidParameter[i].lMinTakeScore		= m_AccountsDBAide.GetValue_LONGLONG(TEXT("MinTakeScore"));
			tagApplyInfo.AndroidParameter[i].lMaxTakeScore		= m_AccountsDBAide.GetValue_LONGLONG(TEXT("MaxTakeScore"));

			m_AccountsDBModule->MoveToNext();
		}

		WORD wHeadSize=sizeof(tagApplyInfo)-sizeof(tagApplyInfo.AndroidParameter);
		WORD wDataSize=tagApplyInfo.wAndroidCount*sizeof(tagApplyInfo.AndroidParameter[0]);

		if (m_pGameServiceOption->wServerType == GAME_GENRE_MATCH)
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_APPLY_ANDROID,dwContextID,&tagApplyInfo,wHeadSize+wDataSize);
		else
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_ANDRIOD_APPLY,dwContextID,&tagApplyInfo,wHeadSize+wDataSize);


		return true;
	}
	catch (IDataBaseException* pIException)
	{
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestAndriodApply Error"),TraceLevel_Exception);
		return false;
	}
}

//刷新机器人时间戳
bool CDataBaseEngineSink::OnRequestAndriodRefresh(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(DBR_GR_GameAndriodRefresh));
		if (wDataSize != sizeof(DBR_GR_GameAndriodRefresh)) return false;

		DBR_GR_GameAndriodRefresh* pAndriodApplly = (DBR_GR_GameAndriodRefresh*)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@room_guid"), pAndriodApplly->guidRoom);

		if (m_AccountsDBAide.ExecuteProcess(TEXT("Android_Refresh"), false)==DB_SUCCESS)
			return true;

		return false;
	}
	catch (IDataBaseException* pIException)
	{
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestAndriodRefresh Error"),TraceLevel_Exception);
		return false;
	}
}
//释放单个机器人
bool CDataBaseEngineSink::OnRequestAndriodRelease(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(DBR_GR_GameAndriodRelease));
		if (wDataSize != sizeof(DBR_GR_GameAndriodRelease)) return false;

		DBR_GR_GameAndriodRelease* pAndriodApplly = (DBR_GR_GameAndriodRelease*)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@room_guid"), pAndriodApplly->guidRoom);
		m_AccountsDBAide.AddParameter(TEXT("@user_id"), pAndriodApplly->dwUserId);

		if (m_AccountsDBAide.ExecuteProcess(TEXT("Android_Release"), false)==DB_SUCCESS)
			return true;

		return false;
	}
	catch (IDataBaseException* pIException)
	{
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestAndriodRelease Error"),TraceLevel_Exception);
		return false;
	}
}
//释放所有机器人
bool CDataBaseEngineSink::OnRequestAndriodALL(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(DBR_GR_GameAndriodAll));
		if (wDataSize != sizeof(DBR_GR_GameAndriodAll)) return false;

		DBR_GR_GameAndriodAll* pAndriodApplly = (DBR_GR_GameAndriodAll*)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@room_guid"), pAndriodApplly->guidRoom);

		if (m_AccountsDBAide.ExecuteProcess(TEXT("Android_ReleaseAll"), false)==DB_SUCCESS)
			return true;

		return false;
	}
	catch (IDataBaseException* pIException)
	{
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestAndriodALL Error"),TraceLevel_Exception);
		return false;
	}
}
//批量释放机器人
bool CDataBaseEngineSink::OnRequestAndriodArray(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(DBR_GR_GameAndriodArray));
		if (wDataSize != sizeof(DBR_GR_GameAndriodArray)) return false;

		DBR_GR_GameAndriodArray* pAndriodApplly = (DBR_GR_GameAndriodArray*)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@room_guid"), pAndriodApplly->guidRoom);
		m_AccountsDBAide.AddParameter(TEXT("@user_id_array"), pAndriodApplly->szUserArray);

		if (m_AccountsDBAide.ExecuteProcess(TEXT("Android_ReleaseArray"), false)==DB_SUCCESS)
			return true;

		return false;
	}
	catch (IDataBaseException* pIException)
	{
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestAndriodArray Error"),TraceLevel_Exception);
		return false;
	}
}

bool CDataBaseEngineSink::OnRequestAndriodReleaseAndDelete(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		ASSERT_ERR(wDataSize == sizeof(DBR_GR_GameAndriodReleaseAndDelete));
		if (wDataSize != sizeof(DBR_GR_GameAndriodReleaseAndDelete)) return false;

		DBR_GR_GameAndriodReleaseAndDelete* pAndriodApplly = (DBR_GR_GameAndriodReleaseAndDelete*)pData;

		//不进行数据库处理 在result中投递
		//m_AccountsDBAide.ResetParameter();
		//m_AccountsDBAide.AddParameter(TEXT("@room_guid"), pAndriodApplly->guidRoom);
		//m_AccountsDBAide.AddParameter(TEXT("@user_id"), pAndriodApplly->dwUserId);

		//if (m_AccountsDBAide.ExecuteProcess(TEXT("Android_Release"), false)==DB_SUCCESS)
		//{
		DBO_GR_GameAndriodRelease request = {0};
		request.dwUserId = pAndriodApplly->dwUserId;
		request.wBindId	 = pAndriodApplly->wBingId;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_ANDRIOD_RELEASE_DELTE, 0xffff, &request, sizeof request);
		return true;
		//}

		//return false;
	}
	catch (IDataBaseException* pIException)
	{
		TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		TRACE_LOG(TEXT("OnRequestAndriodReleaseAndDelete Error"),TraceLevel_Exception);
		return false;
	}
}


//通用属性刷新
bool CDataBaseEngineSink::OnUpdateAttribute(DWORD dwContextID, DBO_UPDATE_ATTRIBUTE::attribute attr,SCORE wScore )
{
	DBO_UPDATE_ATTRIBUTE attrJewel={0};
	attrJewel.wType = attr;
	attrJewel.wScore = wScore;
	return m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_UPDATE_ATTRIBUTE, dwContextID, &attrJewel, sizeof(DBO_UPDATE_ATTRIBUTE));
}

////////////////////////////////////////////////////////////////////////////////////////////////
