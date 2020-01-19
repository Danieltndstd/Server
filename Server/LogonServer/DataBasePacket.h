#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//登录命令
#define	DBR_GP_LOGON_GAMEID			1									//I D 登录
#define	DBR_GP_LOGON_ACCOUNTS		2									//帐号登录
#define DBR_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define DBR_GP_COLLECTION			4									//收藏列表
#define DBR_GP_LOGON_ThirdParty		5									//三方登录
#define DBR_GP_REGISTER_THIRD		6									//三方注册
#define DBR_GP_LOGON_Third			7									//三方登录
#define DBR_GP_LOGON_OUT			8									//帐号登出
#define DBR_GP_WEB_REGISTER_ACCOUNTS	9								//Web注册

//账号服务
#define DBR_GP_MODIFY_MACHINE		10									//修改机器
#define DBR_GP_MODIFY_LOGON_PASS	11									//修改登录密码
#define DBR_GP_MODIFY_INSURE_PASS	12									//修改密码
#define DBR_GP_MODIFY_UNDER_WRITE	13									//修改签名
#define DBR_GP_MODIFY_NICKNAME		14									//修改昵称
#define DBR_GP_MODIFY_INDIVIDUAL	15									//修改资料
#define DBR_GP_BIND_VISITOR_ACCOUNT 16									//游客转正
#define DBR_GP_PW_RECOVERY_ID		17									//身份证ID找回密码
#define DBR_GP_PW_RECOVERY_MOBILE	18									//手机找回密码
#define DBR_GP_PW_RECOVERY			19									//找回密码
#define DBR_GP_BIND_MOBILE			20									//绑定手机
#define DBR_GP_GET_HONOR			21									//请求称号
#define DBR_GP_SET_HONOR			22									//设置称号

//头像命令
#define DBR_GP_GET_AVATAR_INFO		23									//获取头像
#define DBR_GP_GET_IMAGE_INFO		24									//获取形象
#define DBR_GP_MODIFY_AVATAR		25									//修改头像
#define DBR_GP_MODIFY_IMAGE			26									//修改形象

//银行命令
#define DBR_GP_USER_SAVE_SCORE		30									//存入元宝
#define DBR_GP_USER_TAKE_SCORE		31									//提取元宝
#define DBR_GP_USER_TRANSFER_SCORE	32									//转账元宝
#define DBR_GP_USER_CHANGE_SCORE	33									//改变元宝

//查询命令
#define DBR_GP_QUERY_INDIVIDUAL		40									//查询资料
#define DBR_GP_QUERY_INSURE_INFO	41									//查询银行
#define DBR_GP_QUERY_USER_INFO	    42									//查询用户

//系统命令
#define DBR_GP_LOAD_GAME_LIST		50									//加载列表
#define DBR_GP_ONLINE_COUNT_INFO	51									//在线信息
#define DBR_GP_BROADCASE_MSG		52									//广播消息
#define	DBR_GP_BROADCAST_TYPHON		53									//喇叭消息
#define DBR_GP_GET_RANK				54									//排行榜数据
#define DBR_GP_CONTROL_ROOM_STATUS	55									//打开房间

#define DBR_GP_TODAY_IS_SIGNIN      59								     //今天是否已经签到
#define DBR_GP_GET_SIGNIN			60									//获取签到
#define DBR_GP_SET_SIGNIN			61									//签到
#define DBR_GP_USER_DAILYGET		62									//低保领取
#define DBR_GP_REQUEST_MAIL			63									//请求邮件
#define DBR_GP_READ_MAIL			64									//读取邮件
#define DBR_GP_DELETE_MAIL			65									//删除邮件

// 转盘
#define DBR_GP_GETDAILINFO			66									// 获取转盘信息
#define DBR_GP_ROTATEDAILAWARD		67									// 转到转盘获得奖励


#define DBR_GP_GET_STORE			70									//获取商城道具
#define DBR_GP_GET_AUCTION			71									//拍卖商城
#define	DBR_GP_BUY_STORE_ITEM		72									//购买商城道具
#define DBR_GP_BUY_AUCTION_ITEM		73									//购买拍卖道具
#define	DBR_GP_ONSALE_AUCTION_ITEM	74									//上架/下架拍卖道具
#define DBR_GP_USE_ITEM				75									//使用道具
#define DBR_GP_GET_PACK				76									//背包获取
#define DBR_GP_GET_STOREINFO		77									//获取商城道具信息（带实时库存）
#define DBR_GP_GET_ITEMINFO			78									//获取道具信息
#define DBR_GP_GET_SELF_AUCTION		79									//拍卖商城
#define DBR_GP_OFFSALE_AUCTION_ITEM 80
#define DBR_GP_EXTRACT_SCORE		81
#define DBR_GP_RESHELF_AUCTION_ITEM 82									//重新上架
#define DBR_GP_GRANT_ITEM			83									//赠与道具

//好友服务
#define DBR_GP_REQUEST_FRIEND		84									//申请好友
#define DBR_GP_REQUEST_REPLY		85									//好友申请（同意/拒绝）
#define DBR_GP_REQUEST_REPLYS		86									//好友申请（全部同意/拒绝）
#define DBR_GP_DELETE_FRIEND		87									//删除好友
#define DBR_GP_GET_FRIEND			88									//获取好友
#define DBR_GP_GET_REQUEST			89									//获取申请
#define DBR_GP_SEARCH_USER			90									//搜索用户

#define DBR_GP_GET_FAMILY			91									//获取家族
#define DBR_GP_SET_FAMILY			92									//加入家族
#define DBR_GP_QUIT_FAMILY			93									//退出家族
#define DBR_GP_SEND_RED				94									//大厅发红包
#define DBR_GP_GARP_RED				95									//抢红包
#define DBR_GP_BUY_POWER			96									//刷新体力
#define DBR_GP_GET_EXP				97									//获取经验
#define DBR_GP_GET_SYSEXP			98									//获取经验系统
#define DBR_GP_GET_TASK				99									//获取任务
#define DBR_GP_REFRESH_TASK			100									//刷新任务
#define DBR_GP_TAKE_TASK			101									//领取任务奖励
#define DBR_GP_GET_USERACHIEVE		102									//获取成就配置
#define DBR_GP_GET_USERWAGES		103									//领取成就工资
#define DBR_GP_INSERT_USERTASK		104									//插入玩家任务
#define DBR_GP_INSERT_USERACHIEVE	105									//插入玩家成就
#define DBR_GP_INSERT_USERFAME		106									//插入玩家荣誉
#define DBR_GP_GRANT_GOODS			107									//好友赠送体力
#define DBR_GP_ALTER_REMARK			108									//修改好友备注
#define DBR_GP_MATCH_DESCRIPTION	109									//获取比赛描述
#define DBR_GP_MATCH_RULE			110									//获取比赛规则
#define DBR_GP_MATCH_REWARD			111									//获取比赛奖励
#define DBR_GP_RECOMMENDFRIEND		112									//获取推荐好友
#define DBR_GP_GET_USERFAME			113									//获取荣誉配置

#define DBR_GP_BUY_VIP				114									//购买VIP
#define DBR_GP_MODIFY_SECOND_PASS	115									//设置二级密码
#define DBR_GP_GET_RED_INFO			116									//获取红包规则数据
#define DBR_GP_SET_LEAVE_MESSAGE    117									//设置排行榜留言
#define DBR_GP_CHECK_SECOND_PASS	118									//

//聚宝盆
#define DBR_GP_GET_USER_GEM			119									//获取宝石背包
#define DBR_GP_GET_USER_ORNAMENT    120									//获取饰品背包
#define DBR_GP_BURNISH_GEM				121								//打磨宝石
#define DBR_GP_CARRY_GEM				122								//携带宝石
#define DBR_GP_INPUT_GEM				123								//种植宝石
#define	DBR_GP_WEAR_ORNAMENT			124								//穿戴饰品
#define DBR_GP_GET_GEM_FRIENDS			125								//获取宝石好友

#define DBR_GP_RETRIEVE_PASSWORD        126								//找回密码（包含验证账号，验证手机）

#define DBR_GP_GET_PERSONAL_INFO		127								//获取个人信息（游戏内信息，昵称，等级，vip，签名，称号，技能，大厅等级，普通斗等级，欢乐斗等级....）

#define DBR_GP_RECOMMENDFRIEND2			130									//获取推荐好友
#define DBR_GP_FEEDBACK_SYSTEM			131								//反馈信息系统
#define DBR_GP_PUSH_RED					132								//获取存在的红包
#define DBR_GP_GET_DELETE_RED			133								//删除红包

#define DBR_GP_RELOAD_SCORE				135								//czg刷新积分

//房间基本信息
#define DBR_GP_GET_TIME_RANK_INFO		136								//房间定时排名奖励信息
#define DBR_GP_GET_ROOM_SUMMARIZE		137								//获取房间概述
#define DBR_GP_GET_USER_RESULTS			138								//获取用户的战绩

#define DBR_GP_GET_GAME_RECORDS         139								//获取用户游戏记录
#define DBR_GP_GET_MATCH_APPLY_STATUS   140								//获取比赛报名状态

#define DBR_GP_INPUT_INVITE_CODE		141								//输入邀请码（接收别人邀请）
#define DBR_GP_GET_TODATY_SENDRED       142								//获取今天发的红包
#define DBR_GP_GET_RECEIVE_RED_INFO		143								//获取抢红包详情

#define DBR_GP_GET_INVITE_CODE_REWARD_INFO		144							//加载邀请码奖励规则
#define DBR_GP_OBTAIN_INVITE_CODE_REWARD		145							//领取邀请码奖励
#define DBR_GP_GET_FEEDBACK_REWARD_STATUE		146							//获取某一个邀请码回馈奖励的状态
#define DBR_GP_GET_USER_INVITE_CODE_INFO		147							//获取用户邀请码详细信息

#define DBR_GP_LOAD_SPREAD_INFO 148											//读取推广奖励配置
#define DBR_GP_DISTANCE_REWARD_TIME 149										//距离下次奖励时间
#define DBR_GP_CALCULATION_SPREAD_REWARD 150								//计算推广奖励

#define DBR_GP_CACHE_RANK 151												//缓存排行榜

#define DBR_GP_GET_USER_PERFECT_INFORMATION   152							//获取用户完善信息
#define DBR_GP_SET_USER_PERFECT_INFORMATION   153							//设置用户完善信息

#define DBR_GP_IS_SPREAD_REGISTER	  154										//推广注册
#define DBR_GP_BIND_USER_SPREAD		155											//绑定用户推广
#define DBR_GP_IS_BIND_SPREAD		156											//是否已经绑定推广
#define DBR_GP_IS_ALREADY_MODIFY_NICKNAME 157								//是否已经修改过昵称
#define DBR_GP_IS_TODAY_FIRST_TIME_LOGIN 158								//是否今天第一次登录
#define DBR_GP_CLIENT_LOGIN_SUCCESS 159								//客户端登录完成

#define DBR_GP_KILL_LOCK	160										//删除卡线
#define DBR_GR_LOAD_SCROLL_MESSAGE		161							//读取滚动消息
#define DBR_GR_CLEAR_SCROLL_MESSAGE     162							//清除滚动记录


#define DBR_GR_CHICKBINDBANK			165							// 查询绑定银行卡
#define DBR_GR_BINDBANK					166							// 绑定银行卡
#define DBR_GR_CHICKBINDZFB				167							// 查询绑定支付宝
#define DBR_GR_BINDZFB					168							// 绑定支付宝

#define DBR_GR_REQUEST_MAIL				170									//请求邮件
#define DBR_GR_READ_DELETE_MAIL			171									//读取删除邮件

//////////////////////////////////////////////////////////////////////////////////

//客户端相关信息
struct DBR_GP_ClientInfo
{
	unsigned long                       dwKindID;                   //类型标识
	unsigned long						dwVersion;					//版本
};

//ID 登录
struct DBR_GP_LogonGameID
{
	//登录信息
	DWORD							dwGameID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_GP_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_GP_LogonThird
{
	//登录信息
	DWORD							dwUserID;							//用户ID
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//账号注销
struct DBR_GP_LogonOut
{
	DWORD							dwUserID;							//用户ID
	DWORD							dwInOutIndex;						//索引
};

struct DBR_GP_LogonThirdParty
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
};

//Web 帐号注册
struct DBR_GP_Web_RegisterAccounts
{
	//注册信息
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	//附加信息
	DWORD							dwClientAddr;						//连接地址
	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_GP_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//第三方帐号注册
struct DBR_GP_RegisterThird
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数

	//第三方账号
	TCHAR							szThirdParty[LEN_ACCOUNTS];			//第三方登录帐号
};

//记录第三方数据表信息
struct DBR_GP_Insert_Accounts
{
	TCHAR							szThirdAccounts[LEN_ACCOUNTS];			//三方帐号
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
};

//修改机器
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改密码
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户账号
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码	
};

//修改密码
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//手机找回密码
struct DBR_GP_PWRecoveryMobile
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			// 用户账号
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	// 预留手机号
};

//绑定手机
struct DBR_GP_BindMobile
{
	DWORD							dwUserID;							// 用户ID
	TCHAR							szMobliePhone[LEN_MOBILE_PHONE];	// 绑定手机号
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//修改资料
struct DBR_GP_ModifyIndividual
{
	//验证资料
	DWORD							dwUserID;							//用户 I D
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实姓名
	TCHAR							szPassportID[LEN_PASS_PORT_ID];		//身份证 I D
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//预留手机号
	TCHAR							szPassword[LEN_MD5];				//密码
};

//存入元宝
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入元宝
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出元宝
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取元宝
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账元宝
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账元宝
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//改变元宝
struct DBR_GP_CHANGE_SCORE
{
	DWORD							dwUserID;							//用户 I D
	SCORE							changeScore;						//用户金币
	SCORE							changeInsureScore;					//银行金币
};

//用户资料
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//用户 I D
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
};

//称号信息
struct DBO_GP_HonorInfo
{
	DWORD							dwHonorID;							//称号ID
	DWORD							dwHonorType;						//称号类型
};

//头像/形象信息
struct DBO_GP_AvatarInfo
{
	BYTE							cbAvatarType;						//头像/形象
	DWORD							dwAvatarID;							//ID
	DWORD							dwRemainTime;						//剩余时间（最小单位：分）
};

//在线信息
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//类型数目
	DWORD							dwOnLineCountSum;					//总在线数
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//类型在线
};

//广播消息
struct DBR_GP_OnBroadcastMsg
{
	TCHAR							szNickName[32];
	TCHAR							szKindName[32];
	TCHAR							szScore[32];
};

//游客转正
struct DBR_GP_BindVisitorAccount
{
	DWORD							dwUserID;
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szMachineID[LEN_MACHINE_ID];			//机器ID
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};

// 获取转盘
struct CMD_GP_GetDailInfo
{
	DWORD							dwUserID;
	DWORD							dwClientAddr;						//连接地址
};

// 转动转盘领取奖励
struct CMD_GP_RotateDailAward
{
	DWORD							dwUserID;							// 用户ID
	DWORD							dwDailType;							// 转盘类型
	DWORD							dwClientAddr;						//连接地址
};

// 查询绑定银行卡
struct CMD_GP_ChickBindBank
{
	DWORD							dwClientAddr;						//连接地址
	DWORD							wSubCommandID;						//消息ID
	DWORD							dwUserID;							// 查询用户
};

// 绑定银行卡
struct CMD_GP_BindBankCard
{
	DWORD							dwClientAddr;						//连接地址
	DWORD							wSubCommandID;						//消息ID
	DWORD							dwUserID;							// 查询用户
	TCHAR							szBankName[32];							// 银行名字
	TCHAR							szBankCardNum[32];						// 银行卡号
	TCHAR							szAccountHolder[32];					// 开户人
};

// 查询绑定支付宝
struct CMD_GP_ChickBindZFB
{
	DWORD							dwClientAddr;						//连接地址
	DWORD							wSubCommandID;						//消息ID
	DWORD							dwUserID;								// 查询用户
};

// 绑定支付宝
struct CMD_GP_BindZFB
{
	DWORD							dwClientAddr;						//连接地址
	DWORD							wSubCommandID;						//消息ID
	DWORD							dwUserID;							// 查询用户
	TCHAR							szZFBName[32];							// 支付宝账号
	TCHAR							szNickName[32];							// 用户名字
};

// 请求邮件
struct CMD_GP_RequestMail
{
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwUserID;							// 查询用户
};

// 读取删除邮件
struct CMD_GP_ReadDeleteMail
{
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwUserID;							// 查询用户

	DWORD							dwOpenType;							// 操作标识 0 读取 1 删除
	DWORD							dwMailIndex;						// 邮件标识
};

//////////////////////////////////////////////////////////////////////////////////
//输出信息

//登录结果
#define DBO_GP_LOGON_SUCCESS		100									//登录成功
#define DBO_GP_LOGON_FAILURE		101									//登录失败
#define DBR_GP_VALIDATE_MBCARD		102									//登录失败
#define DBO_GP_THIRD_REGISTER		103									//三方注册
#define DBO_GP_THIRD_LOGON			104									//三方登录
#define DBO_GP_WEB_LOGON_SUCCESS	105									//Web登录成功

//操作结果
#define DBO_GP_USER_INDIVIDUAL		110									//用户资料

//银行命令
#define DBO_GP_USER_INSURE_INFO		120									//银行资料
#define DBO_GP_USER_INSURE_SUCCESS	121									//银行成功
#define DBO_GP_USER_INSURE_FAILURE	122									//银行失败
#define DBO_GP_USER_INSURE_USER_INFO   123								//用户资料

//列表结果
#define DBO_GP_GAME_TYPE_ITEM		130									//种类信息
#define DBO_GP_GAME_KIND_ITEM		131									//类型信息
#define DBO_GP_GAME_NODE_ITEM		132									//节点信息
#define DBO_GP_GAME_PAGE_ITEM		133									//定制信息
#define DBO_GP_GAME_LIST_RESULT		134									//加载结果
#define DBO_GP_GAME_COLLECTION		135								    //收藏信息
#define DBO_GP_BROADCAST_MSG		139									//广播消息
#define DBO_GP_BROADCAST_TYPHON		145									//喇叭消息

//用户服务
#define DBO_GP_GET_HONOR			150									//称号信息
#define DBO_GP_AVATAR_INFO			151									//头像
#define DBO_GP_IMAGE_INFO			152									//形象

//服务结果
#define DBO_GP_OPERATE_RESULT		500									//操作结果

//查看用户签到
#define DBO_GP_GET_SIGNIN			700									//获得用户签到
#define DBO_GP_SET_SIGNIN			701									//设置用户签到
#define DBO_GP_TODAY_IS_SIGNIN      702									//获取今天是否签到

#define DBO_GP_GET_DAIL_INFO		705									//获取转盘信息
#define DBO_GP_ROTATEDAILAWARD		706									// 用户抽奖信息

#define DBR_GP_CHICKBINDBANK		710									// 绑定银行卡
#define DBR_GP_CHICKBINDZFB			712									// 绑定支付宝

#define DBR_GP_REQUESTMAIL			713									// 请求邮件
#define DBR_GP_READDELEETETAIL		714									// 读取删除邮件

#define DBO_GP_USER_DAILYGET		200									//低保结果
#define DBO_GP_MAIL					201									//邮件数据
#define DBO_GP_MAILDETAIL			202									//邮件详细

#define DBO_GP_GET_PACK				204									//背包结果
#define DBO_GP_GET_STORE			205									//普通道具商城
#define DBO_GP_GET_AUCTION			206									//拍卖商城
#define DBO_GP_GET_SELF_AUCTION		207									//拍卖商城

//好友服务
#define DBO_GP_GET_FRIEND			210									//获取好友
#define DBO_GP_GET_REQUEST			211									//好友申请
#define DBO_GP_SEARCH_USER			212									//搜寻用户
#define DBO_GP_REQUEST_NOTICE		213									//申请通知对方
#define DBO_GP_AGREE_REQUEST		214									//同意申请通知对方
#define DBO_GP_DEL_FRIEND_NOTICE	215									//删除好友通知
#define DBO_GP_REFUSE_NOTICE		216									//拒绝申请通知对方

#define DBO_GP_GET_RANK_GOLD		220									//金币排行
#define DBO_GP_GET_RANK_ACHIEVE		221									//成就排行
#define DBO_GP_GET_RANK_MASTER		222									//大师分排行
#define DBO_GP_LOBBY_SENDRED		223									//大厅发红包
#define DBO_GP_GRAPRED				224									//抢红包结果
#define DBO_GP_BUY_POWER			225									//刷新体力
#define DBO_GP_GETSTOREINFO			226									//商城信息（实时库存
#define DBO_GP_GETSTOREINFO_SCORE	198									//商城信息（实时库存）
#define DBO_GP_GETSTOREINFO_JEWEL	199									//商城信息（实时库存）
#define DBO_GP_GETITEMINFO			227									//单个道具信息
#define DBO_GP_GETUSEREXP			228									//获取经验信息
#define DBO_GP_GETUSERTASK			229									//获取用户任务
#define DBO_GP_GETACHIEVEINFO		230									//整体成就信息
#define DBO_GP_GETSPECIFICACHIEVE	231									//具体成就信息	
#define DBO_GP_GRANT_POWER			232									//赠送体力结果
#define DBO_GP_DEL_FRIEND			233									//删除好友
#define DBO_GP_GET_MATCHDES			234									//获取比赛概述
#define DBO_GP_GET_MATCHRULE		235									//获取比赛规则
#define DBO_GP_GET_MATCHSOLUTION	236									//获取比赛解决方案
#define DBO_GP_GET_MATCHREWARD		237									//获取比赛奖励
#define DBO_GP_GET_MATCHINFOEND		238									//获取比赛信息结束
#define DBO_GP_RECOMMENDFRIEND		239									//推荐好友
#define DBO_GP_GETFAMEINFO			240									//整体荣誉信息
#define DBO_GP_GETSPECIFICFAME		241									//具体荣誉信息	
#define DBO_GP_RECOMMENDFRIEND2		242									//推荐好友

#define DBO_GP_GET_RANK				243									//排行榜
#define DBO_GP_SET_LEAVE_MESSAGE    244									//设置排行榜留言
#define DBO_GP_GRANT_SCORE			245									//赠送好友金币
#define DBO_GP_GET_RED_INFO			246									//获取红包规则数据

#define DBO_GP_GET_USER_GEM			247									//获取宝石背包
#define DBO_GP_GET_USER_ORNAMENT    248									//获取饰品背包
#define DBO_GP_BURNISH_GEM				249								//打磨宝石
#define DBO_GP_CARRY_GEM				250								//携带宝石
#define DBO_GP_INPUT_GEM				251									//种植宝石
#define	DBO_GP_WEAR_ORNAMENT			252									//穿戴饰品
#define DBO_GP_GET_GEM_FRIENDS			253								//获取宝石好友

#define DBO_GP_RETRIEVE_PASSWORD        254								//找回密码（包含验证账号，验证手机）
#define DBO_GP_GET_PERSONAL_INFO		255								//获取个人信息（游戏内信息，昵称，等级，vip，签名，称号，技能，大厅等级，普通斗等级，欢乐斗等级....）

			
#define DBO_GP_UPDATE_ATTRIBUTE			256								//czg更新属性
#define DBO_GP_PUSH_RED				257									//获取存在的红包

#define DBO_GP_MODIFY_AVATAR		258									//修改头像
#define DBO_GP_MODIFY_IMAGE			259									//修改形象
#define DBO_GP_MODIFY_HONOR			260									//修改称号

#define DBO_GP_RELOAD_SCORE		261									//更新积分


#define DBO_GP_UPDATE_ITEM			262									//更新一个道具
#define DBO_GP_DELETE_ITEM			263									//删除一个道具

//房间基本信息
#define DBO_GP_GET_TIME_RANK_INFO	264									//获取定时排名详细信息
#define DBO_GP_GET_ROOM_SUMMARIZE	265									//获取房间概述	
#define DBO_GP_GET_USER_RESULTS		266									//获取用户战绩

#define DBO_GP_GET_GAME_RECORDS		267									//获取用户游戏记录

#define DBO_GP_GET_MATCH_APPLY_STATUS	277								//获取比赛报名状态
#define DBO_GP_GET_TODATY_SENDRED		278								//获取今天的发送的红包
#define DBO_GP_GET_RECEIVE_RED_INFO		279								//获取抢红包详情

#define DBO_GP_GET_USER_INVITE_CODE_INFO     280							//获取用户邀请码信息
#define DBO_GP_GET_USER_INVITE_CODE_REWARD_INFO     281					//获取邀请码奖励信息
#define DBO_GP_GET_FEEDBACK_REWARD_STATUS     282						//获取回馈奖励的状态

#define DBO_GR_LOAD_SPREAD_INFO		283									//读取推广奖励配置

#define DBO_GP_GET_USER_PERFECT_INFORMATION   284							//获取用户完善信息

#define DBO_GP_IS_SPREAD_REGISTER				285									//是否推广注册
#define DBO_GP_IS_BIND_SPREAD					286									//是否已经绑定推广
#define DBO_GP_IS_ALREADY_MODIFY_NICKNAME					287									//是否已经修改过昵称
#define DBO_GP_BIND_USER_SPREAD								288									//绑定用户推广
#define DBO_GP_IS_TODAY_FIRST_TIME_LOGIN					289								//是否是今天第一次登录
#define DBO_GP_IS_LOST_CONNECTION_LOGIN					290								//是否是掉线重新登录
#define DBO_GR_LOAD_SCROLL_MESSAGE					291								//读取滚动消息

//登录成功
struct DBO_GP_LogonSuccess
{
	//属性资料
	DWORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定索引
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//拓展信息
	TCHAR							szCompellation[16];				//真实姓名
	TCHAR							szQQ[16];								//QQ
	TCHAR							szEMail[32];							//电子邮件
	TCHAR							szSeatPhone[32];					//固定电话
	TCHAR							szMobilePhone[16];				//手机号码
	TCHAR							szDwellingPlace[128];			//联系地址
	TCHAR							szUserNote[256];					//用户备注

	//用户成绩
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;							//用户银行

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//会员资料
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
	SYSTEMTIME						LastLogonDate;						//最近登录

	TCHAR							szLastClientAddr[16];
	TCHAR							szClientAddr[16];
	//描述信息
	TCHAR							szDescribeString[128];				//描述消息
};

//第三方登录成功
struct DBO_GP_ThirdPartySuccess
{
	//登录信息
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//登录失败
struct DBR_GP_ValidateMBCard
{
	UINT							uMBCardID;						//机器序列
};

//头像信息
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwCustomID;							//自定索引
};

//头像信息
struct DBO_GP_UserAvatarInfo
{
	WORD							wAvatarID;
	SYSTEMTIME						sysLimit;
};

//形像信息
struct DBO_GP_UserImageInfo
{
	WORD							wImageID;
	SYSTEMTIME						sysLimit;
};

//个人资料
struct DBO_GP_UserIndividual
{
	//用户信息
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szPassportID[LEN_PASS_PORT_ID];		//身份证
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//预留手机号
	TCHAR							szEmail[LEN_EMAIL];					//预留邮箱
};

//银行资料
struct DBO_GP_UserInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户元宝
	SCORE							lUserInsure;						//银行元宝
	SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来元宝
	SCORE							lSourceInsure;						//原来元宝
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lVariationScore;					//元宝变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//操作结果（eg:代码、用途、描述信息）
struct DBO_GP_OperateResult
{	
	WORD							wSubCommandID;						//消息用途
	DWORD							dwResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
};

//邮件数据
struct DBO_GP_Mail
{
	BYTE							cbMailType;					//邮件类型，0系统 非0好友
	BYTE							cbOutDate;					//是否过期
	BYTE							cbRead;						//是否已读
	WORD							wLimitType;					//邮件期限
	DWORD							dwMailID;					//邮件ID
	BYTE							cbContentType;				//内容类型
	BYTE							cbRewardCount;				//奖励个数
	DWORD							dwSendTimeDiff;				//时间间隔

	BYTE							cbTitleSize;
	BYTE							cbNickNameSize;
	BYTE							cbSendTimeSize;

	/*TCHAR							szTitle[LEN_NICKNAME];		// 邮件标题
	TCHAR							szNickName[LEN_NICKNAME];	// 邮件发送者
	TCHAR							szSendTime[25];				// 发送时间*/

	char							szExtra[0];
};

//邮件详情
struct DBO_GP_MailDetail
{		
	DWORD							dwItemID;					//奖励道具ID 共6个
	WORD							wItemCount;					//道具数量
	DWORD							dwSecondItemID;
	WORD							wSecondItemCount;
	DWORD							dwThirdItemID;
	WORD							wThirdItemCount;
	DWORD							dwFourItemID;
	WORD							wFourItemCount;
	DWORD							dwFiveItemID;
	WORD							wFiveItemCount;
	DWORD							dwSixItemID;
	WORD							wSixItemCount;
	WORD							wPower;						
	WORD							wLoveLiness;

	TCHAR							szContent[80];				// 发送内容*/
};
//////////////////////////////////////////////////////////////////////////////////

//游戏类型
struct DBO_GP_GameType
{
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wTypeID;							//类型索引
	TCHAR							szTypeName[LEN_TYPE];				//种类名字
	
};

//游戏种类
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//类型索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wGameID;							//模块索引
	TCHAR							szKindName[LEN_KIND];				//游戏名字
	TCHAR							szProcessName[LEN_PROCESS];			//进程名字
	TCHAR							szDownLoadUrl[100];		//下载地址
};

//游戏节点
struct DBO_GP_GameNode
{
	WORD							wKindID;							//名称索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wNodeID;							//节点索引
	TCHAR							szNodeName[LEN_NODE];				//节点名称
};

//定制类型
struct DBO_GP_GamePage
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wPageID;							//定制索引
	WORD							wOperateType;						//控制类型
	TCHAR							szDisplayName[LEN_PAGE];			//显示名称
};

//收藏类型
struct DBO_GP_GameCollection
{
	WORD							wUserID; //用户索引
	WORD							wSortID;//排序索引
	TCHAR							szDisplayName[LEN_KIND]; //游戏名称
	WORD							wResult;
	WORD							wFlag;
	WORD							wCount;
	tagGameCollection				Collection[MAX_KIND];
};

struct DBO_GP_BroadcastMsg
{
	tagBroadcastMsg					ArrayBroadcastMsg[10];
	WORD							wCount;
};

struct DBO_GP_BroadcastTyphon
{
	BYTE							cbType;								
	BYTE							cbSuccess;
	DWORD							dwUserID;
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szContent[128];
};

//加载结果
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//成功标志
};

#define SIGNCOUNT 256
#define SIGNLEN	  10

//低保结果
struct DBO_GP_UserDailyGet
{
	BYTE							cbSuccess;
	TCHAR							szMessage[128];
};


// 用户信息
struct DBO_GP_UserInfo
{
	DWORD						dwUserID;					// ID
	DWORD						wFaceID;					// 头像ID
	DWORD						wLobbyLV;					// 大厅等级
	BYTE						cbOnLine;					// 是否在线
	TCHAR						szNickName[LEN_NICKNAME];	// 昵称
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//手机数据包

//登录命令
#define	DBR_MB_LOGON_GAMEID			500									//I D 登录
#define	DBR_MB_LOGON_ACCOUNTS		501									//帐号登录
#define DBR_MB_REGISTER_ACCOUNTS	502									//注册帐号
#define DBR_MB_REGISTER_MOBILE		503									//手机注册
#define DBR_MB_VISITOR_ACCOUNTS		504									//游客登录

//登录结果
#define DBO_MB_LOGON_SUCCESS		600									//登录成功
#define DBO_MB_LOGON_FAILURE		601									//登录失败

//登录命令
#define	DBR_MB_LOGON_GAMEID_BCode			550									//I D 登录
#define	DBR_MB_LOGON_ACCOUNTS_BCode			551									//帐号登录
#define DBR_MB_REGISTER_ACCOUNTS_BCode		552									//注册帐号
#define DBR_MB_REGISTER_MOBILE_BCode		553									//手机注册
#define DBR_MB_VISITOR_ACCOUNTS_BCode		554									//游客登录

//登录结果
#define DBO_MB_LOGON_SUCCESS_BCode			650									//登录成功
#define DBO_MB_LOGON_FAILURE_BCode			651									//登录失败

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_MB_LogonGameID
{
	//登录信息
	DWORD							dwGameID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_MB_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数

	BYTE							cbDeviceType;                       //设备类型

	DWORD							dwInviteID;							// 邀请码
	DWORD							dwSortID;							// 分类码

	DWORD							dwLogonType;						// 登录类型 0 登录 1 注册 2 不区分
};

//帐号注册
struct DBR_MB_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_MB_RegisterAccounts_BCode
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数

	DWORD							dwSpreaderID;							//推广码

};

//手机注册
struct DBR_MB_RegisterMobile
{
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMobile[LEN_MOBILE_PHONE];			//手机号码
	TCHAR							szLogonPass[LEN_PASSWORD];			//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	LPVOID							pBindParameter;						//绑定参数
};

//游客帐号
struct DBR_MB_VisitorAccounts
{
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szLogonPass[LEN_MD5];				//登录密码

	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szSpreader[LEN_NICKNAME];			//推广员账号

	LPVOID							pBindParameter;						//绑定参数

	DWORD							dwInviteID;							// 邀请码
	DWORD							dwSortID;							// 分类码

	DWORD							dwLogonType;						// 登录类型 0 登录 1 注册 2 不区分
};

//登录成功
struct DBO_MB_LogonSuccess
{
	//用户属性
	DWORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	DWORD							dwVipGrowthValue;					//VIP成长值
	DWORD							dwLoveLiness;						//用户魅力
	BYTE							cbMemberOrder;						//会员等级

	//用户成绩
	DWORD							dwUserMedal;						//用户奖牌
	SCORE							lUserScore;							//用户元宝
	SCORE							lUserInsure;						//用户银行

	//扩展信息
	DWORD							dwInoutIndex;						//记录索引
	DWORD							dwHonorID;							//荣誉ID
	DWORD							wAvatarID;							//形象ID
	WORD							wPackGridCount;						//背包格子数
	WORD							wMarketGridCount;					//市场格子数

	LONG							lPower;								//用户体力
	LONG							lTimeDiff;							//时间差（分）
	LONG							lJewel;								//用户钻石
	SCORE							lRevenue;							//用户税收

	SCORE							lLobbyLV;							//大厅经验

	SCORE							lMasterScore;						//大师分
	SCORE							lHonorScore;						//战功分
	SCORE							lRaceScore;							//竞赛分
	SCORE							lAchieveScore;						//成就分

	BYTE							cbVisitorClient;					//游客标识 0否 1是
	BYTE							cbSecondPass;						//二级密码标识，0没有，1有

	//描述信息
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//登录成功
struct DBO_MB_LogonSuccess_BCode
{
	//用户属性
	DWORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	BYTE							cbMemberOrder;						//会员等级

	//用户成绩
	DWORD							dwUserMedal;						//用户奖牌
	SCORE							lUserScore;							//用户元宝
	SCORE							lUserInsure;						//用户银行

	//扩展信息
	DWORD							dwInoutIndex;						//记录索引

	SCORE							lRevenue;							//用户税收
	SCORE							lLobbyLV;							//大厅经验

	BYTE							cbVisitorClient;					//游客标识 0否 1是

	//描述信息
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	DWORD							dwServerID;							//房间ID，0不是断线重连
};



//////////////////////////////////////////////////////////////////////////////////

//获取个人信息（游戏内信息，昵称，等级，vip，签名，称号，技能....）
struct DBO_GP_GetPersonalInfo
{
	DWORD							dwUserID;							//用户ID
	DWORD							dwFaceID;							//头像 I D
	BYTE							cbVIP;								// VIP
	DWORD							dwHonorID;							//称号ID

	DWORD							dwLobbyLv;							//大厅等级
	DWORD							dwAchieveScore;						//成就
	DWORD							dwRaceScore;						//竞赛分
	DWORD							dwZhanGong;							//战功
	DWORD							dwMasterScore;						//大师分
	DWORD							dwAvatarID;							//形象ID

	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//定时排名
struct DBO_GP_GetTimeRankInfo
{
	WORD						wRankUpper;					//起始名次
	WORD						wRankLower;					//结束名次
	DWORD						dwRewardType;				//奖励
	DWORD						dwRewardCount;				//奖励数量
};

//房间概述
struct DBO_GP_GetRoomSummarize
{
	WORD						wMinPower;				//入场体力需求
	DWORD						dwMinEnterScore;		//入场费用
	DWORD						dwObtainBlood;			//获得血量

	DWORD						dwGameBase;				//游戏基数

	TCHAR						szRoomName[LEN_SERVER];	//房间名称
	TCHAR						szBoxIntroduce[128];    //对局宝箱描述
};

//获取用户战绩
struct DBO_GP_GetUserResults
{
	DWORD						dwMasterScore;			//大师分
	DWORD						dwMedal;				//亿币
	DWORD						dwZhanGong;				//战功
	DWORD						dwExp;					//经验
	DWORD						dwBastRanking;			//最佳排名
	DWORD						dwFirstRankCount;		//获得第一名次数
};

//获取用户游戏记录（大师分，战功，场次，胜率，总经验）
struct DBO_GP_GetUserGameRecords
{
	DWORD						dwUserlevel;			//用户等级
	BYTE						cbUserVIP;				//vip
	DWORD						dwMasterScore;			//大师分
	DWORD						dwZhanGong;				//战功
	DWORD						dwGamesNumber;			//游戏次数
	DWORD						dwWinProportion;		//胜利比例
	DWORD						dwExp;					//总经验

	TCHAR						szNickName[LEN_NICKNAME];	//昵称
};

//获取比赛报名状态
struct DBO_GP_GetMatchApplyStatus
{
	DWORD dwMatchID;              //比赛ID
	BYTE cbMatchStatus;			//状态
};

//今天发的红包
struct DBO_GP_GetTodaySendRed
{
	DWORD dwRedID;				//红包ID
	DWORD dwSendScore;			//发送的金额
	TCHAR szSendRedTime[32];		//发红包时间
};

//获取抢红包详情
struct DBO_GP_GetReceiveRedInfo
{
	DWORD   dwReceiveScore;				//抢红包金额
	TCHAR	szReceiveTime[32];		//抢红包时间
	TCHAR	szNickName[LEN_NICKNAME];	//昵称
};

//获取邀请码信息
struct DBO_GP_GetInviteCodeInfo
{
	DWORD   dwInviteSuccessNumber;				//邀请成功人数
	BYTE	cbIsReceiveInvite;					//是否已经接受邀请
	TCHAR	szInviteCode[LEN_INVITE_CODE];	//邀请码
};

//邀请码奖励信息
struct DBO_GP_InviteCodeRewardInfo
{
	DWORD dwID;							//奖励的检索ID
	DWORD dwFeedbackRewardUserID;         //回馈奖励的用户ID，0没有需要领取的奖励
};

//邀请码奖励记录
struct DBO_GP_InviteCodeRewardRecord
{
	LONGLONG llFinishRewardRecord;    //完成的奖励记录
	LONGLONG llReceiveRewardRecord;	 //已经领取的奖励记录
};

//邀请码回馈奖励状态
struct DBO_GP_GetFeedbackRewardStatus
{
	DWORD dwFeedbackRewardUserID;     //回馈奖励的用户ID，0没有需要领取的奖励
};

//用户推广奖励记录
struct DBR_GP_UserSpreadReward
{
	DWORD dwUserID;				//用户ID
	WORD wFirestLevelSpread;	//一级推广
	WORD wScondLevelSpread;		//二级推广
	WORD wThreeLevelSpread;		//三级推广
	DWORD dwFirestRewardScore;	//一级奖励金币
	DWORD dwScondRewradScore;	//二级奖励金币
	DWORD dwThreeRewradScore;	//三级奖励金币
	DWORD dwSpreadRewardCount;  //推广奖励的周期计数
	DWORD dwUserRevenue;		//用户这期的抽水数
};

//获取签到结果
struct DBO_GP_GetSignIn
{
	WORD wSignInNum;		//已经签到天数
	BYTE cbTodayIsSinIn;    //今天是否已经签到 0没有 1已经签到
	DWORD	dwResultCode;						//操作代码
	TCHAR	szDescribeString[128];				//描述消息
};

//获取转盘信息
struct DBO_GP_GetDailInfo
{
	SCORE					dwSilverConsume;				//白银消耗
	SCORE					dwGoldConsume;					//黄金消耗
	SCORE					dwJewelConsume;					//钻石消耗

	SCORE					dwDrawScore;					//抽奖积分
	SCORE					dwAddScore;						//下注积分

	SCORE					dwDailInfo[3][8];				// 转盘信息
};

//获取转盘信息
struct DBO_GP_RotateDailAwardInfo
{
	DWORD					dwResultCode;						//操作代码
	TCHAR					szDescribeString[128];				//描述消息

	DWORD					dwUserID;						// 用户信息
	DWORD					dwDailType;						// 奖励类型

	DWORD					dwDailAward;					// 奖励数量
	DWORD					dwAwardID;						// 奖励ID
	DWORD					dwConsumeSum;					// 消耗数量

	SCORE					dwSilverConsume;				//白银消耗
	SCORE					dwGoldConsume;					//黄金消耗
	SCORE					dwJewelConsume;					//钻石消耗

	SCORE					dwDrawScore;					//抽奖积分
	SCORE					dwAddScore;						//下注积分

	SCORE					dwDailInfo[3][8];				// 转盘信息
};

// 绑定银行卡信息
struct DBO_GP_BindBankInfo
{
	WORD							wSubCommandID;						//消息用途
	DWORD							dwResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息

	TCHAR							szBankName[32];						// 银行名字
	TCHAR							szBankCardNum[32];					// 银行卡号
	TCHAR							szAccountHolder[32];				// 开户人
};

// 绑定支付宝信息
struct DBO_GP_BindZFBInfo
{
	WORD							wSubCommandID;						//消息用途
	DWORD							dwResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息

	TCHAR							szZFBName[32];						// 支付宝账号
	TCHAR							szNickName[32];						// 用户名字
};

// 邮件信息
struct RequestMail
{
	DWORD							dwMailIndex;						// 邮件标识
	DWORD							dwStartUsing;						// 邮件读取
	TCHAR							szMailContent[128];					// 邮件内容
};
// 请求邮件
struct DBO_GP_RequestMailToal
{
	DWORD							dwUserID;							// 用户ID
	DWORD							dwMailCount;						// 邮件数量
	TCHAR							szDescribeString[128];				// 描述消息
	RequestMail						stRequestMail[15];					// 邮件内容
};

// 读取删除邮件
struct DBO_GP_ReadDeleteMailToal
{
	DWORD							dwUserID;							// 用户ID
	DWORD							dwOpenType;							// 操作标识 0 读取 1 删除
	DWORD							dwResultCode;						// 操作代码
	TCHAR							szDescribeString[128];				// 描述消息
	DWORD							dwMailCount;						// 邮件数量
	RequestMail						stRequestMail[15];					// 邮件内容
};

//获取用户详细信息
struct DBO_GP_GetUserPerfectInformation
{
	TCHAR szCompellation[LEN_COMPELLATION];	//用户姓名
	TCHAR szIdentifyCode[50];	//QQ号或手机号码
};

//用户推广奖励记录
struct DBO_GP_UserSpreadReward
{
	DWORD dwUserID;					//用户ID

	WORD wFirestLevelReward;	//一级奖励级别
	WORD wSecondLevelReward;	//二级奖励级别
	WORD wThreeLevelReward;		//三级奖励级别

	DWORD dwFirestRewardScore;	//一级奖励金币
	DWORD dwSecondRewradScore;	//二级奖励金币
	DWORD dwThreeRewradScore;	//三级奖励金币

	DWORD dwUserRevenue;				//用户抽水
	DWORD dwSecondLevelUserRevenueSum;	//二级用户抽水总数
	DWORD dwThreeLevelUserRevenueSum;	//三级用户抽水

	DWORD dwSecondLevelUserNum;		//二级用户数
	DWORD dwThreeLevelUserNum;		//三级用户数
		
	DWORD dwSecondLevelUserReachNum;  //二级用户达标数
	DWORD dwThreeLevelUserReachNum;   //三级用户达标数

	DWORD dwCurrentRewardScore;		//当前的奖励数
	DWORD dwSpreadRewardCount;	    //推广奖励周期计数
};

//获取推广奖励时间
struct  DBR_GP_LastSpreadRewardTime
{
	DWORD dwLastSpreadRewardCount;		//最后一次奖励周期
	SYSTEMTIME stLastSpreadRewardTime;	//最后一次领奖时间
};

//计算奖励
struct DBO_GP_CalculateUserReward
{
	DWORD dwUserID;									//用户ID
	DWORD dwLastSpreadRewardCount;						//最后一次推广奖励周期数
	enCalculateSpreadRewardOperationType dwOperation;  //操作类型
};

//用户抽水信息
struct DBO_GP_UserRevenueInfo
{
	DWORD dwUserID;
	DWORD dwUserRevenue;
};

//是否是推广注册
struct DBO_GP_IsSpreadRegister
{
	BYTE cbIsSpreadRegister;  //0不是推广注册，1是
	DWORD dwRewardScore;	  //奖励的金币数量
};

//弹窗消息
struct DBO_GR_ScrollMessage
{
	WORD wLength;					//字符串长度
	TCHAR szScrollMessage[256];		//内容
};

struct DBR_GP_ControlRoomStauts
{
	WORD wServerID;
	BYTE cbIsOpen;
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
		enumAttr_Email		=20,		//邮件
	};

	WORD wType;
	SCORE wScore;
};


#pragma pack()
#endif