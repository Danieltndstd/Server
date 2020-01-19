#pragma once

#define IDI_GAME_OPTION				(TIME_TABLE_SINK_RANGE+2)			//断线标识
//数组定义
typedef IServerUserItem * CTableUserItemArray[MAX_CHAIR];				//游戏数组
typedef CWHArray<IServerUserItem *> CLookonUserItemArray;				//旁观数组
typedef CWHArray<tagGameScoreRecord *> CGameScoreRecordArray;			//记录数组
typedef CWHArray<tagFishRecordInfo *> CFishRecordArray;					//捕鱼游戏记录数组
interface IMatchTableFrameNotify;
//////////////////////////////////////////////////////////////////////////////////


//桌子框架
class CTableFrame : public ITableFrame
{
	//游戏属性
protected:
	WORD							m_wTableID;							//桌子号码
	WORD							m_wChairCount;						//椅子数目
	BYTE							m_cbStartMode;						//开始模式

	//状态变量
protected:
	bool							m_bGameStarted;						//游戏标志
	bool							m_bDrawStarted;						//游戏标志
	bool							m_bTableStarted;					//游戏标志

	//状态变量
protected:
	bool							m_bAllowLookon[MAX_CHAIR];			//旁观标志
	SCORE							m_lFrozenedScore[MAX_CHAIR];		//服务费用

	//游戏变量
protected:
	LONG							m_lCellScore;						//单元积分
	BYTE							m_cbGameStatus;						//游戏状态

	//时间变量
protected:
	DWORD							m_dwDrawStartTime;					//开始时间
	SYSTEMTIME						m_SystemTimeStart;					//开始时间
	DWORD							m_wDrawCount;						//游戏局数

	//动态属性
protected:
	DWORD							m_dwTableOwnerID;					//桌主用户
	TCHAR							m_szEnterPassword[LEN_PASSWORD];	//进入密码

	//断线变量
protected:
	WORD							m_wOffLineCount[MAX_CHAIR];			//断线次数
	DWORD							m_dwOffLineTime[MAX_CHAIR];			//断线时间

	//用户数组
protected:
	CTableUserItemArray				m_TableUserItemArray;				//游戏用户
	CLookonUserItemArray			m_LookonUserItemArray;				//旁观用户

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	ITableFrameSink	*				m_pITableFrameSink;					//桌子接口
	IMainServiceFrame *				m_pIMainServiceFrame;				//服务接口
	IAndroidUserManager *			m_pIAndroidUserManager;				//机器接口

	//扩展接口
protected:
	ITableUserAction *				m_pITableUserAction;				//动作接口
	ITableUserRequest *				m_pITableUserRequest;				//请求接口
	ITableUserAction *				m_pIMatchTableAction;				//动作接口(比赛用)

	//数据接口
protected:
	IDataBaseEngine *				m_pIKernelDataBaseEngine;			//内核数据
	IDataBaseEngine *				m_pIRecordDataBaseEngine;			//记录数据

	//配置信息
protected:
	tagGameParameter *				m_pGameParameter;					//配置参数
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//服务属性
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置

	//游戏记录
protected:
	CGameScoreRecordArray			m_GameScoreRecordActive;			//游戏记录
	static CGameScoreRecordArray	m_GameScoreRecordBuffer;			//游戏记录

	CFishRecordArray				m_FishRecordActive;					//捕鱼游戏记录
	static CFishRecordArray			m_FishRecordBuffer;					//捕鱼游戏记录

	//比赛接口
protected:
	IGameMatchSink					* m_pIGameMatchSink;				//比赛接口

	//排行信息
protected:
	tagRankTopList					m_RankTopList;

	CFileLog						m_fileLog;
	CFileLog						m_ServerFileLog;

	//函数定义
public:
	//构造函数
	CTableFrame();
	//析构函数
	virtual ~CTableFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//属性接口
public:
	//桌子号码
	virtual WORD GetTableID() { return m_wTableID; }
	//游戏人数
	virtual WORD GetChairCount() { return m_wChairCount; }

	//配置参数
public:
	//自定配置
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//服务属性
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//服务配置
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }

	//配置接口
public:
	//开始模式
	virtual BYTE GetStartMode() { return m_cbStartMode; }
	//开始模式
	virtual VOID SetStartMode(BYTE cbStartMode) { m_cbStartMode=cbStartMode; }

	//单元积分
public:
	//单元积分
	virtual LONG GetCellScore() { return m_lCellScore; }
	//单元积分
	virtual VOID SetCellScore(LONG lCellScore) { m_lCellScore=lCellScore;m_pGameServiceOption->lCellScore=lCellScore; m_pITableFrameSink->SetGameBaseScore(lCellScore);}

	//状态接口
public:
	//获取状态
	virtual BYTE GetGameStatus() { return m_cbGameStatus; }
	//设置状态
	virtual VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; }

	//信息接口
public:
	//游戏状态
	virtual bool IsGameStarted() { return m_bGameStarted; }
	//游戏状态
	virtual bool IsDrawStarted() { return m_bDrawStarted; }
	//游戏状态
	virtual bool IsTableStarted() { return m_bTableStarted; }
	//锁定状态
	virtual bool IsTableLocked() { return (m_szEnterPassword[0]!=0); }

	//控制接口
public:
	//开始游戏
	virtual bool StartGame();
	//解散游戏
	virtual bool DismissGame();
	//结束游戏
	virtual bool ConcludeGame(BYTE cbGameStatus);
	//结束桌子
	virtual bool ConcludeTable();

	//用户接口
public:
	//寻找用户
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID);
	//游戏用户
	virtual IServerUserItem * GetTableUserItem(WORD wChairID);
	//旁观用户
	virtual IServerUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//写分接口
public:
	// 捕鱼写记录
	virtual bool WriteFishRecord(WORD wChairID, tagFishRecordInfo & FishRecordInfo, BYTE WriteScore = 0);
	// 子游戏 滚动消息触发
	virtual bool GameScrollMessage(VOID * pData, WORD wDataSize);
	//写入积分
	virtual bool WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo, BYTE WriteScore = 0);
	//写入积分
	virtual bool WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount);
	//写入奖牌
	virtual bool WriteUserMedal(WORD wChairID, tagMedalInfo & MedalInfo);
	//写入奖牌
	virtual bool WriteTableMedal(tagMedalInfo MedalInfoArray[], WORD wCount);
	//写入录像
	virtual bool WriteVideoRecord(tagVideo& video) { return true; }

	//计算接口
public:
	//计算税收
	virtual SCORE CalculateRevenue(WORD wChairID, SCORE lScore);
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);

	//时间接口
public:
	//设置时间
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//删除时间
	virtual bool KillGameTimer(DWORD dwTimerID);

	//游戏用户
public:
	//发送数据
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID);
	//发送数据
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize,WORD wMainCmdID=MDM_GF_GAME);

	//旁观用户
public:
	//发送数据
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID);
	//发送数据
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//所有用户
public:
	//发送数据
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID);
	//发送数据
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//系统消息
public:
	//发送消息
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType);
	//游戏消息
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);
	//房间消息
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);

	//动作处理
public:
	//起立动作
	virtual bool PerformStandUpAction(IServerUserItem * pIServerUserItem,BYTE cbNotifyClient = TRUE);
	//起立动作
	virtual bool PerformStandUpActionH5(IServerUserItem * pIServerUserItem,BYTE cbNotifyClient = TRUE);
	//旁观动作
	virtual bool PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//坐下动作
	virtual bool PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword=NULL);
	//坐下动作
	virtual bool PerformSitDownActionH5(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword = NULL);

	//功能接口
public:
	//发送场景
	virtual bool SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//比赛接口
public:
	virtual bool SetMatchInterface(IUnknownEx * pIUnknownEx) { return true; }
	//获取比赛桌子接口
	virtual IGameMatchSink* GetGameMatchSink(){return m_pIGameMatchSink;}

	//功能函数
public:
	//获取空位
	WORD GetNullChairID();
	//随机空位
	WORD GetRandNullChairID();
	//用户数目
	WORD GetSitUserCount();
	//旁观数目
	WORD GetLookonUserCount();
	//断线数目
	WORD GetOffLineUserCount();

	//功能函数
public:
	//用户状况
	WORD GetTableUserInfo(tagTableUserInfo & TableUserInfo);
	//配置桌子
	bool InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter);

	//用户事件
public:
	//断线事件
	bool OnEventUserOffLine(IServerUserItem * pIServerUserItem);
	//积分事件
	bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//系统事件
public:
	//时间事件
	bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//游戏事件
	bool OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架事件
	bool OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//辅助函数
public:
	//桌子状态
	bool SendTableStatus();
	//请求失败
	bool SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode);

	//效验函数
public:
	//开始效验
	bool EfficacyStartGame(WORD wReadyChairID);
	//地址效验
	bool EfficacyIPAddress(IServerUserItem * pIServerUserItem);
	//积分效验
	bool EfficacyScoreRule(IServerUserItem * pIServerUserItem);
	//机器人存取款
	void ToSaveScore(IServerUserItem* pIServerUserItem);

public:
	virtual bool  WriteGameEvent(WORD wChairID,void* pData);
};
