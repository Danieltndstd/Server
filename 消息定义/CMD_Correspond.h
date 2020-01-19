#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//注册命令

#define MDM_CS_REGISTER				1									//服务注册

//服务注册
#define SUB_CS_C_REGISTER_PLAZA		100									//注册广场
#define SUB_CS_C_REGISTER_SERVER	101									//注册房间
#define SUB_CS_C_REGISTER_MATCH		102									//注册比赛

//注册结果
#define SUB_CS_S_REGISTER_FAILURE	200									//注册失败

//////////////////////////////////////////////////////////////////////////////////

//注册广场
struct CMD_CS_C_RegisterPlaza
{
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//服务器名
};

//注册游戏
struct CMD_CS_C_RegisterServer
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
	BYTE							cbRoomType;							//房间级别
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//限制配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分
	SCORE							lMinEnterTiLi;						//最低体力


	WORD							wCardBoxIndex;						//宝箱索引
};

//注册比赛
struct CMD_CS_C_RegisterMatch
{
	/*
	WORD				wOrderID;						//比赛排序
	WORD				wMatchTab;						//比赛标签
	WORD				wMatchType;						//比赛类型
	DWORD				dwJoinUserCountUpper;			//最大参赛人数		
	TCHAR				szMatchReward[32];				//冠军奖品
	TCHAR				szMatchName[80];				//比赛全称
	*/
	//CMD_GR_MatchDateTime		DateTime;				//比赛时间
	//tagMatchRequireInfo			Require;					//报名条件
	//tagMatchFeeInfo				Fee;						//报名费用
	CMD_CS_C_RegisterServer		RegServer;
	//tagMatchReviveInfo			ReviveInfo;					//比赛复活
	//tagMatchTagout				Tagout;						//比赛挂牌
};

//注册失败
struct CMD_CS_S_RegisterFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//////////////////////////////////////////////////////////////////////////////////
//服务信息

#define MDM_CS_SERVICE_INFO			2									//服务信息

//房间信息
#define SUB_CS_C_SERVER_ONLINE		1									//房间人数
#define SUB_CS_C_SERVER_MODIFY		2									//房间修改

//好友命令
#define SUB_CS_C_FRIEND_REQUEST		10									//好友申请
#define SUB_CS_C_AGREE_REQUEST		11									//好友同意申请
#define SUB_CS_C_DEL_FRIEND			12									//删除好友
#define SUB_CS_C_REFUSE				13									//拒绝申请


#define SUB_CS_S_FRIEND_REQUEST		20									//好友申请
#define SUB_CS_S_AGREE_REQUEST		21									//好友申请同意
#define SUB_CS_S_DEL_FRIEND			22									//删除好友
#define SUB_CS_S_REFUSE				23									//拒绝申请

//广场命令
#define SUB_CS_S_PLAZA_INFO			100									//广场信息
#define SUB_CS_S_PLAZA_INSERT		101									//广场列表
#define SUB_CS_S_PLAZA_REMOVE		103									//广场删除
#define SUB_CS_S_PLAZA_FINISH		104									//广场完成

//房间命令
#define SUB_CS_S_SERVER_INFO		110									//房间信息
#define SUB_CS_S_SERVER_ONLINE		111									//房间人数
#define SUB_CS_S_SERVER_INSERT		112									//房间列表
#define SUB_CS_S_SERVER_MODIFY		113									//房间修改
#define SUB_CS_S_SERVER_REMOVE		114									//房间删除
#define SUB_CS_S_SERVER_FINISH		115									//房间完成

#define SUB_CS_S_MATCH_INSERT		116									//比赛列表
//////////////////////////////////////////////////////////////////////////////////
//房间数据
struct CMD_CS_C_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//房间人数
struct CMD_CS_C_ServerOnLine
{
	DWORD							dwOnLineCount;						//在线人数
};

//用户获取排行榜
struct CMD_CS_C_UserRankGet
{
	DWORD							dwSocketID;							//
};

//////////////////////////////////////////////////////////////////////////////////

//广场删除
struct CMD_CS_S_PlazaRemove
{
	DWORD							dwPlazaID;							//广场标识
};

//房间人数
struct CMD_CS_S_ServerOnLine
{
	WORD							wServerID;							//房间标识
	DWORD							dwOnLineCount;						//在线人数
};

//房间修改
struct CMD_CS_S_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerID;							//房间标识
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//房间删除
struct CMD_CS_S_ServerRemove
{
	WORD							wServerID;							//房间标识
};

//玩家排名分组
struct CMD_CS_S_UserRankGroup
{
	DWORD							dwSocketID;							// 发送用户
	int								iGroup;								// 分组
	TCHAR							szName[64];							// 分组名称
	int								iYear;								// 年
	int								iMon;								// 月
	int								iDay;								// 日
};


//玩家排名分组下的项
struct CMD_CS_S_UserRank
{
	DWORD							dwSocketID;							// 发送用户
	int								iGroup;								// 对应分组
	TCHAR							szNickName[LEN_NICKNAME];			// 昵称
	LONGLONG						lScore;								// 积分
};
//////////////////////////////////////////////////////////////////////////////////
//用户汇总

#define MDM_CS_USER_COLLECT			3									//用户汇总

//用户状态
#define SUB_CS_C_USER_ENTER			1									//用户进入
#define SUB_CS_C_USER_LEAVE			2									//用户离开
#define SUB_CS_C_USER_FINISH		3									//用户完成

#define SUB_CS_C_USER_BLOCK			4									//用户卡线

//用户状态
#define SUB_CS_S_COLLECT_REQUEST	100									//汇总请求

#define SUB_CS_S_USER_BLOCK			101									//用户卡线信息

//////////////////////////////////////////////////////////////////////////////////

//用户进入
struct CMD_CS_C_UserEnter
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
};

//用户离开
struct CMD_CS_C_UserLeave
{
	DWORD							dwUserID;							//用户标识
};

struct CMD_CS_C_UserBlock
{
	DWORD							dwSocket;							//用户socket
	DWORD							dwUserID;							//用户标识
	WORD							wCurServerID;						//当前房间标识
	WORD							wKindID;							//房间标识
	WORD							wServerID;							//类型索引
	TCHAR							szErrorMsg[128];					//错误问题
};

struct CMD_CS_S_UserBlock
{
	DWORD							dwSocket;							//用户socket
	DWORD							dwUserID;							//用户标识
	WORD							wCurServerID;						//当前房间标识
	WORD							wServerID;							//房间标识
	TCHAR							szAddr[32];							//房间地址
	WORD							wPort;								//类型索引
	TCHAR							szErrorMsg[128];					//错误问题
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_CS_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_CS_C_SEARCH_DATABASE	1									//数据查找
#define SUB_CS_C_SEARCH_CORRESPOND	2									//协调查找
#define SUB_CS_C_SEND_RED			3									//协调红包
#define SUB_CS_C_GRAP_RED			4									//协调抢红包
#define SUB_CS_C_INVITE_FRIEND		5									//邀请好友

//存储
#define SUB_CS_SAVE_REDINFO		6									//存储红包数据

#define SUB_CS_C_APPLY_RED			7									//申请红包


//查找结果
#define SUB_CS_S_SEARCH_DATABASE	100									//数据查找
#define SUB_CS_S_SEARCH_CORRESPOND	101									//协调查找
#define SUB_CS_S_SEND_RED			102									//协调红包
#define SUB_CS_S_GRAP_RED			103									//协调抢红包
#define SUB_CS_S_INVITE_FRIEND		104									//邀请好友
#define SUB_CS_S_PUSH_RED			105									//推送红包
#define SUB_CS_S_DELETE_RED			106									//删除红包

#define SUB_CS_S_USER_CLEAR			200									//踢掉玩家

//踢掉玩家
struct CMD_CS_S_ClearUser
{
	DWORD							dwServerID;							//房间ID
	DWORD							dwUserID;							//UserID
	DWORD							dwLimitTime;						//限时
};

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_CS_C_SearchCorrespond
{
	//连接参数
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址

	//查找数据
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_CS_S_SearchCorrespond
{
	//连接信息
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址

	//数据信息
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

//协调红包
struct CMD_CS_C_SendRed
{
	DWORD dwSocketID;			//网络标识
	DWORD dwSendUserID;			//发红包玩家ID
	DWORD dwSendUserNum;		//红包总人数
	DWORD dwSendScore;			//红包金额
	BYTE cbRedType;			//红包类型
	TCHAR szSendContent[128];	//红包附加内容
};

//协调红包结果
struct CMD_CS_S_SendRed
{
	DWORD dwSocketID;			//网络标识
	BYTE  cbSuccess;			//是否成功
	DWORD dwSendUserID;			//发红包玩家ID
	DWORD dwSendUserNum;		//红包总人数
	DWORD dwSendScore;			//红包金额
	BYTE  cbRedType;			//红包类型
	TCHAR szSendContent[128];	//红包附加内容
};

//红包信息（map存储）
struct CMD_MB_MAP_SENDRED
{
	DWORD dwSendUserID;			//发红包玩家ID
	DWORD dwSendUserNum;		//红包总人数
	DWORD dwSendScore;			//红包金额
	DWORD dwPerScore;			//每个人可领取的金额
	DWORD dwReceiveCount;		//已领取人数
	TCHAR szSendContent[128];	//红包附加内容
};

//协调抢红包
struct CMD_CS_C_GrapRed
{
	DWORD dwSocketID;		//网络标识
	DWORD dwSendUserID;		//发红包玩家ID
	DWORD dwGrapUserID;		//抢红包玩家ID
	DWORD dwGrapRedID;		//抢的红包ID
};

//协调抢红包结果
struct CMD_CS_S_GrapRed
{
	DWORD dwSocketID;		//网络标识
	BYTE  cbSuccess;		//是否成功
	DWORD dwSendUserID;		//发红包玩家ID
	DWORD dwGrapUserID;		//抢红包玩家ID
	DWORD dwRecvScore;		//抢到的金额
	TCHAR szSendContent[128];	//红包附加内容
	DWORD dwGrapRedID;		//抢的红包ID
};

//协调申请红包
struct CMD_CS_C_ApplyRed
{
	DWORD dwSocketID;			//申请人的网络标识
	DWORD dwUserID;				//申请用户ID
};

//协调申请红包
struct CMD_CS_S_ApplyRed
{
	DWORD dwSocketID;			//申请人的网络标识
	BYTE  cbSuccess;			//是否成功
	DWORD dwUserID;				//申请用户ID
};

//删除红包
struct CMD_CS_S_DeleteRed
{
	DWORD dwSendUserID;			//发红包的ID
	DWORD dwResidueScore;		//红包剩余金额
};

//////////////////////////////////////////////////////////////////////////////////
//管理服务

#define MDM_CS_MANAGER_SERVICE		5									//管理服务

#define SUB_CS_C_SYSTEM_MESSAGE 	1									//系统消息
#define SUB_CS_C_PROPERTY_TRUMPET 	2									//喇叭消息
#define SUB_CS_C_USER_CHAT			6									//玩家聊天

#define SUB_CS_S_SYSTEM_MESSAGE 	100									//系统消息
#define SUB_CS_S_PROPERTY_TRUMPET  	200									//喇叭消息
#define SUB_CS_S_GLAD_MESSAGE	  	300									//喜报消息
#define SUB_CS_S_HIGH_SCORE_LIST	400									//最高分数消息列表
#define SUB_CS_S_HIGH_SCORE_LIST_R	401									//最高分数消息列表重置
#define SUB_CS_S_HIGH_SCORE			500									//单个最高分数消息
#define SUB_CS_S_HIGH_SCORE_GET		600									//获取高分数消息
#define SUB_CS_S_USER_CHAT			700									//玩家聊天

//发送喇叭
struct CMD_CS_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          dwSendUserID;                         //用户 I D
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szSendNickName[32];				    //玩家昵称 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//发送喜报
struct CMD_CS_S_SendGladMsg
{
	TCHAR							szContent[TRUMPET_MAX_CHAR];		//内容
	TCHAR							szNickName[LEN_NICKNAME];			//昵称
	TCHAR							szNum[128];							//数字
	DWORD							colText;							//内容颜色
	DWORD							colName;							//昵称颜色
	DWORD							colNum;								//数字颜色
};

//好友聊天


//开赛服务
#define MDM_CS_MATCH_SERVICE       6


//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif