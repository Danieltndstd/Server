// DlgGameInsureZJG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgGameInsureZJG.h"
#include "afxdialogex.h"

// CDlgGameInsureZJG �Ի���

#define IDC_ZJG_CLOSE			500
#define IDC_ZJG_LINK			501
#define IDC_ZJG_SURE			503
#define IDC_ZJG_ALL				504

//��ʱ��I D
#define IDI_CHARMVALUE_UPDATE_VIEW		2								//���½���
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//���½���

//////////////////////////////////////////////////////////////

//ת���ַ���
void MakeStringZJG(CString &strNum,SCORE lNumber)
{
	CString strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	for (int i = 0; i < int((nLength-1)/3); i++)
		strTempNum.Insert(nLength - 3*(i+1), _T(","));
	strNum.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
}

//ת������
void MakeStringToNumZJG(CString str,SCORE &Num)
{
	CString strNumber = str;
	strNumber.Remove(',');
	strNumber.Trim();

#ifndef _UNICODE
	_snscanf(strNumber.GetBuffer(),strNumber.GetLength(),_TEXT("%I64d"),&Num);
#else
	_snwscanf(strNumber.GetBuffer(),strNumber.GetLength(),_TEXT("%I64d"),&Num);
#endif
	strNumber.ReleaseBuffer();
}


//////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CDlgGameInsureZJG, CDialog)

CDlgGameInsureZJG::CDlgGameInsureZJG(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGameInsureZJG::IDD, pParent)
{
	m_OrInCount = 0L;
	m_bankGold  = 0L;
	m_helpCount = 0L;
}

CDlgGameInsureZJG::~CDlgGameInsureZJG()
{
}

void CDlgGameInsureZJG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZJG_MONEY, m_money);
	DDX_Control(pDX, IDC_ZJG_PASSWORD, m_password);
}


BEGIN_MESSAGE_MAP(CDlgGameInsureZJG, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ZJG_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_ZJG_LINK, OnBnClickedLink)
	ON_BN_CLICKED(IDC_ZJG_ALL, OnBnClickedAll)
	ON_BN_CLICKED(IDC_ZJG_SURE, OnBnClickedMakeSure)
	ON_EN_CHANGE(IDC_ZJG_MONEY, OnEnChangeEditMoney)
END_MESSAGE_MAP()


// CDlgGameInsureZJG ��Ϣ�������
BOOL CDlgGameInsureZJG::PreTranslateMessage(MSG* pMsg)
{
	if(GetFocus()==GetDlgItem(IDC_ZJG_MONEY)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue - '0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CDlgGameInsureZJG::OnInitDialog()
{
	CDialog::OnInitDialog();

	//���ý���λ��
	SetWindowPos(NULL, 0, 0, 353, 237, 0);
	GetDlgItem(IDC_ZJG_MONEY)->MoveWindow(164, 100, 136, 23, 0);
	GetDlgItem(IDC_ZJG_PASSWORD)->MoveWindow(164, 140, 136, 23, 0);

	ShowItem();

	CFont font;
	font.CreatePointFont(120, _T("����"));

	m_money.SetFont(&font);
	m_password.SetFont(&font);
	font.DeleteObject();

	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);
	

	m_pngNumber.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), _T("ZJGBANKNUM"));

	return TRUE;
}

void CDlgGameInsureZJG::SetPostPoint(CPoint Point)
{
	//����λ��
//  	CSize SizeRestrict;
//  	SizeRestrict.SetSize(353,237);
//  
//  	//����λ��
//  	CRect rcArce;
//  	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);
//   
//   	//�ƶ�����
//  	CRect rcNormalSize;
//   	rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
//   	rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
//   	rcNormalSize.right=rcNormalSize.left+SizeRestrict.cx;
//   	rcNormalSize.bottom=rcNormalSize.top+SizeRestrict.cy;
//  
//   	SetWindowPos(NULL,rcNormalSize.left,rcNormalSize.top,rcNormalSize.Width(),rcNormalSize.Height(),SWP_NOZORDER);

	CenterWindow();

}

void CDlgGameInsureZJG::OnPaint()
{
	CPaintDC dc(this);
}

int CDlgGameInsureZJG::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct)==-1) return -1;

	CRect rect(0, 0, 0, 0);
	m_btAll.Create(NULL, WS_CHILD|WS_VISIBLE|WS_DISABLED, rect, this, IDC_ZJG_ALL);	
	m_btClose.Create(NULL, WS_CHILD|WS_VISIBLE|WS_DISABLED, rect, this, IDC_ZJG_CLOSE);
	//m_btLink.Create(NULL, WS_CHILD|WS_VISIBLE|WS_DISABLED, rect, this, IDC_ZJG_LINK);
	m_btOkay.Create(NULL, WS_CHILD|WS_VISIBLE|WS_DISABLED, rect, this, IDC_ZJG_SURE);

	m_btAll.SetWindowPos(NULL, 303, 100, 46, 23,SWP_NOSIZE);
	//m_btLink.SetWindowPos(NULL, 270, 70, 61,14, SWP_NOSIZE);
	m_btClose.SetWindowPos(NULL, 315, 10, 28, 26, SWP_NOSIZE);
	m_btOkay.SetWindowPos(NULL, 125, 195, 105, 34, SWP_NOSIZE);

	return true;
}

void CDlgGameInsureZJG::FreshBank()
{
	//��������
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	QueryInsureInfo.cbActivityGame=TRUE;

	//��������
	SendSocketData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));

}

void CDlgGameInsureZJG::SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserDatas)
{
	//����Ч��
	ASSERT(pClientKernel!=NULL);
	if(pClientKernel==NULL) return;

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pMeUserData=const_cast<IClientUserItem *>(pMeUserDatas);

	FreshBank();
}

void CDlgGameInsureZJG::SetBankerActionType(bool bStorage)
{
	if (bStorage)
		m_blUsingPassWord = false;
	else
		m_blUsingPassWord = true;
}

void CDlgGameInsureZJG::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);

	SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0); 
}

void CDlgGameInsureZJG::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:
		{
			UpdateView();

			break;
		}

	default: 
		break;
	}
}

BOOL CDlgGameInsureZJG::OnEraseBkgnd(CDC * pDC)
{
	CDialog::OnEraseBkgnd(pDC);

	CDC dcMem;      
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bmpBackground;     
	bmpBackground.LoadBitmap(IDB_ZJG_MAINBAKER);
	dcMem.SelectObject(&bmpBackground);
	pDC->StretchBlt( 0, 0, 353, 255, &dcMem, 0, 0, 353, 255, SRCCOPY);

	DrawNumberString(pDC, m_bankGold, 165, 70);
	return TRUE;
}

void CDlgGameInsureZJG::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
{
	//ת������
	TCHAR szControl[128]=TEXT("");
	CString cs;

	//ת����ʽ
	MakeStringZJG(cs,lScore);
	_sntprintf(szControl,CountArray(szControl),TEXT("%s"),cs);

	//��������
	INT nXDrawPos=nXPos;
	INT nScoreLength=lstrlen(szControl);

	//�滭�ж�
	if (nScoreLength>0L)
	{
		//��ȡ��С
		CSize SizeNumber;
		SizeNumber.SetSize(m_pngNumber.GetWidth()/12L, m_pngNumber.GetHeight());

		//�滭����
		for (INT i=0;i<nScoreLength;i++)
		{
			//�滭����
			if (szControl[i]==TEXT(','))
			{
				m_pngNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*10L,0L);
			}

			//�滭���
			if (szControl[i]==TEXT('.'))
			{
				m_pngNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*11L,0L);
			}

			//�滭����
			if (szControl[i]>=TEXT('0')&&szControl[i]<=TEXT('9'))
			{
				m_pngNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*(szControl[i]-TEXT('0')),0L);
			}

			//����λ��
			nXDrawPos+=SizeNumber.cx;
		}
	}

	return;
}

void CDlgGameInsureZJG::UpdateBanker()
{
	Invalidate(TRUE);
}

void CDlgGameInsureZJG::UpdateView()
{
	//��ȡ����
	CString strinCount;
	GetDlgItemText(IDC_ZJG_MONEY,strinCount);
	SCORE OutNum = 0;
	MakeStringToNumZJG(strinCount,OutNum);

	//��������
	if(m_helpCount!= OutNum)
	{
		m_helpCount = OutNum;
		if(OutNum!=0)
		{
			MakeStringZJG(strinCount,OutNum);
			SetDlgItemText(IDC_ZJG_MONEY,strinCount);
			((CEdit*)GetDlgItem(IDC_ZJG_MONEY))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
		}
	}
}

void CDlgGameInsureZJG::ShowItem()
{
	HINSTANCE hInstance = GetModuleHandle(GAME_FRAME_DLL_NAME);	
	m_btAll.SetButtonImage(IDB_ZJG_ALL, hInstance, false, false);
	m_btClose.SetButtonImage(IDB_ZJG_CLOSE, hInstance, false, false);
	//m_btLink.SetButtonImage(IDB_ZJG_LINK, hInstance, false, false);
	m_btOkay.SetButtonImage(IDB_ZJG_MAKESURE, hInstance, false, false);

	m_btOkay.EnableWindow(TRUE);
	m_btClose.EnableWindow(TRUE);
	//m_btLink.EnableWindow(TRUE);
	m_btAll.EnableWindow(TRUE);
}

HBRUSH CDlgGameInsureZJG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_ZJG_MONEY:
	case IDC_ZJG_PASSWORD:
		{
			pDC->SetTextColor(RGB(222, 188, 125));
			pDC->SetBkMode(TRANSPARENT);             // ���ñ���͸��  
			return (HBRUSH)::CreateSolidBrush(RGB(51, 31, 16));  
		}
		
	default:
		break;;
	}

	return hBrush;
}

void CDlgGameInsureZJG::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//����Ч��
	ASSERT(m_pIClientKernel!=NULL);

	//������Ϣ
	if(m_pIClientKernel) m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
}

void CDlgGameInsureZJG::OnEnChangeEditMoney()
{
	//״̬Ч��
	if(!m_pMeUserData) return;

	//��ȡ����
	CString strCount;
	GetDlgItem(IDC_ZJG_MONEY)->GetWindowText(strCount);

	//ȥ��ǰ���0
	CString strLeft = strCount.Left(1);
	if(strLeft.Find('0') != -1)
	{
		strCount = strCount.Right(strCount.GetLength() - 1);
		SetDlgItemText(IDC_ZJG_MONEY,strCount);
	}

	//������Ϣ
	SCORE lInCount = m_bankGold;
	MakeStringToNumZJG(strCount, m_OrInCount);
	
	if((m_OrInCount > lInCount || m_OrInCount < 0L))
	{
		m_OrInCount = lInCount;
		CString strlGameGold;
		MakeStringZJG(strlGameGold, m_OrInCount);
		SetDlgItemText(IDC_ZJG_MONEY,strlGameGold);
	}
}

void CDlgGameInsureZJG::OnBnClickedAll()
{
	m_OrInCount = m_bankGold;
	CString strlGameGold;
	MakeStringZJG(strlGameGold, m_OrInCount);
	SetDlgItemText(IDC_ZJG_MONEY,strlGameGold);
}

void CDlgGameInsureZJG::OnBnClickedMakeSure()
{
	//����ת��
	CString strInCount;
	GetDlgItem(IDC_ZJG_MONEY)->GetWindowText(strInCount);
	MakeStringToNumZJG(strInCount, m_OrInCount);

	//������֤
	if ((m_OrInCount <= 0 || m_OrInCount > m_bankGold))
	{
		//������Ϣ
		CString strMessage;
		if(m_bankGold>0)strMessage.Format(TEXT("��������ڱ�ֵ������1��%I64d֮��"), m_bankGold);
		else strMessage.Format(TEXT("��Ĵ洢�ڱ���ĿΪ0,���ܽ�����ȡ����!"));

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(strMessage,MB_ICONERROR,30);

		GetDlgItem(IDC_ZJG_MONEY)->SetFocus();
		((CEdit*)GetDlgItem(IDC_ZJG_MONEY))->SetSel(0,-1);
		GetDlgItem(IDC_ZJG_PASSWORD)->SetWindowText(_TEXT(""));
		return;
	}

	//������֤
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	GetDlgItemText(IDC_ZJG_PASSWORD,szPassword,CountArray(szPassword));
	if (szPassword[0]==0&&m_blUsingPassWord)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���벻��Ϊ�գ��������������룡"),MB_ICONERROR,30);

		GetDlgItem(IDC_ZJG_PASSWORD)->SetWindowText(_TEXT(""));
		GetDlgItem(IDC_ZJG_PASSWORD)->SetFocus();
		return;
	}

	//�������
	if(m_blUsingPassWord)
	{
		TCHAR szTempPassword[LEN_PASSWORD]=TEXT("");
		CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
		CWHEncrypt::MD5Encrypt(szTempPassword,szPassword);
	}

	//��������
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));
	TakeScoreRequest.cbActivityGame	= TRUE;
	TakeScoreRequest.lTakeScore		= m_OrInCount;
	CopyMemory(TakeScoreRequest.szInsurePass, szPassword, sizeof(TakeScoreRequest.szInsurePass));

	//��������
	SendSocketData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	GetDlgItem(IDC_ZJG_PASSWORD)->SetWindowText(_TEXT(""));
}

void CDlgGameInsureZJG::OnBnClickedLink()
{

}

void CDlgGameInsureZJG::OnBnClickedClose()
{
	DestroyWindow();
}

