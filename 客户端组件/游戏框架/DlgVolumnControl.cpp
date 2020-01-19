// DlgVolumnControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgVolumnControl.h"
#include "DSoundManager.h"
#include "afxdialogex.h"


// CDlgVolumnControl �Ի���

//��ť
#define IDC_VOLUMN_OK		601
#define IDC_VOLUMN_CANCEL	602
#define IDC_VOLUMN_CLOSE	603

//λ����Ϣ
#define LEFT_EDGE	110

IMPLEMENT_DYNAMIC(CDlgVolumnControl, CDialog)

CDlgVolumnControl::CDlgVolumnControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVolumnControl::IDD, pParent)
{
	m_bEffect = true;
	m_bMusic = true;
	m_bDefEffet = true;
	m_bDefMusic = true;
	m_CurVolume = 100;
	m_DefVolume = 100;

	m_pSoundMgr = nullptr;
	m_isSelect = FALSE;
}

CDlgVolumnControl::~CDlgVolumnControl()
{
}

void CDlgVolumnControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CDlgVolumnControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ȡ���ֹ�����ָ��
	m_pSoundMgr = CDSoundManager::GetHinstance();

	//������Դ
	HINSTANCE hResInstance = GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_ImageBack.LoadImage(hResInstance, TEXT("IDP_BACK"));
	m_ImageSliderBack.LoadImage(hResInstance, TEXT("IDP_SLIDER_BACK"));
	m_ImageSlider.LoadImage(hResInstance, TEXT("IDP_SLIDER"));
	m_ImageShoe.LoadImage(hResInstance, TEXT("IDP_SHOE"));
	m_ImageText1.LoadImage(hResInstance, TEXT("IDP_TEXT1"));
	m_ImageText2.LoadImage(hResInstance, TEXT("IDP_TEXT2"));
	m_ImageText3.LoadImage(hResInstance, TEXT("IDP_TEXT3"));
	m_ImageCheck1.LoadImage(hResInstance, TEXT("IDP_CHECK"));
	m_ImageCheck2.LoadImage(hResInstance, TEXT("IDP_CHECK"));

	//��ʾ��ť��ʹ����Ч
	m_btOk.SetButtonImage(IDB_VOLUMN_OK, hResInstance, false, false);
	m_btCancel.SetButtonImage(IDB_VOLUMN_CANCEL, hResInstance, false, false);
	m_btClose.SetButtonImage(IDB_VOLUMN_CLOSE, hResInstance, false, false);
	m_btOk.EnableWindow(TRUE);
	m_btCancel.EnableWindow(TRUE);
	m_btClose.EnableWindow(TRUE);

	//����λ��
	m_CurPos.X = 313;
	m_CurPos.Y = 110;
	m_DefPos.X = 313;
	m_DefPos.Y = 110;

	//��������
	m_bEffect = m_bDefEffet = m_pSoundMgr->sound_ffect;
	m_bMusic = m_bDefMusic = m_pSoundMgr->music_effect;
	m_CurVolume = m_DefVolume = m_pSoundMgr->volume_value;
	m_CurPos.X = max(96 + 217 * m_CurVolume / 100.0, 110);
	m_DefPos.X = max(96 + 217 * m_CurVolume / 100.0, 110);

	MoveWindow(0, 0, m_ImageBack.GetWidth(), m_ImageBack.GetHeight());

	//������ʾ
	CenterWindow();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgVolumnControl, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_VOLUMN_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_VOLUMN_CLOSE, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_VOLUMN_CANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

// CDlgVolumnControl ��Ϣ�������

BOOL CDlgVolumnControl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CDlgVolumnControl::OnPaint()
{
	CPaintDC dc(this);

	//��������DC
	if (m_dcMem.GetSafeHdc() == nullptr)
		m_dcMem.CreateCompatibleDC(&dc);

	CBitmap tmp;
	CRect rc;
	GetClientRect(&rc);
	tmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	m_dcMem.SelectObject(&tmp);
	tmp.DeleteObject();

	//����ͼ
	m_ImageBack.DrawImage(&m_dcMem, 0, 0);
	m_ImageSliderBack.DrawImage(&m_dcMem, 110, 115);
	m_ImageCheck1.DrawImage(&m_dcMem, 30, 48, 30, 33, m_bEffect ? 30 : 0, 0);
	m_ImageText1.DrawImage(&m_dcMem, 62, 57);
	m_ImageCheck2.DrawImage(&m_dcMem, 145, 48, 30, 33, m_bMusic ? 30 : 0, 0);
	m_ImageText2.DrawImage(&m_dcMem, 178, 57);
	m_ImageText3.DrawImage(&m_dcMem, 30, 115);

	//������
	m_ImageSlider.DrawImage(&m_dcMem, 110, 115, 217 * (m_CurVolume / 100.0), 20, 0, 0);
	m_ImageShoe.DrawImage(&m_dcMem, m_CurPos.X, m_CurPos.Y);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &m_dcMem, 0, 0, SRCCOPY);
}

void CDlgVolumnControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);

	//	�������ƶ�
	CRect rcClose(0, 0, 325, 35);			//�رհ�ť
	CRect rcEffect(31, 54, 59, 80);		//��Ч����
	CRect rcMusic(148, 54, 176, 80);	//���ֿ���
	CRect rcShoe(m_CurPos.X, m_CurPos.Y, m_CurPos.X + 14, m_CurPos.Y + 30);	//����

	if (rcClose.PtInRect(point))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	else if (rcEffect.PtInRect(point))
	{
		m_bEffect = !m_bEffect;

		m_pSoundMgr->sound_ffect = m_bEffect;
		m_pSoundMgr->Flush();

		Invalidate(FALSE);
	}
	else if (rcMusic.PtInRect(point))
	{
		m_bMusic = !m_bMusic;

		m_pSoundMgr->music_effect = m_bMusic;
		m_pSoundMgr->Flush();

		Invalidate(FALSE);
	}
	else if (rcShoe.PtInRect(point))	//ѡ�л���
	{
		SetCapture();
		m_isSelect = TRUE;
	}
}

void CDlgVolumnControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_isSelect = FALSE;

	ReleaseCapture();

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgVolumnControl::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);

	if (point.x < LEFT_EDGE)
		return;

	if (m_isSelect)
	{
		m_CurPos.X = min(313, point.x);

		m_CurVolume = (int)((m_CurPos.X - LEFT_EDGE) / 201.0 * 100);
		m_pSoundMgr->volume_value = m_CurVolume;
		m_pSoundMgr->Flush();

		Invalidate(FALSE);
	}
}

int CDlgVolumnControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//������ť
	CRect rc(0, 0, 0, 0);
	m_btOk.Create(nullptr, WS_CHILD | WS_VISIBLE | WS_DISABLED, rc, this, IDC_VOLUMN_OK);
	m_btCancel.Create(nullptr, WS_CHILD | WS_VISIBLE | WS_DISABLED, rc, this, IDC_VOLUMN_CANCEL);
	m_btClose.Create(nullptr, WS_CHILD | WS_VISIBLE | WS_DISABLED, rc, this, IDC_VOLUMN_CLOSE);

	//���ð�ťλ��
	m_btOk.SetWindowPos(nullptr, 76, 170, 83, 35, SWP_NOSIZE);
	m_btCancel.SetWindowPos(nullptr, 199, 170, 46, 35, SWP_NOSIZE);
	m_btClose.SetWindowPos(nullptr, 325, 2, 87, 29, SWP_NOSIZE);

	return 0;
}

void CDlgVolumnControl::OnBnClickedOk()
{
	//����Ĭ��ֵ
	m_bDefEffet = m_bEffect;
	m_bDefMusic = m_bMusic;
	m_DefVolume = m_CurVolume;
	m_DefPos = m_CurPos;

	//д��Ini�ļ�
	m_pSoundMgr->WriteSoundIni();

	//����
	ShowWindow(SW_HIDE);
}

void CDlgVolumnControl::OnBnClickedCancel()
{
	//�ظ��ϴ�ֵ
	m_bEffect = m_bDefEffet;
	m_bMusic = m_bDefMusic;
	m_CurVolume = m_DefVolume;
	m_CurPos = m_DefPos;

	//���ֹ�����ˢ��
	m_pSoundMgr->sound_ffect = m_bEffect;
	m_pSoundMgr->music_effect = m_bMusic;
	m_pSoundMgr->volume_value = m_CurVolume;

	m_pSoundMgr->Flush();

	//����
	ShowWindow(SW_HIDE);
}