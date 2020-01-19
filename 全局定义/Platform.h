#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//定义文件
#include "Macro.h"
#include "Define.h"

//结构文件
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//模板文件
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//程序版本
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)					//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(15,3,0)					//大厅版本
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(6,0,3)					//手机版本
#define VERSION_MOBILE_IOS			PROCESS_VERSION(6,0,3)					//手机版本

//版本定义
#define VERSION_EFFICACY			0										//效验版本
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)					//框架版本

//////////////////////////////////////////////////////////////////////////////////
//发布版本

//链接服务器地址
#define SERVER_LINK_ADDRESS			"127.0.0.1"	
//#define SERVER_LINK_ADDRESS			"182.254.220.84"	

#ifndef _DEBUG

//平台常量
const TCHAR	szProduct[]=TEXT("亿人公社");									//产品名字
const TCHAR szPlazaClass[]=TEXT("AQPGamePlaza");							//广场类名
const TCHAR szProductKey[]=TEXT("AQPGamePlatform");							//产品主键

//地址定义
const TCHAR szCookieUrl[]=TEXT(SERVER_LINK_ADDRESS);						//记录地址
const TCHAR szLogonServer[]=TEXT(SERVER_LINK_ADDRESS);						//登录地址
const TCHAR szPlatformLink[]=TEXT("http://127.0.0.1");							//平台网站

#else

//////////////////////////////////////////////////////////////////////////////////
//内测版本

//平台常量
const TCHAR	szProduct[]=TEXT("亿人公社");									//产品名字
const TCHAR szPlazaClass[]=TEXT("AQPGamePlaza");							//广场类名
const TCHAR szProductKey[]=TEXT("AQPGamePlatform");							//产品主键

//地址定义
const TCHAR szCookieUrl[]=TEXT(SERVER_LINK_ADDRESS);						//记录地址
const TCHAR szLogonServer[]=TEXT(SERVER_LINK_ADDRESS);						//登录地址
const TCHAR szPlatformLink[]=TEXT("http://127.0.0.1");					//平台网站

#endif

//////////////////////////////////////////////////////////////////////////////////

//数据库名
const TCHAR szPlatformDB[]=TEXT("QPPlatformDB");							//平台数据库
const TCHAR szAccountsDB[]=TEXT("QPAccountsDB");							//用户数据库
const TCHAR	szTreasureDB[]=TEXT("QPTreasureDB");							//财富数据库
const TCHAR	szExerciseDB[]=TEXT("QPExerciseDB");							//练习数据库
//const TCHAR szBillboardDB[]=TEXT("QPBillboard");						//排行榜数据库

//////////////////////////////////////////////////////////////////////////////////

//授权信息
const TCHAR szCompilation[]=TEXT("9E0F66DC-C919-4413-8C83-8759EF60E563");

//////////////////////////////////////////////////////////////////////////////////

#endif