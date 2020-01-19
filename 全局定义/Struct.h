#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE


#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//游戏货币,积分类型
enum enScoreType
{
	ScoreType_Score=1,							//金币
	ScoreType_Insure=2,							//银行
	ScoreType_Medal=3,							//奖章
	ScoreType_Jewel=4,							//钻石
	ScoreType_LobbyLV=5,						//大厅经验
	ScoreType_GameExp=104,						//游戏经验
	ScoreType_Master=9,							//大师分
	ScoreType_Compete=10,						//竞技分
	ScoreType_Race=11,							//竞赛积分
	ScoreType_Power=12,							//体力
	ScoreType_LoveLiness=13,					//魅力
	ScoreType_Honor=14,							//荣誉称号
	ScoreType_Achievements=15,					//成就
	ScoreType_VIP=20,							//VIP					
};


//计算推广奖励操作
enum enCalculateSpreadRewardOperationType
{
	SpreadRewardOperationType_Record = 0,		//不奖励，只记录
	SpreadRewardOperationType_Reward = 1		//奖励，并记录
};

/////////////////////////////////////////////////////////////////////////////////
//游戏列表

//游戏类型
struct tagGameType
{
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wTypeID;							//类型索引
	TCHAR							szTypeName[LEN_TYPE];				//种类名字
};

//游戏种类
struct tagGameKind
{
	WORD							wTypeID;							//类型索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wGameID;							//模块索引
	WORD							wIsRoomOnline;						//是否有房间开启
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szKindName[LEN_KIND];				//游戏名字
	TCHAR							szProcessName[LEN_PROCESS];			//进程名字
	TCHAR							szDownLoadUrl[100];					//下载地址
};

//游戏种类
struct tagGameKind_
{
	WORD							wKindID;							//类型索引
	WORD							wGameID;							//模块索引
	WORD							wIsRoomOnline;						//是否有房间开启
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR								szKindName[LEN_KIND];				//游戏名字
};

//游戏节点
struct tagGameNode
{
	WORD							wKindID;							//名称索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wNodeID;							//节点索引
	TCHAR							szNodeName[LEN_NODE];				//节点名称
};

struct tagGameCollection
{
	DWORD						dwUserID;
	WORD						wSortID;
	WORD						wKindID;
	TCHAR						szDisplayName[LEN_TYPE];
};

//定制类型
struct tagGamePage
{
	WORD							wPageID;							//页面索引
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wOperateType;						//控制类型
	TCHAR							szDisplayName[LEN_PAGE];			//显示名称
};

//游戏房间
struct tagGameServer
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wServerID;							//房间索引
	WORD							wServerPort;						//房间端口
	WORD							wServerType;						//房间类型
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	SCORE							lCellScore;							//房间低分
	BYTE							cbRoomType;

	TCHAR							szServerAddr[32];					//房间名称
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//限制配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分
	SCORE							lMinEnterTiLi;						//最低体力


	WORD							wCardBoxIndex;						//宝箱索引
};

//游戏房间手机端
struct tagGameServer_
{
	WORD							wServerID;							//房间索引
	WORD							wServerPort;							//房间端口
	WORD							wServerType;						//房间类型
	DWORD							dwOnLineCount;					//在线人数
	DWORD							dwFullCount;						//满员人数
	long long 						lCellScore;							//房间低分	游戏基数
	BYTE							cbRoomType;							// 0免费 1初级 2中级 3高级
	TCHAR							szServerAddr[LEN_SERVER];		//房间IP
	TCHAR							szServerName[LEN_SERVER];	//房间名称

	//限制配置
	SCORE							lMinEnterScore;						//最低入场分
	SCORE							lMinTableScore;						//最低坐下分	
	SCORE							lMaxEnterScore;						//最高入场分

	WORD							wKindID;							//游戏类型

};

//比赛房间
struct tagGameMatch
{
	WORD				wServerID;						//比赛索引
	/*
	WORD				wOrderID;						//比赛排序
	WORD				wMatchTab;						//比赛标签
	WORD				wMatchType;						//比赛类型
	SYSTEMTIME			sysMatchStartTime;				//开赛时间
	SYSTEMTIME			sysMatchJoinTime;				//报名时间
	WORD				wJoinCount;						//条件个数
	//tagMatchJoin		MatchJoin[20];					//报名费用
	TCHAR				szMatchReward[32];				//冠军奖品
	TCHAR				szMatchName[80];				//比赛全称
	*/

	//tagMatchRequireInfo			Require;				//报名条件
	//tagMatchFeeInfo				Fee;					//报名费用
	//tagMatchReviveInfo			ReviveInfo;				//比赛复活
	//CMD_GR_MatchDateTime		DateTime;				//比赛时间
	//tagMatchTagout				Tagout;					//挂牌
};

//视频配置
struct tagAVServerOption
{
	WORD							wAVServerPort;						//视频端口
	DWORD							dwAVServerAddr;						//视频地址
};

//在线信息
struct tagOnLineInfoKind
{
	WORD							wKindID;							//类型标识
	DWORD							dwOnLineCount;						//在线人数
};

//在线信息
struct tagOnLineInfoServer
{
	WORD							wServerID;							//房间标识
	DWORD							dwOnLineCount;						//在线人数
};

struct tagBroadcastMsg
{
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szKindName[LEN_KIND];
	TCHAR							szScore[32];
};

//////////////////////////////////////////////////////////////////////////////////
//用户信息

//桌子状态
struct tagTableStatus
{
	BYTE							cbTableLock;						//锁定标志
	BYTE							cbPlayStatus;						//游戏标志
};

//用户状态
struct tagUserStatus
{
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子位置
	BYTE							cbUserStatus;						//用户状态
};

//用户属性
struct tagUserAttrib
{
	BYTE							cbCompanion;						//用户关系
};

//用户积分
struct tagUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行

	//输赢信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数

	//全局信息
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//用户经验
	LONG							lLoveLiness;						//用户魅力
};

//用户积分
struct tagMobileUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lInsure;							//用户银行

	//输赢信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数

	//全局信息
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//用户经验
};


//道具使用
struct tagUsePropertyInfo
{
	WORD                            wPropertyCount;                     //道具数目
	WORD                            dwValidNum;						    //有效数字
	DWORD                           dwEffectTime;                       //生效时间
};


//用户道具
struct tagUserProperty
{
	WORD                            wPropertyUseMark;                   //道具标示
	tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//使用信息   
};

//道具包裹
struct tagPropertyPackage
{
	WORD                            wTrumpetCount;                     //小喇叭数
	WORD                            wTyphonCount;                      //大喇叭数
};

//时间信息
struct tagTimeInfo
{
	DWORD						dwEnterTableTimer;						//进出桌子时间
	DWORD						dwLeaveTableTimer;						//离开桌子时间
	DWORD						dwStartGameTimer;						//开始游戏时间
	DWORD						dwEndGameTimer;							//离开游戏时间
};

struct tagBaseScore
{
	WORD						wScoreType;
	SCORE						lScore;
};

struct tagScoreList
{
	WORD						wCount;
	tagBaseScore				BaseScore[20];
};


//用户信息
struct tagUserInfo
{
	//基本属性
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团 I D
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//头像信息
	DWORD							wFaceID;							//头像索引
	DWORD							dwCustomID;							//自定标识
	DWORD							wAvatarID;							//形象索引

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
	BYTE							cbClientKind;						//连接类型

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态
	WORD							wLastTableID;						//最近桌子
	WORD							wLastChairID;						//最近椅子

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行
	SCORE							lRechargeScore;						//充值分数
	SCORE							lExchangeScore;						//兑换分数
	SCORE							lRevenue;							//用户税收

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//用户经验
	LONG							lLoveLiness;						//用户魅力

	//////////////////////////////////////////////////////////////////////////
};

//用户宝箱
struct tagUserCardBox
{
	WORD							wCardCount;							//宝箱局数
	BYTE							cbBoxAwardCount;					//剩余领奖次数
	BYTE							cbIsComplete;						//是否完成
	BYTE							cbBoxIndex;							//宝箱索引
};

//用户信息
struct tagUserInfoHead
{
	//用户属性
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGroupID;							//社团 I D

	//头像信息
	DWORD							wFaceID;							//头像索引
	DWORD							dwCustomID;							//自定标识
	DWORD							dwAvatarID;							//形象索引

	//用户属性
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//当前经验
	LONG							lLoveLiness;						//用户魅力


	SCORE							lRevenue;							//用户税收
	SCORE							lRechargeScore;						//充值分数
	SCORE							lExchangeScore;						//兑换分数
};

//用户信息
struct tagUserInfoH5
{
	//用户属性
	DWORD							dwUserID;							//用户 I D
	DWORD							wFaceID;							//头像索引
	BYTE							cbGender;							//用户性别
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态
	SCORE							lScore;								//用户分数
																
	TCHAR							szNickName[LEN_NICKNAME];
};

//头像信息
struct tagCustomFaceInfo
{
	DWORD							dwDataSize;							//数据大小
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//用户信息
struct tagUserRemoteInfo
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//等级信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//位置信息
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	TCHAR							szGameServer[LEN_SERVER];			//房间位置
};

//称号信息
struct tagHonorInfo
{
	DWORD							dwHonorID;							//称号ID
	DWORD							dwTypeID;							//类型ID
};

//////////////////////////////////////////////////////////////////////////////////

//广场子项
struct tagGamePlaza
{
	WORD							wPlazaID;							//广场标识
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[32];					//服务器名
};

//级别子项
struct tagLevelItem
{
	LONG							lLevelScore;						//级别积分
	TCHAR							szLevelName[16];					//级别描述
};

//会员子项
struct tagMemberItem
{
	BYTE							cbMemberOrder;						//等级标识
	TCHAR							szMemberName[16];					//等级名字
};

//管理子项
struct tagMasterItem
{
	BYTE							cbMasterOrder;						//等级标识
	TCHAR							szMasterName[16];					//等级名字
};

//列表子项
struct tagColumnItem
{
	BYTE							cbColumnWidth;						//列表宽度
	BYTE							cbDataDescribe;						//字段类型
	TCHAR							szColumnName[16];					//列表名字
};

//地址信息
struct tagAddressInfo
{
	TCHAR							szAddress[32];						//服务地址
};

//数据信息
struct tagDataBaseParameter
{
	WORD							wDataBasePort;						//数据库端口
	TCHAR							szDataBaseAddr[32];					//数据库地址
	TCHAR							szDataBaseUser[32];					//数据库用户
	TCHAR							szDataBasePass[32];					//数据库密码
	TCHAR							szDataBaseName[32];					//数据库名字
};

//房间配置
struct tagServerOptionInfo
{
	//挂接属性
	WORD							wKindID;							//挂接类型
	WORD							wNodeID;							//挂接节点
	WORD							wSortID;							//排列标识

	//税收配置
	WORD							wRevenueRatio;						//税收比例
	SCORE							lServiceScore;						//服务费用

	//房间配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分
	SCORE							lMinEnterTiLi;						//最低体力

	//会员限制
	BYTE							cbMinEnterMember;					//最低会员
	BYTE							cbMaxEnterMember;					//最高会员

	//房间属性
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//用户信息
struct tagMobileUserInfoHead
{
	//用户属性
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwUserID;							//用户 I D

	//头像信息
	DWORD							wFaceID;							//头像索引
	DWORD							dwCustomID;							//自定标识

	//用户属性
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwExperience;						//用户经验
};

struct tagMatchStatus
{
	BYTE							cbMatchStatus;						//比赛状态
	DWORD							dwUserID;							//用户 ID
};

struct tagMatchNotify
{
	BYTE							cbNotify;
	WORD							wServerID;							//房间ID
	WORD							wArraySize;							
	DWORD							UserIDArray[MAX_PATH];				//用户ID
};

struct tagMatchingNotify
{
	BYTE							cbNotify;
	WORD							wServerID;							//房间ID
};

struct tagGameUserFace
{
	unsigned long					user_id;
	unsigned short					face_id;
};

struct tagUserTaskInfo
{
	BYTE							cbIsComplete;
	WORD							wTaskID;
	WORD							wCurrentProcess;
	WORD							wTotalProcess;
	WORD							wItemCount;
	DWORD							dwRewardMedal;
	DWORD							dwHours;
	SCORE							lRewardGold;
	TCHAR							szTaskKind[32];
	TCHAR							szItemName[32];
	TCHAR							szTaskName[32];
	TCHAR							szTaskDesc[MAX_PATH];
};

struct tagUserTaskList
{
	WORD						wCount;
	tagUserTaskInfo				TaskInfo[10];
};

struct tagRankTopInfo
{
	WORD						wKindID;
	WORD						wServerID;
	DWORD						dwUserID;
	DWORD						RewardMedal;
	SCORE						lScore;
	TCHAR						szNickName[32];
};

struct tagRankTopList
{
	WORD							wCount;
	tagRankTopInfo					RankInfo[6];
};

struct tagGameItemInfo
{
	WORD							wItemID;
	WORD							wCount;
	TCHAR							szItemName[32];
};

//加载道具
struct tagGameItemList
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wCount;
	DWORD							dwUserID;
	tagGameItemInfo					GameItemInfo[8];
};



/////////////////游戏任务定义/////////////////////////////////

enum enTaskType
{
	enUnknowTask=-1,
	enCardTask,			//对局
	enWinTask,			//胜局
	enWinningStreak,	//连胜
	enScoreTask,		//积分排名
	enLimitTimeTask		//限时
};

struct tagTaskBase
{
	enTaskType							TaskType;						//任务类型 
	WORD								wTaskID;						//任务标识
	WORD								wTotalProcess;					//总进度
};

struct tagTaskReward
{
	SCORE								lGold;
	DWORD								dwMedal;
	WORD								wItem;
	TCHAR								szItemName[32];
};

struct tagTaskItem
{
	BYTE								cbCompleteState;				//完成标识 1 0否
	WORD								wCurProcess;					//当前进度
	tagTaskBase							TaskBase;
	tagTaskReward						TaskReward;
};

struct tagTaskInfo
{
	DWORD	dwTaskID;
	WORD	wTaskWeight;	
};

struct tagTaskRefresh
{
	DWORD	dwUserID;	
	DWORD	dwTaskID;
	BYTE	cbTaskNum;	//任务序号
};

struct tagTaskAward
{
	DWORD	dwUserID;	
	DWORD	dwTaskID;
	BYTE	cbTaskNum;	//任务序号
};
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////比赛定义//////////////////////////////////////////////


//邀请码奖励详细信息
// struct tagInviteCodeRewardInfo
// {
// 	DWORD dwID;						//奖励ID
// 	BYTE cbCondition;				//奖励条件
// 	DWORD dwTopUpLimit;				//充值限制
// 	DWORD dwLevelLimit;				//等级限制
// 	DWORD dwRewardItem;				//奖励物品
// 	WORD wRewardNumber;				//奖励数量
// 
// 	TCHAR szConditionExplain[100];   //条件说明
// };
// 
// //推广详细信息
// struct tagSpreadInfo
// {
// 	DWORD dwSpreadID;				 //推广奖励ID
// 	WORD wSpreadLevel;				//推广等级
// 	WORD wRewardLevel;				//奖励等级
// 	WORD wReachUser;				//达标人数标准
// 	DWORD dwReachRevenue;			//达标抽水标准
// 	WORD wRewardPercent;			//奖励百分比
// 	DWORD dwRewardTimeOne;			//奖励时间1
// 	DWORD dwRewardTimeTwo;			//奖励时间2
// };

#pragma pack()

#endif