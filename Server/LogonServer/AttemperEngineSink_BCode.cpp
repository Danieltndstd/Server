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

	//���ֽ�תխ�ֽ�
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

	//խ�ֽ�ת���ֽ�
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

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineBegin_BCode(IUnknownEx * pIUnknownEx)
{

	//�¼�ע��
	{
		// ������ע��
		{
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_MB_LOGON_BCode, CAttemperEngineSink::OnTCPNetworkMainMBLogon_BCode, this);	// ��¼����
			REGISTER_FUNCTION_4(m_mMainFunctions, MDM_GR_HEART, CAttemperEngineSink::OnTCPNetworkMainHeart, this);	// ��������
		}

		// ���ݿ��¼�ע��
		{
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_MB_LOGON_SUCCESS_BCode, CAttemperEngineSink::OnDBMBLogonSuccess_BCode, this);	// ��¼�ɹ�	
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_MB_LOGON_FAILURE_BCode, CAttemperEngineSink::OnDBMBLogonFailure_BCode, this);	// ��¼ʧ��

			//ǩ��
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_TODAY_IS_SIGNIN,CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_TODAY_IS_SIGNIN); //�����Ƿ�ǩ��
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_SIGNIN,CAttemperEngineSink::OnDBMBGetUserSignIn, this); //��ȡǩ��
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_SET_SIGNIN,CAttemperEngineSink::OnDBMBSetUserSignIn, this); //ǩ��

			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_DAIL_INFO, CAttemperEngineSink::OnDBMBGetDailInfo, this); //��ȡת��
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_ROTATEDAILAWARD, CAttemperEngineSink::OnDBMBRotateDailAward, this); //��ó齱��Ϣ
			
			
			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_CHICKBINDBANK, CAttemperEngineSink::OnDBMBBindBank, this); //���п���Ϣ
			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_CHICKBINDZFB, CAttemperEngineSink::OnDBMBBindZFB, this); //֧������Ϣ

			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_REQUESTMAIL, CAttemperEngineSink::OnDBMBRequestMail, this); //�����ʼ�
			REGISTER_FUNCTION_3(m_mDbFunctions, DBR_GP_READDELEETETAIL, CAttemperEngineSink::OnDBMBReadDeleteMail, this); //��ȡɾ���ʼ�

			

			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_GET_USER_PERFECT_INFORMATION, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_GET_USER_PERFECT_INFORMATION);	//��ȡ�û�������Ϣ
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_SPREAD_REGISTER, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_IS_SPREAD_REGISTER);	//�Ƿ���ע���¼
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_BIND_SPREAD, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_IS_BIND_SPREAD);	//�Ƿ��Ѿ����ƹ�
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_ALREADY_MODIFY_NICKNAME, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_IS_ALREADY_MODIFY_NICKNAME);	//�Ƿ��Ѿ��޸Ĺ��ǳ�	
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_BIND_USER_SPREAD, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_BIND_USER_SPREAD);	//���û��ƹ���
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_TODAY_FIRST_TIME_LOGIN, CAttemperEngineSink::OnDBEventDeal, this, MDM_GP_USER_SERVICE, SUB_GP_IS_TODAY_FIRST_TIME_LOGIN);	//�Ƿ��ǽ����һ�ε�¼
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GP_IS_LOST_CONNECTION_LOGIN, CAttemperEngineSink::OnDBIsLostConnectionLogin, this);	//�Ƿ��ǵ������µ�¼
			REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOAD_SCROLL_MESSAGE, CAttemperEngineSink::OnDBLoadScrollMseeage, this);	// ��ȡ������Ϣ
			
			
		}
	}



	return true;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon_BCode(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubMBLogonAccounts_BCode(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubMBRegisterAccounts_BCode(pData,wDataSize,dwSocketID);
		}                                          
	case SUB_MB_VISITOR_ACCOUNTS: //�ο����
		{
			return OnTCPNetworkSubMBVisitorAccounts_BCode(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_REGISTER_MOBILE: //�ֻ�ע��
		{
			return OnTCPNetworkSubMBRegisterMobile_BCode(pData, wDataSize, dwSocketID);
		}
	}

	return false;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	//ASSERT_ERR(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	// (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false; 

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	//CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	//pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	//pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	//pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	//pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind= CLIENT_KIND_WEBSOCKET;
	//pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	//if (CheckMBPlazaVersion(pLogonAccounts->cbDeviceType, pLogonAccounts->dwPlazaVersion, pLogonAccounts->wModuleID, dwSocketID) == false) return true;

	LogonServer::PROTO_LogonAccountsBCode ProtoLogonAccountsBCode;
	ProtoLogonAccountsBCode.ParseFromArray(pData, wDataSize);


	//��������
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts, Utilss::ConvertA2W(ProtoLogonAccountsBCode.szaccounts()).c_str(),CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword, Utilss::ConvertA2W(ProtoLogonAccountsBCode.szpassword()).c_str(),CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID, Utilss::ConvertA2W(ProtoLogonAccountsBCode.szmachineid()).c_str(),CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone, Utilss::ConvertA2W(ProtoLogonAccountsBCode.szmobilephone()).c_str(),CountArray(LogonAccounts.szMobilePhone));
	LogonAccounts.cbDeviceType= ProtoLogonAccountsBCode.cbdevicetype();
	LogonAccounts.dwInviteID = ProtoLogonAccountsBCode.dwinviteid();
	LogonAccounts.dwSortID= ProtoLogonAccountsBCode.dwsortid();
	LogonAccounts.dwLogonType = ProtoLogonAccountsBCode.dwlogontype();
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS_BCode,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBLogonAccounts_BCode %s"),pLogonAccounts->szAccounts);
	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT_ERR(wDataSize>=sizeof(CMD_MB_RegisterAccounts_BCode));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts_BCode)) return false;

	//��������
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
// 			GetPrivateProfileString(TEXT("Parameter"),TEXT("Desc"),TEXT("ƽ̨����ת������ϵ�ͷ���ȡ��ƽ̨��ַ"),szDescribeString,CountArray(szDescribeString),szFullPath);
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

	//������Ϣ
// 	CMD_MB_RegisterAccounts_BCode * pRegisterAccounts=(CMD_MB_RegisterAccounts_BCode *)pData;
// 	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
// 	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
// 	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
// 	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
// 	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
// 	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	LogonServer::PROTO_RegisterAccountsBCode ProtoRegisterAccountsBCode;
	ProtoRegisterAccountsBCode.ParseFromArray(pData, wDataSize);


	//��������
	pBindParameter->cbClientKind= CLIENT_KIND_WEBSOCKET;
	//��������

	DBR_MB_RegisterAccounts_BCode RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
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

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS_BCode,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBRegisterAccounts_BCode %s"),pRegisterAccounts->szAccounts);
	return true;
}

//�ο͵�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBVisitorAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	LogonServer::PROTO_LogonVisitor ProtoLogonVisitor;
	ProtoLogonVisitor.ParseFromArray(pData, wDataSize);


	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);
	//��������
	pBindParameter->cbClientKind = CLIENT_KIND_WEBSOCKET;
	pBindParameter->dwPlazaVersion = 0;


	//��������
	DBR_MB_VisitorAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szSpreader, Utilss::ConvertA2W(ProtoLogonVisitor.szspreader()).c_str(),CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szAccounts, Utilss::ConvertA2W(ProtoLogonVisitor.szmachineid()).c_str(), CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szMachineID, Utilss::ConvertA2W(ProtoLogonVisitor.szmachineid()).c_str(),CountArray(RegisterAccounts.szMachineID));
	RegisterAccounts.dwInviteID = ProtoLogonVisitor.dwinviteid();
	RegisterAccounts.dwSortID = ProtoLogonVisitor.dwsortid();

	RegisterAccounts.dwLogonType = ProtoLogonVisitor.dwlogontype();
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_VISITOR_ACCOUNTS_BCode,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	//I_TRACE(TEXT("OnTCPNetworkSubMBVisitorAccounts_BCode %s"),pRegisterAccounts->szMachineID);
	return true;
}

// �ֻ�ע���˺�
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterMobile_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	// Ч�����
	ASSERT_ERR(wDataSize == sizeof(CMD_MB_RegisterMobile));
	if (wDataSize != sizeof(CMD_MB_RegisterMobile))
		return false;

	// ��������
	DWORD wBindIndex = LOWORD(dwSocketID);
	tagBindParameter* pBindParameter = (m_pBindParameter + wBindIndex);

	// ������Ϣ
	CMD_MB_RegisterMobile* pRegisterMobile = (CMD_MB_RegisterMobile*)pData;
	pRegisterMobile->szMachineID[CountArray(pRegisterMobile->szMachineID) - 1] = 0;
	pRegisterMobile->szLogonPass[CountArray(pRegisterMobile->szLogonPass) - 1] = 0;
	pRegisterMobile->szMobilePhone[CountArray(pRegisterMobile->szMobilePhone) - 1] = 0;
	pRegisterMobile->szVerificationCode[CountArray(pRegisterMobile->szVerificationCode) - 1] = 0;

	// ��������
	pBindParameter->cbClientKind = CLIENT_KIND_MOBILE;

	// ��������
	DBR_MB_RegisterMobile RegisterMobile = { 0 };

	// ������Ϣ
	RegisterMobile.pBindParameter = (m_pBindParameter + LOWORD(dwSocketID));

	// ��������
	RegisterMobile.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterMobile.szMobile, pRegisterMobile->szMobilePhone, CountArray(RegisterMobile.szMobile));
	lstrcpyn(RegisterMobile.szMachineID, pRegisterMobile->szMachineID, CountArray(RegisterMobile.szMachineID));
	lstrcpyn(RegisterMobile.szLogonPass, pRegisterMobile->szLogonPass, CountArray(RegisterMobile.szLogonPass));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_MOBILE_BCode, dwSocketID, &RegisterMobile, sizeof(RegisterMobile));

	//I_TRACE(TEXT("OnTCPNetworkSubMBRegisterMobile_BCode %s"), pRegisterMobile->szMachineID);
	return true;
}

bool CAttemperEngineSink::OnDBMBLogonFailure_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT_ERR(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_OperateResult * pLogonFailure = (DBO_GP_OperateResult *)pData;

	//��������
	LogonFailure.lResultCode = pLogonFailure->dwResultCode;
	_tcscpy_s(LogonFailure.szDescribeString, pLogonFailure->szDescribeString);

	LogonServer::PROTO_OperateResult ProtoOperateResult;
	ProtoOperateResult.set_wsubcommandid(0);
	ProtoOperateResult.set_dwresultcode(LogonFailure.lResultCode);
	ProtoOperateResult.set_szdescribestring(Utilss::ConvertFromUtf16ToUtf8(LogonFailure.szDescribeString));
	std::string serializeStr;
	ProtoOperateResult.SerializeToString(&serializeStr);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_MB_LOGON_BCode, SUB_MB_LOGON_FAILURE, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	//��������
	//m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON_BCode,SUB_MB_LOGON_FAILURE,&LogonFailure,sizeof(CMD_MB_LogonFailure));

	//�ر�����
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//֪ͨ�ͻ���ʧ����Ϣ
bool CAttemperEngineSink::OnDBMBFailure_BCode(DWORD dwSocketID, LPCTSTR strErrorString)
{
	//��������
	CMD_GP_LogonFailure LogonFailure = { 0 };

	//��������
	lstrcpyn(LogonFailure.szDescribeString, strErrorString, CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize = CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize = sizeof(LogonFailure) - sizeof(LogonFailure.szDescribeString) + wStringSize;

	m_pITCPNetworkEngine->SendData(dwSocketID, MDM_MB_LOGON_BCode, SUB_GP_LOGON_FAILURE, &LogonFailure, wSendSize);

	return true;
}

//��ȡǩ������
bool CAttemperEngineSink::OnDBMBGetUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
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

// ����ǩ������
bool CAttemperEngineSink::OnDBMBSetUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
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

// ��ȡת��
bool CAttemperEngineSink::OnDBMBGetDailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
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

// ��ó齱��Ϣ
bool CAttemperEngineSink::OnDBMBRotateDailAward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
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

// �����п���Ϣ
bool CAttemperEngineSink::OnDBMBBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
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
// ��֧������Ϣ
bool CAttemperEngineSink::OnDBMBBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
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

// �����ʼ�
bool CAttemperEngineSink::OnDBMBRequestMail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
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
// ��ȡɾ���ʼ�
bool CAttemperEngineSink::OnDBMBReadDeleteMail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
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

//��¼�ɹ�
bool CAttemperEngineSink::OnDBMBLogonSuccess_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	// �ж�����
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID)
		return true;

	{


		// ��¼�ɹ�����
		auto pDBOLogonSuccess = (DBO_MB_LogonSuccess_BCode*)pData;
		auto wBindIndex = LOWORD(dwContextID);
		auto pBindParameter = GetBindParameter(wBindIndex);
		auto bMobileUser = (pBindParameter->cbClientKind == CLIENT_KIND_MOBILE);

		// �������
		tagUserInfo UserInfo = { 0 };
		tagUserInfoPlus UserInfoPlus = { 0 };

		//��������
		UserInfo.wFaceID = pDBOLogonSuccess->wFaceID;
		UserInfo.dwUserID = pDBOLogonSuccess->dwUserID;
		UserInfo.dwGameID = pDBOLogonSuccess->dwGameID;
		lstrcpyn(UserInfo.szNickName, pDBOLogonSuccess->szNickName, CountArray(UserInfo.szNickName));

		//�û�����
		UserInfo.cbGender = pDBOLogonSuccess->cbGender;
		UserInfo.cbMemberOrder = pDBOLogonSuccess->cbMemberOrder;
		UserInfo.cbMasterOrder = 0;
		lstrcpyn(UserInfo.szUnderWrite, pDBOLogonSuccess->szUnderWrite, CountArray(UserInfo.szUnderWrite));

		//״̬����
		UserInfo.cbUserStatus = US_FREE;
		UserInfo.wTableID = INVALID_TABLE;
		UserInfo.wChairID = INVALID_CHAIR;

		//������Ϣ
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

		//��¼��Ϣ
		UserInfoPlus.dwLogonTime = (DWORD)time(NULL);
		UserInfoPlus.dwInoutIndex = pDBOLogonSuccess->dwInoutIndex;

		//�û�Ȩ��
		UserInfoPlus.dwUserRight = 0;
		UserInfoPlus.dwMasterRight = 0;

		//��������
		UserInfoPlus.bMobileUser = bMobileUser;
		UserInfoPlus.bAndroidUser = false;
		UserInfoPlus.lRestrictScore = 0;

		//������Ϣ
		UserInfoPlus.wBindIndex = LOWORD(dwContextID);
		UserInfoPlus.dwClientAddr = pBindParameter->dwClientAddr;
		ZeroMemory(UserInfoPlus.szMachineID, CountArray(UserInfoPlus.szMachineID));

		// �л��û�
		auto pIServerUserItem = m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
		if (pIServerUserItem)
		{
			WORD wSourceIndex = pIServerUserItem->GetBindIndex();
			tagBindParameter * pSourceParameter = GetBindParameter(wSourceIndex);
			if ( pSourceParameter->dwSocketID != dwContextID ) //socketid��ͬ�л�����
				SwitchUserItemConnect(pIServerUserItem,wBindIndex);
			//m_ServerUserManager.DeleteUserItem(pIServerUserItem);
		}

		//�����û�
		m_ServerUserManager.InsertUserItem(&pIServerUserItem, UserInfo, UserInfoPlus);

		//�����û�
		pBindParameter->pIServerUserItem = pIServerUserItem;

		//�����������
		//m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_INSERT_USERTASK, 0, &pDBOLogonSuccess->dwUserID, sizeof(DWORD));
		//������ҳɾ�
		//m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_INSERT_USERACHIEVE, 0, &pDBOLogonSuccess->dwUserID, sizeof(DWORD));
		//�����������
		//m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_INSERT_USERFAME, 0, &pDBOLogonSuccess->dwUserID, sizeof(DWORD));


		m_pITCPNetworkEngine->AllowBatchSend(dwContextID,true,BG_MOBILE);
	}

	//���͵�¼�ɹ�����
	OnDBEventDeal(dwContextID, pData, wDataSize, MDM_MB_LOGON_BCode, SUB_MB_LOGON_SUCCESS);

	//���ͷ���
	WORD wIndex = LOWORD(dwContextID);

	//������Ϸ��Ϣ
	//SendMobileKindInfo(dwContextID, (m_pBindParameter + wIndex)->wModuleID);
	//SendMobileServerInfo(dwContextID,INVALID_WORD);
	//SendGameServerMBInfo(dwContextID,INVALID_WORD);

	//����Ƿ��������
	DBO_MB_LogonSuccess_BCode * pDBOLogonSuccess = (DBO_MB_LogonSuccess_BCode*)pData;

	
// 	if (pDBOLogonSuccess->dwServerID != 0 )  //�Ƕ�������,��ȡ������Ϣ
// 	{
// 
// 		TCHAR strOut[128];
// 		_sntprintf(strOut,sizeof(strOut),TEXT("��������ServerID:%d"),pDBOLogonSuccess->dwServerID);
// 		CTraceService::TraceString(strOut,TraceLevel_Info);
// 
// 		//ö������
// 		POSITION Position = NULL;
// 		CGameServerItem * pGameServerItem = NULL;
// 		DWORD dwRand = 0;
// 		DWORD dwRand_ = 0;
// 
// 		//ö������
// 		for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
// 		{
// 			//��ȡ����
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
// 				//��������
// 				GameServer.lMinEnterScore	= pGameServerItem->m_GameServer.lMinEnterScore	;		//����볡��
// 				GameServer.lMinTableScore	= pGameServerItem->m_GameServer.lMinTableScore	;		//������·�
// 				GameServer.lMaxEnterScore	= pGameServerItem->m_GameServer.lMinEnterTiLi	;		//����볡��
// 
// 				GameServer.wKindID = pGameServerItem->m_GameServer.wKindID;     //��������
// 
// 				m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON_BCode, SUB_MB_IS_LOST_CONNECTION_LOGIN,&GameServer,sizeof(tagGameServer_));
// 				
// 				break;
// 			}
// 		}
//	}
//	else
	{
		//��¼�ɹ�
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
	//Ч�����
	ASSERT(wDataSize == sizeof(StructType));
	if (wDataSize != sizeof(StructType)) return false;

	//Ͷ�����ݿ��¼�
	m_pIDataBaseEngine->PostDataBaseRequest(wRequestID, dwContextID, pData, wDataSize);

	return true;
}

//�ͻ��˵�¼���
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_ClientLoginSuccess(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CLIENT_LOGIN_SUCCESS,dwSocketID,pData,sizeof(DWORD));
	return true;
}

//�ͻ����˳�����
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_ClientExitRoom(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//�����Ƿ�ǩ��
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TODAY_IS_SIGNIN, dwSocketID, pData, sizeof(DWORD));

	return true;
}

//��ȡǩ��
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_GetUserSignIn(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	LogonServer::PROTO_GetUserSignIn ProtoGetUserSingIn;
	ProtoGetUserSingIn.ParseFromArray(pData, wDataSize);
	CMD_GP_GetUserSignIn GetUserSignIn;
	ZeroMemory(&GetUserSignIn, sizeof(GetUserSignIn));
	GetUserSignIn.dwUserID = ProtoGetUserSingIn.dwuserid();
	//��ȡǩ��
	return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_SIGNIN, dwSocketID, &GetUserSignIn, sizeof(GetUserSignIn));
}

//ǩ��
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_SetUserSignIn(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	LogonServer::PROTO_SetUserSignIn ProtoSetUserSingIn;
	ProtoSetUserSingIn.ParseFromArray(pData, wDataSize);
	CMD_GP_SetUserSignIn SetUserSignIn;
	ZeroMemory(&SetUserSignIn, sizeof(SetUserSignIn));
	SetUserSignIn.dwUserID = ProtoSetUserSingIn.dwuserid();
	//return PostDataBaseRequest<DBR_GP_SET_SIGNIN,DWORD>(dwSocketID,pData,wDataSize);
	//ǩ��
	return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SET_SIGNIN, dwSocketID, &SetUserSignIn, sizeof(SetUserSignIn));
}

//��ȡ�û�������Ϣ
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_GetUserPerfectInformation(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return PostDataBaseRequest<DBR_GP_GET_USER_PERFECT_INFORMATION,DWORD>(dwSocketID,pData,wDataSize);	
}

//�����û�������Ϣ
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_SetUserPerfectInformation(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	ASSERT_ERR(wDataSize == sizeof(CMD_GP_SetUserPerfectInformation));
	if (wDataSize != sizeof(CMD_GP_SetUserPerfectInformation)) return false;
	if (NULL == pData) return false;

	CMD_GP_SetUserPerfectInformation * pSetUserPerfectInformation = (CMD_GP_SetUserPerfectInformation*)pData;

	return PostDataBaseRequest<DBR_GP_SET_USER_PERFECT_INFORMATION, CMD_GP_SetUserPerfectInformation>(dwSocketID, pData, wDataSize);
}

//���û��ƹ�
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_BindUserSpread(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return PostDataBaseRequest<DBR_GP_BIND_USER_SPREAD,CMD_GP_BindUserSpread>(dwSocketID,pData,wDataSize);	
}

//�Ƿ��Ѿ����ƹ�
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_IsBindSpread(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return PostDataBaseRequest<DBR_GP_IS_BIND_SPREAD,DWORD>(dwSocketID,pData,wDataSize);	
}

//�Ƿ��Ѿ��޸Ĺ��ǳ�
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService_IsAlreadyModifyNickName(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return PostDataBaseRequest<DBR_GP_IS_ALREADY_MODIFY_NICKNAME,DWORD>(dwSocketID,pData,wDataSize);
}

//�Ƿ��ǵ������µ�¼
bool CAttemperEngineSink::OnDBIsLostConnectionLogin(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT_ERR(wDataSize==sizeof(DWORD));
	if(wDataSize != sizeof(DWORD)) return false;
	if(NULL == pData) return false;

	DWORD * pServerID = (DWORD*)pData;
	//ö������
	POSITION Position = NULL;
	CGameServerItem * pGameServerItem = NULL;
	DWORD dwRand = 0;
	DWORD dwRand_ = 0;

	//ö������
	for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
	{
		//��ȡ����
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

			//��������
			GameServer.lMinEnterScore	= pGameServerItem->m_GameServer.lMinEnterScore	;		//���Ѫ��
			GameServer.lMinTableScore	= pGameServerItem->m_GameServer.lMinTableScore	;		//��ͻ���
			GameServer.lMaxEnterScore	= pGameServerItem->m_GameServer.lMaxEnterScore	;		//�������
			
			GameServer.wKindID = pGameServerItem->m_GameServer.wKindID;

			m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE, SUB_GP_IS_LOST_CONNECTION_LOGIN,&GameServer,sizeof(tagGameServer_));
		}
	}
	
	return true;
}

//��Ϸ��Ϣ
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

//��������
bool CAttemperEngineSink::OnTCPNetworkMainHeart(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//I_TRACE(TEXT("OnTCPNetworkMainHeart "));
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT_ERR(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//if ( wSubCmdID == SUB_GR_HEART )
	{
		DWORD dwCurTime = (DWORD)time(NULL);
		//I_TRACE(TEXT("%s ��������,%d"),pIServerUserItem->GetNickName(),dwCurTime);
		pIServerUserItem->SetHeartTime(dwCurTime);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
