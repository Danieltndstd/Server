#ifndef _UserListControl2_H_
#define _UserListControl2_H_

#include "GameFrameHead.h"

//用户列表
class CUserListControl2 : public CSkinListCtrlEx
{
	//状态变量
protected:
	bool							m_bCreateColumn;					//创建列项
	DWORD							m_dwServerRule;						//房间规则
	DWORD							m_dwMySelfUserID;					//自己信息

	//列表信息
protected:
	WORD							m_wColumnCount;						//列表数目
	BYTE							m_cbDataDescribe[MAX_COLUMN];		//数据描述

	//接口变量
protected:
	IGameLevelParser *				m_pIGameLevelParser;				//等级接口
	IGameUserManager *				m_pIGameUserManager;				//用户接口

	//资源变量
protected:
	tagColorUserList				m_ColorUserList;					//颜色定义
	static CImageList				m_ImageUserStatus;					//状态位图

	//函数定义
public:
	//构造函数
	CUserListControl2();
	//析构函数
	virtual ~CUserListControl2();

	//重载函数
public:
	//控件绑定
	virtual VOID PreSubclassWindow();

	//数据控制
protected:
	//获取位置
	virtual INT GetInsertIndex(VOID * pItemData);
	//排列数据
	virtual INT SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	//描述字符
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);

	//绘画控制
protected:
	//获取颜色
	virtual VOID GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF&crColorText, COLORREF&crColorBack);
	//绘画数据
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex);

	//扩展控制
protected:
	//用户解释
	virtual IClientUserItem * GetClientUserItem(VOID * pItemData);

	//配置函数
public:
	//设置接口
	bool SetServerRule(DWORD dwServerRule);
	//设置接口
	bool SetMySelfUserID(DWORD dwMySelfUserID);
	//设置接口
	bool SetGameLevelParser(IUnknownEx * pIUnknownEx);
	//设置接口
	bool SetGameUserManager(IUnknownEx * pIUnknownEx);

	//列表配置
public:
	//设置颜色
	bool SetColorUserList(tagColorUserList&ColorUserList);
	//设置列表
	bool SetColumnDescribe(tagColumnItem ColumnItem[], WORD wColumnCount);

	//消息映射
protected:
	//销毁消息
	VOID OnNcDestroy();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif