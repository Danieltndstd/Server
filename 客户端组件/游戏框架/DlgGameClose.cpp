// DlgGameClose.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgGameClose.h"
#include <afxcontrolbarutil.h>

// CDlgGameClose �Ի���

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

	//��ť
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

BOOL CDlgGameClose::OnInitDialog()
{
	__super::OnInitDialog();

	//ͼƬ
	auto hResInstance = GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_ImageBack.LoadImage(hResInstance, TEXT("IDP_CLOSE_BACK"));

	//�ƶ�׼��
	m_btOk.SetButtonImage(IDB_BT_OK, hResInstance, false, false);
	m_btCancel.SetButtonImage(IDB_BT_CANCEL, hResInstance, false, false);

	auto hDwp = BeginDeferWindowPos(32);
	auto uFlags = SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER;

	DeferWindowPos(hDwp, m_btOk, nullptr, m_ImageBack.GetWidth() / 2 - 125, m_ImageBack.GetHeight() - 43+5, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btCancel, nullptr, m_ImageBack.GetWidth() / 2 + 20, m_ImageBack.GetHeight() - 43+5, 0, 0, uFlags | SWP_NOSIZE);

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	//����
	CPoint pPos;
	::ClientToScreen(GetParent()->m_hWnd, &pPos);

	//���óߴ�
	MoveWindow(pPos.x + 512 - m_ImageBack.GetWidth()/2, pPos.y + 369 - m_ImageBack.GetHeight()/2, m_ImageBack.GetWidth(), m_ImageBack.GetHeight());

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgGameClose, CDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlgGameClose ��Ϣ�������
void CDlgGameClose::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	//��������DC
	CMemDCEx	memDC(&dc, rc);

	//����ͼ
	m_ImageBack.DrawImage(&memDC, 0, 0);

	//����
	CFont font;
	font.CreatePointFont(120, TEXT("����"));

	//ѡ��
	memDC->SelectObject(&font);

	memDC->SetBkMode(TRANSPARENT);
	memDC->SetTextColor(RGB(255, 255, 0));
	memDC->DrawText(m_strInfo, CRect(16, 46, rc.right - 13, rc.bottom - 53), DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
} 

LRESULT CDlgGameClose::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UINT lResult = CDialog::OnNcHitTest(point);

	return HTCAPTION;
}
