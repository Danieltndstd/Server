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
#define IDI_DELETE_OUT_USER		 	(IDI_MATCH_MODULE_START+2)					//延迟一段时间删除淘汰用户 
#define IDI_DELETE_OVER_MATCH		(IDI_MATCH_MODULE_START+3)					//删除结束的比赛组
#define IDI_MATCH_POST_ANDROID		(IDI_MATCH_MODULE_START+4)					//检测机器人数量
#define IDI_ANDROID_SIGN_UP			(IDI_MATCH_MODULE_START+5)					//机器人定时报名

//ChampionshipMatch计时器
#define IDI_GROUP_START				(IDI_MATCH_MODULE_START+60)					//比赛组内的定时器
#define IDI_CHECK_MATCH_START		(IDI_GROUP_START+1)							//比赛开始检测定时器
#define IDI_CHECK_MATCH_GROUP		(IDI_GROUP_START+2)							//轮巡所有开赛比赛组 
#define IDI_START_MATCH_START		(IDI_GROUP_START+3)							//开始一场比赛
#define IDI_MATCH_TIME_START		(IDI_GROUP_START+4)							//定时赛开赛定时器

//基数增长公式
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
	//用户报名
	virtual bool OnUserSignUp(IServerUserItem *pUserItem)=NULL;
	//用户退赛
	virtual bool OnUserQuitMatch(IServerUserItem *pUserItem,bool bMatchNotOpen=false)=NULL;
	//分配桌子
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
	//用户淘汰
	virtual bool  OnUserOut(IServerUserItem *pUserItem,CChampionshipMatch *pMatch)=NULL;
	//发送数据
	virtual bool  SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//定时器
	virtual bool  SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch)=NULL;
	//关闭定时器
	virtual bool  KillGameTimer(DWORD dwTimerID)=NULL;
	//发送消息
	virtual bool  SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType,CChampionshipMatch *pMatch=NULL)=NULL;
	//写入奖励
	virtual bool  WriteUserAward(IServerUserItem *pIServerUserItem, DWORD dwRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch)=NULL;
	//比赛开始
	virtual bool  OnEventMatchStart(CChampionshipMatch *pMatch)=NULL;
	//比赛结束
	virtual bool  OnEventMatchOver(CChampionshipMatch *pMatch)=NULL;
	//发送用户分数
	virtual bool  SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)=NULL;
	//发送状态
	virtual bool  SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)=NULL;
	//发送数据到服务器 
	virtual bool  SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//发送数据到一组用户
	virtual bool  SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch)=NULL;
	//发送比赛状态
	virtual bool SendMatchStatus(IServerUserItem * pIServerUserItem,BYTE cbStatus,CChampionshipMatch *pMatch=NULL)=NULL;
	//获取一个空闲的机器人
	virtual IAndroidUserItem * GetFreeAndroidUserItem()=NULL;
	//设置替补机器人
	virtual bool SetReplaceAndroidItem(IServerUserItem *pIServerUserItem)=NULL;
	//时间赛开赛前检测
	virtual bool StartMatchCheck()=NULL;
	//获取桌子指针
	virtual ITableFrame* GetTableFrame(IServerUserItem *pIServerUserItem)=NULL;
	//报名添加玩家
	virtual void AddMatchUserItem(IServerUserItem *pIServerUserItem)=NULL;
	//分配桌子
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

//比赛状态
enum enMatchStatus
{
	MS_NULL_STATUS=0,
	MS_STOP_OUT,		//淘汰停止
	MS_NEXT_ROUND,		//下一轮
	MS_SIT_DOWN,		//坐下
	MS_MATCH_START,		//比赛进行
	MS_MATCH_MATCHING,	//
	MS_MATCH_JOIN,		//报名状态
};

class CChampionshipMatch:public IGameEventSink
{
	friend class CGameMatchServiceManager;

protected:
	WORD							m_wChairCount;					//椅子数目
	DWORD							m_dwMatchID;
	CUserItemMap					m_OnMatchUserMap;				//参赛用户
	CUserItemMap					m_OnSignUpUserMap;				//报名用户,开赛就清除
	IChampionshipMatchSink			*m_pMatchSink;
	ITableFramEx					*m_pAssignTable;
	WORD							m_wTableCount;					//分配的桌子的个数
	DWORD							m_dwCurBase;					// 当前的基数
	DWORD							m_dwInitalScore;				//用户初始积分
	bool							m_bAssignTable;					//桌子分配与否
	bool							m_bGameEnd;		
	enMatchStatus					m_enMatchStatus;				//比赛阶段
	WORD							m_wFinishTable;					//完成比赛的桌子数 定局积分
	//CTableUserArray					m_NextRoundUser;
	//CTableUserArray					m_ReadyOutUser;
	CUserItemMap					m_NextRoundUserMap;					//可能淘汰玩家
	CUserItemMap					m_ReadyOutUserMap;					//已经淘汰玩家
	WORD							m_wAndroidUserCount;			//机器人个数

	CMD_GR_MatchGlobal				*m_pMatchGlobal;
	BYTE							m_cbCurStage;					//当前阶段
	BYTE							m_dwCurStageType;				//当前赛制
	BYTE							m_cbCurRound;					//当前轮数
	BYTE							m_cbCurCount;					//当前局数
	DOUBLE							m_fRatio;						//晋级积分压缩系数
	DOUBLE							m_fMeanValue;					//赛事规定均值分数
	DWORD							m_dwMatchUserCount;				//开赛人数
	BYTE							m_cbMatchType;					//比赛类型
	DWORD							m_dwAddBaseTime;				//累计基数
	DWORD							m_dwWaitTableTime;				//累计分桌
	bool							m_bStarted;						//比赛开始标识
	bool							m_bSnakeSort;					//蛇形排位标识
	CMD_GR_StageArray				m_MatchStageInfo;				//比赛赛制信息
	DWORD							m_dwCurRoundUserCount;			//当前轮人数
	bool							m_cbWait;
	DWORD							m_dwSolutionCount[3];
	DWORD							m_dwSolutionID[3];
	DWORD							m_dwCurSolutionID;				//当前方案ID
	DWORD							m_cbSolutionIndex;				//方案索引

public:
	CChampionshipMatch(CMD_GR_MatchGlobal* pMatchGlobal,DWORD dwMatchID,WORD wChairCount,IChampionshipMatchSink *pSink);
	virtual ~CChampionshipMatch(void);

public:
 	//释放对象
 	virtual VOID  Release(){}
 	//接口查询
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);
	//用户报名
	bool OnUserSignUp(IServerUserItem *pUserItem);
	//用户退赛
	BOOL OnUserQuitMatch(IServerUserItem *pUserItem,bool bMatchNotOpen=false);
	//分配桌子
	void SetMatchTable(ITableFrame *pTable[], WORD wTableCount);
	//获取空座位
	ITableFramEx* GetEmptyChair(WORD &wTableID, WORD &wChairID);
	//定时器
	bool OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	//游戏开始
	virtual bool  OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	//游戏结束
	virtual bool  OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser){return true;}
	//用户同意
	virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){return true;}
	//玩家断线或重连 
	virtual void  SetUserOffline(WORD wTableID,IServerUserItem *pUserItem, bool bOffline){return ;}

protected:
	//玩家排序
	WORD SortUserItemArray(tagMatchScore score[], CMatchUserItemArray &MatchUserItemArray);
	//获取名次
	WORD GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame=NULL);
	//获取用户
	IServerUserItem * GetSeatUserItem(DWORD dwSeatID){return NULL;}
	
	//比赛结束
	void MatchOver();
	//用户数目
	WORD GetSitUserCount(ITableFrame *pTableFrame);
	//获取空椅
	WORD GetNullChairID(ITableFrame *pTableFrame);
	//发送名次
	void SendUserRank(ITableFrame *pTableFrame);
	//获取参赛用户，包括晋级，可能淘汰，待淘汰
	DWORD GetMatchUserCount();
	//正在比赛的桌子
	WORD GetPlayingTable(); 
	//比赛开始标识
	enMatchStatus GetCurMatchStatus(){return m_enMatchStatus;}
	void SetCurMatchStatus(enMatchStatus MatchStatus){m_enMatchStatus=MatchStatus;}

//打立出局业务
protected:
	void DaLi_AddBaseScore();//基数增长
	bool DaLi_WaitTable();//等待分桌时间
	bool DaLi_MatchProc();//打立出局赛制处理
	void DaLi_OnEventGameEnd(ITableFrame *pTableFrame);//打立出局游戏结束
	bool AssDaLi_MatchProc();//ASS打立出局赛制处理
	void AssDaLi_OnEventGameEnd(ITableFrame *pTableFrame);
//定局积分
protected:
	bool CountScore_MatchProc();
	void CountScore_OnEventGameEnd(ITableFrame *pTableFrame);
	void CountScore_Func();
	bool CountScore_NextRoundCheck();

//瑞士移位
protected:
	bool SwissShift_MatchProc();
	void SwissShift_OnEventGameEnd(ITableFrame* pTableFrame);

//节分瑞士
protected:
	bool SwissScore_MatchProc();
	void SwissScore_OnEventGameEnd(ITableFrame* pTableFrame);
protected:
	//比赛起立接口
	bool MatchStandUpAction(IServerUserItem * pIServerUserItem,bool bLog=false,bool bAllUser=false);
	//比赛坐下接口
	bool MatchSitDownAction(bool bSort=false,bool bLog=false);
	//比赛积分更新接口
	void MatchScoreUpdateAction();
	//比赛阶段递增接口
	bool MatchAddStageAction();
	//比赛晋级排序接口
	bool MatchPromotionAction(DWORD dwCount);
	//比赛大分晋级排序接口
	bool MatchGradePromotionAction(DWORD dwCount);
	//比赛积分排序接口
	DWORD MatchScoreSortAction(tagMatchScore score[]);
	//比赛大分排序接口
	DWORD MatchGradeSortAction(tagMatchScore score[]);
	//蛇形排位椅子接口
	ITableFramEx* GetSnakeEmptyChair(WORD &wTableID, WORD &wChairID);

	//淘汰玩家积分排序
	DWORD OutUserSortAction(tagMatchScore score[]);
	//可能晋级玩家积分排序
	DWORD NextRoundUserSortAction(tagMatchScore score[]);

protected:
	//比赛类型
	bool InitMatchType();
	//游戏低分，初始积分
	bool InitMatchBaseScore();
	//开赛人数
	bool InitMatchUserCount();
	//比赛方案
	bool InitMatchSolution();
	//定时赛方案
	bool InitMatchSolutionForTime();

protected:
	//比赛结束发送奖励
	bool MatchEndAndReward();
};


#endif