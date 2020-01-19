// DlgRecordPlayZJG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgRecordPlayZJG.h"
#include "afxdialogex.h"
#include <numeric>

// DlgRecordPlayZJG �Ի���

//��ť
#define IDC_ZJG_RECORD_MIN		501
#define IDC_ZJG_RECORD_CLOSE	502
#define IDC_ZJG_RECORD_PLAY		503
#define IDC_ZJG_RECORD_PAUSE	504
#define IDC_ZJG_RECORD_OPEN		505

//������
#define IDC_ZJG_RECORD_SLIDER	601

IMPLEMENT_DYNAMIC(CDlgRecordPlayZJG, CDialog)

CDlgRecordPlayZJG::CDlgRecordPlayZJG(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecordPlayZJG::IDD, pParent)
{
	//��ʼ��˽������
	m_CurrentPos = 0;
	m_pParent = nullptr;
	m_AlreadyCount = 0;
}

CDlgRecordPlayZJG::~CDlgRecordPlayZJG()
{
}

void CDlgRecordPlayZJG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CDlgRecordPlayZJG::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//������Դ
	HINSTANCE hResInstance = GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_ImageBack.LoadImage(hResInstance, TEXT("IDP_RECORD_BACK"));
	m_ImageSliderBack.LoadImage(hResInstance, TEXT("IDP_RECORD_SLIDER_BACK"));
	m_ImageSlider.LoadImage(hResInstance, TEXT("IDP_RECORD_SLIDER"));
	m_ImageShoe.LoadImage(hResInstance, TEXT("IDP_RECORD_SHOE"));

	//��ȡ��������С
	m_SliderLength = m_ImageSlider.GetWidth();
	m_SliderHeight = m_ImageSlider.GetHeight();

	//��ʾ��ť��ʹ����Ч
	m_btRecordMin.SetButtonImage(IDB_BT_RECORD_MIN, hResInstance, false, false);
	m_btRecordClose.SetButtonImage(IDB_BT_RECORD_CLOSE, hResInstance, false, false);
	m_btRecordPlay.SetButtonImage(IDB_BT_RECORD_PLAY, hResInstance, false, false);
	m_btRecordPause.SetButtonImage(IDB_BT_RECORD_PAUSE, hResInstance, false, false);
	m_btRecordOpen.SetButtonImage(IDB_BT_RECORD_OPEN, hResInstance, false, false);
	m_btRecordMin.EnableWindow(TRUE);
	m_btRecordClose.EnableWindow(TRUE);
	m_btRecordPlay.EnableWindow(TRUE);
	m_btRecordPause.EnableWindow(TRUE);
	m_btRecordOpen.EnableWindow(TRUE);

	/*������ʾ*/
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BEGIN_MESSAGE_MAP(CDlgRecordPlayZJG, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ZJG_RECORD_MIN, OnBnClickedMin)
	ON_BN_CLICKED(IDC_ZJG_RECORD_CLOSE, OnBnClickedClose)
	ON_MESSAGE(WM_OPRATION, OnOperationDo)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CDlgRecordPlayZJG ��Ϣ�������

void CDlgRecordPlayZJG::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//��������DC
	if(m_dcMem.GetSafeHdc() == nullptr)
		m_dcMem.CreateCompatibleDC(&dc);

	CBitmap tmp;
	CRect rc;
	GetClientRect(&rc);
	tmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	m_dcMem.SelectObject(&tmp);
	tmp.DeleteObject();

	//����ͼ
	m_ImageBack.DrawImage(&m_dcMem, 0, 0);
	m_ImageSliderBack.DrawImage(&m_dcMem, 17, 61);

	//��������������
	int sWidth = m_ImageShoe.GetWidth();
	m_ImageSlider.DrawImage(&m_dcMem, 21, 63, m_CurrentPos, m_SliderHeight, 0, 0);
	m_ImageShoe.DrawImage(&m_dcMem,  21-sWidth/2 + min(352, m_CurrentPos), 57);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &m_dcMem, 0, 0, SRCCOPY);
}

void CDlgRecordPlayZJG::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnLButtonDown(nFlags, point);
	
	//�ж�����Ƿ��ڻ�����
	int nWidth = m_ImageShoe.GetWidth();
	int nLeft = 21-nWidth/2+m_CurrentPos;
	int nRight = nLeft + 14;
	CRect rc(nLeft, 57, nRight, 79);
	if(rc.PtInRect(point))
	{
		SetCapture();
		m_IsSelect = TRUE;
		m_DownPt = point;
	}
	else
		//����ʵ���ޱ������϶�
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}

void CDlgRecordPlayZJG::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_IsSelect = FALSE;

	ReleaseCapture();

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgRecordPlayZJG::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnMouseMove(nFlags, point);

	if(point.x < m_DownPt.x)
		return;

	if(m_IsSelect)
	{
		//��ǰλ��
		m_CurrentPos = min(352, point.x - m_DownPt.x + m_CurrentPos);
		m_DownPt = point;
		Invalidate(FALSE);
		if(m_pParent->m_hWnd)
			::SendMessage(m_pParent->m_hWnd, WM_OPRATIONEX, (WPARAM)(m_CurrentPos*m_dRatio), m_AlreadyCount);
	}
}

int CDlgRecordPlayZJG::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//������ť
	CRect rc(0, 0, 0, 0);
	m_btRecordMin.Create(nullptr, WS_CHILD | WS_VISIBLE | WS_DISABLED, rc, this, IDC_ZJG_RECORD_MIN);
	m_btRecordClose.Create(nullptr, WS_CHILD | WS_VISIBLE | WS_DISABLED, rc, this, IDC_ZJG_RECORD_CLOSE);
	m_btRecordPlay.Create(nullptr, WS_CHILD | WS_VISIBLE | WS_DISABLED, rc, this, IDC_ZJG_RECORD_PLAY);
	m_btRecordPause.Create(nullptr, WS_CHILD | WS_VISIBLE | WS_DISABLED, rc, this, IDC_ZJG_RECORD_PAUSE);
	m_btRecordOpen.Create(nullptr, WS_CHILD | WS_VISIBLE | WS_DISABLED, rc, this, IDC_ZJG_RECORD_OPEN);

	//���ð�ťλ��
	m_btRecordMin.SetWindowPos(nullptr, 295, 0, 46, 27, SWP_NOSIZE);
	m_btRecordClose.SetWindowPos(nullptr, 341, 0, 46, 27, SWP_NOSIZE);
	m_btRecordPlay.SetWindowPos(nullptr, 36, 103, 87, 31, SWP_NOSIZE);
	m_btRecordPause.SetWindowPos(nullptr, 159, 103, 87, 31, SWP_NOSIZE);
	m_btRecordOpen.SetWindowPos(nullptr, 276, 103, 87, 31, SWP_NOSIZE);

	return 0;
}

LRESULT CDlgRecordPlayZJG::OnOperationDo(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
		m_CurrentPos = m_SliderLength;
	else
	{
		UNREFERENCED_PARAMETER(lParam);
		SetPos((int)wParam);
	}

	m_AlreadyCount++;

	Invalidate(FALSE);

	return 0L;
}

void CDlgRecordPlayZJG::SetRange(vector<UINT>& vElapsed)
{
	if(m_SliderLength == 0 || vElapsed.empty())
		return;

	m_vElapsed = vElapsed;

	//������ʱ��
	auto timeSum = accumulate(vElapsed.begin(), vElapsed.end(), 0);

	//�������
	m_dRatio = ((double)timeSum) / m_SliderLength;
}

CDlgRecordPlayZJG& CDlgRecordPlayZJG::GetInstance()
{
	static CDlgRecordPlayZJG globalRecordDlg;
	return globalRecordDlg;
}
