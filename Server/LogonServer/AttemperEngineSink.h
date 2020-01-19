#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "DataBasePacket.h"
#include "SensitiveWordsFilter.h"
#include "../redisdb/RedisClient.h"
#include <map>
#include <vector>
#include <functional>



using namespace std;

//////////////////////////////////////////////////////////////////////////////////
//Ⱥ������
#define BG_MOBILE					(BYTE)(0x01)						//�ֻ�Ⱥ��
#define BG_COMPUTER					(BYTE)(0x02)						//����Ⱥ��
#define BG_ALL_CLIENT				(BYTE)(0xFF)						//ȫ��Ⱥ��

//���ݶ���

//��������
#define CLIENT_KIND_FALSH			1									//��ҳ����
#define CLIENT_KIND_MOBILE			2									//�ֻ�����
#define CLIENT_KIND_COMPUTER		3									//��������
#define CLIENT_KIND_WEBSOCKET		4									//WS����

//�󶨲���
struct tagBindParameter
{
	//�������
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��

	//������Ϣ
	WORD							wModuleID;							//ģ���ʶ
	BYTE							cbClientKind;						//��������
	DWORD							dwPlazaVersion;						//�����汾

	IServerUserItem *				pIServerUserItem;					//�û��ӿ�
};

//////////////////////////////////////////////////////////////////////////////////




//���ȹ���
class CAttemperEngineSink : public IAttemperEngineSink,public IServerUserItemSink
{
	//��Ԫ����
	friend class CServiceUnits;
	friend class CDataBaseEngineSink;

	//״̬����
protected:
	bool							m_bNeekCorrespond;					//Э����־
	bool                            m_bShowServerStatus;                //��ʾ������״̬	


	//��������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagBindParameter *				m_pBindParameter;					//��������

	//�������
protected:
	CServerListManager				m_ServerListManager;				//�б����

	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	ITCPSocketService *				m_pITCPSocketService;				//Э������

protected:
	CServerUserManager				m_ServerUserManager;				//�û�����

protected:
	map<WORD, function<bool(WORD, VOID*, WORD, DWORD)>> m_mMainFunctions;	//�������
	map<WORD, function<bool(DWORD, VOID*, WORD)>>		m_mDbFunctions;		//���ݿ�ص����������ֺ���������ǰ����������


	CSensitiveWordsFilter			m_WordsFilter;						//���ֹ���

protected:
	//KRedisConnParam					m_ConnParam;
	//CRedisClient					m_RedisClient;						//Redisʵ��

	CFileLog					m_fileLog;

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool OnAttemperEngineBegin(IUnknownEx * pIUnknownEx);
	//�����¼�
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//�Զ��¼�
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���Ӵ���
protected:
	//ע���¼�
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//Զ�̷���
	bool OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//��¼����
	bool OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������
	bool OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Զ�̴���
	bool OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//��¼����
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������
	bool OnTCPNetworkMainMBUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������Ϣ
	bool OnTCPNetworkMainMBMatchInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//���ͷ���
	void SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID);
	//��������
	VOID SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID);
	//���ͱ���
	void SendMobileMatchInfo(DWORD dwSocketID, WORD wModuleID);
	//�û�����
	bool OnTCPNetworkSubUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//���дʹ���
	virtual void SensitiveWordFilter(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen);

	//���ݿⷴ��
protected:
	//��¼�ɹ�
	bool OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼�ɹ�
	bool OnDBWebLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��ѯ����
	bool OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���гɹ�
	bool OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�������
	bool OnDBPCOperateResult(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�ڵ�
	bool OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�б�
	bool OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ղ��б�
	bool OnDBPCGameCollection(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼�ɹ�
	bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ע��
	bool OnDBThirdPartyRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������¼�ɹ�
	bool OnDBThirdPartySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�㲥��Ϣ
	bool OnDBBroadcastMsg(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnDBBroadcastTyphon(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ǩ��
	bool OnDBTodayIsSignin(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//���ݿ���Ϣ�����ƺš����ѡ��û�ͷ���û���Ϣ��������Ϣ��
	bool OnDBEventDeal(DWORD dwContextID, VOID* pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID);
	//���ݿ�֪ͨЭ����
	bool OnDBCorrespondEventDeal(DWORD dwContextID, VOID* pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID);


	//czgˢ��ͨ������
	bool OnDBUpdateAttribute(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������¼
	bool OnTCPNetworkSubPCLogonThirdparty(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//91��¼
	bool OnTCPNetworkSubPCLogon91Sdk(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��(�ֻ�)
	bool OnTCPNetworkSubPCRegisterAccountsMB(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������ע��
	bool OnTCPNetworkSubPCRegisterAccountsThird(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ȡ��֤��
	bool OnTCPNetworkSubPCGetValidateCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//QQ��΢��������¼
	bool OnTCPNetworkSubPCLogonThird(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Web ��¼
	bool OnTcpNetworkSumWebRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ο͵�¼
	bool OnTCPNetworkSubMBVisitorAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ֻ�ע��(��֤��)
	bool OnTCPNetworkSubMBRegisterMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID);

/************************************************************************/
/*                         B���¼�                                      */
/************************************************************************/
	//�ֻ��¼�
protected:
	bool OnAttemperEngineBegin_BCode(IUnknownEx * pIUnknownEx);
	//��¼����
	bool OnTCPNetworkMainMBLogon_BCode(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//I D ��¼
	bool OnTCPNetworkSubMBLogonGameID_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubMBLogonAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubMBRegisterAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ο͵�¼
	bool OnTCPNetworkSubMBVisitorAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ֻ�ע��(��֤��)
	bool OnTCPNetworkSubMBRegisterMobile_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��������
	bool OnTCPNetworkMainHeart(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���ݿⷴ��
protected:
	// ��¼�ɹ�
	bool OnDBMBLogonSuccess_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ʧ��֪ͨ
	bool OnDBMBLogonFailure_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ֪ͨ�ͻ���ʧ����Ϣ
	bool OnDBMBFailure_BCode(DWORD dwSocketID, LPCTSTR strErrorString);
	// ��ȡǩ������
	bool OnDBMBGetUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ����ǩ������
	bool OnDBMBSetUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��ȡת��
	bool OnDBMBGetDailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��ó齱��Ϣ
	bool OnDBMBRotateDailAward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// �����п���Ϣ
	bool OnDBMBBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��֧������Ϣ
	bool OnDBMBBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// �����ʼ�
	bool OnDBMBRequestMail(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��ȡɾ���ʼ�
	bool OnDBMBReadDeleteMail(DWORD dwContextID, VOID * pData, WORD wDataSize);

protected:
	//�ͻ��˵�¼���
	bool OnTCPNetworkMainPCUserService_ClientLoginSuccess(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ͻ����˳�����
	bool OnTCPNetworkMainPCUserService_ClientExitRoom(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ȡǩ��
	bool OnTCPNetworkMainPCUserService_GetUserSignIn(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//ǩ��
	bool OnTCPNetworkMainPCUserService_SetUserSignIn(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ȡ�û�������Ϣ
	bool OnTCPNetworkMainPCUserService_GetUserPerfectInformation(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�����û�������Ϣ
	bool OnTCPNetworkMainPCUserService_SetUserPerfectInformation(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//���û��ƹ�
	bool OnTCPNetworkMainPCUserService_BindUserSpread(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�Ƿ��Ѿ����ƹ�
	bool OnTCPNetworkMainPCUserService_IsBindSpread(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�Ƿ��Ѿ��޸Ĺ��ǳ�
	bool OnTCPNetworkMainPCUserService_IsAlreadyModifyNickName(VOID * pData, WORD wDataSize, DWORD dwSocketID);
protected:
	//�Ƿ��ǵ������µ�¼
	bool OnDBIsLostConnectionLogin(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ������Ϣ
	bool OnDBLoadScrollMseeage(DWORD dwContextID, VOID * pData, WORD wDataSize);
/********************************End****************************************/


	//��Ϸ�¼�
protected:
	//�����֪ͨ���ݿ�
	template<WORD, typename StructType>
	bool PostDataBaseRequest(DWORD dwContextID, VOID* pData, WORD wDataSize);

	//�ֻ��¼�
protected:

	//���ݿ��¼��ɹ�

	//��������
protected:
	//�汾���
	bool CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer=true);
	//��ȡ�豸�汾
	void GetDeviceVersion(BYTE cbDeviceType, WORD wModuleID, BYTE* cbArray);
	//�ֻ������汾���
	bool CheckMBPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, WORD wModuleID, DWORD dwSocketID);
	//��������
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�л�����
	bool SwitchUserItemConnect(IServerUserItem * pIServerUserItem, WORD wTargetIndex);

	//�����б�
protected:
	//��������
	VOID SendGameTypeInfo(DWORD dwSocketID);
	//��������
	VOID SendGameKindInfo(DWORD dwSocketID);
	//���ͽڵ�
	VOID SendGameNodeInfo(DWORD dwSocketID, WORD wKindID);
	//���Ͷ���
	VOID SendGamePageInfo(DWORD dwSocketID, WORD wKindID);
	//���ͷ���
	VOID SendGameServerInfo(DWORD dwSocketID, WORD wKindID);
	//���ͷ���
	VOID SendGameServerMBInfo(DWORD dwSocketID, WORD wKindID);
	//�����ղ�
	VOID SendCollectionInfo(DWORD dwSocketID, CMD_GP_Collection collection);

	//�û��ӿ�
public:
	//�û�����
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason){return true;}
	//�û�״̬
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR, BYTE cbNotifyClient = TRUE);
	//�û�Ȩ��
	virtual bool OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight=true){return true;}
	//�û�����
	virtual bool OnEventUserItemTask(IServerUserItem *pIServerUserItem,WORD wTask,WORD wCount,BYTE IsSubmit=false){return true;}

	//����ӿ�
public:
	//��������
	bool SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//��������
public:
	//���û�
	IServerUserItem * GetBindUserItem(DWORD dwSocketID);
	//�󶨲���
	tagBindParameter * GetBindParameter(WORD wBindIndex);

protected:
	map<std::wstring,std::wstring>	m_ValidateCodeMap;//��֤��
};

//////////////////////////////////////////////////////////////////////////////////

#endif