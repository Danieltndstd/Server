#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"


//////////////////////////////////////////////////////////////////////////////////


namespace Utils1 {
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
// 获取分割字符串
CStringArray* DivString(CString strValue, CString strDiv)
{
	CStringArray* m_result = new CStringArray;
	while (TRUE)
	{
		int index = strValue.Find(strDiv);
		if (index == -1)
		{
			m_result->Add(strValue);
			return m_result;
		}
		CString temp = strValue.Left(index);
		m_result->Add(temp);
		strValue = strValue.Right(strValue.GetLength() - index - 1);
	}
}
bool CAttemperEngineSink::OnAttemperEngineBegin_BCode(IUnknownEx * pIUnknownEx)
{
	//数据库事件注册
	{
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_LOAD_SCROLL_MESSAGE, CAttemperEngineSink::OnDBLoadScrollMseeage, this);	// 读取滚动消息
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_GET_FIRST_REWARD_STATUS, CAttemperEngineSink::OnDBGetFirstRewardStatus, this);	// 获取一局奖励状态		
		REGISTER_FUNCTION_3(m_mDbFunctions, DBO_GR_SCROLL_MESSAGE, CAttemperEngineSink::OnDBLoadScrollMessageNew, this);	// 读取滚动消息
	}

	return true;
}
// 读取滚动消息
bool CAttemperEngineSink::OnDBLoadScrollMessageNew(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT_ERR(wDataSize == sizeof(DBO_GR_LoadScrollMessage));
	if (wDataSize != sizeof(DBO_GR_LoadScrollMessage)) return false;
	if (NULL == pData) return false;

	DBO_GR_LoadScrollMessage * pLoadScrollMessage = (DBO_GR_LoadScrollMessage*)pData;

	CString str;
	str.Format(TEXT("发送滚动消息 \n	%d-%d-%s"),
		pLoadScrollMessage->dwID, pLoadScrollMessage->dwMessageType, pLoadScrollMessage->szScrollMessage);
	//CTraceService::TraceString(str, TraceLevel_Exception);

	GameServer::SQL_S_Scroll_Message ScrollMessage;
	ScrollMessage.set_bmessagetype(pLoadScrollMessage->dwMessageType);
	//if (pLoadScrollMessage->dwMessageType == 0)
	{
		CStringArray* strArray = DivString(pLoadScrollMessage->szScrollMessage, TEXT("#"));
		
		for (int i = 0; i < strArray->GetSize(); i++)
		{
			if (strArray->GetAt(i)!=TEXT("") && strArray->GetAt(i) != TEXT("@") && strArray->GetAt(i) != TEXT("NULL"))
			{
				CString strMessage = strArray->GetAt(i);
				//CTraceService::TraceString(strMessage, TraceLevel_Exception);
				ScrollMessage.add_sscrollmsg(Utils1::ConvertFromUtf16ToUtf8(strMessage.GetBuffer()).c_str());
			}
		}
	}
	std::string serializeStr;
	ScrollMessage.SerializeToString(&serializeStr);

	//发送用户
	WORD wIndex = 0;
	do
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_ServerUserManager.EnumUserItem(wIndex++);
		if (pIServerUserItem == NULL) break;
		if (pIServerUserItem->IsAndroidUser())continue;;

		


		//发送数据
		SendData(pIServerUserItem, MDM_GR_LOGON, SUB_GF_REDIS_SCROLL_MES, (void*)serializeStr.c_str(), (WORD)serializeStr.size());
	} while (true);

	return true;
}
//游戏消息
bool CAttemperEngineSink::OnDBLoadScrollMseeage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	return true;
	ASSERT_ERR(wDataSize == sizeof(DBO_GR_ScrollMessage));
	if (wDataSize != sizeof(DBO_GR_ScrollMessage)) return false;
	if (NULL == pData) return false;

	//发送数据
	m_AndroidUserManager.SendDataToClient(MDM_GF_FRAME,SUB_GF_SCROLL_MESSAGE,pData,wDataSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SCROLL_MESSAGE,pData,wDataSize,BG_COMPUTER);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SCROLL_MESSAGE,pData,wDataSize,BG_MOBILE);

	return true;
}

//获取一局奖励状态
bool CAttemperEngineSink::OnDBGetFirstRewardStatus(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	map<DWORD,DBO_GR_TipMessage> & mTipMessage = CDataCenter::Instance()->m_mTipMessage;

	for (auto it = mTipMessage.begin(); it != mTipMessage.end() ; it++)
	{
		DWORD id = it->first;
		DBO_GR_TipMessage tTipMessage = it->second;

		if (id == 0)
		{
			//组装数据
			CMD_GR_TipMessage pTipMessage = {0};

			lstrcpyn(pTipMessage.szTipMessage,tTipMessage.szSystemMessage,CountArray(tTipMessage.szSystemMessage));

			SendData(dwContextID,MDM_GF_FRAME,SUB_GP_FINISHED_FIRST_GAME,&pTipMessage,sizeof(CMD_GR_TipMessage));

			break;
		}
	}

	return true;
}

//SYSTEMTIME转time_t
time_t CAttemperEngineSink::SystemTimeToTime_t( const SYSTEMTIME& st )
{
	tm temptm = {st.wSecond, 
		st.wMinute, 
		st.wHour, 
		st.wDay, 
		st.wMonth - 1, 
		st.wYear - 1900, 
		st.wDayOfWeek, 
		0, 
		0};

	return mktime(&temptm);
}

//////////////////////////////////////////////////////////////////////////////////
