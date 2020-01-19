#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"
#include <map>
#include <functional>

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//�û����ݿ�
protected:
	CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
	CDataBaseHelper					m_AccountsDBModule;					//�û����ݿ�

	//Ԫ�����ݿ�
protected:
	CDataBaseAide					m_TreasureDBAide;					//Ԫ�����ݿ�
	CDataBaseHelper					m_TreasureDBModule;					//Ԫ�����ݿ�

	//ƽ̨���ݿ�
protected:
	CDataBaseAide					m_PlatformDBAide;					//ƽ̨���ݿ�
	CDataBaseHelper					m_PlatformDBModule;					//ƽ̨���ݿ�

	//�������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//������
protected:
	std::map<WORD, std::function<bool(DWORD, VOID*, WORD)>>	m_mFunctions;	// �¼�����

	//����
protected:
	std::map<WORD, DWORD>	m_mLobbyExp;	// Lobby����
	std::map<WORD, DWORD>	m_mLandExp;		// Land����
	std::map<WORD, DWORD>	m_mDLandExp;	// DLand����
	std::map<WORD, DWORD>	m_mHLandExp;	// HLand����

protected:
	

	//��������
public:
	//���캯��
	CDataBaseEngineSink();
	//��������
	virtual ~CDataBaseEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineBegin(IUnknownEx * pIUnknownEx);
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����¼�
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��¼����
protected:
	//I D ��¼
	bool OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺŵ�¼
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺ�ע��
	bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// Webע��
	bool OnRequestWebRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������¼
	bool OnRequesLogonThirdParty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ע��
	bool OnRequestRegisterThirdParty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������¼
	bool OnRequestLogonThird(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺŵǳ�
	bool OnRequestLogonOut(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ֻ���¼
protected:
	//I D ��¼
	bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺŵ�¼
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺ�ע��
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ֻ�ע��
	bool OnMobileRegisterMobile(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ο͵�¼
	bool OnMobileVisitorAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�˺ŷ���
protected:
	//�޸Ļ���
	bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ǩ��
	bool OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸��ǳ�
	bool OnRequestModifyNickName(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//���ֻ�
	bool OnRequestBindMobilePhone(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//�һ�����
	bool OnRequestRecoveryPass(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//��ȡͷ��/����
	bool OnRequestGetAvatarFaceInfo(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//�޸�ͷ��
	bool OnRequestModifyAvatar(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ͷ��
	bool OnRequestModifyImage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ο�ת��
	bool OnRequestBindVisitorAccount(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���һ����루������֤�˺ţ���֤���빦�ܣ�
	bool OnRequestRetrievePassword(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////


	//ˢ�»���
	bool OnReloadScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
protected:
	//���Ը���
	bool OnUpdateAttribute(DWORD dwContextID, DBO_UPDATE_ATTRIBUTE::attribute attr,SCORE wScore );

	//����Ԫ��
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡԪ��
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ת��Ԫ��
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��Ϣ��ѯ
protected:
	//��ѯ����
	bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ����
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�û�
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//ϵͳ����
protected:
	//�����б�
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�ղ�
	bool OnRequestCollection(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�򿪷���
	bool OnRequestOpenRoomStatus(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�������
protected:
	// Web ��¼���
	VOID OnLogonDisposeWebResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, bool bVisitorClient = false);
	//��¼���
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bVisitorClient=false);
	//���н��
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	/*
	* @dwContextID : ����ID
	* @dwErrorCode : �������(0:�ɹ�)
	* @pszErrorString : ������Ϣ
	* @wSubCommandID : ��;
	*/
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, WORD wSubCommandID);
	//��������¼���
	VOID OnLogonThirdPartyResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, DBR_GP_Insert_Accounts &insertAccounts);


/************************************************************************/
/*                         B���¼�                                      */
/************************************************************************/
	//�ֻ���¼
protected:
	bool OnDataBaseEngineBegin_BCode(IUnknownEx * pIUnknownEx);
	//I D ��¼
	bool OnMobileLogonGameID_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺŵ�¼
	bool OnMobileLogonAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺ�ע��
	bool OnMobileRegisterAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ֻ�ע��
	bool OnMobileRegisterMobile_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ο͵�¼
	bool OnMobileVisitorAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�������
protected:
	//��¼���
	VOID OnLogonDisposeResult_BCode(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bVisitorClient=false);

protected:
	//��ȡǩ��
	bool OnRequestGetSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ǩ��
	bool OnRequestSetSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�����Ƿ��Ѿ�ǩ��
	bool OnRequestTotayIsSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���ת����Ϣ
	bool OnRequestGetDailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ת��ת�̻�ý���
	bool OnRequestRotateDailAward(DWORD dwContextID, VOID * pData, WORD wDataSize);

	// ��
	// ��ѯ�����п�
	bool OnRequestChickBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// �����п�
	bool OnRequestBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��ѯ��֧����
	bool OnRequestChickBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��֧����
	bool OnRequestBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize);

	// �ʼ�
	// �����ʼ�
	bool OnRequestRequestMail(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��ȡɾ���ʼ�
	bool OnRequestReadDeleteMail(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��ȡ�û�������Ϣ
	bool OnRequestGetUserPerFectInformation(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����û�������Ϣ
	bool OnRequestSetUserPerFectInformation(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�Ƿ����ƹ�ע��
	bool OnRequestIsSpreadRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//���û��ƹ�
	bool OnRequestBindUserSpread(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�Ƿ��Ѿ����ƹ�
	bool OnRequestIsBindSpread(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�Ƿ��Ѿ��޸Ĺ��ǳ�
	bool OnRequestIsAlreadyModifyNickName(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�Ƿ��ǽ����һ�ε�¼
	bool OnRequestIsTodayFirstTimeLogin(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�Ƿ��ǵ������µ�¼
	bool OnRequestIsLostConnectionLogin(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ͻ��˵�¼���
	bool OnRequestClientLoginSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	
	//�������
	bool OnRequestKillLocker(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//��ȡ������Ϣ
	bool OnLoadScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���������¼
	bool OnClearScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	

/********************************End****************************************/
};


//////////////////////////////////////////////////////////////////////////////////
// ���ݿ⴦���
#define PROCESS_BEGIN()	try{
#define PROCESS_END()	} \
	catch(IDataBaseException * pIException) \
{ \
	TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception); \
	OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), SUB_GP_OPERATE_FAILURE); \
	return false; \
} \
	return true;

//λ�����
#define VALUE_OF_BIT(data,num)	((data&(1<<(num-1)))?1:0)   //��ȡĳһλ��ֵ
#define SET_BIT_1(data,num)		data=(data|(1<<(num-1)))	//ָ��λ��1
#define SET_BIT_0(data,num)		data=(data&(1<<(num-1)))	//ָ��λ��0

#endif