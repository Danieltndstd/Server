#ifndef IPC_GAME_FRAME_HEAD_FILE
#define IPC_GAME_FRAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//网络消息

#define IPC_CMD_GF_SOCKET			1									//网络消息

#define IPC_SUB_GF_SOCKET_SEND		1									//网络发送
#define IPC_SUB_GF_SOCKET_RECV		2									//网络接收

//网络发送
struct IPC_GF_SocketSend
{
	TCP_Command						CommandInfo;						//命令信息
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//数据缓冲
};

//网络接收
struct IPC_GF_SocketRecv
{
	TCP_Command						CommandInfo;						//命令信息
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//数据缓冲
};

//////////////////////////////////////////////////////////////////////////////////
//控制消息

#define IPC_CMD_GF_CONTROL			2									//控制消息

#define IPC_SUB_GF_CLIENT_READY		1									//准备就绪
#define IPC_SUB_GF_CLIENT_CLOSE		2									//进程关闭
#define IPC_SUB_GF_MATCH_READY		3									//比赛开始
#define IPC_SUB_GF_MATCH_OUT		4									//玩家淘汰
#define IPC_SUB_GF_MATCH_SIGNUP		5									//比赛报名
#define IPC_SUB_GF_MATCH_NULL		6									//玩家退赛
#define IPC_SUB_GF_MATCH_INFO		7									//比赛信息

#define IPC_SUB_GF_CLOSE_PROCESS	100									//关闭进程
#define IPC_SUB_GF_ACTIVE_PROCESS	101									//激活进程

#define IPC_SUB_GF_BOSS_COME		200									//老板来了
#define IPC_SUB_GF_BOSS_LEFT		201									//老板走了


#define CLIENT_MSG_TIPS				0									//只是消息提示

//客户端提示消息
struct IPC_GR_ClientMessage
{
	WORD							wType;								// 消息类型
	TCHAR							szMsg[128];							// 消息文本
};

//客户端分数不够关闭消息
struct IPC_GR_ClientCloseMessage
{
	TCHAR							szMsg[256];							// 消息文本
};


//////////////////////////////////////////////////////////////////////////////////
//配置消息

#define IPC_CMD_GF_CONFIG			3									//配置消息

#define IPC_SUB_GF_LEVEL_INFO		100									//等级信息
#define IPC_SUB_GF_COLUMN_INFO		101									//列表信息
#define IPC_SUB_GF_SERVER_INFO		102									//房间信息
#define IPC_SUB_GF_PROPERTY_INFO	103									//道具信息
#define IPC_SUB_GF_CONFIG_FINISH	104									//配置完成
#define IPC_SUB_GF_USER_RIGHT		107									//玩家权限
#define IPC_SUB_GF_URLS				108									//网页链接
#define	IPC_SUB_GF_LAYER_INFO		110									//分层消息

//房间信息
struct IPC_GF_ServerInfo
{
	//用户信息
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子号码
	DWORD							dwUserID;							//用户 I D

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//房间信息
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//视频配置
	WORD							wAVServerPort;						//服务端口
	DWORD							dwAVServerAddr;						//服务地址
};

//房间信息
struct IPC_GF_ServerInfoEx
{
	//用户信息
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子号码
	DWORD							dwUserID;							//用户 I D

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//房间信息
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//视频配置
	WORD							wAVServerPort;						//服务端口
	DWORD							dwAVServerAddr;						//服务地址

	LONGLONG						lCellScore;							//房间低分
	WORD							wServerTypeEx;						//房间类型
};


//等级信息
struct IPC_GF_LevelInfo
{
	BYTE							cbItemCount;						//列表数目
	tagLevelItem					LevelItem[64];						//等级描述
};

//列表配置
struct IPC_GF_ColumnInfo
{
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
};

//道具配置
struct IPC_GF_PropertyInfo
{
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具描述
};

//玩家权限
struct IPC_GF_UserRight
{
	DWORD							dwUserRight;						//玩家权限
};

//分层窗口信息
struct IPC_GF_LayerWndInfo
{
	WORD							wLayerType;
	DWORD							dwXPos;
	DWORD							dwYPos;
	DWORD							dwSrcWidth;
	DWORD							dwSrcHeight;
	DWORD							dwLayerHand;
};
//////////////////////////////////////////////////////////////////////////////////
//用户消息

#define IPC_CMD_GF_USER_INFO		4									//用户消息

#define IPC_SUB_GF_USER_ENTER		100									//用户进入
#define IPC_SUB_GF_USER_SCORE		101									//用户分数
#define IPC_SUB_GF_USER_STATUS		102									//用户状态
#define IPC_SUB_GF_USER_ATTRIB		103									//用户属性
#define IPC_SUB_GF_CUSTOM_FACE		104									//自定头像
#define IPC_SUB_GR_KICK_USER        105                                 //用户踢出
#define IPC_SUB_GR_USER_TASK		106									//用户任务
#define IPC_SUB_GR_USER_ITEM		107									//用户道具

//用户信息
struct IPC_GF_UserInfo
{
	BYTE							cbCompanion;						//用户关系
	tagUserInfoHead					UserInfoHead;						//用户信息
};

//用户积分
struct IPC_GF_UserScore
{
	DWORD							dwUserID;							//用户标识
	tagUserScore					UserScore;							//用户积分
};

//用户状态
struct IPC_GF_UserStatus
{
	DWORD							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态
};

//用户属性
struct IPC_GF_UserAttrib
{
	DWORD							dwUserID;							//用户标识
	tagUserAttrib					UserAttrib;							//用户属性
};

//用户头像
struct IPC_GF_CustomFace
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwCustomID;							//自定标识
	tagCustomFaceInfo				CustomFaceInfo;						//自定头像
};

//用户踢出
struct IPC_GF_KickUser
{
	DWORD                          dwTargetUserID;                      //目标用户
};

//用户任务
struct IPC_GF_UserTaskList
{
	DWORD							dwUserID;						
	tagUserTaskList					UserTaskList;
};

//用户道具
struct IPC_GF_UserItemList
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wCount;
	DWORD							dwUserID;
	tagGameItemInfo					GameItemInfo[8];
};
//////////////////////////////////////////////////////////////////////////////////
//道具消息
#define IPC_CMD_GF_PROPERTY_INFO		5								//道具消息

#define IPC_SUB_GF_BUY_PROPERTY 		100								//购买道具
#define IPC_SUB_GF_PROPERTY_SUCCESS		101								//道具成功
#define IPC_SUB_GF_PROPERTY_FAILURE     102                             //道具失败 
#define IPC_SUB_GR_PROPERTY_MESSAGE     103                             //道具消息
#define IPC_SUB_GR_PROPERTY_TRUMPET     104                             //道具喇叭
#define IPC_SUB_GR_PROPERTY_GLAD_MSG    105                             //滚动消息
#define IPC_SUB_GF_PROPERTY_CHAT_MSG	106								//喇叭消息
#define IPC_SUB_GF_PROPERTY_PING_MSG	107								//测速消息
#define IPC_SUB_GR_PROPERTY_GLAD_MSG2   108								//喜报消息

//购买道具
struct IPC_GF_BuyProperty
{
	WORD							wItemCount;                         //购买数目  
	WORD							wPropertyIndex;                     //道具索引 
	TCHAR							szNickName[LEN_NICKNAME];           //对方昵称
};

//使用道具
struct IPC_GR_PropertySuccess
{
	WORD							wPropertyIndex;                     //道具索引
	WORD                            wPropertyCount;                     //道具数目 
	DWORD                           dwSourceUserID;                     //用户 ID
	DWORD                           dwTargetUserID;                     //用户 ID
	TCHAR                           szSourceNickName[LEN_NICKNAME];     //用户昵称
};

//道具消息
struct IPC_GR_PropertyMessage
{
	WORD                           wPropertyIndex;                     //道具索引
	WORD                           wPropertyCount;                     //道具数目 
	TCHAR                          szSourceNickName[LEN_NICKNAME];     //用户昵称
	TCHAR                          szTargerNickName[LEN_NICKNAME];     //用户昵称
};

//道具失败
struct IPC_GR_PropertyFailure
{
	LONG						   lErrorCode;							//错误代码
	TCHAR						   szDescribeString[256];				//描述信息
};


//道具喇叭
struct IPC_GR_PropertyTrumpet
{
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD						   dwSendUserID;                        //用户 I D
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szSendNickName[32];				    //玩家昵称 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//喇叭消息
struct IPC_GR_PropertyGladMsg
{
	TCHAR							szContent[TRUMPET_MAX_CHAR];		//内容
	TCHAR							szNickName[32];						//昵称
	TCHAR							szNum[128];							//数字
	DWORD							colText;							//内容颜色
	DWORD							colName;							//昵称颜色
	DWORD							colNum;								//数字颜色
};

struct IPC_GR_PropertyNoticeMsg
{
	TCHAR							szAccount[32];
	TCHAR							szKindName[32];
	TCHAR							szScore[32];
};

struct IPC_GF_PropertyChatMsg
{
	WORD							wChannel;
	TCHAR							szNickName[128];
	TCHAR							szChatMsg[256];
};

//游戏音乐
#define IPC_SUB_GR_GAME_MUSIC			700									

struct IPC_GR_GameMusic
{
	unsigned short					volume;								//音量
	bool							effect;								//音效
	bool							sound;								//背景音乐
};

#define IPC_CMD_GR_CONTROL					10

#define IPC_SUB_GF_CLIENT_SHOW_FACE			904

struct IPC_GR_GameUserFace
{
	tagGameUserFace					GameUserFace[64];
	WORD							wCount;
};

struct IPC_GR_WndMove
{
	DWORD							dwX;
	DWORD							dwY;
};

//////////////////////////////////////////////////////////////////////////////////

#endif