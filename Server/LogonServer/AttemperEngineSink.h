#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "DataBasePacket.h"
#include "SensitiveWordsFilter.h"
#include "../redisdb/RedisClient.h"
#include <map>
#include <vector>
#include <functional>



using namespace std;

//////////////////////////////////////////////////////////////////////////////////
//群发掩码
#define BG_MOBILE					(BYTE)(0x01)						//手机群发
#define BG_COMPUTER					(BYTE)(0x02)						//电脑群发
#define BG_ALL_CLIENT				(BYTE)(0xFF)						//全体群发

//数据定义

//连接类型
#define CLIENT_KIND_FALSH			1									//网页类型
#define CLIENT_KIND_MOBILE			2									//手机类型
#define CLIENT_KIND_COMPUTER		3									//电脑类型
#define CLIENT_KIND_WEBSOCKET		4									//WS类型

//绑定参数
struct tagBindParameter
{
	//网络参数
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwActiveTime;						//激活时间

	//连接信息
	WORD							wModuleID;							//模块标识
	BYTE							cbClientKind;						//连接类型
	DWORD							dwPlazaVersion;						//大厅版本

	IServerUserItem *				pIServerUserItem;					//用户接口
};

//////////////////////////////////////////////////////////////////////////////////




//调度钩子
class CAttemperEngineSink : public IAttemperEngineSink,public IServerUserItemSink
{
	//友元定义
	friend class CServiceUnits;
	friend class CDataBaseEngineSink;

	//状态变量
protected:
	bool							m_bNeekCorrespond;					//协调标志
	bool                            m_bShowServerStatus;                //显示服务器状态	


	//变量定义
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagBindParameter *				m_pBindParameter;					//辅助数组

	//组件变量
protected:
	CServerListManager				m_ServerListManager;				//列表管理

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	IDataBaseEngine *				m_pIDataBaseEngine;					//数据引擎
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//网络引擎
	ITCPSocketService *				m_pITCPSocketService;				//协调服务

protected:
	CServerUserManager				m_ServerUserManager;				//用户管理

protected:
	map<WORD, function<bool(WORD, VOID*, WORD, DWORD)>> m_mMainFunctions;	//主命令函数
	map<WORD, function<bool(DWORD, VOID*, WORD)>>		m_mDbFunctions;		//数据库回调函数（部分函数采用提前声明参数）


	CSensitiveWordsFilter			m_WordsFilter;						//脏字过滤

protected:
	//KRedisConnParam					m_ConnParam;
	//CRedisClient					m_RedisClient;						//Redis实例

	CFileLog					m_fileLog;

	//函数定义
public:
	//构造函数
	CAttemperEngineSink();
	//析构函数
	virtual ~CAttemperEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//异步接口
public:
	//启动事件
	virtual bool OnAttemperEngineBegin(IUnknownEx * pIUnknownEx);
	//启动事件
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//事件接口
public:
	//控制事件
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//自定事件
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//内核事件
public:
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//连接事件
public:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//网络事件
public:
	//应答事件
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//关闭事件
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//读取事件
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//连接处理
protected:
	//注册事件
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//服务信息
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//远程服务
	bool OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//登录处理
	bool OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//列表处理
	bool OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//服务处理
	bool OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//远程处理
	bool OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//手机事件
protected:
	//登录处理
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//列表处理
	bool OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//服务处理
	bool OnTCPNetworkMainMBUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//比赛信息
	bool OnTCPNetworkMainMBMatchInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//发送房间
	void SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID);
	//发送类型
	VOID SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID);
	//发送比赛
	void SendMobileMatchInfo(DWORD dwSocketID, WORD wModuleID);
	//用户聊天
	bool OnTCPNetworkSubUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//敏感词过滤
	virtual void SensitiveWordFilter(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen);

	//数据库反馈
protected:
	//登录成功
	bool OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录成功
	bool OnDBWebLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录失败
	bool OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 查询银行
	bool OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行成功
	bool OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行失败
	bool OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//操作结果
	bool OnDBPCOperateResult(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//游戏种类
	bool OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏类型
	bool OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏节点
	bool OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏定制
	bool OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏列表
	bool OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//收藏列表
	bool OnDBPCGameCollection(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录成功
	bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录失败
	bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//三方注册
	bool OnDBThirdPartyRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//三方登录成功
	bool OnDBThirdPartySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//广播消息
	bool OnDBBroadcastMsg(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//喇叭消息
	bool OnDBBroadcastTyphon(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 签到
	bool OnDBTodayIsSignin(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//数据库信息处理（称号、好友、用户头像、用户信息、银行信息）
	bool OnDBEventDeal(DWORD dwContextID, VOID* pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID);
	//数据库通知协调服
	bool OnDBCorrespondEventDeal(DWORD dwContextID, VOID* pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID);


	//czg刷新通用属性
	bool OnDBUpdateAttribute(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//网络事件
protected:
	//I D 登录
	bool OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号登录
	bool OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//三方登录
	bool OnTCPNetworkSubPCLogonThirdparty(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//91登录
	bool OnTCPNetworkSubPCLogon91Sdk(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号注册
	bool OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号注册(手机)
	bool OnTCPNetworkSubPCRegisterAccountsMB(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//第三方注册
	bool OnTCPNetworkSubPCRegisterAccountsThird(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//获取验证码
	bool OnTCPNetworkSubPCGetValidateCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//QQ，微博三方登录
	bool OnTCPNetworkSubPCLogonThird(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Web 登录
	bool OnTcpNetworkSumWebRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//手机事件
protected:
	//I D 登录
	bool OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号登录
	bool OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号注册
	bool OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//游客登录
	bool OnTCPNetworkSubMBVisitorAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//手机注册(验证码)
	bool OnTCPNetworkSubMBRegisterMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID);

/************************************************************************/
/*                         B版事件                                      */
/************************************************************************/
	//手机事件
protected:
	bool OnAttemperEngineBegin_BCode(IUnknownEx * pIUnknownEx);
	//登录处理
	bool OnTCPNetworkMainMBLogon_BCode(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//I D 登录
	bool OnTCPNetworkSubMBLogonGameID_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号登录
	bool OnTCPNetworkSubMBLogonAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号注册
	bool OnTCPNetworkSubMBRegisterAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//游客登录
	bool OnTCPNetworkSubMBVisitorAccounts_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//手机注册(验证码)
	bool OnTCPNetworkSubMBRegisterMobile_BCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//心跳处理
	bool OnTCPNetworkMainHeart(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//数据库反馈
protected:
	// 登录成功
	bool OnDBMBLogonSuccess_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 失败通知
	bool OnDBMBLogonFailure_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 通知客户端失败消息
	bool OnDBMBFailure_BCode(DWORD dwSocketID, LPCTSTR strErrorString);
	// 获取签到返回
	bool OnDBMBGetUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 设置签到返回
	bool OnDBMBSetUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 获取转盘
	bool OnDBMBGetDailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 获得抽奖信息
	bool OnDBMBRotateDailAward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 绑定银行卡信息
	bool OnDBMBBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 绑定支付宝信息
	bool OnDBMBBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 请求邮件
	bool OnDBMBRequestMail(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 读取删除邮件
	bool OnDBMBReadDeleteMail(DWORD dwContextID, VOID * pData, WORD wDataSize);

protected:
	//客户端登录完成
	bool OnTCPNetworkMainPCUserService_ClientLoginSuccess(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//客户端退出房间
	bool OnTCPNetworkMainPCUserService_ClientExitRoom(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//获取签到
	bool OnTCPNetworkMainPCUserService_GetUserSignIn(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//签到
	bool OnTCPNetworkMainPCUserService_SetUserSignIn(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//获取用户完善信息
	bool OnTCPNetworkMainPCUserService_GetUserPerfectInformation(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//设置用户完善信息
	bool OnTCPNetworkMainPCUserService_SetUserPerfectInformation(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//绑定用户推广
	bool OnTCPNetworkMainPCUserService_BindUserSpread(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//是否已经绑定推广
	bool OnTCPNetworkMainPCUserService_IsBindSpread(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//是否已经修改过昵称
	bool OnTCPNetworkMainPCUserService_IsAlreadyModifyNickName(VOID * pData, WORD wDataSize, DWORD dwSocketID);
protected:
	//是否是掉线重新登录
	bool OnDBIsLostConnectionLogin(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//读取滚动消息
	bool OnDBLoadScrollMseeage(DWORD dwContextID, VOID * pData, WORD wDataSize);
/********************************End****************************************/


	//游戏事件
protected:
	//服务端通知数据库
	template<WORD, typename StructType>
	bool PostDataBaseRequest(DWORD dwContextID, VOID* pData, WORD wDataSize);

	//手机事件
protected:

	//数据库事件成功

	//辅助函数
protected:
	//版本检测
	bool CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer=true);
	//获取设备版本
	void GetDeviceVersion(BYTE cbDeviceType, WORD wModuleID, BYTE* cbArray);
	//手机大厅版本检测
	bool CheckMBPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, WORD wModuleID, DWORD dwSocketID);
	//发送请求
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);
	//切换连接
	bool SwitchUserItemConnect(IServerUserItem * pIServerUserItem, WORD wTargetIndex);

	//发送列表
protected:
	//发送类型
	VOID SendGameTypeInfo(DWORD dwSocketID);
	//发送种类
	VOID SendGameKindInfo(DWORD dwSocketID);
	//发送节点
	VOID SendGameNodeInfo(DWORD dwSocketID, WORD wKindID);
	//发送定制
	VOID SendGamePageInfo(DWORD dwSocketID, WORD wKindID);
	//发送房间
	VOID SendGameServerInfo(DWORD dwSocketID, WORD wKindID);
	//发送房间
	VOID SendGameServerMBInfo(DWORD dwSocketID, WORD wKindID);
	//发送收藏
	VOID SendCollectionInfo(DWORD dwSocketID, CMD_GP_Collection collection);

	//用户接口
public:
	//用户积分
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason){return true;}
	//用户状态
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR, BYTE cbNotifyClient = TRUE);
	//用户权限
	virtual bool OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight=true){return true;}
	//用户任务
	virtual bool OnEventUserItemTask(IServerUserItem *pIServerUserItem,WORD wTask,WORD wCount,BYTE IsSubmit=false){return true;}

	//网络接口
public:
	//发送数据
	bool SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//辅助函数
public:
	//绑定用户
	IServerUserItem * GetBindUserItem(DWORD dwSocketID);
	//绑定参数
	tagBindParameter * GetBindParameter(WORD wBindIndex);

protected:
	map<std::wstring,std::wstring>	m_ValidateCodeMap;//验证码
};

//////////////////////////////////////////////////////////////////////////////////

#endif