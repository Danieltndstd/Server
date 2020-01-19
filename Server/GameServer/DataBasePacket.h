#pragma once

#pragma pack(1)
//请求数据包

//用户事件
#define	DBR_GR_LOGON_USERID			100									//I D 登录
#define	DBR_GR_LOGON_MOBILE			101									//手机登录
#define	DBR_GR_LOGON_ACCOUNTS		102									//帐号登录
#define DBR_GR_LOGON_TOKENID		103									//Token登录

//用户事件
#define	DBR_GR_LOGON_USERID_BCode			150									//I D 登录
#define	DBR_GR_LOGON_MOBILE_BCode			151									//手机登录
#define	DBR_GR_LOGON_ACCOUNTS_BCode			152									//帐号登录

//系统事件
#define DBR_GR_WRITE_GAME_SCORE		200									//游戏积分
#define DBR_GR_LEAVE_GAME_SERVER	201									//离开房间
#define DBR_GR_GAME_SCORE_RECORD	202									//积分记录
#define DBR_GR_MANAGE_USER_RIGHT	203									//权限管理
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//系统消息
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//敏感词
#define DBR_GR_USER_SITUP			206									//坐桌进出
#define DBR_GR_WRITE_GAME_MEDAL		207									//游戏奖牌
#define DBR_GP_GAME_FISH_RECORD		208									// 捕鱼游戏记录
#define DBR_GP_GAME_SCROLL_MESSAGE	209									// 游戏滚动消息

#define DBR_GR_SCROLL_MESSAGE				221							// 滚动消息读取
#define DBR_GR_LOAD_TIP_MESSAGE		222									//弹窗消息
#define DBR_GR_LOAD_SCROLL_MESSAGE		223									//读取滚动消息
#define DBR_GR_CLEAR_SCROLL_MESSAGE		224									//清除滚动消息

#define DBR_GR_SET_USER_OFFLINE_RECORD     225								//设置用户掉线记录
#define DBR_GR_GET_FIRST_REWARD_STATUS     226								//获取一局奖励状态

#define  DRB_GR_STATUS_USER_OFFLINE			227								//掉线玩家


//配置事件
#define DBR_GR_LOAD_PARAMETER		300									//加载参数
#define DBR_GR_LOAD_GAME_COLUMN		301									//加载列表
#define DBR_GR_LOAD_ANDROID_USER	302									//加载机器
#define DBR_GR_LOAD_GAME_PROPERTY	303									//加载道具

//银行事件
#define DBR_GR_USER_SAVE_SCORE		400									//存入元宝
#define DBR_GR_USER_TAKE_SCORE		401									//提取元宝
#define DBR_GR_USER_TRANSFER_SCORE	402									//转账元宝
#define DBR_GR_QUERY_INSURE_INFO	403									//查询银行
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    404							//查询用户
#define DBR_GR_USER_INSURE_SYNC		405									//同步元宝

//游戏事件
#define DBR_GR_PROPERTY_REQUEST		500									//购买礼物
#define DBR_GR_GAME_FRAME_REQUEST	502									//游戏请求
#define DBR_GR_KILL_PALAYER_LOCKER  503									//清除卡线


//机器人加载事件
#define DBR_GR_ANDRIOD_APPLY		700									//申请机器人id
#define DBR_GR_ANDRIOD_CURRENTVALID 701									//验证合法性
#define DBR_GR_ANDRIOD_REFRESH		702									//刷新时间戳
#define DBR_GR_ANDRIOD_RELEASE      703									//释放单个机器人
#define DBR_GR_ANDRIOD_ALL			704									//释放所以机器人
#define DBR_GR_ANDRIOD_ARRAY		705									//批量释放机器人
#define DBR_GR_ANDRIOD_RELEASE_DELETE	706								//释放单个机器人，并删除机器人

//道具事件
#define  DBR_GR_ITEM_LOAD			820									//加载道具

//////////////////////////////////////////////////////////////////////////////////
//输出信息

//逻辑事件
#define DBO_GR_LOGON_SUCCESS		100									//登录成功
#define DBO_GR_LOGON_FAILURE		101									//登录失败

#define DBO_GR_LOGON_SUCCESS_H5		110									//登录成功
#define DBO_GR_LOGON_SUCCESS_BCode	150									//登录成功

//配置事件
#define DBO_GR_GAME_PARAMETER		200									//配置信息
#define DBO_GR_GAME_COLUMN_INFO		201									//列表信息
#define DBR_GR_GAME_ANDROID_INFO	202									//机器信息
#define DBO_GR_GAME_PROPERTY_INFO	203									//道具信息

#define DBO_GP_UPDATE_ATTRIBUTE			256								//czg更新属性

//银行命令
#define DBO_GR_USER_INSURE_INFO		300									//银行资料
#define DBO_GR_USER_INSURE_SUCCESS	301									//银行成功
#define DBO_GR_USER_INSURE_FAILURE	302									//银行失败
#define DBO_GR_USER_INSURE_USER_INFO   303								//用户资料

//游戏事件
#define DBO_GR_PROPERTY_SUCCESS			400								//道具成功
#define DBO_GR_PROPERTY_FAILURE			401								//道具失败
#define DBO_GR_GAME_FRAME_RESULT		402								//游戏结果

//比赛事件
#define DBO_GR_MATCH_EVENT_START		500								//事件范围
#define DBO_GR_MATCH_RANK				501								//比赛名次
#define DBO_GR_MATCH_APPLY_ANDROID		503								//比赛申请机器人

#define	DBO_GR_MATCH_TAGOUT				512								//比赛挂牌
#define	DBO_GR_MATCH_BASE				513								//比赛配置
#define DBO_GR_MATCH_FEE				514								//
#define DBO_GR_MATCH_SOLUTION			515								//
#define DBO_GR_MATCH_JOIN				516								//比赛报名
#define DBO_GR_MATCH_QUIT				517								//比赛退赛
#define	DBO_GR_MATCH_RESULT				518								//获取奖励
#define	DBO_GR_MATCH_STAGE				519	
#define	DBO_GR_MATCH_DATE				520								//比赛时间
#define DBO_GR_MATCH_FINISH				521								//配置完成
#define	DBO_GR_MATCH_REVIVE				522								//比赛复活
#define DBO_GR_MATCH_REWARD				523								//比赛奖励
#define DBO_GR_MATCH_EXP_REWARD			524								//经验奖励

#define DBO_GR_MATCH_EVENT_END		    599								//事件范围

//系统事件
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600									//消息结果
#define DBO_GR_SENSITIVE_WORDS		    601									//敏感词
#define DBO_GR_LOAD_SCROLL_MESSAGE		602									//读取滚动信息
#define DBO_GR_GET_FIRST_REWARD_STATUS		603								//获取一局奖励状态

#define DBO_GR_SCROLL_MESSAGE			604									// 加载滚动消息


			

//机器人加载事件
#define DBO_GR_ANDRIOD_APPLY		700									//申请机器人id
#define DBO_GR_ANDRIOD_CURRENTVALID 701									//验证合法性
#define DBO_GR_ANDRIOD_REFRESH		702									//刷新时间戳
#define DBO_GR_ANDRIOD_RELEASE      703									//释放单个机器人
#define DBO_GR_ANDRIOD_ALL			704									//释放所以机器人
#define DBO_GR_ANDRIOD_ARRAY		705									//批量释放机器人
#define DBO_GR_ANDRIOD_RELEASE_DELTE	706								//释放单个机器人，并删除机器人

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_GR_LogonUserID
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//Token登录
struct DBR_GR_LogonTokenID
{
	//登录信息
	DWORD							dwMerchantID;						//商户ID
	DWORD							dwUserID;							//用户 I D
	TCHAR							szTokenID[LEN_MD5];				//登录密码

	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szClientIP[16];						//客户端ip
	BYTE                            cbDeviceType;                       //设备类型
};

//ID 登录
struct DBR_GR_LogonMobile
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//帐号登录
struct DBR_GR_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//游戏积分
struct DBR_GR_WriteGameScore
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址

	//用户信息
	DWORD							dwDBQuestID;						//请求标识
	DWORD							dwInoutIndex;						//进出索引

	WORD							wTableID;							//椅子ID
	//成绩变量
	tagVariationInfo				VariationInfo;						//提取信息
};


//离开房间
struct DBR_GR_LeaveGameServer
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwInoutIndex;						//记录索引
	DWORD							dwLeaveReason;						//离开原因
	DWORD							dwOnLineTimeCount;					//在线时长

	//成绩变量
	tagVariationInfo				RecordInfo;							//记录信息
	tagVariationInfo				VariationInfo;						//提取信息

	//系统信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//捕鱼游戏记录
struct DBR_GP_GameFishRecord
{
	//桌子信息
	WORD							wTableID;							//桌子号码
	//积分记录
	WORD							wRecordCount;						//记录数目
	tagFishRecordInfo				FishRecord[MAX_CHAIR];				//游戏记录
};

//游戏记录
struct DBR_GR_GameScoreRecord
{
	//桌子信息
	WORD							wTableID;							//桌子号码
	WORD							wUserCount;							//用户数目
	WORD							wAndroidCount;						//机器数目

	//损耗税收
	SCORE							lWasteCount;						//损耗数目
	SCORE							lRevenueCount;						//税收数目

	//统计信息
	DWORD							dwUserMemal;						//奖牌数目
	DWORD							dwPlayTimeCount;					//游戏时间

	//时间信息
	SYSTEMTIME						SystemTimeStart;					//开始时间
	SYSTEMTIME						SystemTimeConclude;					//结束时间

	//积分记录
	WORD							wRecordCount;						//记录数目
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//游戏记录
};

struct DBR_GP_UserSitUp
{
	DWORD							dwUserID;							//用户ID
	WORD							wKindID;							//游戏ID
	WORD							wServerID;							//房间ID
	WORD							wTableID;							//桌子ID
	WORD							wChairID;							//椅子ID
	SCORE							lScore;								//用户分数
	WORD							wInOutFlag;							//进出标记
};

//存入元宝
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入元宝
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出元宝
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取元宝
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账元宝
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账元宝
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//查询用户
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	DWORD							dwUserID;							//用户 I D
};

//道具请求
struct DBR_GR_PropertyRequest
{
	//购买信息
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//购买对象
	DWORD							dwTargetUserID;						//使用对象

	//消费模式
	BYTE                            cbRequestArea;			            //使用范围 
	BYTE							cbConsumeScore;						//积分消费
	SCORE							lFrozenedScore;						//冻结积分

	//用户权限
	DWORD                           dwUserRight;						//会员权限

	//系统信息
	WORD							wTableID;							//桌子号码
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//录像请求
struct DBR_GR_WriteVideo
{
	BYTE							szRoomName[31];						//房间名称
	BYTE							szFileName[100];						//文件名称
	BYTE							cbStreamContent[VIDEO_LEN];			//内容长度
};


//////////////////////////////////////////////////////////////////////////////////

//登录成功
struct DBO_GR_LogonSuccess
{
	//属性资料
	DWORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团 I D
	DWORD							dwCustomID;							//自定头像
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
	BYTE							cbIsAndroid;						

	//积分信息
	SCORE							lScore;								//用户分数
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
	DWORD							wAvatarID;							//形象ID

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//索引变量
	DWORD							dwInoutIndex;						//记录索引
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识

	//手机定义
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//辅助信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szDescribeString[128];				//错误消息
};

//登录失败
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//配置信息
struct DBO_GR_GameParameter
{
	//汇率信息
	WORD							wMedalRate;							//奖牌汇率
	WORD							wRevenueRate;						//银行税收

	//版本信息
	DWORD							dwClientVersion;					//客户版本
	DWORD							dwServerVersion;					//服务版本
};

//列表信息
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//列表信息
};

//机器信息
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//结果代码
	WORD							wAndroidCount;						//用户数目
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//机器信息
};

//道具信息
struct DBO_GR_GamePropertyInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具信息
};

//银行资料
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户元宝
	SCORE							lUserInsure;						//银行元宝
	SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来元宝
	SCORE							lSourceInsure;						//原来元宝
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lFrozenedScore;						//冻结积分
	SCORE							lVariationScore;					//元宝变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//用户资料
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwGameID;							//用户 I D
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
};

//道具成功
struct DBO_GR_S_PropertySuccess
{
	//道具信息
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//购买对象
	DWORD							dwTargetUserID;						//使用对象

	//消费模式
	BYTE                            cbRequestArea;						//请求范围
	BYTE							cbConsumeScore;						//积分消费
	SCORE							lFrozenedScore;						//冻结积分

	//会员权限
	DWORD                           dwUserRight;						//会员权限

	//结果信息
	SCORE							lConsumeGold;						//消费元宝
	LONG							lSendLoveLiness;					//赠送魅力
	LONG							lRecvLoveLiness;					//接受魅力

	//会员信息
	BYTE							cbMemberOrder;						//会员等级
};

//道具失败
struct DBO_GR_PropertyFailure
{
	BYTE                            cbRequestArea;						//请求范围
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//比赛名次
struct DBO_GR_MatchRank
{
	BYTE							cbRank;								//比赛名次
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	LONG							lMatchScore;						//用户得分
};

//权限管理
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//目标用户
	DWORD							dwAddRight;							//添加权限
	DWORD							dwRemoveRight;						//删除权限
	bool							bGameRight;							//游戏权限
};

//比赛报名
struct DBR_GR_UserMatchJoin
{
	DWORD							dwUserID;							//用户ID
	WORD							wServerID;							//房间ID
	DWORD							dwMatchID;							//比赛ID
	BYTE							cbQuitMatch;						//是否退赛
	DWORD							dwJoinType;
	DWORD							dwJoinFee;
	DWORD							dwJoinTypeTwo;
	DWORD							dwJoinFeeTwo;
	DWORD							dwJoinTypeThree;
	DWORD							dwJoinFeeThree;
	DWORD							dwJoinTypeFour;
	DWORD							dwJoinFeeFour;
	DWORD							dwJoinTypeFive;
	DWORD							dwJoinFeeFive;
};

struct tagRewardItem
{
	DWORD							dwRewardType;
	DWORD							dwCount;
};

//比赛奖励
struct DBR_GR_MatchReward
{
	DWORD								dwUserID;							//用户 I D
	WORD								wServerID;
	DWORD								dwMatchID;							//比赛 I D 
	DWORD								dwRank;								//比赛名次
	SCORE								lMatchScore;						//比赛得分
	tagRewardItem						Item[5];
};

//经验奖励
struct DBR_GR_MatchExpReward
{
	DWORD								dwUserID;							//用户 I D
	WORD								wServerID;
	DWORD								dwMatchID;							//比赛 I D 
	DWORD								dwRank;								//比赛名次
	DWORD								dwExpValue;
};

struct DBO_GR_MatchRewardProp
{
	LONG							lResultCode;						//结果代码
	WORD							wCount;								//用户数目
	tagMatchPropertyName			MatchPropertyName[32];
};

//系统消息
struct DBR_GR_SystemMessage
{
	BYTE                            cbMessageID;                        //消息ID
	BYTE							cbMessageType;						//消息类型
	BYTE                            cbAllRoom;                          //全体房间
	DWORD							dwTimeRate;						    //时间频率
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

// 查询黑白名单。
struct DBR_BlackOrWhite
{
	DWORD							UserID;								//消息ID
	char*							pBlackOrWhite;						//消息类型
};

//申请机器请求
struct DBR_GR_GameAndroidApply
{		
	WCHAR							guidRoom[40];						//房间guid
	DWORD							dwCount;							//申请数目	
	DWORD							dwRoomID;							//房间id
};

//机器信息
struct DBO_GR_GameAndroidApply
{
	LONG							lResultCode;						//结果代码
	WORD							wAndroidCount;						//用户数目
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//机器信息
};

//当前有效机器人请求
struct DBR_GR_GameAndriodCurrentValid
{
	WCHAR							guidRoom[40];						//房间id
};

//当前有效机器人
struct DBO_GR_GameAndriodCurrentValid
{
	LONG							lResultCode;						//结果代码
	WORD							wAndroidCount;						//用户数目
	DWORD							dwUserID[MAX_ANDROID];		//机器信息
};

//刷新时间戳请求
struct DBR_GR_GameAndriodRefresh
{
	WCHAR							guidRoom[40];						//房间id
};

//释放单个机器人
struct DBR_GR_GameAndriodRelease
{
	WCHAR							guidRoom[40];						//房间id
	DWORD							dwUserId;							//用户id	
};

struct DBR_GR_GameAndriodReleaseAndDelete
{
	WCHAR							guidRoom[40];						//房间id
	DWORD							dwUserId;							//用户id	
	WORD							wBingId;							//绑定id
};


//批量释放机器人
struct DBR_GR_GameAndriodArray
{
	TCHAR							szUserArray[8024];					//对应数据库的max，格式：id,...,id,id
	WCHAR							guidRoom[40];						//房间id
};
//释放全部机器人
struct DBR_GR_GameAndriodAll
{
	TCHAR							guidRoom[40];						//房间id
};

struct DBO_GR_GameAndriodRelease
{
	DWORD							dwUserId;							//机器人ID
	WORD							wBindId;							//绑定ID							
};


//加载任务
struct DBO_GR_GameTaskLoad
{
	WORD							wKindID;
	WORD							wServerID;
	DWORD							dwUserID;
};

//写入任务
struct DBO_GR_GameTaskWrite
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wTaskID;
	WORD							wCount;
	DWORD							dwUserID;
};
//提交任务
struct DBO_GR_GameTaskSubmit
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wTaskID;
	DWORD							dwUserID;
};

//任务成功
struct DBO_GR_GameTaskSuccess
{
	WORD							wCount;
	DWORD							dwUserID;
	tagUserTaskInfo				TaskSuccess[10];
};

//加载排行榜
struct DBO_GR_RankTopSuccess
{
	WORD							wCount;
	WORD							wTableID;
	tagRankTopInfo					RankInfo[6];
};

//加载道具
struct DBO_GR_GameItemLoad
{
	WORD							wKindID;
	WORD							wServerID;
	DWORD							dwUserID;
};

//加载道具
struct DBO_GR_GameItemSuccess
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wCount;
	DWORD							dwUserID;
	tagGameItemInfo					GameItemInfo[8];
};


////////////////////////////比赛定义//////////////////////////////////////////////
//报名结果
struct DBO_GR_UserMatchJoin
{
	WORD				wResultCode;
	TCHAR				szMsg[128];
};

//奖励加过
struct DBO_GR_MatchResult
{
	DWORD							dwRank;								//名次
};

//
struct tagRewardInfo
{
	DWORD							dwOrderID;
	DWORD							dwRankUpper;
	DWORD							dwRankLower;
	DWORD							dwRewardType;
	DWORD							dwRewardCount;
};

//比赛奖励
struct CMD_GR_RewardArray
{
	tagRewardInfo					Reward[50];
	DWORD							dwCount;
	DWORD							dwSolutionID;
};

//
struct tagExpReward
{
	DWORD							dwOrderID;
	DWORD							dwRankUpper;
	DWORD							dwRankLower;
	DWORD							dwExpValue;
};

//经验奖励
struct CMD_GR_ExpReward
{
	tagExpReward					Exp[5];
	DWORD							dwCount;
	DWORD							dwSolutionID;
};

//补血数据
struct	DBR_GR_BloodInfo
{
	WORD	wBloodOrder;			//补血索引
	WORD	wPayType;				//支付类型 12体力 1金币
	DWORD	dwBloodPrice;			//价格
	DWORD	dwBloodValue;			//获得血量

};

struct	DBR_GR_BloodArray
{
	WORD			 wCount;
	DBR_GR_BloodInfo  BloodInfo[4];
};

//对局宝箱
struct tagCardBoxInfo
{
	WORD		wBoxID;
	WORD		wCardCount;
	DWORD		dwRewardType;
	DWORD		dwRewardCount;
};

struct DBR_GR_CardBoxArray
{
	WORD				wCount;
	tagCardBoxInfo		BoxInfo[4];
};

// 好友信息
struct DBO_GP_ExpRank
{
	DWORD						dwUserID;					// 好友ID
	DWORD						wFaceID;					// 头像ID
	DWORD						dwExpValue;					// 经验
	TCHAR						szNickName[LEN_NICKNAME];	// 昵称
	TCHAR						szRemark[LEN_NICKNAME];		// 别名
};

//定时排名
struct tagTimeRank
{
	WORD						wRewardGroup;
	WORD						RankUpper;
	WORD						RankLower;
	DWORD						dwRewardType;
	DWORD						dwRewardCount;
};

struct DBR_GR_TimeRankArray
{
	WORD						wCount;
	tagTimeRank					Rank[15];
};

//排名比例
struct DBR_GR_RankRatio
{
	DWORD						dwRatioID;
	WORD						wInningsFirst;
	BYTE						cbRatioFirst;
	WORD						wInningsSecond;
	BYTE						cbRatioSecond;
	WORD						wInningsThird;
	BYTE						cbRatioThird;
	WORD						wInningsFour;
	BYTE						cbRatioFour;
};

struct DBR_GR_GetTimeRank
{
	WORD						wRank;
	DWORD						dwRankScore;
	TCHAR						szNickName[LEN_NICKNAME];
};

struct DBR_GR_GameHappyLand
{
	DWORD							dwUserID;
	BYTE							cbBanker;							//当地主
	BYTE							cbRobBanker;						//抢地主
	BYTE							cbFarmer;							//当农民
	BYTE							cbSpring;							//春天次数
	BYTE							cbRocket;							//火箭次数
};

struct DBR_GR_GameLand
{
	DWORD							dwUserID;
	BYTE							cbBanker;							//当地主
	BYTE							cbRobBanker;						//抢地主
	BYTE							cbFarmer;							//当农民
	BYTE							cbSpring;							//春天次数
	BYTE							cbRocket;							//火箭次数
};

struct DBR_GR_GameDoubleLand
{
	DWORD							dwUserID;
	BYTE							cbBanker;							//当地主
	BYTE							cbRobBanker;						//抢地主
	BYTE							cbFarmer;							//当农民
	BYTE							cbSpring;							//春天次数
	BYTE							cbRocket;							//火箭次数
};

//弹窗消息
struct DBO_GR_TipMessage
{
	TCHAR szSystemMessage[1024];
	SYSTEMTIME startTime;
	SYSTEMTIME endTime;
};

//弹窗消息
struct DBO_GR_ScrollMessage
{
	TCHAR szScrollMessage[256];
};

struct DBO_GR_LoadScrollMessage
{
	DWORD dwID;							// 消息ID
	DWORD dwMessageType;				// 消息类型 0 系统 1 游戏
	TCHAR szScrollMessage[1024];		// 消息内容
};


//////////////////////////////////////////////////////////////////////////////////
//czg属性刷新
struct DBO_UPDATE_ATTRIBUTE
{
	enum  attribute{
		enumAttr_error		=0,		//错误
		enumAttr_exp			=1,		//经验数值
		enumAttr_vip			=2,		//VIP成长值
		enumAttr_liness		=3,		//用户魅力
		enumAttr_order		=4,		//会员等级
		enumAttr_Medal		=5,		//用户奖牌
		enumAttr_gold			=6,		//用户金币
		enumAttr_Insure		=7,		//用户银行
		enumAttr_HonorID		=8,		//荣誉id 称号
		enumAttr_AvatarID		=9,		//形象id
		enumAttr_Power		=10,		//用户体力
		enumAttr_Jewel		=11,		//用户钻石
		enumAttr_lRevenue		=12,		//用户税收
		enumAttr_lLobbyLv		=13,		//大厅等级
		enumAttr_lMasterScore	=14,		//大师分
		enumAttr_lHonorScore	=15,		//战功分
		enumAttr_lRaceScore	=16,		//竞赛分
		enumAttr_lAchieveScore	=17,		//成就分
		enumAttr_FaceID		=18,		//头像id
		enumAttr_Gender		=19,		//性别

	};

	WORD wType;
	SCORE wScore;
};

//操作结果（eg:代码、用途、描述信息）
struct DBO_GP_OperateResult
{	
	WORD							wSubCommandID;						//消息用途
	DWORD							dwResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
};

#pragma pack()