#pragma once
#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"
#include <map>
#include <functional>

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink, public IGameDataBaseEngine
{
	//��Ԫ����
	friend class CServiceUnits;

	//��������
protected:
	DBO_GR_LogonFailure				m_LogonFailure;						//��¼ʧ��
	DBO_GR_LogonSuccess				m_LogonSuccess;						//��¼�ɹ�

	//�û����ݿ�
protected:
	CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
	CDataBaseHelper					m_AccountsDBModule;					//�û����ݿ�

	//��Ϸ���ݿ�
protected:
	CDataBaseAide					m_GameDBAide;						//��Ϸ����
	CDataBaseHelper					m_GameDBModule;						//��Ϸ����

	//Ԫ�����ݿ�
protected:
	CDataBaseAide					m_TreasureDBAide;					//Ԫ�����ݿ�
	CDataBaseHelper					m_TreasureDBModule;					//Ԫ�����ݿ�

	//ƽ̨���ݿ�
protected:
	CDataBaseAide					m_PlatformDBAide;					//ƽ̨���ݿ�
	CDataBaseHelper					m_PlatformDBModule;					//ƽ̨���ݿ�

	//�������ݿ�
protected:
	CDataBaseAide					m_GameWebDBAide;
	CDataBaseHelper					m_GameWebModule;

	//���ñ���
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagGameParameter *				m_pGameParameter;					//���ò���
	tagDataBaseParameter *			m_pDataBaseParameter;				//������Ϣ
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//�������
protected:
	IDataBaseEngine *				m_pIDataBaseEngine;					//����ӿ�
	IGameServiceManager *			m_pIGameServiceManager;				//�������
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//��ѯ�ӿ�
protected:
	IGameDataBaseEngineSink *		m_pIGameDataBaseEngineSink;			//���ݽӿ�

	//�������
public:
	IDBCorrespondManager *          m_pIDBCorrespondManager;            //����Э��

	//������
protected:
	std::map<WORD, std::function<bool(DWORD, VOID*, WORD, DWORD&)>> m_mFunctions;

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

	//���ò���
public:
	//�Զ�����
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//��������
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//��������
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }

	//��ȡ����
public:
	//��ȡ����
	virtual VOID * GetDataBase(REFGUID Guid, DWORD dwQueryVer);
	//��ȡ����
	virtual VOID * GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//Ͷ�ݽ��
	virtual bool PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);
	//ע���¼�
	void RegisterRequestEvent();

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
	bool OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//I D ��¼
	bool OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�ʺŵ�¼
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//Token��¼
	bool OnRequestLogonTokenID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//ϵͳ����
protected:
	//��Ϸд��
	bool OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�뿪����
	bool OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��Ϸ��¼
	bool OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//������Ϸ��¼
	bool OnRequestGameFishRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	// ��Ϸ������Ϣ
	bool OnRequestGameScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���ز���
	bool OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�����б�
	bool OnRequestLoadGameColumn(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���ػ���
	bool OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���ص���
	bool OnRequestLoadGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��������
	bool OnRequestPropertyRequest(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�û�Ȩ��
	bool OnRequestManageUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//ϵͳ��Ϣ
	bool OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�������д�
	bool OnRequestLoadSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�������
	bool OnRequestKillLocker(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//д�����
	bool OnRequestWriteUserStatus(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//�����˴���
protected:
	//���������
	bool OnRequestAndriodApply(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//ˢ��ʱ���
	bool OnRequestAndriodRefresh(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�ͷŵ���������
	bool OnRequestAndriodRelease(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�ͷ�ȫ��������
	bool OnRequestAndriodALL(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�����Ƿ������
	bool OnRequestAndriodArray(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�ͷŲ�ɾ��������
	bool OnRequestAndriodReleaseAndDelete(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���з���
protected:
	//����Ԫ��
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ȡԪ��
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//ת��Ԫ��
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ѯ����
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ѯ�û�
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��������
private:
	//��¼���
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbDeviceType=DEVICE_TYPE_PC,WORD wBehaviorFlags=0,WORD wPageTableCount=0);
	//���н��
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbActivityGame=FALSE);
	//�����¼�
protected:
	//���Ը���
	bool OnUpdateAttribute(DWORD dwContextID, DBO_UPDATE_ATTRIBUTE::attribute attr,SCORE wScore );

protected:

/************************************************************************/
/*                         B���¼�                                      */
/************************************************************************/

public:
	//ע���¼�
	void RegisterRequestEvent_BCode();
	
	

protected:
	//��ȡ��ɵ�һ����Ϸ����
	bool OnLoadFinishedFirstGameRreward(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ȡ������Ϣ
	bool OnLoadTipMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ȡ������Ϣ
	bool OnLoadScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���������Ϣ
	bool OnClearScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ȡһ�ֽ���״̬
	bool OnGetFirstRewardStatus(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	// ������Ϣ��ȡ
	bool OnLoadScrollMessageNew(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	
/********************************End****************************************/
};



#define PROCESS_BEGIN() try{
#define PROCESS_END()	} \
	catch (IDataBaseException* pIException) \
{ \
	TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception); \
	OnLogonDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"), false); \
	return false; \
} \
	return true;