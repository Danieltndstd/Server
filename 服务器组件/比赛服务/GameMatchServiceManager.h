#ifndef GAME_MATCH_SERVICE_MANAGER_HEAD_FILE
#define GAME_MATCH_SERVICE_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "MatchServiceHead.h"
#include "ChampionshipMatch.h"
#include "..\..\Server\GameServer\DataBasePacket.h"
//////////////////////////////////////////////////////////////////////////
//��ʱ������
struct tagMatchTimerItem
{
	DWORD								dwTimerID;						//��ʱ�� ID
	DWORD								dwRealID;						//ʵ��ID
	DWORD								dwRepeatTimes;					//�ظ�����
	WPARAM								wBindParam;						//�󶨲���
};

#define	MATCH_CLOSE	0
#define MATCH_OPEN	1

class CChampionshipMatch;

//��˵��
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;
typedef CWHArray<CChampionshipMatch*>CMatchGroupArray;
typedef CWHArray<tagMatchTimerItem *> CTimerItemPtr;

//��Ϸ������������
class MATCH_SERVICE_CLASS CGameMatchServiceManager : public IGameMatchServiceManager, public IChampionshipMatchSink
{
	//��������
protected:
	CMatchUserItemArray					m_OnMatchUserItem;				//�����û�
	CMatchUserItemArray					m_OutUserItem;					//��̭���û�
	const tagGameServiceOption *		m_pGameServiceOption;			//��������
	const tagGameServiceAttrib *		m_pGameServiceAttrib;			//��������

	//�ӿڱ���
protected:
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;		//����ӿ�
	IDataBaseEngine *					m_pIDataBaseEngine;				//��������
	ITimerEngine *						m_pITimerEngine;				//ʱ������
	IMainServiceFrame				*  m_pIGameServiceFrame;			//���ܽӿ�
	IServerUserManager *				m_pIServerUserManager;			//�û�����
	ITableFrame							** m_ppITableFrame;				//��ܽӿ�
	IAndroidUserManager					*m_pAndroidUserManager;			//��������
	bool								*m_bTableUsed;					//�����Ƿ��Ѿ�������

	CMatchGroupArray					m_MatchGroup;	
	CChampionshipMatch					*m_pCurMatch;
	CMatchGroupArray					m_OverGroup;					//�Ѿ��������������ӵķ���
	CWHArray<WORD,WORD>					m_UnRecovery;					//û�л��յ�����

	CTimerItemPtr						m_TimerItemFree;				//��������
	CTimerItemPtr						m_TimerItemActive;				//�����

	WCHAR								m_guidRoom[40];					//guidRoom

	CMD_GR_MatchGlobal					m_MatchGlobal;
	CMD_GR_MatchJoin					m_MatchJoin;
	CMD_GR_RewardArray					m_Reward[3];
	BYTE								m_cbMatchOpen;					//��ʱ����λ��ʶ
	BYTE								m_cbRealUserJoin;				//������Ҳ���
	bool								m_bMatchOver;					//�����������Ʊ���
	BYTE								m_WeekDay[8];
	CMD_GR_ExpReward					m_ExpReward;

public:
	CGameMatchServiceManager(void);
	virtual ~CGameMatchServiceManager(void);
public:
	virtual VOID  Release() { }
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);
public:
	virtual bool  StopService();
	virtual bool  StartService();
public:
	virtual bool  InitMatchInterface(ITCPNetworkEngineEvent *pTCPNetworkEngine,IDataBaseEngine *pIDataBaseEngine, IServerUserManager *pIServerUserManager,
		IMainServiceFrame *pIMainServiceFrame,ITimerEngine *pITimerEngine,IAndroidUserManager *pAndroidUserManager, WCHAR guidRoom[]);
	virtual bool  InitTableFrame(ITableFrame * pTableFrame,WORD wTableID);
public:
	virtual bool  OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
public:
	virtual bool  RectifyServiceOption(tagGameServiceOption * pGameServiceOption,tagGameServiceAttrib *pGameServiceAttrib);
public:
	//�û��μӱ���
	virtual bool  OnUserJoinGame(IServerUserItem * pIServerUserItem);
	//�û��˳�����
	virtual bool  OnUserQuitGame(IServerUserItem * pIServerUserItem, BYTE cbReason,WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD);
	//ʱ��������ǰ���
	virtual bool StartMatchCheck();
public:
	virtual VOID *  CreateGameMatchSink(REFGUID Guid, DWORD dwQueryVer);
	//����ӿ�
public:
	//��������
	virtual bool  SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//����ȫ��
	virtual void GetMatchGlobal(CMD_GR_MatchGlobal& MatchGlobal);

	//�����˹��ܺ���
public:
	bool PostAndroidApply(DWORD dwCount);//���������
	bool PostReleaseSingleAndroid(DWORD dwUserID);//�ͷŵ���������
	bool PostReleaseArrayAndroid(CString androids);//�����ͷŻ�����
	bool PostReleaseAllAdroid();//�ͷ����Ի�����
	bool PostReleaseAndDeleteAndroid(DWORD dwUserID, WORD dwBind);//�ͷŲ�ɾ�������˹��������������
	//�ڲ�����
private:
	
	bool DeleteUserItem(DWORD dwUserIndex);//ɾ���û�
	virtual bool AssignTable(CChampionshipMatch *pMatch, WORD wTableCount);//��������
	virtual bool AssignTableForFullUser(CChampionshipMatch *pMatch, WORD wTableCount);//��������
	int GetFreeTableCount();//��ȡ����������
	bool ReadMatchRule();	//�����ݿ��ȡ��������
	
	void MatchJoinPostDB(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);//���˱���Ͷ�����ݿ�
	void InitMatchConfig();//��ʼ����������
	bool CheckUserMatchFee(IServerUserItem * pIServerUserItem);//У���û�������
	void CreateMatchInstance();
	void CreateMatchInstanceAfter();
	void CreateMatchInstanceEnd();
	void InitMatchTime();
	void InitTimer();
	bool  WriteExpAward(IServerUserItem *pIServerUserItem, DWORD dwRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch);

	//�������У��
protected:
	bool MatchJoinCheck(IServerUserItem * pIServerUserItem);
	bool MatchJoinRequireCheck(IServerUserItem * pIServerUserItem);
	bool MatchJoinFeeCheck(IServerUserItem * pIServerUserItem);
	bool MatchJoinTimeCheck(IServerUserItem * pIServerUserItem);
	
public:
	virtual bool  OnEventMatchStart(CChampionshipMatch *pMatch);//������������ʼ����
	virtual bool  OnEventMatchOver(CChampionshipMatch *pMatch);//��������
	virtual bool  OnUserOut(IServerUserItem *pUserItem,CChampionshipMatch *pMatch);//�û���̭
	virtual bool  SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);//��������
	virtual bool  SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch);
	virtual bool  KillGameTimer(DWORD dwTimerID);
	virtual bool  SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType,CChampionshipMatch *pMatch=NULL);
	bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType,CChampionshipMatch *pMatch=NULL);
	virtual bool  WriteUserAward(IServerUserItem *pIServerUserItem, DWORD dwRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch);
	virtual bool  SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch);//�����û�����	
	virtual bool  SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch);//����״̬	
	virtual bool SendMatchStatus(IServerUserItem * pIServerUserItem,BYTE cbStatus,CChampionshipMatch *pMatch=NULL);//���ͱ���״̬
	virtual bool  SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);	//�������ݵ������� 	
	virtual bool  SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch);//�������ݵ�һ���û� 	
	virtual IAndroidUserItem * GetFreeAndroidUserItem();//��ȡһ�����еĻ�����	
	virtual bool SetReplaceAndroidItem(IServerUserItem *pIServerUserItem);//�����油������	
	virtual ITableFrame* GetTableFrame(IServerUserItem *pIServerUserItem);//��ȡ����ָ��
	virtual void AddMatchUserItem(IServerUserItem *pIServerUserItem);//����������

public:
	virtual IServerUserItem * SearchMatchUser(DWORD dwUserID);
};

//////////////////////////////////////////////////////////////////////////
//�������

//////////////////////////////////////////////////////////////////////////

#endif