#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameGladMessage.h"
#include "UIUtils.h"


#define IDI_UPDATE		1212
#define TIME_UPDATE		20
#define FRAME_SIZE		5
#define BORDER_BOTTOM	16
#define ALPHA_COLORKEY	RGB(250,250,250)
#define MSG_MOVE_SPEED  3
#define MSG_STAND_COUNT 20

// 显示块
struct IBlock
{
	virtual ~IBlock() {}
	virtual CRect Draw(CDC* pDC, INT &x, INT &y, INT nWidth, INT nHeight) = 0;
};

// 静态图片显示
struct ImageBlock
	: public IBlock
{
public:
	CRect Draw(CDC* pDC, INT &x, INT &y, INT nWidth, INT nHeight)
	{
		CRect rc(0, 0, 0, 0);
		
		return rc;
	}
}; // ImageBlock

// GIF图片显示
struct GIFBlock
	: public IBlock
{
public:
	GIFBlock()
	{
		m_img	= 0;
		m_index = 0;
	}
	~GIFBlock()
	{
		//if (!img.IsNull())
		//{
		//	img.DestroyImage();
		//}
	} // ~GIFBlock

	CRect Draw(CDC* pDC, INT &x, INT &y, INT nWidth, INT nHeight)
	{
		if (m_img == 0)
		{
			return CRect(0, 0, 0, 0);
		}

		int xT = x;
		int yT = y + nHeight - m_img->GetHeight();
		m_img->DrawImageFrame(pDC, xT, yT, m_index);
		CRect rc(xT, yT, xT + m_img->GetWidth(), yT + m_img->GetHeight());
		x += m_img->GetWidth();

		if (m_dwStart != 0)
		{
			if (GetTickCount() - m_dwStart > 100)
			{
				m_dwStart = GetTickCount();
				m_index = (m_index + 1) % (int)m_img->GetFrameCount();
			}
		}
		else
		{
			m_dwStart = GetTickCount();
		}
		return rc;
	} // Draw

	void LoadFromeFile(LPCTSTR pszFileName)
	{
		//if (!img.IsNull())
		//{
		//	img.DestroyImage();
		//}

		//wprintf(TEXT("加载图片 %s\n"), pszFileName);

		//img.LoadFromFile(pszFileName);
	} // LoadFromeFile

	void SetExpression(CExpression2* pExpression)
	{
		m_img   = pExpression->GetImage();
		m_index = 0;
	}
private:
	CGIFImage* m_img;
	int		   m_index;
	int        m_dwStart;
}; // GIFBlock

// 文本显示
struct TextBlock
	: public IBlock
{
public:
	TextBlock()
		: m_pString(0)
		, m_iLen(0)
	{}
	~TextBlock()
	{
		if (m_pString != 0)
		{
			delete []m_pString;
			m_pString = 0;
			m_iLen    = 0;
		}
	}
	CRect Draw(CDC* pDC, INT &x, INT &y, INT nWidth, INT nHeight)
	{
		if (m_pString == 0)
		{
			return CRect(0, 0, 0, 0);
		}
		

		//RectF rc = GDIFont::GetBounds(m_pString, 0, 0, 18, FontStyleBold, L"幼圆");
		//int xT   = x;
		//int yT   = y + (nHeight - (int)rc.Height - 3);
		//GDIFont::Render(pDC->GetSafeHdc(), m_pString, xT, yT, 18, 3.0f, 0xFFFFFFFF, 0xFF000000, FontStyleBold, L"幼圆");

		//x += (int)rc.Width;

		//return CRect((int)rc.GetLeft(), (int)rc.GetTop(), (int)rc.GetRight(), (int)rc.GetBottom());
		CFont Font;
		Font.CreateFont(-24,0,0,0, 700,0,0,0,GB2312_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH|FF_MODERN,TEXT("幼圆"));
		pDC->SelectObject(&Font);
		int iOldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_crColor);

		CSize cs = pDC->GetTextExtent(m_pString, m_iLen);
		int xT = x;
		int yT = y + nHeight - cs.cy;
		CRect rc(xT, yT , xT + cs.cx, yT + cs.cy);

		pDC->DrawText(m_pString, m_iLen, &rc, 0);
		pDC->SetBkMode(iOldMode);
		x += cs.cx;
		return rc;
	}

	void SetText(LPCTSTR pszString, COLORREF crColor)
	{
		if (pszString == 0)
		{
			return;
		}

		if (m_pString)
		{
			delete []m_pString;
			m_pString = 0;
			m_iLen    = 0;
		}

		m_iLen = lstrlen(pszString);
		m_pString = new TCHAR[m_iLen + 1];
		lstrcpy(m_pString, pszString);
		m_pString[m_iLen] = TEXT('\0');
		m_crColor = crColor;
		m_dwColor = 0xFF000000 | (((crColor & 0xFF) << 16) & ((crColor & 0xFF00)) & ((crColor & 0xFF0000) >> 16));
	} // SetText

private:
	TCHAR*	 m_pString;
	int		 m_iLen;
	DWORD    m_crColor;
	DWORD    m_dwColor;
}; // TextBlock


// 换行显示
struct ReturnBlock
	: public IBlock
{
public:
	CRect Draw(CDC* pDC, INT &x, INT &y, INT nWidth, INT nHeight)
	{
		CRect rc(0, 0, 0, 0);

		return rc;
	}
}; // ReturnBlock

class CMsgBlock
{
public:
	CMsgBlock()
	{
		m_iState = -1;
		m_iOffsetX = 0;
	}

	~CMsgBlock()
	{
		for (int i = 0, l = (int)m_vecBlocks.size(); i < l; ++i)
		{
			delete m_vecBlocks[i];
		}

		m_vecBlocks.clear();
	} // ~CMsgBlock()

	void AddBlock(IBlock* block)
	{
		if (block == 0)
		{
			return;
		}

		m_vecBlocks.push_back(block);
	} // AddBlock


	void Draw(CDC* pDC, INT nWidth, INT nHeight)
	{
		if (m_iState == -1 || m_iState == 3)
		{
			return;
		}

		INT x = m_iX + m_iOffsetX;
		INT y = m_iY;
		INT w = 0;
		for (int i = 0, l = (int)m_vecBlocks.size(); i < l; ++i)
		{
			CRect rc = m_vecBlocks[i]->Draw(pDC, x, y, nWidth, nHeight);
			w += rc.Width();
		}

		//if (w > nWidth && m_iOffsetX + w <= nWidth)
		//{
		//	// 绘制尾巴
		//	INT x = m_iX + m_iOffsetX + w;
		//	INT y = m_iY;
		//	for (int i = 0, l = (int)m_vecBlocks.size(); i < l; ++i)
		//	{
		//		m_vecBlocks[i]->Draw(pDC, x, y, nWidth, nHeight);
		//	}
		//}

		if (w > nWidth && m_iState == 1 && (m_iStandBy--) <= 0 && (m_iOffsetX > nWidth - w))
		{
			if (m_iDir == 0)
			{
				m_iDir = MSG_MOVE_SPEED;
				m_dwDelayShow += 3 * (w - nWidth) * 1000 / (m_iDir * 150);
			}

			m_iOffsetX -= m_iDir;
			int iLimit  = nWidth - w;

			if (m_iOffsetX <= iLimit)
			{
				//m_iOffsetX -= iLimit;
				//m_iDir     = MSG_MOVE_SPEED;
				//m_iStandBy = MSG_STAND_COUNT;
			}
			//else if (m_iOffsetX >= 0)
			//{
			//	m_iOffsetX = 0;
			//	m_iDir     = -MSG_MOVE_SPEED;
			//	m_iStandBy = MSG_STAND_COUNT;
			//}
		}
	} // Draw

	void SetShowDelay(DWORD dwDelay)
	{
		m_dwDelayShow = dwDelay;
	} // SetShowDelay

	void SetXY(int x, int y)
	{
		m_iX = x;
		m_iY = y;
	} // SetXY

	void Born(int from, int to, float duration, DWORD dwDelay)
	{
		m_tween.Clear();
		m_tween.Add(from, to);
		m_tween.Start(duration, dwDelay);
		m_iState = 0;
	} // Born

	void Die(int to, float duration, DWORD dwDelay)
	{
		m_tween.Clear();
		m_tween.Add(m_iY, to);
		m_tween.Start(duration, dwDelay);
		m_iState = 2;
	} // Die

	bool IsDeath() const
	{
		return m_iState == 3;
	} // IsDeath

	void Update()
	{
		if (m_iState == -1 || m_iState == 3)
		{
			return;
		}

		bool bOver = m_tween.Update();
		m_iY = m_tween.GetValue(0);

		switch (m_iState)
		{
		case 0:
			{
				if (bOver)
				{
					m_iState   = 1;
					m_dwStart  = GetTickCount();
					m_iDir     = 0;
					m_iStandBy = MSG_STAND_COUNT;
				}
				break;
			}
		case 1:
			{
				if (GetTickCount() - m_dwStart > m_dwDelayShow)
				{
					Die(m_iY + 30, 0.2f, 0);
				}
				break;
			}
		case 2:
			{
				if (bOver)
				{
					m_iState = 3;
				}
				break;
			}
		}
	} // Update

	

private:
	DWORD				 m_dwStart;
	DWORD				 m_dwDelayShow;
	int					 m_iState;
	int					 m_iX;
	int                  m_iY;
	int					 m_iOffsetX;
	int					 m_iDir;
	int                  m_iStandBy;
	easing::CTween<int>  m_tween;
	std::vector<IBlock*> m_vecBlocks;
}; // CMsgBlock

//插入图片
IBlock* InsertGifImage(CExpression2* pExpression)
{
	GIFBlock* block = new GIFBlock();
	block->SetExpression(pExpression);
	return block;
} // InsertGifImage

IBlock* InsertString(LPCTSTR pszString, COLORREF crColor)
{
	TextBlock* block = new TextBlock();
	block->SetText(pszString, crColor);
	return block;
} // InsertString

// 解析字符串
CMsgBlock* TranslateString(LPCTSTR pszString, COLORREF crColor)
{
	CExpressionManager2* expressionMgr = CExpressionManager2::GetInstance();

	//翻译判断
	if (expressionMgr==NULL) return 0;

	CMsgBlock* msgBlock = new CMsgBlock();
	//变量定义
	tagTranslateResult2 TranslateResult;
	ZeroMemory(&TranslateResult,sizeof(TranslateResult));

	//字符变量
	INT nTranslateIndex=0;
	INT nStringLength=lstrlen(pszString);

	while ((nStringLength-nTranslateIndex)>0)
	{
		//解释字符
		bool bSuccess=expressionMgr->TranslateString(&pszString[nTranslateIndex],TranslateResult);

		//结果处理
		if (bSuccess==true)
		{
			//插入字符
			if (TranslateResult.nStartPos>0)
			{
				//变量定义
				TCHAR szString[LEN_USER_CHAT]=TEXT("");
				WORD wCopyCount=__min(TranslateResult.nStartPos,CountArray(szString));

				//构造字符
				szString[wCopyCount]=0;
				CopyMemory(szString,&pszString[nTranslateIndex],wCopyCount*sizeof(TCHAR));

				//插入字符
				msgBlock->AddBlock(InsertString(szString,crColor));
			}

			////构造路径
			//TCHAR ImagePath[MAX_PATH]=TEXT("");
			//TranslateResult.pExpressionItem->GetExpressionPath(ImagePath,CountArray(ImagePath));

			//插入表情
			msgBlock->AddBlock(InsertGifImage(TranslateResult.pExpressionItem));
			//设置索引
			nTranslateIndex+=TranslateResult.nStartPos+TranslateResult.nDescribeLen;
		}
		else
		{
			//插入字符
			msgBlock->AddBlock(InsertString(&pszString[nTranslateIndex],crColor));
			break;
		}

	}

	return msgBlock;
} // TranslateString


//////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGameGladMessage, CDialog)
	//系统消息
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

CGameGladMessage* CGameGladMessage::s_instance = 0;


CGameGladMessage* CGameGladMessage::GetInstance()
{
	return s_instance;
} // GetInstance

//构造函数
CGameGladMessage::CGameGladMessage() : CDialog(IDD_GAME_GLAD_MESSAGE)
{
	m_iAniState  = 0;
	m_IsTimering = false;
	s_instance   = this;
	return;
}

//析构函数
CGameGladMessage::~CGameGladMessage()
{
	s_instance = 0;
	return;
}

bool CGameGladMessage::InsertUserTyphon(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor)
{
	AddMsg(TranslateString(pszString, crColor));
	return true;
}

bool CGameGladMessage::InsertGladMessage(LPCTSTR pszContent, LPCTSTR pszNickName, LPCTSTR pszNumber, COLORREF colText, COLORREF colName,COLORREF colNum)
{
	int nLen = lstrlen(pszContent);

	if (nLen == 0)
	{
		return false;
	}

	CMsgBlock* msgBlock = new CMsgBlock();

	int iStrStart    = 0;
	int iStrLen      = 0;
	TCHAR szBuf[256] = TEXT("");

	//colText = RGB(255, 255, 255);
	//colName = RGB(255, 179, 0);
	//colNum  = RGB(255, 179, 0);

	for (int i = 0; i < nLen; ++i)
	{
		if (pszContent[i] == TEXT('%') && (i + 1 < nLen) && pszContent[i + 1] == TEXT('n'))
		{
			lstrcpyn(szBuf, &pszContent[iStrStart], iStrLen + 1);
			msgBlock->AddBlock(InsertString(szBuf, colText));
			msgBlock->AddBlock(InsertString(pszNickName, colName));
			i += 2;
			iStrStart = i;
			iStrLen   = 1;
		}
		else if (pszContent[i] == TEXT('%') && (i + 1 < nLen) && pszContent[i+1] == TEXT('s'))
		{
			lstrcpyn(szBuf, &pszContent[iStrStart], iStrLen + 1);
			msgBlock->AddBlock(InsertString(szBuf, colText));
			msgBlock->AddBlock(InsertString(pszNumber, colNum));
			i += 2;
			iStrStart = i;
			iStrLen   = 1;
		}
		else 
		{
			++iStrLen;
		}
	}

	if (iStrStart <= nLen - 1)
	{
		msgBlock->AddBlock(InsertString(&pszContent[iStrStart], colText));
	}

	AddMsg(msgBlock);

	return true;
}

void CGameGladMessage::AddMsg(CMsgBlock* msg)
{
	if (msg == 0)
	{
		return;
	}
	int sx = ImageTL.GetWidth() + FRAME_SIZE;
	int sy = -ImageTL.GetHeight();
	msg->SetXY(sx, sy);
	msg->SetShowDelay(8000);

	if (m_ltMsg.size() == 0)
	{
		int before  = m_twBar.GetSize() == 0 ? -ImageTL.GetHeight() : m_twBar.GetValue(0);
		m_iAniState = 1;
		m_twBar.Clear();
		m_twBar.Add(before, 0);
		m_twBar.Start(0.2f);
		msg->Born(0, 0, 0, 200);
	}
	else
	{
		DWORD dwDelay = 0;
		if (m_ltMsg.size() != 0)
		{
			dwDelay = 300;
			m_ltMsg.front()->SetShowDelay(dwDelay);
			dwDelay += 100;
		}

		msg->Born(sy, 0, 0.3f, dwDelay * m_ltMsg.size());	
	}

	m_ltMsg.push_back(msg);
	Render();
	if (!m_IsTimering)
	{
		m_IsTimering = true;
		KillTimer(IDI_UPDATE);
		SetTimer(IDI_UPDATE, TIME_UPDATE, 0);
		Render();
	}
	
} // AddMsg


//控件绑定
VOID CGameGladMessage::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	return;
}

//配置函数
BOOL CGameGladMessage::OnInitDialog()
{
	__super::OnInitDialog();

	ModifyStyle(WS_CAPTION | WS_BORDER | WS_DISABLED, 0, 0);
	ModifyStyle(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE, 0, 0);
	
	ModifyStyleEx(0, WS_EX_LAYERED);
	SetLayeredWindowAttributes(ALPHA_COLORKEY, 0, LWA_COLORKEY);

	Init();
	//SetTimer(IDI_UPDATE, TIME_UPDATE, 0);
	return FALSE;
}

//调整控件
VOID CGameGladMessage::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if ((nWidth==0)||(nHeight==0)) return;

	CRect rcClient(0, 0, nWidth, nHeight);

	if (m_rgnContent.GetSafeHandle() != NULL)
	{
		m_rgnContent.DeleteObject();
	}

	m_rgnContent.CreateRectRgn(ImageTL.GetWidth() + FRAME_SIZE, 0, nWidth - FRAME_SIZE - FRAME_SIZE, ImageTL.GetHeight() - BORDER_BOTTOM);

	//////////////////////////////////////////////////////////////////////////
	// 重建缓冲
	UIUtils::RebuildImage(m_imgBgBuffer, rcClient);
	UIUtils::RebuildImage(m_imgCanvas, rcClient);
	RenderBackground();
	Render();
	Invalidate(FALSE);
	return;
}


VOID CGameGladMessage::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1212:
		Render();
		Invalidate(FALSE);
		return;
	}

	__super::OnTimer(nIDEvent);
}


//绘画背景
BOOL CGameGladMessage::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

VOID CGameGladMessage::OnPaint()
{	
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	CPaintDC dc(this);
	dc.FillSolidRect(&rcClient, ALPHA_COLORKEY);

	if (m_iAniState == 0)
	{
		return;
	}

	//创建 DC
	CImageDC dcImg(m_imgCanvas);
	CDC * dcCanvas=CDC::FromHandle(dcImg);

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),dcCanvas,0,0,SRCCOPY);
}
//位置消息
VOID CGameGladMessage::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);
	return;
}


//////////////////////////////////////////////////////////////////////////
//
void CGameGladMessage::Init()
{
	static bool s_isInit = false;

	if (s_isInit)
	{
		return;
	}

	s_isInit = true;
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	ImageTL.LoadImage(hInstance,TEXT("GLAD_FRAME_TL"));
	ImageTM.LoadImage(hInstance,TEXT("GLAD_FRAME_TM"));
	ImageTR.LoadImage(hInstance,TEXT("GLAD_FRAME_TR"));
} // Init

void CGameGladMessage::Deinit()
{
	if (!m_imgBgBuffer.IsNull())
	{
		m_imgBgBuffer.Destroy();
	}

	if (!m_imgCanvas.IsNull())
	{
		m_imgCanvas.Destroy();
	}
} // Deinit

void CGameGladMessage::RenderBackground()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建 DC
	CImageDC BufferDC(m_imgBgBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	pBufferDC->FillSolidRect(&rcClient, ALPHA_COLORKEY);

	// 绘制地图
	int ltW = ImageTL.GetWidth();
	int mtW = ImageTM.GetWidth();
	int mtH = ImageTM.GetHeight();
	int rtW = ImageTR.GetWidth();

	int right = rcClient.Width() - rtW - FRAME_SIZE;

	if (mtW != 0)
	{
		//绘制MiddleTop
		for (int nXPos = ltW + FRAME_SIZE; nXPos < right; nXPos += mtW)
		{
			ImageTM.DrawImage(pBufferDC, nXPos, 0, min(mtW, right - nXPos), mtH, 0, 0);
		}
	}
	
	if (ltW != 0)
	{
		// 绘制LeftTop
		ImageTL.DrawImage(pBufferDC, FRAME_SIZE, 0);
	}

	if (rtW != 0)
	{
		// 绘制RightTop
		ImageTR.DrawImage(pBufferDC, right, 0);
	}
	
} // RenderBackground

void CGameGladMessage::Render()
{
	if (m_iAniState == 0)
	{
		return;
	}

	switch (m_iAniState)
	{
	case 0:
		return;
	case 1:
		{
			if (m_twBar.Update())
			{
				m_iAniState = 2;
			}
		}
		break;
	case 2:
		{
			
		}
		break;
	case 3:
		{
			if (m_twBar.Update())
			{
				m_iAniState = 0;
				m_IsTimering = false;
				KillTimer(IDI_UPDATE);
			}
		}
		break;
	}
	
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	CImageDC dcImg(m_imgCanvas);
	CDC * dcCanvas=CDC::FromHandle(dcImg);
	dcCanvas->FillSolidRect(&rcClient, ALPHA_COLORKEY);

	if (m_iAniState == 0)
	{
		return;
	}

	// 绘制背景
	CImageDC BufferDC(m_imgBgBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);
	dcCanvas->BitBlt(0, m_twBar.GetValue(0),rcClient.Width(),rcClient.Height(), pBufferDC, 0, 0,SRCCOPY);

	if (m_iAniState <= 1 || m_iAniState >= 3)
	{
		return;
	}

	dcCanvas->SelectClipRgn(&m_rgnContent);

	int w  = rcClient.Width() - ImageTL.GetWidth() - 3 * FRAME_SIZE;
	int h  = rcClient.Height() - BORDER_BOTTOM;

	std::list<CMsgBlock*>::iterator it = m_ltMsg.begin();
	std::list<CMsgBlock*>::iterator itNext;

	for (; it != m_ltMsg.end();)
	{
		itNext = it++;

		CMsgBlock* block = *itNext;
		block->Update();
		block->Draw(dcCanvas, w, h);

		if (block->IsDeath())
		{
			m_ltMsg.erase(itNext);
		}
	}

	dcCanvas->SelectClipRgn(NULL);

	if (m_ltMsg.size() == 0)
	{
		m_twBar.Clear();
		m_twBar.Add(0, -ImageTL.GetHeight());
		m_twBar.Start(0.2f, 100);
		m_iAniState = 3;
	}
} // Render

//控件颜色
HBRUSH CGameGladMessage::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}