#ifndef CHAMPIONSHIP_MATCH_HEAD_FILE
#define CHAMPIONSHIP_MATCH_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameMatchSink.h"
#include "MatchServiceHead.h"
#include "..\..\Server\GameServer\DataBasePacket.h"

typedef CMap<DWORD,DWORD,IServerUserItem *, IServerUserItem *>CUserItemMap;
typedef CMap<IServerUserItem *, IServerUserItem *, DWORD, DWORD>CUserSeatMap;

typedef CMap<DWORD,DWORD,BYTE,BYTE> MatchStatusMap;



#define IDI_TURN_MATCH_START		(IDI_MATCH_MODULE_START+1)
#define IDI_DELETE_OUT_USER		 	(IDI_MATCH_MODULE_START+2)					//�ӳ�һ��ʱ��ɾ����̭�û� 
#define IDI_DELETE_OVER_MATCH		(IDI_MATCH_MODULE_START+3)					//ɾ�������ı�����
#define IDI_MATCH_POST_ANDROID		(IDI_MATCH_MODULE_START+4)					//������������
#define IDI_ANDROID_SIGN_UP			(IDI_MATCH_MODULE_START+5)					//�����˶�ʱ����

//ChampionshipMatch��ʱ��
#define IDI_GROUP_START				(IDI_MATCH_MODULE_START+60)					//�������ڵĶ�ʱ��
#define IDI_CHECK_MATCH_START		(IDI_GROUP_START+1)							//������ʼ��ⶨʱ��
#define IDI_CHECK_MATCH_GROUP		(IDI_GROUP_START+2)							//��Ѳ���п��������� 
#define IDI_START_MATCH_START		(IDI_GROUP_START+3)							//��ʼһ������
#define IDI_MATCH_TIME_START		(IDI_GROUP_START+4)							//��ʱ��������ʱ��

//����������ʽ
#define  HundredPercent(s)					(100+s)/100


#ifdef _UNICODE
#define VER_IChampionshipMatch INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatch={0x786c5501,0xa7a4,0x4531,0x0091,0xc5,0x3e,0x07,0x3e,0xa6,0x0a,0xf9};
#else
#define VER_IChampionshipMatch INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatch={0x1a4e0406,0x8ddc,0x4e34,0x0083,0x89,0xb5,0x5e,0x4e,0xfa,0x6b,0xb7};
#endif

interface IChampionshipMatch :public IUnknownEx
{
	//�û�����
	virtual bool OnUserSignUp(IServerUserItem *pUserItem)=NULL;
	//�û�����
	virtual bool OnUserQuitMatch(IServerUserItem *pUserItem,bool bMatchNotOpen=false)=NULL;
	//��������
	virtual void SetMatchTable(ITableFrame *pTable[], WORD wTableCount)=NULL;
};

#ifdef _UNICODE
#define VER_IChampionshipMatchSink INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatchSink={0x30e4794a,0x610e,0x4873,0x00bd,0x23,0x9c,0xc7,0x2c,0xbe,0xd4,0xc5};
#else
#define VER_IChampionshipMatchSink INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatchSink={0xfc8dc7b5,0x2318,0x411f,0x00b9,0x09,0x6c,0xdd,0x39,0x38,0x0d,0x18};
#endif

class CChampionshipMatch;

interface IChampionshipMatchSink:public IUnknownEx
{
	//�û���̭
	virtual bool  OnUserOut(IServerUserItem *pUserItem,CChampionshipMatch *pMatch)=NULL;
	//��������
	virtual bool  SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��ʱ��
	virtual bool  SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch)=NULL;
	//�رն�ʱ��
	virtual bool  KillGameTimer(DWORD dwTimerID)=NULL;
	//������Ϣ
	virtual bool  SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType,CChampionshipMatch *pMatch=NULL)=NULL;
	//д�뽱��
	virtual bool  WriteUserAward(IServerUserItem *pIServerUserItem, DWORD dwRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch)=NULL;
	//������ʼ
	virtual bool  OnEventMatchStart(CChampionshipMatch *pMatch)=NULL;
	//��������
	virtual bool  OnEventMatchOver(CChampionshipMatch *pMatch)=NULL;
	//�����û�����
	virtual bool  SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)=NULL;
	//����״̬
	virtual bool  SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)=NULL;
	//�������ݵ������� 
	virtual bool  SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//�������ݵ�һ���û�
	virtual bool  SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch)=NULL;
	//���ͱ���״̬
	virtual bool SendMatchStatus(IServerUserItem * pIServerUserItem,BYTE cbStatus,CChampionshipMatch *pMatch=NULL)=NULL;
	//��ȡһ�����еĻ�����
	virtual IAndroidUserItem * GetFreeAndroidUserItem()=NULL;
	//�����油������
	virtual bool SetReplaceAndroidItem(IServerUserItem *pIServerUserItem)=NULL;
	//ʱ��������ǰ���
	virtual bool StartMatchCheck()=NULL;
	//��ȡ����ָ��
	virtual ITableFrame* GetTableFrame(IServerUserItem *pIServerUserItem)=NULL;
	//����������
	virtual void AddMatchUserItem(IServerUserItem *pIServerUserItem)=NULL;
	//��������
	virtual bool AssignTable(CChampionshipMatch *pMatch, WORD wTableCount)=NULL;
	//
	virtual IServerUserItem * SearchMatchUser(DWORD dwUserID) = NULL;
};
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;

struct ITableFramEx 
{
	ITableFrame *pTableFrame;
	BYTE cbGameCount;
	DWORD dwBase;
	BYTE cbOnSeatUserCount;
	BYTE cbStartTimes;
};
typedef CWHArray<ITableFramEx *>CTableFrameMananerArray;
struct tagMatchScore
{
	DWORD dwUserID;
	IServerUserItem *pUserItem;
	SCORE  lScore;
	SCORE  lGrade;
};

typedef CWHArray<tagMatchScore *> CUserMatchScoreArray;

struct ITableUser
{
	ITableFrame *pTableFrame;
	IServerUserItem *pUserItem;
};

typedef CWHArray<ITableUser *> CTableUserArray;

//����״̬
enum enMatchStatus
{
	MS_NULL_STATUS=0,
	MS_STOP_OUT,		//��ֹ̭ͣ
	MS_NEXT_ROUND,		//��һ��
	MS_SIT_DOWN,		//����
	MS_MATCH_START,		//��������
	MS_MATCH_MATCHING,	//
	MS_MATCH_JOIN,		//����״̬
};

class CChampionshipMatch:public IGameEventSink
{
	friend class CGameMatchServiceManager;

protected:
	WORD							m_wChairCount;					//������Ŀ
	DWORD							m_dwMatchID;
	CUserItemMap					m_OnMatchUserMap;				//�����û�
	CUserItemMap					m_OnSignUpUserMap;				//�����û�,���������
	IChampionshipMatchSink			*m_pMatchSink;
	ITableFramEx					*m_pAssignTable;
	WORD							m_wTableCount;					//��������ӵĸ���
	DWORD							m_dwCurBase;					// ��ǰ�Ļ���
	DWORD							m_dwInitalScore;				//�û���ʼ����
	bool							m_bAssignTable;					//���ӷ������
	bool							m_bGameEnd;		
	enMatchStatus					m_enMatchStatus;				//�����׶�
	WORD							m_wFinishTable;					//��ɱ����������� ���ֻ���
	//CTableUserArray					m_NextRoundUser;
	//CTableUserArray					m_ReadyOutUser;
	CUserItemMap					m_NextRoundUserMap;					//������̭���
	CUserItemMap					m_ReadyOutUserMap;					//�Ѿ���̭���
	WORD							m_wAndroidUserCount;			//�����˸���

	CMD_GR_MatchGlobal				*m_pMatchGlobal;
	BYTE							m_cbCurStage;					//��ǰ�׶�
	BYTE							m_dwCurStageType;				//��ǰ����
	BYTE							m_cbCurRound;					//��ǰ����
	BYTE							m_cbCurCount;					//��ǰ����
	DOUBLE							m_fRatio;						//��������ѹ��ϵ��
	DOUBLE							m_fMeanValue;					//���¹涨��ֵ����
	DWORD							m_dwMatchUserCount;				//��������
	BYTE							m_cbMatchType;					//��������
	DWORD							m_dwAddBaseTime;				//�ۼƻ���
	DWORD							m_dwWaitTableTime;				//�ۼƷ���
	bool							m_bStarted;						//������ʼ��ʶ
	bool							m_bSnakeSort;					//������λ��ʶ
	CMD_GR_StageArray				m_MatchStageInfo;				//����������Ϣ
	DWORD							m_dwCurRoundUserCount;			//��ǰ������
	bool							m_cbWait;
	DWORD							m_dwSolutionCount[3];
	DWORD							m_dwSolutionID[3];
	DWORD							m_dwCurSolutionID;				//��ǰ����ID
	DWORD							m_cbSolutionIndex;				//��������

public:
	CChampionshipMatch(CMD_GR_MatchGlobal* pMatchGlobal,DWORD dwMatchID,WORD wChairCount,IChampionshipMatchSink *pSink);
	virtual ~CChampionshipMatch(void);

public:
 	//�ͷŶ���
 	virtual VOID  Release(){}
 	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);
	//�û�����
	bool OnUserSignUp(IServerUserItem *pUserItem);
	//�û�����
	BOOL OnUserQuitMatch(IServerUserItem *pUserItem,bool bMatchNotOpen=false);
	//��������
	void SetMatchTable(ITableFrame *pTable[], WORD wTableCount);
	//��ȡ����λ
	ITableFramEx* GetEmptyChair(WORD &wTableID, WORD &wChairID);
	//��ʱ��
	bool OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	//��Ϸ����
	virtual bool  OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//�û�����
	virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser){return true;}
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){return true;}
	//��Ҷ��߻����� 
	virtual void  SetUserOffline(WORD wTableID,IServerUserItem *pUserItem, bool bOffline){return ;}

protected:
	//�������
	WORD SortUserItemArray(tagMatchScore score[], CMatchUserItemArray &MatchUserItemArray);
	//��ȡ����
	WORD GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame=NULL);
	//��ȡ�û�
	IServerUserItem * GetSeatUserItem(DWORD dwSeatID){return NULL;}
	
	//��������
	void MatchOver();
	//�û���Ŀ
	WORD GetSitUserCount(ITableFrame *pTableFrame);
	//��ȡ����
	WORD GetNullChairID(ITableFrame *pTableFrame);
	//��������
	void SendUserRank(ITableFrame *pTableFrame);
	//��ȡ�����û�������������������̭������̭
	DWORD GetMatchUserCount();
	//���ڱ���������
	WORD GetPlayingTable(); 
	//������ʼ��ʶ
	enMatchStatus GetCurMatchStatus(){return m_enMatchStatus;}
	void SetCurMatchStatus(enMatchStatus MatchStatus){m_enMatchStatus=MatchStatus;}

//��������ҵ��
protected:
	void DaLi_AddBaseScore();//��������
	bool DaLi_WaitTable();//�ȴ�����ʱ��
	bool DaLi_MatchProc();//�����������ƴ���
	void DaLi_OnEventGameEnd(ITableFrame *pTableFrame);//����������Ϸ����
	bool AssDaLi_MatchProc();//ASS�����������ƴ���
	void AssDaLi_OnEventGameEnd(ITableFrame *pTableFrame);
//���ֻ���
protected:
	bool CountScore_MatchProc();
	void CountScore_OnEventGameEnd(ITableFrame *pTableFrame);
	void CountScore_Func();
	bool CountScore_NextRoundCheck();

//��ʿ��λ
protected:
	bool SwissShift_MatchProc();
	void SwissShift_OnEventGameEnd(ITableFrame* pTableFrame);

//�ڷ���ʿ
protected:
	bool SwissScore_MatchProc();
	void SwissScore_OnEventGameEnd(ITableFrame* pTableFrame);
protected:
	//���������ӿ�
	bool MatchStandUpAction(IServerUserItem * pIServerUserItem,bool bLog=false,bool bAllUser=false);
	//�������½ӿ�
	bool MatchSitDownAction(bool bSort=false,bool bLog=false);
	//�������ָ��½ӿ�
	void MatchScoreUpdateAction();
	//�����׶ε����ӿ�
	bool MatchAddStageAction();
	//������������ӿ�
	bool MatchPromotionAction(DWORD dwCount);
	//������ֽ�������ӿ�
	bool MatchGradePromotionAction(DWORD dwCount);
	//������������ӿ�
	DWORD MatchScoreSortAction(tagMatchScore score[]);
	//�����������ӿ�
	DWORD MatchGradeSortAction(tagMatchScore score[]);
	//������λ���ӽӿ�
	ITableFramEx* GetSnakeEmptyChair(WORD &wTableID, WORD &wChairID);

	//��̭��һ�������
	DWORD OutUserSortAction(tagMatchScore score[]);
	//���ܽ�����һ�������
	DWORD NextRoundUserSortAction(tagMatchScore score[]);

protected:
	//��������
	bool InitMatchType();
	//��Ϸ�ͷ֣���ʼ����
	bool InitMatchBaseScore();
	//��������
	bool InitMatchUserCount();
	//��������
	bool InitMatchSolution();
	//��ʱ������
	bool InitMatchSolutionForTime();

protected:
	//�����������ͽ���
	bool MatchEndAndReward();
};


#endif