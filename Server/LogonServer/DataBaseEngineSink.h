#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"
#include <map>
#include <functional>

//////////////////////////////////////////////////////////////////////////////////

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//用户数据库
protected:
	CDataBaseAide					m_AccountsDBAide;					//用户数据库
	CDataBaseHelper					m_AccountsDBModule;					//用户数据库

	//元宝数据库
protected:
	CDataBaseAide					m_TreasureDBAide;					//元宝数据库
	CDataBaseHelper					m_TreasureDBModule;					//元宝数据库

	//平台数据库
protected:
	CDataBaseAide					m_PlatformDBAide;					//平台数据库
	CDataBaseHelper					m_PlatformDBModule;					//平台数据库

	//组件变量
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

	//函数绑定
protected:
	std::map<WORD, std::function<bool(DWORD, VOID*, WORD)>>	m_mFunctions;	// 事件处理

	//经验
protected:
	std::map<WORD, DWORD>	m_mLobbyExp;	// Lobby经验
	std::map<WORD, DWORD>	m_mLandExp;		// Land经验
	std::map<WORD, DWORD>	m_mDLandExp;	// DLand经验
	std::map<WORD, DWORD>	m_mHLandExp;	// HLand经验

protected:
	

	//函数定义
public:
	//构造函数
	CDataBaseEngineSink();
	//析构函数
	virtual ~CDataBaseEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineBegin(IUnknownEx * pIUnknownEx);
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//内核事件
public:
	//时间事件
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//登录服务
protected:
	//I D 登录
	bool OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// Web注册
	bool OnRequestWebRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//三方登录
	bool OnRequesLogonThirdParty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//三方注册
	bool OnRequestRegisterThirdParty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//三方登录
	bool OnRequestLogonThird(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登出
	bool OnRequestLogonOut(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//手机登录
protected:
	//I D 登录
	bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//手机注册
	bool OnMobileRegisterMobile(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游客登录
	bool OnMobileVisitorAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//账号服务
protected:
	//修改机器
	bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改签名
	bool OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改昵称
	bool OnRequestModifyNickName(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//绑定手机
	bool OnRequestBindMobilePhone(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//找回密码
	bool OnRequestRecoveryPass(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//获取头像/形象
	bool OnRequestGetAvatarFaceInfo(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifyAvatar(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifyImage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改资料
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//游客转正
	bool OnRequestBindVisitorAccount(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//新找回密码（包含验证账号，验证密码功能）
	bool OnRequestRetrievePassword(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////


	//刷新积分
	bool OnReloadScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
protected:
	//属性更新
	bool OnUpdateAttribute(DWORD dwContextID, DBO_UPDATE_ATTRIBUTE::attribute attr,SCORE wScore );

	//存入元宝
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提取元宝
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转账元宝
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//信息查询
protected:
	//查询资料
	bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询银行
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询用户
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//系统功能
protected:
	//加载列表
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//在线信息
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏收藏
	bool OnRequestCollection(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//打开房间
	bool OnRequestOpenRoomStatus(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//结果处理
protected:
	// Web 登录结果
	VOID OnLogonDisposeWebResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, bool bVisitorClient = false);
	//登录结果
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bVisitorClient=false);
	//银行结果
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	/*
	* @dwContextID : 连接ID
	* @dwErrorCode : 错误代码(0:成功)
	* @pszErrorString : 描述消息
	* @wSubCommandID : 用途
	*/
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, WORD wSubCommandID);
	//第三方登录结果
	VOID OnLogonThirdPartyResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, DBR_GP_Insert_Accounts &insertAccounts);


/************************************************************************/
/*                         B版事件                                      */
/************************************************************************/
	//手机登录
protected:
	bool OnDataBaseEngineBegin_BCode(IUnknownEx * pIUnknownEx);
	//I D 登录
	bool OnMobileLogonGameID_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnMobileLogonAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnMobileRegisterAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//手机注册
	bool OnMobileRegisterMobile_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游客登录
	bool OnMobileVisitorAccounts_BCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//结果处理
protected:
	//登录结果
	VOID OnLogonDisposeResult_BCode(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bVisitorClient=false);

protected:
	//获取签到
	bool OnRequestGetSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置签到
	bool OnRequestSetSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取今天是否已经签到
	bool OnRequestTotayIsSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//获得转盘信息
	bool OnRequestGetDailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转动转盘获得奖励
	bool OnRequestRotateDailAward(DWORD dwContextID, VOID * pData, WORD wDataSize);

	// 绑定
	// 查询绑定银行卡
	bool OnRequestChickBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 绑定银行卡
	bool OnRequestBindBank(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 查询绑定支付宝
	bool OnRequestChickBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 绑定支付宝
	bool OnRequestBindZFB(DWORD dwContextID, VOID * pData, WORD wDataSize);

	// 邮件
	// 请求邮件
	bool OnRequestRequestMail(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 读取删除邮件
	bool OnRequestReadDeleteMail(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//获取用户完善信息
	bool OnRequestGetUserPerFectInformation(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置用户完善信息
	bool OnRequestSetUserPerFectInformation(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//是否是推广注册
	bool OnRequestIsSpreadRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//绑定用户推广
	bool OnRequestBindUserSpread(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//是否已经绑定推广
	bool OnRequestIsBindSpread(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//是否已经修改过昵称
	bool OnRequestIsAlreadyModifyNickName(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//是否是今天第一次登录
	bool OnRequestIsTodayFirstTimeLogin(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//是否是掉线重新登录
	bool OnRequestIsLostConnectionLogin(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//客户端登录完成
	bool OnRequestClientLoginSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	
	//清除卡线
	bool OnRequestKillLocker(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//读取滚动消息
	bool OnLoadScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//清除滚动记录
	bool OnClearScrollMessage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	

/********************************End****************************************/
};


//////////////////////////////////////////////////////////////////////////////////
// 数据库处理宏
#define PROCESS_BEGIN()	try{
#define PROCESS_END()	} \
	catch(IDataBaseException * pIException) \
{ \
	TRACE_LOG(pIException->GetExceptionDescribe(), TraceLevel_Exception); \
	OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), SUB_GP_OPERATE_FAILURE); \
	return false; \
} \
	return true;

//位运算宏
#define VALUE_OF_BIT(data,num)	((data&(1<<(num-1)))?1:0)   //获取某一位的值
#define SET_BIT_1(data,num)		data=(data|(1<<(num-1)))	//指定位变1
#define SET_BIT_0(data,num)		data=(data&(1<<(num-1)))	//指定位变0

#endif