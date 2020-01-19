#ifndef GAME_MATCH_SERVICE_MANAGER_HEAD_FILE
#define GAME_MATCH_SERVICE_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "MatchServiceHead.h"
#include "ChampionshipMatch.h"
#include "..\..\Server\GameServer\DataBasePacket.h"
//////////////////////////////////////////////////////////////////////////
//定时器子项
struct tagMatchTimerItem
{
	DWORD								dwTimerID;						//定时器 ID
	DWORD								dwRealID;						//实际ID
	DWORD								dwRepeatTimes;					//重复次数
	WPARAM								wBindParam;						//绑定参数
};

#define	MATCH_CLOSE	0
#define MATCH_OPEN	1

class CChampionshipMatch;

//类说明
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;
typedef CWHArray<CChampionshipMatch*>CMatchGroupArray;
typedef CWHArray<tagMatchTimerItem *> CTimerItemPtr;

//游戏服务器管理类
class MATCH_SERVICE_CLASS CGameMatchServiceManager : public IGameMatchServiceManager, public IChampionshipMatchSink
{
	//变量定义
protected:
	CMatchUserItemArray					m_OnMatchUserItem;				//参赛用户
	CMatchUserItemArray					m_OutUserItem;					//淘汰的用户
	const tagGameServiceOption *		m_pGameServiceOption;			//服务配置
	const tagGameServiceAttrib *		m_pGameServiceAttrib;			//服务属性

	//接口变量
protected:
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;		//网络接口
	IDataBaseEngine *					m_pIDataBaseEngine;				//数据引擎
	ITimerEngine *						m_pITimerEngine;				//时间引擎
	IMainServiceFrame				*  m_pIGameServiceFrame;			//功能接口
	IServerUserManager *				m_pIServerUserManager;			//用户管理
	ITableFrame							** m_ppITableFrame;				//框架接口
	IAndroidUserManager					*m_pAndroidUserManager;			//机器管理
	bool								*m_bTableUsed;					//桌子是否已经分配了

	CMatchGroupArray					m_MatchGroup;	
	CChampionshipMatch					*m_pCurMatch;
	CMatchGroupArray					m_OverGroup;					//已经结束待回收桌子的分组
	CWHArray<WORD,WORD>					m_UnRecovery;					//没有回收的桌子

	CTimerItemPtr						m_TimerItemFree;				//空闲数组
	CTimerItemPtr						m_TimerItemActive;				//活动数组

	WCHAR								m_guidRoom[40];					//guidRoom

	CMD_GR_MatchGlobal					m_MatchGlobal;
	CMD_GR_MatchJoin					m_MatchJoin;
	CMD_GR_RewardArray					m_Reward[3];
	BYTE								m_cbMatchOpen;					//定时赛复位标识
	BYTE								m_cbRealUserJoin;				//真人玩家参赛
	bool								m_bMatchOver;					//比赛结束控制变量
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
	//用户参加比赛
	virtual bool  OnUserJoinGame(IServerUserItem * pIServerUserItem);
	//用户退出比赛
	virtual bool  OnUserQuitGame(IServerUserItem * pIServerUserItem, BYTE cbReason,WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD);
	//时间赛开赛前检测
	virtual bool StartMatchCheck();
public:
	virtual VOID *  CreateGameMatchSink(REFGUID Guid, DWORD dwQueryVer);
	//网络接口
public:
	//发送数据
	virtual bool  SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//命令消息
	virtual bool OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//比赛全局
	virtual void GetMatchGlobal(CMD_GR_MatchGlobal& MatchGlobal);

	//机器人功能函数
public:
	bool PostAndroidApply(DWORD dwCount);//申请机器人
	bool PostReleaseSingleAndroid(DWORD dwUserID);//释放单个机器人
	bool PostReleaseArrayAndroid(CString androids);//批量释放机器人
	bool PostReleaseAllAdroid();//释放所以机器人
	bool PostReleaseAndDeleteAndroid(DWORD dwUserID, WORD dwBind);//释放并删除机器人管理类关联机器人
	//内部功能
private:
	
	bool DeleteUserItem(DWORD dwUserIndex);//删除用户
	virtual bool AssignTable(CChampionshipMatch *pMatch, WORD wTableCount);//分配桌子
	virtual bool AssignTableForFullUser(CChampionshipMatch *pMatch, WORD wTableCount);//分配桌子
	int GetFreeTableCount();//获取空闲桌子数
	bool ReadMatchRule();	//从数据库读取比赛规则
	
	void MatchJoinPostDB(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);//真人报名投递数据库
	void InitMatchConfig();//初始化比赛配置
	bool CheckUserMatchFee(IServerUserItem * pIServerUserItem);//校验用户报名费
	void CreateMatchInstance();
	void CreateMatchInstanceAfter();
	void CreateMatchInstanceEnd();
	void InitMatchTime();
	void InitTimer();
	bool  WriteExpAward(IServerUserItem *pIServerUserItem, DWORD dwRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch);

	//报名检测校验
protected:
	bool MatchJoinCheck(IServerUserItem * pIServerUserItem);
	bool MatchJoinRequireCheck(IServerUserItem * pIServerUserItem);
	bool MatchJoinFeeCheck(IServerUserItem * pIServerUserItem);
	bool MatchJoinTimeCheck(IServerUserItem * pIServerUserItem);
	
public:
	virtual bool  OnEventMatchStart(CChampionshipMatch *pMatch);//报名人满，开始比赛
	virtual bool  OnEventMatchOver(CChampionshipMatch *pMatch);//比赛结束
	virtual bool  OnUserOut(IServerUserItem *pUserItem,CChampionshipMatch *pMatch);//用户淘汰
	virtual bool  SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);//发送数据
	virtual bool  SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch);
	virtual bool  KillGameTimer(DWORD dwTimerID);
	virtual bool  SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType,CChampionshipMatch *pMatch=NULL);
	bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType,CChampionshipMatch *pMatch=NULL);
	virtual bool  WriteUserAward(IServerUserItem *pIServerUserItem, DWORD dwRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch);
	virtual bool  SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch);//发送用户分数	
	virtual bool  SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch);//发送状态	
	virtual bool SendMatchStatus(IServerUserItem * pIServerUserItem,BYTE cbStatus,CChampionshipMatch *pMatch=NULL);//发送比赛状态
	virtual bool  SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);	//发送数据到服务器 	
	virtual bool  SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch);//发送数据到一组用户 	
	virtual IAndroidUserItem * GetFreeAndroidUserItem();//获取一个空闲的机器人	
	virtual bool SetReplaceAndroidItem(IServerUserItem *pIServerUserItem);//设置替补机器人	
	virtual ITableFrame* GetTableFrame(IServerUserItem *pIServerUserItem);//获取桌子指针
	virtual void AddMatchUserItem(IServerUserItem *pIServerUserItem);//报名添加玩家

public:
	virtual IServerUserItem * SearchMatchUser(DWORD dwUserID);
};

//////////////////////////////////////////////////////////////////////////
//组件创建

//////////////////////////////////////////////////////////////////////////

#endif