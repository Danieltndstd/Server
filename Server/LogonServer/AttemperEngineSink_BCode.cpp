#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include <string>
#include <atlimage.h>
#include <afxinet.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
namespace Utilss {
	std::string ConvertFromUtf16ToUtf8(const std::wstring& wstr)
	{
		std::string convertedString;
		int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<char> buffer(requiredSize);
			WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}
		return convertedString;
	}

	std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
	{
		std::wstring convertedString;
		int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<wchar_t> buffer(requiredSize);
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}

		return convertedString;
	}

	//宽字节转窄字节
	std::string ConvertW2A(const std::wstring &wstr)
	{
		setlocale(LC_ALL, ".936");
		size_t nSize = wstr.length() * 2 + 1;
		char *psz = new char[nSize];

		memset(psz, 0, nSize);
		wcstombs(psz, wstr.c_str(), nSize);
		std::string str = psz;
		delete[]psz;

		return str;
	}

	//窄字节转宽字节
	std::wstring ConvertA2W(const string &str)
	{
		setlocale(LC_ALL, ".936");
		size_t nSize = str.length() + 1;
		wchar_t *wpsz = new wchar_t[nSize];

		memset(wpsz, 0, sizeof(wchar_t)*nSize);
		mbstowcs(wpsz, str.c_str(), nSize);
		std::wstring wstr = wpsz;
		delete[]wpsz;

		return wstr;
	}
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineBegin_BCode(IUnknownEx * pIUnknownEx)
{

	//事件注册
	{
		// 主命令注册
		{
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_MB_LOGON_BCode, CAttemperEngineSink::OnTCPNetworkMainMBLogon_BCode, this);	// 登录命令
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_HEART, CAttemperEngineSink::OnTCPNetworkMainHeart, this);	// 心跳命令
		}

		// 数据库事件注册
		{
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_MB_LOGON_SUCCESS_BCode, CAttemperEngineSink::OnDBMBLogonSuccess_BCode, this);	// 登录成功	
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_MB_LOGON_FAILURE_BCode, CAttemperEngineSink::OnDBMBLogonFailure_BCode, this);	// 登录失败

			//签到
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_TODAY_IS_SIGNIN,CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_TODAY_IS_SIGNIN); //今天是否签到
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_SIGNIN,CAttemperEngineSink::OnDBMBGetUserSignIn, this); //获取签到
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_SET_SIGNIN,CAttemperEngineSink::OnDBMBSetUserSignIn, this); //签到

			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_DAIL_INFO, CAttemperEngineSink::OnDBMBGetDailInfo, this); //获取转盘
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_ROTATEDAILAWARD, CAttemperEngineSink::OnDBMBRotateDailAward, this); //获得抽奖信息
			
			
			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_CHICKBINDBANK, CAttemperEngineSink::OnDBMBBindBank, this); //银行卡信息
			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_CHICKBINDZFB, CAttemperEngineSink::OnDBMBBindZFB, this); //支付宝信息

			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_REQUESTMAIL, CAttemperEngineSink::OnDBMBRequestMail, this); //请求邮件
			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_READDELEETETAIL, CAttemperEngineSink::OnDBMBReadDeleteMail, this); //读取删除邮件

			

			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_USER_PERFECT_INFORMATION, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_GET_USER_PERFECT_INFORMATION);	//获取用户完善信息
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_SPREAD_REGISTER, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_IS_SPREAD_REGISTER);	//是否是注册登录
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_BIND_SPREAD, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_IS_BIND_SPREAD);	//是否已经绑定推广
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_ALREADY_MODIFY_NICKNAME, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_IS_ALREADY_MODIFY_NICKNAME);	//是否已经修改过昵称	
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_BIND_USER_SPREAD, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_BIND_USER_SPREAD);	//绑定用户推广码
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_TODAY_FIRST_TIME_LOGIN, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_IS_TODAY_FIRST_TIME_LOGIN);	//是否是今天第一次登录
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_LOST_CONNECTION_LOGIN, CAttemperEngineSink::OnDBIsLostConnectionLogin, this);	//是否是掉线重新登录
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOAD_SCROLL_MESSAGE, CAttemperEngineSink::OnDBLoadScrollMseeage, this);	// 读取滚动消息
			
			
		}
	}



	return true;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon_BCode(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubMBLogonAccounts_BCode(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//帐号注册
		{
			return OnTCPNetworkSubMBRegisterAccounts_BCode(pData,wDataSize,dwSocketID);
		}                                          
	case SUB_MB_VISITOR_ACCOUNTS: //游客身份
		{
			return OnTCPNetworkSubMBVisitorAccounts_BCode(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_REGISTER_MOBILE: //手机注册
		{
			return OnTCPNetworkSubMBRegisterMobile_BCode(pData, wDataSize, dwSocketID);
		}
	}

	return false;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	//ASSERT_ERR(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	// (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false; 

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	//CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	//pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	//pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	//pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	//pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind= CLIENT_KIND_WEBSOCKET;
	//pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//版本判断
	//if (CheckMBPlazaVersion(pLogonAccounts->cbDeviceType, pLogonAccounts->dwPlazaVersion, pLogonAccounts->wModuleID, dwSocketID) == false) return true;

	LogonServer::PROTO_LogonAccountsBCode ProtoLogonAccountsBCode;
	ProtoLogonAccountsBCode.ParseFromArray(pData, wDataSize);


	//变量定义
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//附加信息
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts, Utilss::ConvertA2W(ProtoLogonAccountsBCode.szaccounts()).c_str(),CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword, Utilss::ConvertA2W(ProtoLogonAccountsBCode.szpassword()).c_str(),CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID, Utilss::ConvertA2W(ProtoLogonAccountsBCode.szmachineid()).c_str(),CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone, Utilss::ConvertA2W(ProtoLogonAccountsBCode.szmobilephone()).c_str(),CountArray(LogonAccounts.szMobilePhone));
	LogonAccounts.cbDeviceType= ProtoLogonAccountsBCode.cbdevicetype();
	LogonAccounts.dwInviteID = ProtoLogonAccountsBCode.dwinviteid();
	LogonAccounts.dwSortID= ProtoLogonAccountsBCode.dwsortid();
	LogonAccounts.dwLogonType = ProtoLogonAccountsBCode.dwlogontype();
	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS_BCode,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBLogonAccounts_BCode %s"),pLogonAccounts->szAccounts);
	return true;
}

//帐号注册
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT_ERR(wDataSize>=sizeof(CMD_MB_RegisterAccounts_BCode));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts_BCode)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

// 	{
// 		TCHAR szFilePath[MAX_PATH] = L"";
// 		GetCurrentDirectory(sizeof(szFilePath), szFilePath);
// 
// 		TCHAR szFullPath[MAX_PATH] = L"";
// 		TCHAR szDescribeString[128] = L"";
// 		DWORD dwIsOpenRegister = 1;
// 		swprintf(szFullPath, sizeof(szFullPath), TEXT("%s/Settings/policy.ini"), szFilePath);
// 		dwIsOpenRegister=GetPrivateProfileInt(TEXT("Parameter"),TEXT("IsOpenRegister"),1,(LPCTSTR)szFullPath);
// 		if ( dwIsOpenRegister == 0 )
// 		{
// 			GetPrivateProfileString(TEXT("Parameter"),TEXT("Desc"),TEXT("平台即将转向，请联系客服获取新平台地址"),szDescribeString,CountArray(szDescribeString),szFullPath);
// 
// 			CMD_MB_LogonFailure LogonFailure;
// 			ZeroMemory(&LogonFailure,sizeof(LogonFailure));
// 
// 			LogonFailure.lResultCode = 1;
// 			_tcscpy_s(LogonFailure.szDescribeString, szDescribeString);
// 
// 			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_LOGON_BCode,SUB_MB_LOGON_FAILURE,&LogonFailure,sizeof(CMD_MB_LogonFailure));
// 
// 			return true;
// 		}
// 	}

	//处理消息
// 	CMD_MB_RegisterAccounts_BCode * pRegisterAccounts=(CMD_MB_RegisterAccounts_BCode *)pData;
// 	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
// 	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
// 	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
// 	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
// 	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
// 	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	LogonServer::PROTO_RegisterAccountsBCode ProtoRegisterAccountsBCode;
	ProtoRegisterAccountsBCode.ParseFromArray(pData, wDataSize);


	//设置连接
	pBindParameter->cbClientKind= CLIENT_KIND_WEBSOCKET;
	//变量定义

	DBR_MB_RegisterAccounts_BCode RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.wFaceID = ProtoRegisterAccountsBCode.wfaceid();
	RegisterAccounts.cbGender = ProtoRegisterAccountsBCode.cbgender();
	RegisterAccounts.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
	RegisterAccounts.dwSpreaderID = ProtoRegisterAccountsBCode.dwspreaderid();
	lstrcpyn(RegisterAccounts.szAccounts, Utilss::ConvertA2W(ProtoRegisterAccountsBCode.szaccounts()).c_str(), CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName, Utilss::ConvertA2W(ProtoRegisterAccountsBCode.sznickname()).c_str(), CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID, Utilss::ConvertA2W(ProtoRegisterAccountsBCode.szmachineid()).c_str(), CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass, Utilss::ConvertA2W(ProtoRegisterAccountsBCode.szlogonpass()).c_str(), CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass, Utilss::ConvertA2W(ProtoRegisterAccountsBCode.szinsurepass()).c_str(), CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szMobilePhone, Utilss::ConvertA2W(ProtoRegisterAccountsBCode.szmobilephone()).c_str(), CountArray(RegisterAccounts.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS_BCode,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBRegisterAccounts_BCode %s"),pRegisterAccounts->szAccounts);
	return true;
}

//游客登录
bool CAttemperEngineSink::OnTCPNetworkSubMBVisitorAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	LogonServer::PROTO_LogonVisitor ProtoLogonVisitor;
	ProtoLogonVisitor.ParseFromArray(pData, wDataSize);


	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);
	//设置连接
	pBindParameter->cbClientKind = CLIENT_KIND_WEBSOCKET;
	pBindParameter->dwPlazaVersion = 0;


	//变量定义
	DBR_MB_VisitorAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szSpreader, Utilss::ConvertA2W(ProtoLogonVisitor.szspreader()).c_str(),CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szAccounts, Utilss::ConvertA2W(ProtoLogonVisitor.szmachineid()).c_str(), CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szMachineID, Utilss::ConvertA2W(ProtoLogonVisitor.szmachineid()).c_str(),CountArray(RegisterAccounts.szMachineID));
	RegisterAccounts.dwInviteID = ProtoLogonVisitor.dwinviteid();
	RegisterAccounts.dwSortID = ProtoLogonVisitor.dwsortid();

	RegisterAccounts.dwLogonType = ProtoLogonVisitor.dwlogontype();
	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_VISITOR_ACCOUNTS_BCode,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBVisitorAccounts_BCode %s"),pRegisterAccounts->szMachineID);
	return true;
}

// 手机注册账号
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterMobile_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	// 效验参数
	ASSERT_ERR(wDataSize == sizeof(CMD_MB_RegisterMobile));
	if (wDataSize != sizeof(CMD_MB_RegisterMobile))
		return false;

	// 变量定义
	DWORD wBindIndex = LOWORD(dwSocketID);
	tagBindParameter* pBindParameter = (m_pBindParameter + wBindIndex);

	// 处理消息
	CMD_MB_RegisterMobile* pRegisterMobile = (CMD_MB_RegisterMobile*)pData;
	pRegisterMobile->szMachineID[CountArray(pRegisterMobile->szMachineID) - 1] = 0;
	pRegisterMobile->szLogonPass[CountArray(pRegisterMobile->szLogonPass) - 1] = 0;
	pRegisterMobile->szMobilePhone[CountArray(pRegisterMobile->szMobilePhone) - 1] = 0;
	pRegisterMobile->szVerificationCode[CountArray(pRegisterMobile->szVerificationCode) - 1] = 0;

	// 设置连接
	pBindParameter->cbClientKind = CLIENT_KIND_MOBILE;

	// 变量定义
	DBR_MB_RegisterMobile RegisterMobile = { 0 };

	// 附加信息
	RegisterMobile.pBindParameter = (m_pBindParameter + LOWORD(dwSocketID));

	// 构造数据
	RegisterMobile.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterMobile.szMobile, pRegisterMobile->szMobilePhone, CountArray(RegisterMobile.szMobile));
	lstrcpyn(RegisterMobile.szMachineID, pRegisterMobile->szMachineID, CountArray(RegisterMobile.szMachineID));
	lstrcpyn(RegisterMobile.szLogonPass, pRegisterMobile->szLogonPass, CountArray(RegisterMobile.szLogonPass));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_MOBILE_BCode, dwSocketID, &RegisterMobile, sizeof(RegisterMobile));

	//I_TRACE(TEXT("OnTCPNetworkSubMBRegisterMobile_BCode %s"), pRegisterMobile->szMachineID);
	return true;
}

bool CAttemperEngineSink::OnDBMBLogonFailure_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_OperateResult * pLogonFailure = (DBO_GP_OperateResult *)pData;

	//构造数据
	LogonFailure.lResultCode = pLogonFailure->dwResultCode;
	_tcscpy_s(LogonFailure.szDescribeString, pLogonFailure->szDescribeString);

	LogonServer::PROTO_OperateResult ProtoOperateResult;
	ProtoOperateResult.set_wsubcommandid(0);
	ProtoOperateResult.set_dwresultcode(LogonFailure.lResultCode);
	ProtoOperateResult.set_szdescribestring(Utilss::ConvertFromUtf16ToUtf8(LogonFailure.szDescribeString));
	std::string serializeStr;
	ProtoOperateResult.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_MB_LOGON_BCode, SUB_MB_LOGON_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	//发送数据
	//m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON_BCode,SUB_MB_LOGON_FAILURE,&LogonFailure,sizeof(CMD_MB_LogonFailure));

	//关闭连接
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//通知客户端失败消息
bool CAttemperEngineSink::OnDBMBFailure_BCode(DWORD dwSocketID, LPCTSTR strErrorString)
{
	//变量定义
	CMD_GP_LogonFailure LogonFailure = { 0 };

	//构造数据
	lstrcpyn(LogonFailure.szDescribeString, strErrorString, CountArray(LogonFailure.szDescribeString));

	//发送数据
	WORD wStringSize = CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize = sizeof(LogonFailure) - sizeof(LogonFailure.szDescribeString) + wStringSize;

	m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON_BCode, SUB_GP_LOGON_FAILURE, &LogonFailure, wSendSize);

	return true;
}

//获取签到返回
bool CAttemperEngineSink::OnDBMBGetUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	
	DBO_GP_GetSignIn * pGetSignIn = (DBO_GP_GetSignIn *)pData;

	
	LogonServer::PROTO_GetSignInResult ProtoSignInResult;
	ProtoSignInResult.set_wsigninnum(pGetSignIn->wSignInNum);
	ProtoSignInResult.set_cbtodayissinin(pGetSignIn->cbTodayIsSinIn);
	ProtoSignInResult.set_dwresultcode(pGetSignIn->dwResultCode);
	ProtoSignInResult.set_szdescribestring(Utilss::ConvertFromUtf16ToUtf8(pGetSignIn->szDescribeString));
	std::string serializeStr;
	ProtoSignInResult.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_GET_DIAL_RESULT, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	

	return true;
}

// 设置签到返回
bool CAttemperEngineSink::OnDBMBSetUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;


	DBO_GP_GetSignIn * pGetSignIn = (DBO_GP_GetSignIn *)pData;


	LogonServer::PROTO_GetSignInResult ProtoSignInResult;
	ProtoSignInResult.set_wsigninnum(pGetSignIn->wSignInNum);
	ProtoSignInResult.set_cbtodayissinin(pGetSignIn->cbTodayIsSinIn);
	ProtoSignInResult.set_dwresultcode(pGetSignIn->dwResultCode);
	ProtoSignInResult.set_szdescribestring(Utilss::ConvertFromUtf16ToUtf8(pGetSignIn->szDescribeString));
	std::string serializeStr;
	ProtoSignInResult.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_GET_DIAL_RESULT, (void*)serializeStr.c_str(), (WORD)serializeStr.size());


	return true;
}

// 获取转盘
bool CAttemperEngineSink::OnDBMBGetDailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;


	DBO_GP_GetDailInfo * pGetDailInfo = (DBO_GP_GetDailInfo *)pData;


	LogonServer::PROTO_GetDailInfoResult ProtoDailInfoResult;
	ProtoDailInfoResult.set_dwsilverconsume(pGetDailInfo->dwSilverConsume);
	ProtoDailInfoResult.set_dwgoldconsume(pGetDailInfo->dwGoldConsume);
	ProtoDailInfoResult.set_dwjewelconsume(pGetDailInfo->dwJewelConsume);
	ProtoDailInfoResult.set_dwdrawscore(pGetDailInfo->dwDrawScore);
	ProtoDailInfoResult.set_dwaddscore(pGetDailInfo->dwAddScore);
	for (BYTE i = 0; i < 3; i++)
	{
		for (BYTE j = 0; j < 8; j++)
		{
			ProtoDailInfoResult.add_dwdailinfo(pGetDailInfo->dwDailInfo[i][j]);
		}
	}

	
	std::string serializeStr;
	ProtoDailInfoResult.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_DAIL_INFO_RESULT, (void*)serializeStr.c_str(), (WORD)serializeStr.size());


	return true;
}

// 获得抽奖信息
bool CAttemperEngineSink::OnDBMBRotateDailAward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;


	DBO_GP_RotateDailAwardInfo * pRotateDailAwardInfo = (DBO_GP_RotateDailAwardInfo *)pData;


	LogonServer::PROTO_RotateDailAwardInfo ProtoRotateDailAward;

	ProtoRotateDailAward.set_dwresultcode(pRotateDailAwardInfo->dwResultCode);
	ProtoRotateDailAward.set_szdescribestring(Utilss::ConvertFromUtf16ToUtf8(pRotateDailAwardInfo->szDescribeString));

	ProtoRotateDailAward.set_dwuserid(pRotateDailAwardInfo->dwUserID);
	ProtoRotateDailAward.set_dwdailtype(pRotateDailAwardInfo->dwDailType);

	ProtoRotateDailAward.set_dwdailaward(pRotateDailAwardInfo->dwDailAward);
	ProtoRotateDailAward.set_dwawardid(pRotateDailAwardInfo->dwAwardID);
	ProtoRotateDailAward.set_dwconsumesum(pRotateDailAwardInfo->dwConsumeSum);

	ProtoRotateDailAward.set_dwsilverconsume(pRotateDailAwardInfo->dwSilverConsume);
	ProtoRotateDailAward.set_dwgoldconsume(pRotateDailAwardInfo->dwGoldConsume);
	ProtoRotateDailAward.set_dwjewelconsume(pRotateDailAwardInfo->dwJewelConsume);
	ProtoRotateDailAward.set_dwdrawscore(pRotateDailAwardInfo->dwDrawScore);
	ProtoRotateDailAward.set_dwaddscore(pRotateDailAwardInfo->dwAddScore);
	for (BYTE i = 0; i < 3; i++)
	{
		for (BYTE j = 0; j < 8; j++)
		{
			ProtoRotateDailAward.add_dwdailinfo(pRotateDailAwardInfo->dwDailInfo[i][j]);
		}
	}


	std::string serializeStr;
	ProtoRotateDailAward.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_RPTATEDAILAWARD_RESULT, (void*)serializeStr.c_str(), (WORD)serializeStr.size());


	return true;
}

// 绑定银行卡信息
bool CAttemperEngineSink::OnDBMBBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	DBO_GP_BindBankInfo * pBindBankInfo = (DBO_GP_BindBankInfo *)pData;

	LogonServer::PROTO_BindBankInfo ProtoBindBankInfo;
	ProtoBindBankInfo.set_wsubcommandid(pBindBankInfo->wSubCommandID);
	ProtoBindBankInfo.set_dwresultcode(pBindBankInfo->dwResultCode);
	ProtoBindBankInfo.set_szdescribestring(Utilss::ConvertFromUtf16ToUtf8(pBindBankInfo->szDescribeString));

	ProtoBindBankInfo.set_szbankname(Utilss::ConvertFromUtf16ToUtf8(pBindBankInfo->szBankName));
	ProtoBindBankInfo.set_szbankcardnum(Utilss::ConvertFromUtf16ToUtf8(pBindBankInfo->szBankCardNum));
	ProtoBindBankInfo.set_szaccountholder(Utilss::ConvertFromUtf16ToUtf8(pBindBankInfo->szAccountHolder));

	std::string serializeStr;
	ProtoBindBankInfo.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_BANKCARDINFO, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}
// 绑定支付宝信息
bool CAttemperEngineSink::OnDBMBBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	DBO_GP_BindZFBInfo * pBindBankInfo = (DBO_GP_BindZFBInfo *)pData;

	LogonServer::PROTO_BindZFBInfo ProtoBindZFBInfo;
	ProtoBindZFBInfo.set_wsubcommandid(pBindBankInfo->wSubCommandID);
	ProtoBindZFBInfo.set_dwresultcode(pBindBankInfo->dwResultCode);
	ProtoBindZFBInfo.set_szdescribestring(Utilss::ConvertFromUtf16ToUtf8(pBindBankInfo->szDescribeString));

	ProtoBindZFBInfo.set_szzfbname(Utilss::ConvertFromUtf16ToUtf8(pBindBankInfo->szZFBName));
	ProtoBindZFBInfo.set_sznickname(Utilss::ConvertFromUtf16ToUtf8(pBindBankInfo->szNickName));

	std::string serializeStr;
	ProtoBindZFBInfo.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_ZFBBINDINFO, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

// 请求邮件
bool CAttemperEngineSink::OnDBMBRequestMail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	DBO_GP_RequestMailToal * pRequestMailToal = (DBO_GP_RequestMailToal *)pData;

	LogonServer::PROTO_RequestMailToal ProtoRequestMailToal;
	ProtoRequestMailToal.set_dwuserid(pRequestMailToal->dwUserID);
	for (int i = 0; i < pRequestMailToal->dwMailCount; i++)
	{
		ProtoRequestMailToal.add_strequestmail();
		LogonServer::ProtoRequestMail* ProtoRequestMail = ProtoRequestMailToal.mutable_strequestmail(i);
		ProtoRequestMail->set_dwmailindex(pRequestMailToal->stRequestMail[i].dwMailIndex);
		ProtoRequestMail->set_szmailcontent(Utilss::ConvertFromUtf16ToUtf8(pRequestMailToal->stRequestMail[i].szMailContent));
	}

	std::string serializeStr;
	ProtoRequestMailToal.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_REQUEST_MAIL_RESULT, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}
// 读取删除邮件
bool CAttemperEngineSink::OnDBMBReadDeleteMail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT_ERR(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	DBO_GP_ReadDeleteMailToal * pReadDeleteMailToal = (DBO_GP_ReadDeleteMailToal *)pData;

	LogonServer::PROTO_ReadDeleteMailToal ProtoRequestMailToal;
	ProtoRequestMailToal.set_dwuserid(pReadDeleteMailToal->dwUserID);
	ProtoRequestMailToal.set_dwopentype(pReadDeleteMailToal->dwOpenType);
	ProtoRequestMailToal.set_dwresultcode(pReadDeleteMailToal->dwResultCode);
	for (int i = 0; i < pReadDeleteMailToal->dwMailCount; i++)
	{
		ProtoRequestMailToal.add_strequestmail();
		LogonServer::ProtoRequestMail* ProtoRequestMail = ProtoRequestMailToal.mutable_strequestmail(i);
		ProtoRequestMail->set_dwmailindex(pReadDeleteMailToal->stRequestMail[i].dwMailIndex);
		ProtoRequestMail->set_szmailcontent(Utilss::ConvertFromUtf16ToUtf8(pReadDeleteMailToal->stRequestMail[i].szMailContent));
	}

	std::string serializeStr;
	ProtoRequestMailToal.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_READ_DELETE_MAIL_RESULT, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBMBLogonSuccess_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	// 判断在线
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID)
		return true;

	{


		// 登录成功数据
		auto pDBOLogonSuccess = (DBO_MB_LogonSuccess_BCode*)pData;
		auto wBindIndex = LOWORD(dwContextID);
		auto pBindParameter = GetBindParameter(wBindIndex);
		auto bMobileUser = (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE);

		// 数据填充
		tagUserInfo UserInfo = { 0 };
		tagUserInfoPlus UserInfoPlus = { 0 };

		//属性资料
		UserInfo.wFaceID = pDBOLogonSuccess->wFaceID;
		UserInfo.dwUserID = pDBOLogonSuccess->dwUserID;
		UserInfo.dwGameID = pDBOLogonSuccess->dwGameID;
		lstrcpyn(UserInfo.szNickName, pDBOLogonSuccess->szNickName, CountArray(UserInfo.szNickName));

		//用户资料
		UserInfo.cbGender = pDBOLogonSuccess->cbGender;
		UserInfo.cbMemberOrder = pDBOLogonSuccess->cbMemberOrder;
		UserInfo.cbMasterOrder = 0;
		lstrcpyn(UserInfo.szUnderWrite, pDBOLogonSuccess->szUnderWrite, CountArray(UserInfo.szUnderWrite));

		//状态设置
		UserInfo.cbUserStatus = US_FREE;
		UserInfo.wTableID = INVALID_TABLE;
		UserInfo.wChairID = INVALID_CHAIR;

		//积分信息
		UserInfo.lScore = pDBOLogonSuccess->lUserScore;
		UserInfo.lGrade = 0;
		UserInfo.lInsure = pDBOLogonSuccess->lUserInsure;
		UserInfo.dwWinCount = 0;
		UserInfo.dwLostCount = 0;
		UserInfo.dwDrawCount = 0;
		UserInfo.dwFleeCount = 0;
		UserInfo.dwUserMedal = pDBOLogonSuccess->dwUserMedal;
		UserInfo.dwExperience = pDBOLogonSuccess->dwExperience;
		UserInfo.lLoveLiness = 0;
		UserInfo.wAvatarID = 0;

		//登录信息
		UserInfoPlus.dwLogonTime = (DWORD)time(NULL);
		UserInfoPlus.dwInoutIndex = pDBOLogonSuccess->dwInoutIndex;

		//用户权限
		UserInfoPlus.dwUserRight = 0;
		UserInfoPlus.dwMasterRight = 0;

		//辅助变量
		UserInfoPlus.bMobileUser = bMobileUser;
		UserInfoPlus.bAndroidUser = false;
		UserInfoPlus.lRestrictScore = 0;

		//连接信息
		UserInfoPlus.wBindIndex = LOWORD(dwContextID);
		UserInfoPlus.dwClientAddr = pBindParameter->dwClientAddr;
		ZeroMemory(UserInfoPlus.szMachineID, CountArray(UserInfoPlus.szMachineID));

		// 切换用户
		auto pIServerUserItem = m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
		if (pIServerUserItem)
		{
			WORD wSourceIndex = pIServerUserItem->GetBindIndex();
			tagBindParameter * pSourceParameter = GetBindParameter(wSourceIndex);
			if ( pSourceParameter->dwSocketID != dwContextID ) //socketid不同切换连接
				SwitchUserItemConnect(pIServerUserItem,wBindIndex);
			//m_ServerUserManager.DeleteUserItem(pIServerUserItem);
		}

		//激活用户
		m_ServerUserManager.InsertUserItem(&pIServerUserItem, UserInfo, UserInfoPlus);

		//设置用户
		pBindParameter->pIServerUserItem = pIServerUserItem;

		//插入玩家任务
		//m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_INSERT_USERTASK, 0, &pDBOLogonSuccess->dwUserID, sizeof(DWORD));
		//插入玩家成就
		//m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_INSERT_USERACHIEVE, 0, &pDBOLogonSuccess->dwUserID, sizeof(DWORD));
		//插入玩家荣誉
		//m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_INSERT_USERFAME, 0, &pDBOLogonSuccess->dwUserID, sizeof(DWORD));


		m_pITCPNetworkEngine->AllowBatchSend(dwContextID,true,BG_MOBILE);
	}

	//发送登录成功数据
	OnDBEventDeal(dwContextID, pData, wDataSize, MDM_MB_LOGON_BCode, SUB_MB_LOGON_SUCCESS);

	//发送房间
	WORD wIndex = LOWORD(dwContextID);

	//发送游戏信息
	//SendMobileKindInfo(dwContextID, (m_pBindParameter + wIndex)->wModuleID);
	//SendMobileServerInfo(dwContextID,INVALID_WORD);
	//SendGameServerMBInfo(dwContextID,INVALID_WORD);

	//检查是否断线重连
	DBO_MB_LogonSuccess_BCode * pDBOLogonSuccess = (DBO_MB_LogonSuccess_BCode*)pData;

	
// 	if (pDBOLogonSuccess->dwServerID != 0 )  //是断线重连,获取房间信息
// 	{
// 
// 		TCHAR strOut[128];
// 		_sntprintf(strOut,sizeof(strOut),TEXT("断线重连ServerID:%d"),pDBOLogonSuccess->dwServerID);
// 		CTraceService::TraceString(strOut,TraceLevel_Info);
// 
// 		//枚举数据
// 		POSITION Position = NULL;
// 		CGameServerItem * pGameServerItem = NULL;
// 		DWORD dwRand = 0;
// 		DWORD dwRand_ = 0;
// 
// 		//枚举数据
// 		for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
// 		{
// 			//获取数据
// 			pGameServerItem = m_ServerListManager.EmunGameServerItem(Position);
// 			if (pGameServerItem == NULL) break;
// 
// 			tagGameServer pTagGemeServer = pGameServerItem->m_GameServer;
// 
// 			dwRand_ = rand() % 800 + 300;
// 			dwRand = rand() % 100 + dwRand_;
// 
// 			if(pTagGemeServer.wServerID == pDBOLogonSuccess->dwServerID)
// 			{
// 				tagGameServer_ GameServer;
// 				memset(GameServer.szServerName, 0, sizeof GameServer.szServerName);
// 
// 				GameServer.dwFullCount = pGameServerItem->m_GameServer.dwFullCount;
// 				GameServer.dwOnLineCount = pGameServerItem->m_GameServer.dwOnLineCount + dwRand;
// 				GameServer.lCellScore = pGameServerItem->m_GameServer.lCellScore;
// 				_tcscpy_s(GameServer.szServerAddr, pGameServerItem->m_GameServer.szServerAddr);
// 				_tcscpy_s(GameServer.szServerName, pGameServerItem->m_GameServer.szServerName);
// 				GameServer.wServerID = pGameServerItem->m_GameServer.wServerID;
// 				GameServer.wServerPort = pGameServerItem->m_GameServer.wServerPort;
// 				GameServer.wServerType = pGameServerItem->m_GameServer.wServerType;
// 				GameServer.cbRoomType = pGameServerItem->m_GameServer.cbRoomType;
// 
// 				//限制配置
// 				GameServer.lMinEnterScore	= pGameServerItem->m_GameServer.lMinEnterScore	;		//最低入场分
// 				GameServer.lMinTableScore	= pGameServerItem->m_GameServer.lMinTableScore	;		//最低坐下分
// 				GameServer.lMaxEnterScore	= pGameServerItem->m_GameServer.lMinEnterTiLi	;		//最高入场分
// 
// 				GameServer.wKindID = pGameServerItem->m_GameServer.wKindID;     //房间类型
// 
// 				m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON_BCode, SUB_MB_IS_LOST_CONNECTION_LOGIN,&GameServer,sizeof(tagGameServer_));
// 				
// 				break;
// 			}
// 		}
//	}
//	else
	{
		//登录成功
		LogonServer::PROTO_LogonFinish ProtoLogonFinish;
		ProtoLogonFinish.set_cblogonfinish(1);
		std::string serializeStr;
		ProtoLogonFinish.SerializeToString(&serializeStr);

		m_pITCPNetworkEngine->SendData(dwContextID, MDM_MB_LOGON_BCode, SUB_MB_LOGON_FINISH, (void*)serializeStr.c_str(), (WORD)serializeStr.size());

	}

	//I_TRACE(TEXT("OnDBMBLogonSuccess_BCode %s"),pDBOLogonSuccess->szAccounts);
	return true;
}

template<WORD wRequestID, typename StructType>
bool CAttemperEngineSink::PostDataBaseRequest(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize == sizeof(StructType));
	if (wDataSize != sizeof(StructType)) return false;

	//投递数据库事件
	m_pIDataBaseEngine->PostDataBaseRequest(wRequestID, dwContextID, pData, wDataSize);

	return true;
}

//客户端登录完成
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_ClientLoginSuccess(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CLIENT_LOGIN_SUCCESS,dwSocketID,pData,sizeof(DWORD));
	return true;
}

//客户端退出房间
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_ClientExitRoom(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//今天是否签到
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TODAY_IS_SIGNIN, dwSocketID, pData, sizeof(DWORD));

	return true;
}

//获取签到
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_GetUserSignIn(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	LogonServer::PROTO_GetUserSignIn ProtoGetUserSingIn;
	ProtoGetUserSingIn.ParseFromArray(pData, wDataSize);
	CMD_GP_GetUserSignIn GetUserSignIn;
	ZeroMemory(&GetUserSignIn, sizeof(GetUserSignIn));
	GetUserSignIn.dwUserID = ProtoGetUserSingIn.dwuserid();
	//获取签到
	return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_SIGNIN, dwSocketID, &GetUserSignIn, sizeof(GetUserSignIn));
}

//签到
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_SetUserSignIn(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	LogonServer::PROTO_SetUserSignIn ProtoSetUserSingIn;
	ProtoSetUserSingIn.ParseFromArray(pData, wDataSize);
	CMD_GP_SetUserSignIn SetUserSignIn;
	ZeroMemory(&SetUserSignIn, sizeof(SetUserSignIn));
	SetUserSignIn.dwUserID = ProtoSetUserSingIn.dwuserid();
	//return PostDataBaseRequest<DBR_GP_SET_SIGNIN,DWORD>(dwSocketID,pData,wDataSize);
	//签到
	return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SET_SIGNIN, dwSocketID, &SetUserSignIn, sizeof(SetUserSignIn));
}

//获取用户完善信息
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_GetUserPerfectInformation(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return PostDataBaseRequest<DBR_GP_GET_USER_PERFECT_INFORMATION,DWORD>(dwSocketID,pData,wDataSize);	
}

//设置用户完善信息
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_SetUserPerfectInformation(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	ASSERT_ERR(wDataSize == sizeof(CMD_GP_SetUserPerfectInformation));
	if (wDataSize != sizeof(CMD_GP_SetUserPerfectInformation)) return false;
	if (NULL == pData) return false;

	CMD_GP_SetUserPerfectInformation * pSetUserPerfectInformation = (CMD_GP_SetUserPerfectInformation*)pData;

	return PostDataBaseRequest<DBR_GP_SET_USER_PERFECT_INFORMATION, CMD_GP_SetUserPerfectInformation>(dwSocketID, pData, wDataSize);
}

//绑定用户推广
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_BindUserSpread(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return PostDataBaseRequest<DBR_GP_BIND_USER_SPREAD,CMD_GP_BindUserSpread>(dwSocketID,pData,wDataSize);	
}

//是否已经绑定推广
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_IsBindSpread(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return PostDataBaseRequest<DBR_GP_IS_BIND_SPREAD,DWORD>(dwSocketID,pData,wDataSize);	
}

//是否已经修改过昵称
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_IsAlreadyModifyNickName(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return PostDataBaseRequest<DBR_GP_IS_ALREADY_MODIFY_NICKNAME,DWORD>(dwSocketID,pData,wDataSize);
}

//是否是掉线重新登录
bool CAttemperEngineSink::OnDBIsLostConnectionLogin(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT_ERR(wDataSize==sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pServerID = (DWORD*)pData;
	//枚举数据
	POSITION Position = NULL;
	CGameServerItem * pGameServerItem = NULL;
	DWORD dwRand = 0;
	DWORD dwRand_ = 0;

	//枚举数据
	for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
	{
		//获取数据
		pGameServerItem = m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem == NULL) break;

		tagGameServer pTagGemeServer = pGameServerItem->m_GameServer;

		dwRand_ = rand() % 800 + 300;
		dwRand = rand() % 100 + dwRand_;

		if(pTagGemeServer.wServerID == (WORD)*pServerID)
		{
			tagGameServer_ GameServer;
			memset(GameServer.szServerName, 0, sizeof GameServer.szServerName);

			GameServer.dwFullCount = pGameServerItem->m_GameServer.dwFullCount;
			GameServer.dwOnLineCount = pGameServerItem->m_GameServer.dwOnLineCount + dwRand;
			GameServer.lCellScore = pGameServerItem->m_GameServer.lCellScore;
			_tcscpy_s(GameServer.szServerAddr, pGameServerItem->m_GameServer.szServerAddr);
			_tcscpy_s(GameServer.szServerName, pGameServerItem->m_GameServer.szServerName);
			GameServer.wServerID = pGameServerItem->m_GameServer.wServerID;
			GameServer.wServerPort = pGameServerItem->m_GameServer.wServerPort;
			GameServer.wServerType = pGameServerItem->m_GameServer.wServerType;
			GameServer.cbRoomType = pGameServerItem->m_GameServer.cbRoomType;

			//限制配置
			GameServer.lMinEnterScore	= pGameServerItem->m_GameServer.lMinEnterScore	;		//获得血量
			GameServer.lMinTableScore	= pGameServerItem->m_GameServer.lMinTableScore	;		//最低积分
			GameServer.lMaxEnterScore	= pGameServerItem->m_GameServer.lMaxEnterScore	;		//最低体力
			
			GameServer.wKindID = pGameServerItem->m_GameServer.wKindID;

			m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE, SUB_GP_IS_LOST_CONNECTION_LOGIN,&GameServer,sizeof(tagGameServer_));
		}
	}
	
	return true;
}

//游戏消息
bool CAttemperEngineSink::OnDBLoadScrollMseeage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT_ERR(wDataSize == sizeof(DBO_GR_ScrollMessage));
	if (wDataSize != sizeof(DBO_GR_ScrollMessage)) return false;
	if (NULL == pData) return false;


	DWORD dwCount = m_ServerUserManager.GetUserItemCount();
	for (DWORD i = 0;i < dwCount;i++)
	{
		IServerUserItem * pIUserItem=m_ServerUserManager.EnumUserItem(i++);

		if (pIUserItem==NULL) continue;
		if (pIUserItem->GetBindIndex() ==INVALID_WORD) continue;

		DWORD LogonTime = pIUserItem->GetLogonTime();

		DWORD dwCurTime = (DWORD)time(NULL);

		if (dwCurTime-LogonTime > 10)
		{
			WORD wBindIndex=pIUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

			if (pBindParameter)
				m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_GP_USER_SERVICE, SUB_GP_SCROLL_MESSAGE,pData,wDataSize);		
		}
	}
	
	return true;
}

//心跳处理
bool CAttemperEngineSink::OnTCPNetworkMainHeart(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//I_TRACE(TEXT("OnTCPNetworkMainHeart "));
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//if ( wSubCmdID == SUB_GR_HEART )
	{
		DWORD dwCurTime = (DWORD)time(NULL);
		//I_TRACE(TEXT("%s 接收心跳,%d"),pIServerUserItem->GetNickName(),dwCurTime);
		pIServerUserItem->SetHeartTime(dwCurTime);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
