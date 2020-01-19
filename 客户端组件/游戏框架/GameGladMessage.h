#ifndef _GameGladMessage_H_
#define _GameGladMessage_H_

#include <list>
#include "GameFrameHead.h"
#include "../../../游戏组件/开发库/oops/easing/Easing.h"

class CMsgBlock;
//滚动消息
class CGameGladMessage : public CDialog
{
private:
	static CGameGladMessage* s_instance;
public:
	static CGameGladMessage* GetInstance();
	//函数定义
public:
	//构造函数
	CGameGladMessage();
	//析构函数
	virtual ~CGameGladMessage();

	// 添加大喇叭消息
	bool InsertUserTyphon(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor);
	// 喜报消息
	bool InsertGladMessage(LPCTSTR pszContent, LPCTSTR pszNickName, LPCTSTR pszNumber, COLORREF colText, COLORREF colName,COLORREF colNum);

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//配置函数
	virtual BOOL OnInitDialog();
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//
	VOID OnTimer(UINT_PTR nIDEvent);
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//绘制
	VOID OnPaint();
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//背景颜色
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	// 添加消息
	void AddMsg(CMsgBlock* msg);
	// 资源初始化
	void Init();
	// 资源释放
	void Deinit();	
	// 渲染背景
	void RenderBackground();
	// 渲染
	void Render();
private:
	// 背景
	CImage m_imgBgBuffer;
	CImage m_imgCanvas;

	CPngImage	ImageTL;							//位图对象
	CPngImage	ImageTM;							//位图对象
	CPngImage	ImageTR;							//位图对象

	CRgn m_rgnContent;								//内容裁剪区域
	// 数据
	std::list<CMsgBlock *> m_ltMsg;

	bool m_IsTimering;								//时间刷新
	// 信息条动画
	easing::CTween<int> m_twBar;
	// 动画状态
	int m_iAniState; 
	// 开始显示信息时间
	DWORD m_dwShowStart;	
};

//////////////////////////////////////////////////////////////////////////////////

#endif