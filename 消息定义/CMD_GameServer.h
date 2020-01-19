#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

// 回调函数宏
#define CALLBACK_0(__selector__, __target__, ...) std::bind(&__selector__, __target__, ##__VA_ARGS__)
#define CALLBACK_1(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, ##__VA_ARGS__)
#define CALLBACK_2(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CALLBACK_3(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
#define CALLBACK_4(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)

// 注册函数
#define REGISTER_FUNCTION_0(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_0(__selector__, __target__, ##__VA_ARGS__)))
#define REGISTER_FUNCTION_1(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_1(__selector__, __target__, ##__VA_ARGS__)))
#define REGISTER_FUNCTION_2(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_2(__selector__, __target__, ##__VA_ARGS__)))
#define REGISTER_FUNCTION_3(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_3(__selector__, __target__, ##__VA_ARGS__)))
#define REGISTER_FUNCTION_4(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_4(__selector__, __target__, ##__VA_ARGS__)))

//业务层心跳
#define  MDM_GR_HEART				65535
#define  SUB_GR_HEART				65535
//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GR_LOGON				1									//登录信息
#define MDM_GR_LOGON_BCode			2									//登录信息

//登录模式
#define SUB_GR_LOGON_USERID			1									//I D 登录
#define SUB_GR_LOGON_MOBILE			2									//手机登录
#define SUB_GR_LOGON_ACCOUNTS		3									//帐户登录
#define SUB_GR_LOGON_TOKEN			4									//Token登录
#define SUB_GR_LOGON_TOKEN_LUA		9									//LUA登录

//登录结果
#define SUB_GR_LOGON_SUCCESS		100									//登录成功
#define SUB_GR_LOGON_FAILURE		101									//登录失败
#define SUB_GR_LOGON_FINISH			102									//登录完成


//////////////////////////////////////////////////////////////////////////////////

//I D 登录
struct CMD_GR_LogonUserID
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本

	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	WORD							wKindID;							//类型索引
};

//手机登录
struct CMD_GR_LogonMobile
{	
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//Token登录
struct CMD_GR_LogonTokenID
{
	DWORD							dwMerchantID;						//商户ID
	DWORD							dwUserID;							//用户 I D
	TCHAR							szTokenID[LEN_MD5];					//TokenID
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szClientIP[16];						//客户端ip
	BYTE                            cbDeviceType;                       //设备类型
};

//帐号登录
struct CMD_GR_LogonAccounts
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//登录成功
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限
};

struct CMD_GR_LogonUserBlock
{
	WORD							wErrorID;							//错误号
	TCHAR							szAddr[32];							//房间地址
	WORD							wPort;								//类型索引
	TCHAR							szErrorMsg[128];					//错误问题
};

//登录失败
struct CMD_GR_LogonFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//////////////////////////////////////////////////////////////////////////////////
//配置命令

#define MDM_GR_CONFIG				2									//配置信息

#define SUB_GR_CONFIG_COLUMN		100									//列表配置
#define SUB_GR_CONFIG_SERVER		101									//房间配置
#define SUB_GR_CONFIG_PROPERTY		102									//道具配置
#define SUB_GR_CONFIG_FINISH		103									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT	104									//玩家权限

//////////////////////////////////////////////////////////////////////////////////

//列表配置
struct CMD_GR_ConfigColumn
{
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
};

//房间配置
struct CMD_GR_ConfigServer
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目

	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
};

//房间配置
struct CMD_GR_ConfigServerEx
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目

	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则

	SCORE							lCellScore;							//房间低分
	DWORD							dwExpValue;							//每局经验
	DWORD							dwBloodLower;
};

//道具配置
struct CMD_GR_ConfigProperty
{
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具描述
};

//玩家权限
struct CMD_GR_ConfigUserRight
{
	DWORD							dwUserRight;						//玩家权限
};
//////////////////////////////////////////////////////////////////////////////////
//用户命令

#define MDM_GR_USER					3									//用户信息

//用户动作
#define SUB_GR_USER_RULE			1									//用户规则
#define SUB_GR_USER_LOOKON			2									//旁观请求
#define SUB_GR_USER_SITDOWN			3									//坐下请求
#define SUB_GR_USER_STANDUP			4									//起立请求
#define SUB_GR_USER_INVITE			5									//用户邀请
#define SUB_GR_USER_INVITE_REQ		6									//邀请请求
#define SUB_GR_USER_REPULSE_SIT  	7									//拒绝玩家坐下
#define SUB_GR_USER_KICK_USER       8                                   //踢出用户
#define SUB_GR_USER_INFO_REQ        9                                   //请求用户信息
#define SUB_GR_USER_CHAIR_REQ       10                                  //请求更换位置
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //请求椅子用户信息
#define SUB_GR_MB_USER_SITDOWN_REQ  12                                  //手机用户坐下消息
#define SUB_GR_MB_USER_SITDOWN      13                                  //手机用户分配桌椅消息

#define SUB_GR_USER_SITDOWN_H5		15									//H5坐下
#define SUB_GR_USER_STANDUP_H5		16									//H5起立
#define SUB_GR_USER_CHANGE_TABLE_H5 17									//H5换桌

//用户状态
#define SUB_GR_USER_ENTER			100									//用户进入
#define SUB_GR_USER_SCORE			101									//用户分数
#define SUB_GR_USER_STATUS			102									//用户状态
#define SUB_GR_REQUEST_FAILURE		103									//请求失败
#define SUB_GR_USER_LEAVE			104									//用户离开

//聊天命令
#define SUB_GR_USER_CHAT			201									//聊天消息
#define SUB_GR_USER_EXPRESSION		202									//表情消息
#define SUB_GR_WISPER_CHAT			203									//私聊消息
#define SUB_GR_WISPER_EXPRESSION	204									//私聊表情
#define SUB_GR_COLLOQUY_CHAT		205									//会话消息
#define SUB_GR_COLLOQUY_EXPRESSION	206									//会话表情

//道具命令
#define SUB_GR_PROPERTY_BUY			300									//购买道具
#define SUB_GR_PROPERTY_SUCCESS		301									//道具成功
#define SUB_GR_PROPERTY_FAILURE		302									//道具失败
#define SUB_GR_PROPERTY_MESSAGE     303                                 //道具消息
#define SUB_GR_PROPERTY_EFFECT      304                                 //道具效应
#define SUB_GR_PROPERTY_TRUMPET		305                                 //喇叭消息


#define SUB_GR_GLAD_MESSAGE			400                                 //喜报消息
#define SUB_GR_ROOM_RED				401                                 //房间红包
#define SUB_GR_ROOM_GRAPRED			402									//房间抢红包
#define SUB_GR_S_SEND_RED			403									//房间红包（下行）
#define SUB_GR_EXP_RANK				408									//经验排行
#define SUB_GR_GET_TIME_RANK		409									//定时排名

//////////////////////////////////////////////////////////////////////////////////

//旁观请求
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
};

//坐下请求
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	TCHAR							szPassword[LEN_PASSWORD];			//桌子密码
};

//起立请求
struct CMD_GR_UserStandUp
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	BYTE							cbForceLeave;						//强行离开
};

//邀请用户 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//邀请用户请求 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//用户分数
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//用户标识
	tagUserScore					UserScore;							//积分信息
};

//用户分数
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//用户标识
	tagMobileUserScore				UserScore;							//积分信息
};

//用户状态
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态
};

//用户地区
struct CMD_GR_UserAddress
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szAddress[50];						//地区
};

//请求失败
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[256];				//描述信息
};


//用户聊天
struct CMD_GR_C_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户聊天
struct CMD_GR_S_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户表情
struct CMD_GR_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GR_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//用户私聊
struct CMD_GR_C_WisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户私聊
struct CMD_GR_S_WisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//私聊表情
struct CMD_GR_C_WisperExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//私聊表情
struct CMD_GR_S_WisperExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//用户会话
struct CMD_GR_ColloquyChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwConversationID;					//会话标识
	DWORD							dwTargetUserID[16];					//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//邀请用户
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//桌子号码
	DWORD							dwSendUserID;						//发送用户
};

//邀请用户
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//购买道具
struct CMD_GR_C_PropertyBuy
{
	BYTE                            cbRequestArea;						//请求范围
	BYTE							cbConsumeScore;						//积分消费
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引	
	DWORD							dwTargetUserID;						//使用对象
};

//道具成功
struct CMD_GR_S_PropertySuccess
{
	BYTE                            cbRequestArea;						//使用环境
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//目标对象
	DWORD							dwTargetUserID;						//使用对象
};

//道具失败
struct CMD_GR_PropertyFailure
{
	WORD                            wRequestArea;                       //请求区域
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[256];				//描述信息
};

//道具消息
struct CMD_GR_S_PropertyMessage
{
	//道具信息
	WORD                            wPropertyIndex;                     //道具索引
	WORD                            wPropertyCount;                     //道具数目
	DWORD                           dwSourceUserID;                     //目标对象
	DWORD                           dwTargerUserID;                     //使用对象
};


//道具效应
struct CMD_GR_S_PropertyEffect
{
	DWORD                           wUserID;					        //用 户I D
	BYTE							cbMemberOrder;						//会员等级
};

//发送喇叭
struct CMD_GR_C_SendTrumpet
{
	BYTE                           cbRequestArea;                        //请求范围 
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//发送喇叭
struct CMD_GR_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          dwSendUserID;                         //用户 I D
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szSendNickName[32];				    //玩家昵称 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//喜报消息
struct CMD_GR_SendGladMsg 
{
	TCHAR							szContent[TRUMPET_MAX_CHAR];		//内容
	TCHAR							szNickName[32];						//昵称
	TCHAR							szNum[128];							//数字
	DWORD							colText;							//内容颜色
	DWORD							colName;							//昵称颜色
	DWORD							colNum;								//数字颜色

};


//用户拒绝黑名单坐下
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
	DWORD							dwUserID;							//用户 I D
	DWORD							dwRepulseUserID;					//用户 I D
};

//道具成功
struct CMD_GR_GameItemSuccess
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wCount;
	DWORD							dwUserID;
	tagGameItemInfo					GameItemInfo[8];
};

//创建桌子
struct CMD_GR_CreateTableNum
{
	WORD							wTableNum;							//房号
	LONG							lCellScore;							//桌子低分
	TCHAR							szPassword[LEN_PASSWORD];	//房间密码
};

//获取桌子
struct CMD_GR_GetNullTableNum
{
	WORD							wNullTableNum;
	LONG							lCellScoreList[4];
};

struct tagLockTables
{
	BYTE							cbIsLock;		//锁桌标识，保留用
	WORD							wTableNum;		//桌子序号
	LONG							lCellScore;		//本桌低分
};

//获取桌子列表
struct CMD_GR_GetLockTables
{
	WORD							wTableCount;		//锁桌数量
	tagLockTables					LockTables[100];	//一百桌封顶
};
//////////////////////////////////////////////////////////////////////////////////

//规则标志
#define UR_LIMIT_SAME_IP			0x01								//限制地址
#define UR_LIMIT_WIN_RATE			0x02								//限制胜率
#define UR_LIMIT_FLEE_RATE			0x04								//限制逃率
#define UR_LIMIT_GAME_SCORE			0x08								//限制积分

//用户规则
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//规则掩码
	WORD							wMinWinRate;						//最低胜率
	WORD							wMaxFleeRate;						//最高逃率
	LONG							lMaxGameScore;						//最高分数 
	LONG							lMinGameScore;						//最低分数
};

//请求用户信息
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //请求用户
	WORD							wTablePos;							//桌子位置
};

//请求用户信息
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
};
//////////////////////////////////////////////////////////////////////////////////
//状态命令

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define SUB_GR_TABLE_STATUS			101									//桌子状态

//////////////////////////////////////////////////////////////////////////////////

//桌子信息
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//桌子数目
	tagTableStatus					TableStatusArray[512];				//桌子状态
};

//桌子状态
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//桌子号码
	tagTableStatus					TableStatus;						//桌子状态
};

//////////////////////////////////////////////////////////////////////////////////
//银行命令

#define MDM_GR_INSURE				5									//用户信息

//银行命令
#define SUB_GR_QUERY_INSURE_INFO	1									//查询银行
#define SUB_GR_SAVE_SCORE_REQUEST	2									//存款操作
#define SUB_GR_TAKE_SCORE_REQUEST	3									//取款操作
#define SUB_GR_TRANSFER_SCORE_REQUEST	4								//取款操作
#define SUB_GR_QUERY_USER_INFO_REQUEST	5								//查询用户

#define SUB_GR_USER_INSURE_INFO		100									//银行资料
#define SUB_GR_USER_INSURE_SUCCESS	101									//银行成功
#define SUB_GR_USER_INSURE_FAILURE	102									//银行失败
#define SUB_GR_USER_TRANSFER_USER_INFO	103								//用户资料

//////////////////////////////////////////////////////////////////////////////////

//查询银行
struct CMD_GR_C_QueryInsureInfoRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
};

//存款请求
struct CMD_GR_C_SaveScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lSaveScore;							//存款数目
};

//取款请求
struct CMD_GR_C_TakeScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lTakeScore;							//取款数目
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//转账金币
struct CMD_GP_C_TransferScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账金币
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//查询用户
struct CMD_GR_C_QueryUserInfoRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//银行资料
struct CMD_GR_S_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct CMD_GR_S_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lUserScore;							//身上金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct CMD_GR_S_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//用户信息
struct CMD_GR_S_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};
//////////////////////////////////////////////////////////////////////////////////
//管理命令

#define MDM_GR_MANAGE				6									//管理命令

#define SUB_GR_SEND_WARNING			1									//发送警告
#define SUB_GR_SEND_MESSAGE			2									//发送消息
#define SUB_GR_LOOK_USER_IP			3									//查看地址
#define SUB_GR_KILL_USER			4									//踢出用户
#define SUB_GR_LIMIT_ACCOUNS		5									//禁用帐户
#define SUB_GR_SET_USER_RIGHT		6									//权限设置

//房间设置
#define SUB_GR_QUERY_OPTION			7									//查询设置
#define SUB_GR_OPTION_SERVER		8									//房间设置
#define SUB_GR_OPTION_CURRENT		9									//当前设置

#define SUB_GR_LIMIT_USER_CHAT		10									//限制聊天

#define SUB_GR_KICK_ALL_USER		11									//踢出用户
#define SUB_GR_DISMISSGAME		    12									//解散游戏
#define SUB_GR_SCORE_MODIFY			13									//积分修改
#define SUB_GR_SCORE_MODIFY_RESULT	14									//积分修改结果
#define SUB_GR_SCORE_QUERY			15									//查询积分
#define SUB_GR_SCORE_QUERY_RESULT	16									//查询结果
#define SUB_GR_GM2_LIST_QUERY		17									//查询GM2列表
#define SUB_GR_GM2_LIST_ITEM		18									//gm2列表项
#define SUB_GR_GM2_OP_LIST_QUERY	19									//查询GM2操作列表
#define SUB_GR_GM2_OP_LIST_ITEM		20									//操作gm2列表项

#define SUB_GR_SCORE_LIMIT_MODIFY			21							//开分修改
#define SUB_GR_SCORE_LIMIT_MODIFY_RESULT	22							//开分修改结果
#define SUB_GR_SCORE_LIMIT_QUERY			23							//查询开分
#define SUB_GR_SCORE_LIMIT_QUERY_RESULT		24							//查询开分结果
#define SUB_GR_KILL_USER_LOCKER     13                                  //清卡线  

//////////////////////////////////////////////////////////////////////////////////

//发送警告
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//信息长度
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//警告消息
};

//系统消息
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//游戏消息
	BYTE							cbRoom;								//游戏消息
	BYTE							cbAllRoom;							//游戏消息
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//弹窗消息
struct CMD_GR_TipMessage
{
	TCHAR							szTipMessage[1024];		//弹窗消息
};


//查看地址
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//目标用户
};

//踢出用户
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//禁用帐户
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//目标用户
};

//权限设置
struct CMD_GR_SetUserRight
{
	//目标用户
	DWORD							dwTargetUserID;						//目标用户

	//绑定变量
	BYTE							cbGameRight;						//帐号权限
	BYTE							cbAccountsRight;					//帐号权限

	//权限变化
	BYTE							cbLimitRoomChat;					//大厅聊天
	BYTE							cbLimitGameChat;					//游戏聊天
	BYTE							cbLimitPlayGame;					//游戏权限
	BYTE							cbLimitSendWisper;					//发送消息
	BYTE							cbLimitLookonGame;					//旁观权限
};

//房间设置
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//规则掩码
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//房间设置
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//踢出所有用户
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//踢出提示
};

//解散游戏
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //解散桌号
};

//修改积分
struct CMD_GR_ModifyScore
{
	TCHAR							szTargetAccount[32];				//目标账号
	LONGLONG						lScore;								//银行星豆
	LONGLONG						lMedal;								//银行元宝(旧奖牌)
	LONGLONG						lGameScore;							//游戏星豆
	LONGLONG						lGameMedal;							//游戏元宝
	LONG							lLoveLiness;						//奖牌(旧魅力)
	LONG							lUserPower;							//活力
	LONG							lScoreLimit;						//开分上限
	TCHAR							szReason[32];						//修改原因
};

//积分修改结果
struct CMD_GR_ModifyScoreResult
{
	BYTE							cbResult;							//0:成功
	TCHAR							szResult[128];						//结果信息
};

//查询奖牌
struct CMD_GR_CheckMedal
{
	WORD							wTable;								//桌子号
	DWORD							dwUserID;							//用户ID

};

//查询积分
struct CMD_GR_QueryScore
{
	TCHAR							szTargetAccount[32];				//目标账号
};

//积分修改结果
struct CMD_GR_QueryScoreResult
{
	BYTE							cbResult;							//查询结果,0:成功
	TCHAR							szTargetAccount[32];				//目标账号
	LONGLONG						lScore;								//银行星豆
	LONGLONG						lMedal;								//银行元宝(旧奖牌)
	LONGLONG						lGameScore;							//游戏星豆
	LONGLONG						lGameMedal;							//游戏元宝
	LONG							lLoveLiness;						//奖牌(旧魅力)
	LONG							lUserPower;							//用户活力
	LONG							lScoreLimit;						//开分上限
};

//查询gm2列表项
struct CMD_GR_Gm2ListItem
{
	TCHAR							szTargetAccount[32];				//目标账号
	LONGLONG						lMedalLimit;						//上限
	LONGLONG						lMedal;								//累计
};

//查询gm2操作列表项
struct CMD_GR_Gm2OpListItem
{
	TCHAR							szTargetAccount[32];				//目标账号
	LONGLONG						lMedal;								//累计
};

//修改积分
struct CMD_GR_ModifyScoreLimit
{
	TCHAR							szTargetAccount[32];				//目标账号
	LONGLONG						lMedal;								//银行元宝(旧奖牌)
};

//积分修改结果
struct CMD_GR_ModifyScoreLimitResult
{
	BYTE							cbResult;							//0:成功
	TCHAR							szResult[128];						//结果信息
};


//查询积分
struct CMD_GR_QueryScoreLimit
{
};

//积分修改结果
struct CMD_GR_QueryScoreLimitResult
{
	BYTE							cbResult;							//查询结果,0:成功
	LONGLONG						lMedalLimit;						//上限元宝
	LONGLONG						lMedal;								//已开元宝
};
//////////////////////////////////////////////////////////////////////////////////

//设置标志
#define OSF_ROOM_CHAT				1									//大厅聊天
#define OSF_GAME_CHAT				2									//游戏聊天
#define OSF_ROOM_WISPER				3									//大厅私聊
#define OSF_ENTER_TABLE				4									//进入游戏
#define OSF_ENTER_SERVER			5									//进入房间
#define OSF_SEND_BUGLE				12									//发送喇叭

//房间设置
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//设置标志
	BYTE							cbOptionValue;						//设置标志
};

//限制聊天
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//目标用户
	BYTE							cbLimitFlags;						//限制标志
	BYTE							cbLimitValue;						//限制与否
};
//////////////////////////////////////////////////////////////////////////////////
//比赛命令

#define MDM_GR_MATCH				7									//比赛命令

#define SUB_GR_MATCH_FEE			400									//报名费用
#define SUB_GR_MATCH_NUM			401									//等待人数
#define SUB_GR_LEAVE_MATCH			402									//退出比赛
#define SUB_GR_MATCH_INFO			403									//比赛信息
#define SUB_GR_MATCH_WAIT_TIP		404									//等待提示
#define SUB_GR_MATCH_RESULT			405									//比赛结果
#define SUB_GR_MATCH_STATUS			406									//比赛状态
#define SUB_GR_MATCH_USER_COUNT		407									//参赛人数
#define SUB_GR_MATCH_DESC			408									//比赛描述

//服务端发送
#define SUB_GR_MATCH_REWARD			409									//比赛奖励
#define SUB_GR_MATCH_TYPE			411									//比赛类型
#define SUB_GR_MATCH_JOIN			412									//报名方案
#define SUB_GR_MATCH_REVIVE			413									//复活方案
#define SUB_GR_MATCH_RULE			414									//比赛规则
#define SUB_GR_MATCH_USER			415									//比赛选手
#define SUB_GR_MATCH_WARN			416									//比赛提示

//比赛人数
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//等待人数
	DWORD							dwTotal;							//开赛人数
	DWORD							dwMatchTotal;						//总人数
};

//赛事信息
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//信息标题
    WORD							wGameCount;							//游戏局数
};

//提示信息
struct CMD_GR_Match_Warn
{
	SCORE							lScore;								//当前积分
	WORD							wRank;								//当前名次
	WORD							wCurTableRank;						//本桌名次
	WORD							wUserCount;							//当前人数
	WORD							wPlayingTable;						//游戏桌数
};

//
struct CMD_GR_MatchInfoNew
{
	BYTE							cbMatchMold;						//开始方式，预赛开始，复赛开始
	BYTE							cbMatchType;						//比赛类型，人满赛，循环赛，定时赛
	BYTE							cbMatchRule;						//比赛规则，打立出局，末位淘汰
	BYTE							cbRoundCount;						//复赛轮数
	BYTE							cbGameCount;						//一轮局数
	BYTE							cbCurRound;							//当前轮数
	BYTE							cbCurCount;							//当轮局数
	BYTE							cbMatchStatus;						//比赛阶段
	WORD							wCurBase;							//当前基数
	WORD							wOutScore;							//出局低分
	WORD							wCurRank;							//当前排名
	WORD							wMatchUser;							//比赛人数
	WORD							wPromotionCount;					//晋级人数
	WORD							wRemainCount;						//截止人数
	TCHAR							szMatchStatus[64];					//比赛阶段
	TCHAR							szMatchTitle[64];					//比赛名称
};

struct CMD_GR_MatchCurProc
{
	BYTE							cbCurStage;							//当前阶段
	BYTE							cbCurRule;							//当前规则
	BYTE							cbRoundCount;						//一场轮数
	BYTE							cbGameCount;						//一轮局数
	BYTE							cbCurRound;							//当前轮数
	BYTE							cbCurCount;							//当轮局数
	DWORD							dwCurBase;							//当前基数
	DWORD							dwOutScore;							//出局低分
	DWORD							dwCurRank;							//当前排名
	DWORD							dwMatchUser;						//比赛人数
	DWORD							dwPromotionCount;					//晋级人数
	DWORD							dwRemainCount;						//截止人数
};


//比赛奖励结果
struct CMD_GR_MatchResult
{
	DWORD							dwRank;								//比赛排名
};

//比赛描述
struct CMD_GR_MatchDesc
{
};


//比赛状态
struct CMD_GR_MatchStatus
{
	BYTE							cbMatchStatus;						//比赛状态
	DWORD							dwUserID;							//用户 ID
};

//////////////////////////////////////////////////////////////////////////////////
//框架命令

#define MDM_GF_FRAME				100									//框架命令

//////////////////////////////////////////////////////////////////////////////////
//框架命令

//用户命令
#define SUB_GF_GAME_OPTION			1									//游戏配置
#define SUB_GF_USER_READY			2									//用户准备
#define SUB_GF_LOOKON_CONFIG		3									//旁观配置

#define SUB_GF_GAME_OPTION_H5			21									//游戏配置
#define SUB_GF_USER_READY_H5			22									//用户准备
#define SUB_GF_LOOKON_CONFIG_H5			23									//旁观配置

//聊天命令
#define SUB_GF_USER_CHAT			10									//用户聊天
#define SUB_GF_USER_EXPRESSION		11									//用户表情

//游戏信息
#define SUB_GF_GAME_STATUS			100									//游戏状态
#define SUB_GF_GAME_SCENE			101									//游戏场景
#define SUB_GF_LOOKON_STATUS		102									//旁观状态

//系统消息
#define SUB_GF_SYSTEM_MESSAGE		200									//系统消息
#define SUB_GF_ACTION_MESSAGE		201									//动作消息
#define SUB_GF_OPERATE_SUCCESS		202									//操作成功
#define SUB_GF_OPERATE_FAILURE		203									//操作失败
#define SUB_GF_SCROLL_MESSAGE		204									//滚动消息
#define SUB_GF_REDIS_SCROLL_MES		205									// Redis滚动消息

#define SUB_GR_S_CARDBOX			301									//对局宝箱
#define SUB_GR_S_TIMERANK			311									//定时排名
#define SUB_GR_S_RANK_LIST			312									//定时排名

#define SUB_GR_GET_BLOOD			313									//补血配置
#define SUB_GR_SUPPLY_BLOOD			314									//补血
#define SUB_GR_AWARD_CARDBOX		315									//领取宝箱奖励

#define SUB_GP_FINISHED_FIRST_GAME  320									//完成第一局游戏
//////////////////////////////////////////////////////////////////////////////////

//游戏配置
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//旁观标志
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//游戏版本
};

//旁观配置
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbAllowLookon;						//允许旁观
};

//旁观状态
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//允许旁观
};

//游戏环境
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//游戏状态
	BYTE							cbAllowLookon;						//旁观标志
};

//用户聊天
struct CMD_GF_C_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户聊天
struct CMD_GF_S_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户表情
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//宝箱
struct CMD_MB_S_CARDBOX
{
	WORD		wCurCount;			//当前局数
	WORD		wCardCount;			//宝箱局数
	BYTE		cbAwardCount;		//剩余领奖
	BYTE		cbIsComplete;		//是否完成
};

//定时排行
struct CMD_MB_S_TimeAward
{
	DWORD		dwCurRank;			//当前名次
	DWORD		dwRankScore;		//排名积分	
	DWORD		dwTimeLeft;			//颁奖剩余时间 单位秒
};
//////////////////////////////////////////////////////////////////////////////////
//游戏命令

#define MDM_GF_GAME					200									//游戏命令

//////////////////////////////////////////////////////////////////////////////////
//携带信息

//其他信息
#define DTP_GR_TABLE_PASSWORD		1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME			10									//用户昵称
#define DTP_GR_GROUP_NAME			11									//社团名字
#define DTP_GR_UNDER_WRITE			12									//个性签名
#define DTP_GR_ADDRESS				13									//玩家地址

//附加信息
#define DTP_GR_USER_NOTE			20									//用户备注
#define DTP_GR_CUSTOM_FACE			21									//自定头像

//////////////////////////////////////////////////////////////////////////////////

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_NOGOLD		1									//金币不足
#define REQUEST_FAILURE_NOSCORE		2									//积分不足
#define REQUEST_FAILURE_PASSWORD	3									//密码错误
#define REQUEST_FAILURE_NETWORK		4									//网络原因
#define REQUEST_FAILURE_QUIT		5									//失败退出

//////////////////////////////////////////////////////////////////////////////////
//自定义事件
#define SE_KICK_ALL_USER_REQUEST		1								//踢出所有玩家
#define SE_HIGH_SCORE_LIST_REQUEST		2								//请求、玩家列表
#define SE_HIGH_SCORE_REQUEST			3								//请求设置分数
#define SE_HIGH_SCORE_GET_REQUEST		4								//请求获取分数

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif