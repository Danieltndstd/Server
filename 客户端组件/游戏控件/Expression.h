#ifndef EXPRESSION_HEAD_FILE
#define EXPRESSION_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//类型定义

//类说明
class CExpression;
class CExpression2;
class CExpressionManager;
class CExpressionManager2;
class CExpressionControl;

//表情信息
struct tagExpressionInfo
{
	INT								nLength[3];							//描述长度
	TCHAR							szExpression[3][8];					//表情描述
};

//解释结果
struct tagTranslateResult
{
	//数据变量
	INT								nStartPos;							//开始位置
	INT								nDescribeLen;						//描述长度

	//对象变量
	CExpression *					pExpressionItem;					//表情信息
};

struct tagTranslateResult2
{
	//数据变量
	INT								nStartPos;							//开始位置
	INT								nDescribeLen;						//描述长度

	//对象变量
	CExpression2 *					pExpressionItem;					//表情信息
};
//数组定义
typedef CWHArray<CExpression *>		CExpressionArray;					//表情数组
typedef CWHArray<CExpression2 *>		CExpressionArray2;					//表情数组

//////////////////////////////////////////////////////////////////////////////////

//表情接口
interface IExpressionSink
{
	//选择表情
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//聊天表情
class SHARE_CONTROL_CLASS CExpression
{
	//友元定义
	friend class CExpressionControl;
	friend class CExpressionManager;

	//变量定义
protected:
	WORD							m_wIndex;							//索引标志
	CGIFImage						m_ExpressionItem;					//表情资源
	tagExpressionInfo				m_ExpressionInfo;					//表情信息

	//函数定义
protected:
	//构造函数
	CExpression();
	//析构函数
	virtual ~CExpression();

	//功能函数
public:
	//获取索引
	WORD GetIndex() { return m_wIndex; }
	//获取路径
	bool GetExpressionPath(LPTSTR pszImagePath, WORD wMaxCount);
	//设置表情
	bool SetExpressionInfo(WORD wIndex, tagExpressionInfo&ExpressionInfo);
};

//////////////////////////////////////////////////////////////////////////////////

//表情管理
class SHARE_CONTROL_CLASS CExpressionManager
{
	//组件变量
protected:
	CExpressionArray				m_ExpressionArray;					//表情数组

	//静态变量
protected:
	static CExpressionManager *		m_pExpressionManager;				//对象指针

	//函数定义
public:
	//构造函数
	CExpressionManager();
	//析构函数
	virtual ~CExpressionManager();

	//功能函数
public:
	//加载表情
	bool LoadExpression();
	//解释表情
	bool TranslateString(LPCTSTR pszString, tagTranslateResult&TranslateResult);

	//表情信息
public:
	//表情数目
	WORD GetExpressionCount();
	//枚举表情
	CExpression * GetExpressionItem(WORD wIndex);

	//静态函数
public:
	//获取对象
	static CExpressionManager * GetInstance() { return m_pExpressionManager; }
};

//////////////////////////////////////////////////////////////////////////////////

//表情窗口
class SHARE_CONTROL_CLASS CExpressionControl : public CDialog
{
	//变量定义
protected:
	WORD							m_wItemCount;							//表情数目
	WORD							m_wSelectIndex;							//选择索引

	//组件变量
protected:
	IExpressionSink *				m_pIExpressionSink;						//表情接口

	//函数定义
public:
	//构造函数
	CExpressionControl();
	//析构函数
	virtual ~CExpressionControl();

	//功能函数
public:
	//显示控件
	VOID ShowExpression(CWnd * pParentWnd, INT nXPos, INT nYPos, IExpressionSink * pIExpressionSink);

	//辅助函数
protected:
	//绘画头像
	VOID DrawExpression(CDC * pDC, WORD wItemIndex, bool bSelected);

	//消息函数
public:
	//重画消息
	VOID OnPaint();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//焦点消息
	VOID OnKillFocus(CWnd * pNewWnd);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};



//聊天表情
class SHARE_CONTROL_CLASS CExpression2
{
	//友元定义
	friend class CExpressionControl;
	friend class CExpressionManager2;

	//变量定义
protected:
	WORD							m_wIndex;							//索引标志
	CGIFImage						m_ExpressionItem;					//表情资源
	tagExpressionInfo				m_ExpressionInfo;					//表情信息
	CExpressionManager2				*m_pMgr;
	//函数定义
protected:
	//构造函数
	CExpression2(CExpressionManager2 *mgr);
	//析构函数
	virtual ~CExpression2();

	//功能函数
public:
	//获取索引
	WORD GetIndex() { return m_wIndex; }
	//获取图片
	CGIFImage* GetImage() {return &m_ExpressionItem;}
	//获取路径
	bool GetExpressionPath(LPTSTR pszImagePath, WORD wMaxCount);
	//设置表情
	bool SetExpressionInfo(WORD wIndex, tagExpressionInfo&ExpressionInfo);
	// 绘制表情
	void DrawImageFrame(CDC* pDC, INT x, INT y, INT_PTR nFrameIndex = 0L);
	// 获取表情真是
	INT GetImageFrames() ;

};

//////////////////////////////////////////////////////////////////////////////////

//表情管理
class SHARE_CONTROL_CLASS CExpressionManager2
{
	//组件变量
protected:
	CExpressionArray2				m_ExpressionArray;					//表情数组
	TCHAR							m_szDirectory[MAX_PATH];			//表情根路径

	//静态变量
protected:
	static CExpressionManager2 *		m_pExpressionManager;				//对象指针

	//函数定义
public:
	//构造函数
	CExpressionManager2();
	//析构函数
	virtual ~CExpressionManager2();

	//功能函数
public:
	//表情根路径
	LPCTSTR GetDirectory() const;
	//加载表情
	bool LoadExpression(LPCTSTR szDirectory);
	//解释表情
	bool TranslateString(LPCTSTR szDirectory, tagTranslateResult2&TranslateResult);

	//表情信息
public:
	//表情数目
	WORD GetExpressionCount();
	//枚举表情
	CExpression2 * GetExpressionItem(WORD wIndex);

	//静态函数
public:
	//获取对象
	static CExpressionManager2 * GetInstance() { return m_pExpressionManager; }
};
//////////////////////////////////////////////////////////////////////////////////

#endif