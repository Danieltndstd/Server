#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

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

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID				1								//I D 登录
#define SUB_GP_LOGON_ACCOUNTS			2								//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS		3								//注册帐号
#define SUB_GP_GET_VALIDATE_CODE		4								//获取验证码

#define SUB_GP_REGISTER_ACCOUNTS_MB		5								//注册帐号(针对手机端 无验证码)
#define SUB_GP_LOGON_THIRDPARTY			6								//三方登录
#define SUB_GP_REGISTER_ACCOUNTS_THIRD	7								//第三方注册
#define SUB_GP_LOGON_91SDK				8								//91登录

#define SUB_GP_WEB_REGISTER_ACCOUNTS	9								// WEB 消息登录

#define SUB_GP_LOGON_THIRD				14								//

//登录结果
#define SUB_GP_LOGON_SUCCESS		100									//登录成功
#define SUB_GP_LOGON_FAILURE		101									//登录失败
#define SUB_GP_LOGON_FINISH			102									//登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败
#define SUB_GP_VALIDATE_CODE		104                                 //验证码
#define SUB_GP_WEB_LOGON_FINISH		105									//登录完成

//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                                //效验低版本

//I D 登录
struct CMD_GP_LogonGameID
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE							cbValidateFlags;			        //校验标识
};

//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	BYTE							cbValidateFlags;			        //校验标识
};

//三方登录
struct CMD_GP_LogonThird
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwUserID;

	BYTE							cbValidateFlags;			        //校验标识
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

};

//三方登录
struct CMD_GP_LogonThirdParty
{
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
};

//91登录
struct CMD_GP_Logon91Sdk
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	BYTE							cbValidateFlags;			        //校验标识
	TCHAR							szValidateCode[LEN_VALIDATE_CODE];	//图形验证码
};

//第三方注册帐号
struct CMD_GP_RegisterThird
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	BYTE							cbValidateFlags;			        //校验标识
	TCHAR							szValidateCode[LEN_VALIDATE_CODE];	//图形验证码

	//第三方信息
	TCHAR							szThirdParty[LEN_ACCOUNTS];			//第三方帐号
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行

	//配置信息
	BYTE							cbMoorMachine;						//锁定机器
	BYTE                            cbShowServerStatus;                 //显示服务器状态
	BYTE							cbGender;							//用户性别
	
	//拓展信息
	//拓展信息
	wchar_t							szCompellation[LEN_COMPELLATION];				//真实姓名
	wchar_t							szQQ[LEN_QQ];								//QQ
	wchar_t							szEMail[LEN_EMAIL];							//电子邮件
	wchar_t							szSeatPhone[LEN_SEAT_PHONE];					//固定电话
	wchar_t							szMobilePhone[LEN_MOBILE_PHONE];				//手机号码
	wchar_t							szDwellingPlace[LEN_DWELLING_PLACE];			//联系地址
	wchar_t							szUserNote[LEN_USER_NOTE];					//用户备注

	//用户信息
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
};

//第三方登录成功
struct CMD_GP_ThirdPartySuccess
{
	//登录信息
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//登录失败
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//中断时间
	WORD							wOnLineCountTime;					//更新时间
};

//登录失败
struct CMD_GP_ValidateMBCard
{
	UINT							uMBCardID;						//机器序列
};

//升级提示
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

struct CMD_GP_Get_RoomInfo
{
	unsigned int                           dwUserID;
	unsigned int                           dwRoomNum;
	unsigned int                           dwServerID[32];
};

struct CMD_GP_RoomInfo
{
	unsigned int dwServerID;                                              //房间标识
	long long llCellScore;                                                //底注
	long long llMinEnterScore;                                            //最小进入分数
	long long llMaxEnterScore;                                            //最大进入分数
};

struct CMD_GP_ScoreInfo
{
	unsigned int dwWinCount;                                              //胜局数目
	unsigned int dwLostCount;                                             //败局数目
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//社团信息
#define DTP_GP_MEMBER_INFO			2									//会员信息
#define	DTP_GP_UNDER_WRITE			3									//个性签名
#define DTP_GP_STATION_URL			4									//主页信息
#define DTP_GP_ADDRESS_INFO			5									//地址信息

//社团信息
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//社团索引
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
};

//地址信息
struct DTP_GP_AddressInfo
{
	SYSTEMTIME						LastLogonDate;
	TCHAR							szLastClientAddr[32];
	TCHAR							szClientAddr[32];
	TCHAR							szMsgInfo[128];
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_ONLINE			3									//获取在线
#define SUB_GP_GET_COLLECTION		4									//获取收藏
#define SUB_GP_GET_ITEMSTORE		6									//道具商城
#define SUB_GP_GET_MATCH			7									//获取比赛

//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			104									//房间列表
#define SUB_GP_LIST_COLLECTION 		105									//收藏列表
#define SUB_GP_LIST_ITEMSTORE		107									//道具商城

//完成信息
#define SUB_GP_LIST_FINISH			200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成


//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//房间数目
	WORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

//道具商城
struct CMD_GP_ItemStore
{
	WORD						wCount;
	//tagItemStore				ItemStore[128];
};

//人数在线
struct CMD_GP_UserOnline
{
	DWORD							wRoomOnline;
	DWORD							wMatchOnline;
	DWORD							wActiveOnline;
};

#define COLLECTION_QUERY 0
#define COLLECTION_INSERT 1
#define COLLECTION_MODIFY 2
#define COLLECTION_DELETE 3

//游戏收藏
struct CMD_GP_Collection
{
	WORD						wFlag;
	WORD						wUserID;
	WORD						wSortID;
	TCHAR						DisplayName[LEN_KIND];
	WORD						wCollectionCount;
	tagGameCollection			Collection[MAX_KIND];
};

//////////////////////////////////////////////////////////////////////////////////
//服务命令
#define MDM_GP_USER_SERVICE			3									//用户服务

//账号服务
#define SUB_GP_CESHI				99									//测试消息
#define SUB_GP_MODIFY_MACHINE		100									//修改机器
#define SUB_GP_MODIFY_LOGON_PASS	101									//修改密码（原始密码，新密码）
#define SUB_GP_MODIFY_INSURE_PASS	102									//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE	103									//修改签名
#define SUB_GP_MODIFY_NICKNAME		104									//修改昵称
#define SUB_GP_PW_RECOVERY_ID		105									//通过身份证找回密码
#define SUB_GP_PW_RECOVERY_MOBILE	106									//通过手机找回密码
#define SUB_GP_PW_RECOVERY			107									//找回密码（新密码）
#define SUB_MB_GET_VARIFICATE_CODE	108									//获取验证码
#define SUB_GP_MODIFY_EMAIL			109									//修改绑定邮箱
#define SUB_GP_MODIFY_SECOND_PASS   110									//修改二级密码
#define SUB_GP_CHECK_SECOND_PASS	111									//核实二级密码
#define SUB_GP_RETRIEVE_PASSWORD    112									//新找回密码(包含验证账号，验证手机功能)

//新找回密码返回消息
#define SUB_GP_VERIFY_ACCOUNT			113									//验证账号
#define SUB_GP_VERIFY_MOBILE_PHONE		114									//验证手机
#define SUB_GP_VERIFY_VARIFICATE_CODE	115									//检测验证码和修改密码
//////////////////////////////////////////////////////////////////////////

//修改头像
#define SUB_GP_GET_AVATAR_INFO		200									//头像信息
#define SUB_GP_GET_IMAGE_INFO		201									//形象信息
#define SUB_GP_MODIFY_AVATAR		202									//修改头像
#define SUB_GP_MODIFY_IMAGE			203									//修改形象

//个人资料
#define SUB_GP_USER_INDIVIDUAL		301									//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL		302								//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL	303									//修改资料
#define SUB_GP_BIND_VISITOR_ACCOUNTS 304									//游客转正
#define SUB_GP_CHANGE_MOBILE		305									//请求更换手机
#define SUB_GP_BIND_MOBILE_PHONE	306									//绑定手机
#define SUB_GP_GET_HONOR			307									//请求称号
#define SUB_GP_SET_HONOR			308									//设置称号
#define SUB_GP_GET_PERSONAL_INFO    309									//获取个人信息（游戏内信息，昵称，等级，vip，签名，称号，技能，大厅等级，普通斗等级，欢乐斗等级....）

//client->server
#define SUB_GP_FEEDBACK_SYSTEM		310									//反馈信息

//server->client
#define SUB_GP_UPDATE_ATTRIBUTE		314									//czg更新属性

//银行服务
#define SUB_GP_USER_SAVE_SCORE		400									//存款操作
#define SUB_GP_USER_TAKE_SCORE		401									//取款操作
#define SUB_GP_USER_TRANSFER_SCORE	402									//转账操作
#define SUB_GP_USER_INSURE_INFO		403									//银行资料
#define SUB_GP_QUERY_INSURE_INFO	404									//查询银行
#define SUB_GP_USER_INSURE_SUCCESS	405									//银行成功
#define SUB_GP_USER_INSURE_FAILURE	406									//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST	407								//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT	408								//用户信息

//操作结果
#define SUB_GP_OPERATE_SUCCESS		900									//操作成功
#define SUB_GP_OPERATE_FAILURE		901									//操作失败
#define SUB_MB_VARIFICATE_RESULT	902									//获取验证码结果

//客户端-->服务器
#define SUB_GP_GET_USER_SIGNIN		500									//获取签到
#define SUB_GP_SET_USER_SIGNIN		501									//签到
#define SUB_GP_TODAY_IS_SIGNIN      502									//今天是否签到

// 转盘
#define SUB_GP_GET_DIAL_INFO		510									// 获取转盘信息
#define SUB_GP_DAIL_INFO_RESULT		511									// 转盘信息返回
#define SUB_GP_ROTATE_DAIL_AWAED	512									// 转动转盘领取奖励
#define SUB_GP_RPTATEDAILAWARD_RESULT 513								// 转动转盘领奖返回
#define SUB_GP_GET_DIAL_RESULT		520									// 获取签到返回
#define SUB_GP_SET_DIAL_RESULT		521									// 设置签到返回
#define SUB_GP_CHICK_BANKCARD		530									// 查询绑定银行卡
#define SUB_GP_BIND_BANKCARD		531									// 绑定银行卡
#define SUB_GP_CHICK_BIND_ZFB		532									// 查询绑定支付宝
#define SUB_GP_BIND_ZFB				533									// 绑定支付宝

#define SUB_GP_BANKCARDINFO			534									// 绑定银行卡信息返回
#define SUB_GP_ZFBBINDINFO			535									// 绑定支付宝信息返回

//广播服务
#define SUB_GP_BROADCAST_MSG		600									//广播消息
#define	SUB_GP_BROADCAST_TYPHON		601									//大小喇叭

#define SUB_GP_USER_DAILYGET		602									//低保领取
#define SUB_GP_REQUEST_MAIL			603									//请求邮件
#define SUB_GP_READ_MAIL			604									//读取邮件
#define SUB_GP_DELETE_MAIL			605									//删除邮件

#define SUB_GP_REQUEST_MAIL_RESULT				608									//请求邮件返回
#define SUB_GP_READ_DELETE_MAIL_RESULT			609									//读取删除邮件返回

#define SUB_GP_GET_STOCK_JEWEL		598									//实时库存
#define SUB_GP_GET_STOCK_SCORE		606									//实时库存
#define SUB_GP_GET_STORE			607									//普通商城
#define SUB_GP_GET_AUCTION			608									//拍卖商城
#define SUB_GP_BUY_STORE_ITEM		609									//购买商城道具
#define SUB_GP_BUY_AUCTION_ITEM		610									//购买拍卖道具
#define SUB_GP_ONSALE_AUCTION_ITEM	611									//上架拍卖道具
#define SUB_GP_OFFSALE_AUCTION_ITEM	612									//下架拍卖道具
#define SUB_GP_USE_ITEM				613									//使用道具
#define SUB_GP_GRANT_ITEM			614										//赠与道具
#define SUB_GP_GET_PACK				615									//背包获取
#define SUB_GP_GET_RANK				616									//获取排行
#define SUB_GP_GET_ITEMINFO			617									//获取道具信息
#define SUB_GP_GET_SELF_AUCTION		618									//获取个人拍卖
#define SUB_GP_EXTRACT_SCORE		619									//提取拍卖积分
#define SUB_GP_RESHELF_AUCTION_ITEM 599									//重新上架

//好友服务
#define SUB_GP_REQUEST_FRIEND		620									//申请好友
#define SUB_GP_REQUEST_REPLY		621									//好友申请（拒绝/同意）
#define SUB_GP_DELETE_FRIEND		622									//删除好友
#define SUB_GP_UPDATE_FRIEND		623									//刷新好友
#define SUB_GP_GET_REQUEST			624									//获取申请
#define SUB_GP_SEARCH_USER			625									//搜索用户
#define SUB_GP_REQUEST_NOTICE		626									//申请通知

#define SUB_GP_SEND_RED				634									//大厅发红包(客户端)
#define SUB_GP_LOBBY_SERVER_SENDRED		635									//大厅发红包（服务端）
#define SUB_GP_RECEIVE_RED              636									//抢红包
#define SUB_GP_RECEIVE_RED_RESULT       637									//抢红包结果
#define SUB_GP_BUY_POWER				638									//购买体力
#define SUB_GP_GET_EXP					640									//获取经验
#define SUB_GP_GET_TASK					641									//获取任务
#define SUB_GP_REFRESH_TASK				642									//刷新任务
#define SUB_GP_TAKE_TASK				643									//获取任务奖励
#define SUB_GP_GET_USERACHIEVE			644									//获取成就配置
#define SUB_GP_GET_USERWAGES			645									//领取成就工资
#define SUB_GP_GET_SPECIFICRACHIEVE		646									//获取具体成就
#define SUB_GP_SEND_POWER				647									//好友赠送东西
#define SUB_GP_ALTER_REMARK				648									//修改好友备注
#define SUB_GP_RECOMMENDFRIEND			649									//获取推荐好友
#define SUB_GP_GET_USERFAME				650									//获取荣誉配置
#define SUB_GP_GET_SPECIFICFAME			651									//获取具体荣誉

#define SUB_GP_USER_CHAT				652									//玩家聊天
#define SUB_GP_USER_EXPRESSION			653									//玩家表情
#define SUB_GP_BUY_VIP					654									//购买VIP

#define SUB_GP_RECOMMENDFRIEND2			655									//获取推荐好友
#define SUB_GP_SET_LEAVE_MESSAGE		656									//设置留言
#define SUB_GP_INVITE_FRIEND			657									//邀请好友游戏
#define SUB_GP_GRANT_SCORE				658									//赠送金币

#define SUB_GP_GET_USER_GEM				670									//获取宝石背包
#define SUB_GP_GET_USER_ORNAMENT		671									//获取饰品背包
#define SUB_GP_BURNISH_GEM				672									//打磨宝石
#define SUB_GP_CARRY_GEM				673									//携带宝石
#define SUB_GP_INPUT_GEM				674									//种植宝石
#define	SUB_GP_WEAR_ORNAMENT			675									//穿戴饰品
#define SUB_GP_GET_GEM_PRIZES			676									//收获奖品
#define SUB_GP_GET_GEM_FRIENDS			677									//获取宝石好友
#define SUB_GP_PUSH_RED					678									//推送红包
#define SUB_GP_RELOAD_SCORE			680									//刷新积分	

//房间基本信息
#define SUB_GP_GET_TIME_RANK_INFO		679									//获取定时排名奖励信息
#define SUB_GP_GET_ROOM_SUMMARIZE		681									//获取房间概述
#define SUB_GP_GET_USER_RESULTS			682									//获取用户战绩

#define SUB_GP_UPDATE_ITEM				685
#define SUB_GP_DELETE_ITEM				686

#define SUB_GP_GET_GAME_RECORDS			687									//获取用户游戏记录
#define SUB_GP_INPUT_INVITE_CODE		688									//输入邀请码
#define SUB_GP_GET_TODATY_SENDRED	    689									//获取今天发的红包
#define SUB_GP_GET_RECEIVE_RED_INFO	    690									//获取抢红包详情

//邀请码
#define SUB_GP_GET_USER_INVITE_CODE_INFO	691								//获取用户邀请码详细信息
#define SUB_GP_GET_USER_INVITE_CODE_REWARD_INFO	692							//获取用户邀请码奖励信息
#define SUB_GP_OBTAIN_INVITE_CODE_REWARD	693								//领取邀请码奖励
#define SUB_GP_GET_FEEDBACK_REWARD_STATUE	694								//获取回馈奖励的状态

#define SUB_GP_GET_USER_PERFECT_INFORMATION	    695							//获取用户完善信息
#define SUB_GP_SET_USER_PERFECT_INFORMATION     696							//设置用户完善信息

#define SUB_GP_IS_SPREAD_REGISTER     697									//是否是推广注册
#define SUB_GP_BIND_USER_SPREAD		  698									//绑定用户推广
#define SUB_GP_IS_BIND_SPREAD		  699									//是否已经绑定推广
#define SUB_GP_IS_ALREADY_MODIFY_NICKNAME 700								//是否已经修改过昵称
#define SUB_GP_IS_TODAY_FIRST_TIME_LOGIN 701								//是否是今天第一次登录
#define SUB_GP_IS_LOST_CONNECTION_LOGIN  702							        //是否是掉线重新登录
#define SUB_GP_SCROLL_MESSAGE				703						        //滚动消息



#define SUB_GP_FORCE_CLOSE				1000								//强制断开客户端

#define SUB_GP_CLIENT_LOGIN_SUCCESS		2000								//客户端登录完成
#define SUB_GP_CLIENT_EXIT_ROOM			2001								//客户端退出房间

//////////////////////////////////////////////////////////////////////////////////

//核实二级密码
struct CMD_GP_CheckSecondPass
{
	DWORD								dwUserID;
	TCHAR								szPassword[LEN_MD5];				//二级密码
};


// 购买商城道具
struct CMD_GP_BuyStoreItem
{
	DWORD							dwUserID;							// 用户ID
	DWORD							wItemID;							// 道具ID
	WORD							wItemCount;							// 道具数量
	BYTE							cbSaleType;							// 买单类型
};

// 购买拍卖道具
struct CMD_GP_BuyAuctionItem
{
	DWORD							dwUserID;							// 购买用户ID
	DWORD							dwAuctionID;							// 拍卖ID
	WORD							wItemCount;							// 道具数量
	BYTE							cbSaleType;							// 买单类型
};

// 上架拍卖道具
struct CMD_GP_OnSaleAuctionItem
{
	DWORD							dwUserID;							// 出售用户ID
	DWORD							dwItemPrice;						// 道具价格
	DWORD							dwPacketID;							// 背包ID
	WORD							wItemCount;							// 道具数量
};

//重新上架
struct CMD_GP_ReShelfAuctionItem
{
	DWORD							dwUserID;							// 出售用户ID
	DWORD							dwItemPrice;						// 道具价格
	WORD							wItemCount;							// 道具数量
	DWORD							dwAuctionID;						// 拍卖ID
};

//下架拍卖道具
struct CMD_GP_OffSaleAuctionItem
{
	DWORD							dwUserID;							// 出售用户ID
	DWORD							dwAuctionID;							// 拍卖ID
};

//查询道具信息
struct CMD_GP_QueryItemInfo
{
	DWORD							dwUserID;							// 用户ID
	DWORD							dwItemID;							// 道具ID
};

//获取拍卖商城
struct CMD_GP_GetAuction
{
	DWORD							dwUserID;
	BYTE							cbIndex;							//分页索引 从1开始
};


//提取拍卖积分


//道具刷新请求
struct CMD_GP_PackInfoRequest
{
	DWORD						dwUserID;
	DWORD						dwItemID;		//道具ID
	BYTE							IsAutoUse;
};


//使用道具
struct CMD_GP_UseItem
{
	WORD							wCount;							
	DWORD							dwPackID;						//背包ID
	DWORD							dwUserID;						//用户ID
	DWORD							dwFriendID;						//朋友ID
};


struct CMD_GP_BroadcastTyphon
{
	BYTE							cbType;								//3大喇叭所有房间 4小喇叭当前房间
	BYTE							cbSuccess;							//1 成功 0 失败
	DWORD							dwUserID;
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szContent[128];
};

//读取邮件
struct CMD_GP_ReadMail
{
	DWORD							dwUserID;
	DWORD							dwMailID;		//0全部 1单一
	BYTE							cbPowerMail;	//0体力 1非体力
	BYTE							cbMailType;		//0系统 1好友
};

//删除邮件
struct CMD_GP_DeleteMail
{
	DWORD							dwUserID;
	DWORD							dwMailID;		//0全部 1单一
	BYTE							cbPowerMail;	//0体力 1非体力
	BYTE							cbMailType;		//0系统 1好友
};

//广播消息
struct CMD_GP_BroadcastMsg
{
	tagBroadcastMsg				ArrayBroadcastMsg[10];
	WORD						wCount;
};

//低保结果
struct CMD_GP_DailyGetResult
{
	BYTE						cbSuccess;
	TCHAR						szMessage[128];
};

// 操作结果
struct CMD_GP_OperateResult
{
	BYTE						cbStringLength;
	WORD						wSubCommandID;
	TCHAR						szDescribe[128];
};

#define SIGNCOUNT 256
#define SIGNLEN	  10

//改变元宝
struct CMD_GP_CHANGE_SCORE
{
	DWORD							dwUserID;							//用户 I D
	SCORE							changeScore;						//用户金币
	SCORE							changeInsureScore;					//银行金币
};

//修改密码
struct CMD_GP_ModifyLogonPass
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户账号
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//设置二级密码
struct CMD_GP_ModifySecondPass
{

	DWORD							dwUserID;							//用户ID
	DWORD							operationType;						//操作类型，1：设置二级密码，2：修改二级密码
	TCHAR							szSecondPassword[LEN_PASSWORD];		//二级密码
	TCHAR							szLogonPassword[LEN_PASSWORD];		//登录密码
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号码
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE]; // 验证码

};

//修改签名
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//修改昵称
struct CMD_GP_ModifyNickName
{
	DWORD							dwUserID;							//用户 ID
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//修改密码
struct CMD_GP_ModifyPass
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户账号
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//////////////////////////////////////////////////////////////////////////////////

//修改头像
struct CMD_GP_ModifyAvatar
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwAvatarID;							//头像 I D
};

//修改形象
struct CMD_GP_ModifyImage
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwImageID;							//形像 I D
};

//获取头像/形象
struct CMD_GP_GetAvatarInfo
{
	DWORD							dwUserID;
	BYTE							cbAvatarType;
};

//游客转正
struct CMD_GP_BindVisitorAccount
{
	DWORD							dwUserID;
	TCHAR							szMachineID[LEN_MACHINE_ID];			//机器ID
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};

//更换手机请求
struct CMD_GP_ChangeMobilePhone
{
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE]; //验证码
};

//绑定手机请求
struct CMD_GP_BindMobilePhone
{
	DWORD							dwUserID;
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE]; //验证码
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//绑定邮箱请求
struct CMD_GP_BindEmail	
{
	DWORD							dwUserID;							
	TCHAR							szEmailAddress[LEN_EMAIL];			//绑定邮箱地址
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码或二级密码
};

//设置称号
struct CMD_GP_SetHonor
{
	DWORD							dwUserID;
	DWORD							dwHonorID;
};

//////////////////////////////////////////////////////////////////////////////////

//绑定机器
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//个人资料
struct CMD_GP_UserIndividual
{
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实姓名
	TCHAR							szPassportID[LEN_PASS_PORT_ID];		//身份证 I D
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//预留手机号
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;									//用户 I D
	TCHAR							szCompellation[LEN_COMPELLATION];			//真实姓名
	TCHAR							szPassportID[LEN_PASS_PORT_ID];				//身份证 I D
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];			//预留手机号
	TCHAR							szPassword[LEN_MD5];						//密码
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE];	//验证码
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//用户昵称
#define DTP_GP_UI_USER_NOTE			2									//用户说明
#define DTP_GP_UI_UNDER_WRITE		3									//个性签名
#define DTP_GP_UI_QQ				4									//Q Q 号码
#define DTP_GP_UI_EMAIL				5									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		6									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		7									//移动电话
#define DTP_GP_UI_COMPELLATION		8									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	9									//联系地址

//////////////////////////////////////////////////////////////////////////////////

//银行资料
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

//存入金币
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//远程服务
#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

/************************************************************************/
/*									手游定义                                              */
/************************************************************************/
//登录命令
#define MDM_MB_LOGON				100									//广场登录
#define MDM_MB_LOGON_BCode			102									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_MB_VISITOR_ACCOUNTS		4									//游客登录
#define SUB_MB_REGISTER_MOBILE		6									//注册帐号(手机号码注册)

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败
#define SUB_MB_LOGON_FINISH			102									//登录完成
#define SUB_MB_IS_LOST_CONNECTION_LOGIN 103								//是否断线重连


#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////
//列表命令
#define MDM_MB_SERVER_LIST		200									//列表信息

//列表结果
#define SUB_MB_LIST_KIND				101								//种类列表
#define SUB_MB_LIST_SERVER			104									//房间列表
#define SUB_MB_LIST_MATCH			105									//比赛列表
#define SUB_MB_LIST_FINISH			200									//列表完成
#define SUB_MB_LIST_MATCH_FINISH	201									//列表完成

#define SUB_MB_KINE_ONLINE			300									//类型在线

//比赛信息命令
#define	MDM_MB_MATCH_INFO		300									//比赛信息

#define MATCH_MB_INFO					100									//比赛信息（总）
#define	MATCH_MB_FEE					101									//比赛报名费
#define	MATCH_MB_FEE_AND_REQUIR			102									//比赛报名费，以及条件	
#define	MATCH_MB_DESCRIPTION			103									//比赛概述
#define	MATCH_MB_RULE					104									//比赛规则
#define	MATCH_MB_SOLUTION				105									//比赛解决方案
#define	MATCH_MB_REWARD					106									//比赛奖励
#define	MATCH_MB_END					107									//获取比赛信息结束
#define MATCH_MB_APPLY_STATUS			108									//比赛报名状态



//////////////////////////////////////////////////////////////////////////////////

//I D 登录
struct CMD_MB_LogonGameID
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//帐号登录
struct CMD_MB_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	BYTE							cbDeviceType;                       //设备类型

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//注册帐号
struct CMD_MB_RegisterAccounts
{
	//系统信息
	BYTE                            cbDeviceType;                       //设备类型

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//注册帐号
struct CMD_MB_RegisterAccounts_BCode
{
	//系统信息
	BYTE                            cbDeviceType;                       //设备类型

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	DWORD							dwSpreaderID;							//推广码
};

//游客登录
struct CMD_MB_LogonVisitor
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	BYTE							cbDeviceType;                       //设备类型

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szSpreader[LEN_NICKNAME];			//推广员账号
};

//获取验证码
struct CMD_MB_GetVarificateCode
{
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号码
	WORD							wSubCommandID;						//用途
};

//验证码结果
struct CMD_MB_VarificateResult
{
	WORD							wCode;								//结果
	WORD							wSubCommandID;						//用途
};

//手机注册
struct CMD_MB_RegisterMobile
{
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];		//手机号码
	TCHAR							szLogonPass[LEN_MD5];					//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];			//机器标识
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE];//验证码
};

//密码找回(身份证、手机号)
struct CMD_MB_PWRecoveryID
{
	TCHAR							szAccounts[LEN_ACCOUNTS];				// 用户账号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];			// 身份证
};

//密码找回(手机号)
struct CMD_MB_PWRecoveryMobile
{
	TCHAR							szAccounts[LEN_ACCOUNTS];				// 用户账号
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];		// 预留手机号
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE];// 验证码
};

//登录失败
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

//大厅发红包（客户端--服务端）
struct CMD_MB_LOBBY_SENDRED
{
	DWORD dwRedID;				//红包ID
	DWORD dwSendUserID;			//发红包玩家ID
	DWORD dwSendUserNum;		//红包总人数
	DWORD dwSendScore;			//红包金额
	BYTE  cbRedType;			//红包类型
	TCHAR szSendContent[128];	//红包附加内容
};


//抢红包
struct CMD_MB_GRAPRED
{
	DWORD dwSendUserID;		//发红包玩家ID
	DWORD dwGrapUserID;		//抢红包玩家ID
	DWORD dwGrapRedID;		//抢的红包ID
};

/*
//抢红包（数据库存储）
struct CMD_MB_DB_GRAPRED
{
	DWORD dwSendUserID;		//发红包玩家ID
	DWORD dwGrapUserID;		//抢红包玩家ID
	DWORD dwRecvScore;		//抢红包金额
	TCHAR szSendContent[128];	//红包附加内容
};
*/

struct CMD_MB_BuyVip
{
	DWORD	dwUserID;			
	DWORD	PayAmount;			//充值金额
};

//邀请好友
struct CMD_GP_InviteFriend
{
	DWORD							dwUserID;
	TCHAR							dwNickName[LEN_NICKNAME];	
	DWORD							dwFriendID;
	DWORD							dwRoomScore;						//房间入场
	WORD							wTableID;							//指定桌子
	WORD							wServerPort;						//房间端口
	TCHAR							szServerAddr[LEN_SERVER];			//房间地址
};

//留言信息
struct CMD_GP_LeaveMessage
{
	DWORD							dwUserID;							//用户ID
	WORD							dwRankType;							//排行榜类型
	WORD							dwLeaveMassageID;					//留言内容ID
};

//宝石背包
struct CMD_GP_GemPack
{
	DWORD							dwGemID;							//宝石ID
	BYTE							cbIsCarry;							//是否携带
	BYTE							cbIsInPut;							//是否放入
	DWORD							dwWearOrnamentID;					//已穿戴饰品ID
};

//饰品背包
struct CMD_GP_OrnamentPack
{
	DWORD							dwOrnamentID;						//饰品ID
	BYTE							cbIsWear;							//是否穿戴
};

//携带宝石
struct CMD_GP_CarryGem
{
	BYTE							cbCarry;							//1携带 0卸下
	DWORD							dwGemID;
	DWORD							dwUserID;
};

//放入宝石
struct CMD_GP_InPutGem
{
	BYTE							cbInPut;							//1放入 0收回
	DWORD							dwGemID;
	DWORD							dwUserID;
};

//打磨宝石
struct CMD_GP_BurnishGem
{
	DWORD							dwGemID;
	DWORD							dwUserID;
};

//穿戴饰品
struct CMD_GP_WearOrnament
{
	BYTE							cbWear;								//1穿戴 0脱下
	DWORD							dwOrnamentID;
	DWORD							dwUserID;
	DWORD							dwGemID;
};

//收获奖品
struct CMD_GP_GetGemPrize
{
	DWORD							dwPrizeType;						//0一键获取 1金币 4钻石 16特大分 88普斗 413095晶石
	DWORD							dwGemID;
	DWORD							dwUserID;
};

//找回密码（包含验证账号，验证手机功能）
struct CMD_GP_RetrievePassword
{
	BYTE							cbOperationType;						//操作类型，1验证账号，2验证手机，3检测验证码同时修改密码
	TCHAR							szAccounts[LEN_ACCOUNTS];				//用户账号
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];		//手机号码
	TCHAR							szNewLogonPassword[LEN_PASSWORD];			//新登录密码
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE];//验证码
};

//获取用户战绩
struct CMD_GP_GetUserResults
{
	DWORD dwServerID;		
	DWORD dwUserID;			//用户ID
};

//输入邀请码（接收别人邀请）
struct CMD_GP_InputInviteCode
{
	DWORD dwUserID;										//用户ID
	TCHAR dwInviteCode[LEN_INVITE_CODE];		//邀请码
};

//获取今天发的红包
struct CMD_GP_GetTodaySendRed
{
	DWORD dwSendUserID;				//发红包用户ID
	DWORD dwStartRecord;			//起始记录
	DWORD dwGetNum;					//获取数量
};

//获取抢红包详情
struct CMD_GP_GetReceiveRedInfo
{
	DWORD dwSendUserID;				//发红包用户ID
	DWORD dwRedID;					//红包ID
};

//领取邀请码奖励
struct CMD_GP_ObtainInviteCodeReward
{
	DWORD dwID;						//奖励ID
	DWORD dwUserID;					//用户ID
	DWORD dwFeedbackUserID;			//回馈的用户ID
};

//获取一项邀请码回馈奖励状态
struct CMD_GP_GetFeedbackRewardStatus
{
	DWORD dwID;						//奖励ID
	DWORD dwUserID;					//用户ID
};

//设置用户详细信息
struct CMD_GP_SetUserPerfectInformation
{
	DWORD dwUserID;							//用户ID
	TCHAR szCompellation[LEN_COMPELLATION];	//用户姓名
	TCHAR szIdentifyCode[50];				//手机号码
	TCHAR szVerificationCode[LEN_VARIFICATE_CODE]; // 验证码
};

//绑定用户推广
struct CMD_GP_BindUserSpread
{
	DWORD dwUserID;						//用户ID
	DWORD dwSpreadID;					//推广码
};


// 获取签到
struct CMD_GP_GetUserSignIn
{
	DWORD					dwUserID;				// 签到用户
};

// 签到
struct CMD_GP_SetUserSignIn
{
	DWORD					dwUserID;				// 签到用户
};



#pragma pack()

#endif

