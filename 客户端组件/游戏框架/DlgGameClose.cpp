// DlgGameClose.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgGameClose.h"
#include <afxcontrolbarutil.h>

// CDlgGameClose 对话框

IMPLEMENT_DYNAMIC(CDlgGameClose, CDialog)

CDlgGameClose::CDlgGameClose(LPTSTR msg /*=NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGameClose::IDD, pParent)
{
	m_strInfo = msg;
}

CDlgGameClose::~CDlgGameClose()
{
}

void CDlgGameClose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//按钮
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

BOOL CDlgGameClose::OnInitDialog()
{
	__super::OnInitDialog();

	//图片
	auto hResInstance = GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_ImageBack.LoadImage(hResInstance, TEXT("IDP_CLOSE_BACK"));

	//移动准备
	m_btOk.SetButtonImage(IDB_BT_OK, hResInstance, false, false);
	m_btCancel.SetButtonImage(IDB_BT_CANCEL, hResInstance, false, false);

	auto hDwp = BeginDeferWindowPos(32);
	auto uFlags = SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER;

	DeferWindowPos(hDwp, m_btOk, nullptr, m_ImageBack.GetWidth() / 2 - 125, m_ImageBack.GetHeight() - 43+5, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btCancel, nullptr, m_ImageBack.GetWidth() / 2 + 20, m_ImageBack.GetHeight() - 43+5, 0, 0, uFlags | SWP_NOSIZE);

	//移动结束
	EndDeferWindowPos(hDwp);

	//居中
	CPoint pPos;
	::ClientToScreen(GetParent()->m_hWnd, &pPos);

	//设置尺寸
	MoveWindow(pPos.x + 512 - m_ImageBack.GetWidth()/2, pPos.y + 369 - m_ImageBack.GetHeight()/2, m_ImageBack.GetWidth(), m_ImageBack.GetHeight());

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgGameClose, CDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlgGameClose 消息处理程序
void CDlgGameClose::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	//创建缓冲DC
	CMemDCEx	memDC(&dc, rc);

	//背景图
	m_ImageBack.DrawImage(&memDC, 0, 0);

	//文字
	CFont font;
	font.CreatePointFont(120, TEXT("宋体"));

	//选中
	memDC->SelectObject(&font);

	memDC->SetBkMode(TRANSPARENT);
	memDC->SetTextColor(RGB(255, 255, 0));
	memDC->DrawText(m_strInfo, CRect(16, 46, rc.right - 13, rc.bottom - 53), DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
} 

LRESULT CDlgGameClose::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT lResult = CDialog::OnNcHitTest(point);

	return HTCAPTION;
}
