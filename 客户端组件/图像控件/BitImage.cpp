#include "StdAfx.h"
#include "BitImage.h"
#include "D2DEngine.h"

#include "ResDecrypt.h"
#include "AesEncryptor.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CBitImage::CBitImage()
{
	ZeroMemory(m_strImageName, sizeof(m_strImageName) );
}

//析构函数
CBitImage::~CBitImage()
{
	//if ( CD2DEngine::GetD2DEngine() )
	//{
	//	CD2DEngine::GetD2DEngine()->DeleteImage(NULL, m_strImageName);
	//}
}

//创建资源
void CBitImage::LoadFromResource( HINSTANCE hInstance, UINT nIDResource )
{
	if ( CD2DEngine::GetD2DEngine() )
	{
		CD2DEngine::GetD2DEngine()->LaodImage(NULL, hInstance, nIDResource, m_strImageName);
	}
	CImage::LoadFromResource(hInstance, nIDResource);

}

//创建资源
void CBitImage::LoadFromResource( HINSTANCE hInstance, LPCTSTR pszResourceName )
{
	if ( CD2DEngine::GetD2DEngine() )
	{
		CD2DEngine::GetD2DEngine()->LaodImage(NULL, hInstance, pszResourceName, m_strImageName);
	}
	CImage::LoadFromResource(hInstance, pszResourceName);
}

void CBitImage::LoadFromResourceDes( HINSTANCE hInstance, UINT nIDResource )
{
	HRSRC hRsrc = ::FindResource(hInstance, MAKEINTRESOURCE(nIDResource), TEXT("ENCRYBITMAP"));

	if (hRsrc == nullptr) return;

	//资源
	DWORD rcSize = SizeofResource(hInstance, hRsrc);
	BYTE *pDataSize = (BYTE *)LoadResource(hInstance, hRsrc);

	//解密
	CAesEncryptor::aes_decry(pDataSize, pDataSize);
	DecryptData(pDataSize, rcSize);

	//创建数据
	IStream * pIStream=NULL;
	if (CreateStreamOnHGlobal(NULL,TRUE,&pIStream)!=S_OK)
	{
		ASSERT(FALSE);
		return ;
	}

	pIStream->Write(pDataSize, rcSize, nullptr);
	CImage::Load(pIStream);
}

void CBitImage::LoadFromResourceDes(HINSTANCE hInstance, LPCTSTR pszResourceName)
{
	HRSRC hRsrc =::FindResource(hInstance, pszResourceName, TEXT("ENCRYBITMAP"));

	if (hRsrc == nullptr) return;

	//资源
	DWORD rcSize = SizeofResource(hInstance, hRsrc);
	BYTE *pData = (BYTE *)LoadResource(hInstance, hRsrc);

	//解密
	CAesEncryptor::aes_decry(pData, pData);
	DecryptData(pData, rcSize);

	//创建数据
	IStream * pIStream=NULL;
	if (CreateStreamOnHGlobal(NULL,TRUE,&pIStream)!=S_OK)
	{
		ASSERT(FALSE);
		return ;
	}

	pIStream->Write(pData, rcSize, nullptr);
	CImage::Load(pIStream);
}

//创建区域
bool CBitImage::CreateImageRegion(CRgn & ImageRgn, COLORREF crTransColor)
{
	//创建区域
	CreateImageRegion(ImageRgn,0,0,GetWidth(),GetHeight(),crTransColor);

	return true;
}

//创建区域
bool CBitImage::CreateImageRegion(CRgn & ImageRgn, INT nXSrcPos, INT nYSrcPos, INT nSrcWidth, INT nSrcHeight, COLORREF crTransColor)
{
	//判断状态
	ASSERT(IsNull()==false);
	if (IsNull()==true) return false;

	//参数调整
	INT nImageWidht=GetWidth();
	INT nImageHeight=GetHeight();
	if ((nXSrcPos+nSrcWidth)>nImageWidht) nSrcWidth=nImageWidht-nXSrcPos;
	if ((nYSrcPos+nSrcHeight)>nImageHeight) nSrcHeight=nImageHeight-nYSrcPos;

	//创建对象
	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);
	ImageRgn.CreateRectRgn(0,0,0,0);

	//创建位图
	CImage ImageConsult;
	ImageConsult.Create(nSrcWidth,nSrcHeight,32);

	//调整位图
	BITMAP ImageInfo;
	GetObject(ImageConsult,sizeof(ImageInfo),&ImageInfo);
	while ((ImageInfo.bmWidthBytes%4)!=0) ImageInfo.bmWidthBytes++;

	//绘画位图
	BitBlt(ImageConsult.GetDC(),0,0,nSrcWidth,nSrcHeight,nXSrcPos,nYSrcPos);
	ImageConsult.ReleaseDC();

	//获取属性
	BYTE cbRTranColor=GetRValue(crTransColor);
	BYTE cbGTranColor=GetGValue(crTransColor);
	BYTE cbBTranColor=GetBValue(crTransColor);
	BYTE * pcbImageBitIndex=(BYTE *)ImageInfo.bmBits+(ImageInfo.bmHeight-1)*ImageInfo.bmWidthBytes;

	//创建区域
	for (INT nYPos=0;nYPos<ImageInfo.bmHeight;nYPos++)
	{
		for (INT nXPos=0;nXPos<ImageInfo.bmWidth;nXPos++)
		{
			//变量定义
			INT nXStartPos=nXPos;

			//透明判断
			do
			{
				COLORREF crImageColor=*(((LONG *)pcbImageBitIndex)+nXPos);
				if ((cbRTranColor==GetRValue(crImageColor))&&(cbGTranColor==GetGValue(crImageColor))&&(cbBTranColor==GetBValue(crImageColor))) break;
			} while ((++nXPos)<ImageInfo.bmWidth);

			//创建区域
			if ((nXPos-nXStartPos)>0)
			{
				//创建区域
				CRgn RgnUnite;
				RgnUnite.CreateRectRgn(nXStartPos,nYPos,nXPos,nYPos+1);

				//合并区域
				CombineRgn(ImageRgn,RgnUnite,ImageRgn,RGN_OR);
			}
		}

		//设置位置
		pcbImageBitIndex-=ImageInfo.bmWidthBytes;
	}

	return true;
}

//平铺绘画
bool CBitImage::DrawImageTile(CDC * pDC, CRect & rcDestRect)
{
	//平铺绘画
	DrawImageTile(pDC,rcDestRect.left,rcDestRect.top,rcDestRect.Width(),rcDestRect.Height());

	return true;
}

//平铺绘画
bool CBitImage::DrawImageTile(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//效验状态
	ASSERT(IsNull()==false);
	if (IsNull()==true) return false;

	//无效区域
	CRect rcDirty;
	pDC->GetClipBox(&rcDirty);

	//变量定义
	INT nImageWidth=GetWidth();
	INT nImageHeight=GetHeight();

	//平铺绘画
	for (INT nXDrawPos=0;nXDrawPos<nWidth;nXDrawPos+=nImageWidth)
	{
		for (INT nYDrawPos=0;nYDrawPos<nHeight;nYDrawPos+=nImageHeight)
		{
			//计算位置
			INT nDrawWidth=__min(nWidth-nXDrawPos,nImageWidth);
			INT nDrawHeight=__min(nHeight-nYDrawPos,nImageHeight);

			//绘画图形
			bool bIntersect=IsRectIntersect(rcDirty,nXDrawPos+nXPos,nYDrawPos+nYPos,nDrawWidth,nDrawHeight);
			if (bIntersect==true) BitBlt(pDC->m_hDC,nXDrawPos+nXPos,nYDrawPos+nYPos,nDrawWidth,nDrawHeight,0,0);
		}
	}

	return true;
}

//一般绘画
BOOL CBitImage::BitBlt( HDC hDestDC, int xDest, int yDest, DWORD dwROP /*= SRCCOPY*/ )
{
	return BitBlt(hDestDC, xDest, yDest, GetWidth(), GetHeight(), 0, 0);
}

//一般绘画
BOOL CBitImage::BitBlt( HDC hDestDC, const POINT& pointDest, DWORD dwROP /*= SRCCOPY*/ )
{
	return BitBlt(hDestDC, pointDest.x, pointDest.y, GetWidth(), GetHeight(), 0, 0);
}

//一般绘画
BOOL CBitImage::BitBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP /*= SRCCOPY*/ )
{
	if ( CD2DEngine::GetCurrentWnd() && CD2DEngine::GetD2DEngine() )
	{
		return CD2DEngine::GetD2DEngine()->DrawImage(CD2DEngine::GetCurrentWnd(),m_strImageName,xDest,yDest,nDestWidth,nDestHeight,xSrc,ySrc,255);
	}
	else
	{
		return CImage::BitBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, xSrc, ySrc, dwROP );
	}
}

//一般绘画
BOOL CBitImage::BitBlt( HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, DWORD dwROP /*= SRCCOPY*/ )
{
	return BitBlt(hDestDC, rectDest.left, rectDest.top, rectDest.right - rectDest.left, rectDest.bottom - rectDest.top, pointSrc.x, pointSrc.y);
}

//透明绘画
bool CBitImage::TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor)
{
	//绘画图片
	TransDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,crTransColor);

	return true;
}

//透明绘画
bool CBitImage::TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor)
{
	if ( CD2DEngine::GetCurrentWnd() && CD2DEngine::GetD2DEngine() )
	{
		return CD2DEngine::GetD2DEngine()->DrawImage(CD2DEngine::GetCurrentWnd(),m_strImageName,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cxDest,cyDest,crTransColor, 255);
	}

	//无效区域
	CRect rcDirty;
	pDestDC->GetClipBox(&rcDirty);

	//绘画判断
	if (IsNull()==true) return false;
	if (IsRectIntersect(rcDirty,xDest,yDest,cxDest,cyDest)==false) return true;

	//位置调整
	tagImageRender ImageRender;
	GetDrawImageArea(xDest,yDest,cxDest,cyDest,xSrc,ySrc,rcDirty,ImageRender);

	//颜色保存
	COLORREF crTextColor=pDestDC->SetTextColor(RGB(0,0,0));
	COLORREF crBackColor=pDestDC->SetBkColor(RGB(255,255,255));

	//掩码对象
	CDC MaskDC;
	CBitmap MaskImage;
	MaskDC.CreateCompatibleDC(pDestDC);
	MaskImage.CreateBitmap(ImageRender.cxRender,ImageRender.cyRender,1,1,NULL);

	//位图 DC
	CDC * pDCImage=CDC::FromHandle(GetDC());
	COLORREF crImageBkColor=pDCImage->SetBkColor(crTransColor);

	//构造掩码
	MaskDC.SelectObject(&MaskImage);
	MaskDC.BitBlt(0,0,ImageRender.cxRender,ImageRender.cyRender,pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCINVERT);

	//绘画对象
	pDestDC->BitBlt(ImageRender.nXScreen,ImageRender.nYScreen,ImageRender.cxRender,ImageRender.cyRender,
		pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCINVERT);
	pDestDC->BitBlt(ImageRender.nXScreen,ImageRender.nYScreen,ImageRender.cxRender,ImageRender.cyRender,
		&MaskDC,0,0,SRCAND);
	pDestDC->BitBlt(ImageRender.nXScreen,ImageRender.nYScreen,ImageRender.cxRender,ImageRender.cyRender,
		pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCINVERT);

	//颜色还原
	pDestDC->SetBkColor(crBackColor);
	pDestDC->SetTextColor(crTextColor);

	//释放资源
	ReleaseDC();
	MaskDC.DeleteDC();
	MaskImage.DeleteObject();

	return true;
}

//混合绘画
bool CBitImage::TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, COLORREF crTransColor)
{
	if ( CD2DEngine::GetCurrentWnd() && CD2DEngine::GetD2DEngine() )
	{
		return CD2DEngine::GetD2DEngine()->DrawImage(CD2DEngine::GetCurrentWnd(),m_strImageName,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cxSrc,cySrc,crTransColor, 255);
	}

	//创建缓冲
	if ((cxDest!=cxSrc)||(cyDest!=cySrc))
	{
		//创建位图
		CBitImage ImageStretch;
		ImageStretch.Create(cxDest,cyDest,32);

		//拉伸位图
		HDC hStretchDC=ImageStretch.GetDC();
		StretchBlt(hStretchDC,0,0,cxDest,cyDest,xSrc,ySrc,cxSrc,cySrc,SRCCOPY);
		ImageStretch.TransDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,0,0,crTransColor);

		//释放资源
		ImageStretch.ReleaseDC();
	}
	else
	{
		//普通调用
		TransDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,xSrc,ySrc,crTransColor);
	}

	return true;
}

//混合绘画
bool CBitImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth)
{
	//透明绘画
	AlphaDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,cbAlphaDepth);

	return true;
}

//混合绘画
bool CBitImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth)
{
	if ( CD2DEngine::GetCurrentWnd() && CD2DEngine::GetD2DEngine() )
	{
		return CD2DEngine::GetD2DEngine()->DrawImage(CD2DEngine::GetCurrentWnd(),m_strImageName,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cbAlphaDepth);
	}

	//无效区域
	CRect rcDirty;
	pDestDC->GetClipBox(&rcDirty);

	//绘画判断
	if (IsNull()==true) return false;
	if (IsRectIntersect(rcDirty,xDest,yDest,cxDest,cyDest)==false) return true;

	//位置调整
	tagImageRender ImageRender;
	GetDrawImageArea(xDest,yDest,cxDest,cyDest,xSrc,ySrc,rcDirty,ImageRender);

	//创建位图
	CImage ImageResult;
	CImage ImageSource;
	ImageResult.Create(ImageRender.cxRender,ImageRender.cyRender,32);
	ImageSource.Create(ImageRender.cxRender,ImageRender.cyRender,32);

	//绘画位图
	CDC * pDCImage=CDC::FromHandle(GetDC());
	CDC * pDCResult=CDC::FromHandle(ImageResult.GetDC());
	CDC * pDCSource=CDC::FromHandle(ImageSource.GetDC());
	pDCSource->BitBlt(0,0,ImageRender.cxRender,ImageRender.cyRender,pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCCOPY);
	pDCResult->BitBlt(0,0,ImageRender.cxRender,ImageRender.cyRender,pDestDC,ImageRender.nXScreen,ImageRender.nYScreen,SRCCOPY);

	//获取属性
	FLOAT fAlpha=(FLOAT)(cbAlphaDepth/255.0);
	INT nPitchResult=ImageResult.GetPitch();
	INT nPitchSource=ImageSource.GetPitch();

	//获取数据
	LPBYTE cbBitResult=(LPBYTE)ImageResult.GetBits();
	LPBYTE cbBitSource=(LPBYTE)ImageSource.GetBits();

	//创建区域
	for (INT nYPos=0;nYPos<ImageRender.cyRender;nYPos++)
	{
		for (INT nXPos=0;nXPos<ImageRender.cxRender;nXPos++)
		{
			//获取颜色
			COLORREF * pcrResult=(COLORREF *)(cbBitResult+nYPos*nPitchResult+nXPos*4);
			COLORREF * pcrSource=(COLORREF *)(cbBitSource+nYPos*nPitchSource+nXPos*4);

			//结果颜色
			BYTE cbResultR=GetRValue(*pcrResult);
			BYTE cbResultG=GetGValue(*pcrResult);
			BYTE cbResultB=GetBValue(*pcrResult);

			//原图颜色
			BYTE cbSourceR=GetRValue(*pcrSource);
			BYTE cbSourceG=GetGValue(*pcrSource);
			BYTE cbSourceB=GetBValue(*pcrSource);

			//颜色混合
			cbResultR=(BYTE)(cbSourceR*fAlpha+cbResultR*(1.0-fAlpha));
			cbResultG=(BYTE)(cbSourceG*fAlpha+cbResultG*(1.0-fAlpha));
			cbResultB=(BYTE)(cbSourceB*fAlpha+cbResultB*(1.0-fAlpha));

			//颜色混合
			*pcrResult=RGB(cbResultR,cbResultG,cbResultB);
		}
	}

	//绘画界面
	ImageResult.BitBlt(pDestDC->m_hDC,ImageRender.nXScreen,ImageRender.nYScreen);

	//释放对象
	ReleaseDC();
	ImageSource.ReleaseDC();
	ImageResult.ReleaseDC();

	return true;
}

//混合绘画
bool CBitImage::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth)
{
	if ( CD2DEngine::GetCurrentWnd() && CD2DEngine::GetD2DEngine() )
	{
		return CD2DEngine::GetD2DEngine()->DrawImage(CD2DEngine::GetCurrentWnd(),m_strImageName,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cxSrc,cySrc, cbAlphaDepth);
	}

	//创建缓冲
	if ((cxDest!=cxSrc)||(cyDest!=cySrc))
	{
		//创建位图
		CBitImage ImageStretch;
		ImageStretch.Create(cxDest,cyDest,32);

		//拉伸位图
		CImageDC ImageDC(ImageStretch);
		StretchBlt(ImageDC,0,0,cxDest,cyDest,xSrc,ySrc,cxSrc,cySrc,SRCCOPY);

		//混合绘画
		ImageStretch.AlphaDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,0,0,cbAlphaDepth);
	}
	else
	{
		//普通调用
		AlphaDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cbAlphaDepth);
	}

	return true;
}

//混合绘画
bool CBitImage::BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor, BYTE cbAlphaDepth)
{
	//透明绘画
	BlendDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,crTransColor,cbAlphaDepth);

	return true;
}

//混合绘画
bool CBitImage::BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor, BYTE cbAlphaDepth)
{
	if ( CD2DEngine::GetCurrentWnd() && CD2DEngine::GetD2DEngine() )
	{
		return CD2DEngine::GetD2DEngine()->DrawImage(CD2DEngine::GetCurrentWnd(),m_strImageName,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cxDest,cyDest,crTransColor, cbAlphaDepth);
	}

	//无效区域
	CRect rcDirty;
	pDestDC->GetClipBox(&rcDirty);

	//绘画判断
	if (IsNull()==true) return false;
	if (IsRectIntersect(rcDirty,xDest,yDest,cxDest,cyDest)==false) return true;

	//位置调整
	tagImageRender ImageRender;
	GetDrawImageArea(xDest,yDest,cxDest,cyDest,xSrc,ySrc,rcDirty,ImageRender);

	//创建位图
	CImage ImageResult;
	CImage ImageSource;
	ImageResult.Create(ImageRender.cxRender,ImageRender.cyRender,32);
	ImageSource.Create(ImageRender.cxRender,ImageRender.cyRender,32);

	//绘画位图
	CDC * pDCImage=CDC::FromHandle(GetDC());
	CDC * pDCResult=CDC::FromHandle(ImageResult.GetDC());
	CDC * pDCSource=CDC::FromHandle(ImageSource.GetDC());
	pDCSource->BitBlt(0,0,ImageRender.cxRender,ImageRender.cyRender,pDCImage,ImageRender.nXImage,ImageRender.nYImage,SRCCOPY);
	pDCResult->BitBlt(0,0,ImageRender.cxRender,ImageRender.cyRender,pDestDC,ImageRender.nXScreen,ImageRender.nYScreen,SRCCOPY);

	//获取属性
	FLOAT fAlpha=(FLOAT)(cbAlphaDepth/255.0);
	INT nPitchResult=ImageResult.GetPitch();
	INT nPitchSource=ImageSource.GetPitch();

	//获取数据
	LPBYTE cbBitResult=(LPBYTE)ImageResult.GetBits();
	LPBYTE cbBitSource=(LPBYTE)ImageSource.GetBits();

	//创建区域
	for (INT nYPos=0;nYPos<ImageRender.cyRender;nYPos++)
	{
		for (INT nXPos=0;nXPos<ImageRender.cxRender;nXPos++)
		{
			//获取颜色
			COLORREF * pcrResult=(COLORREF *)(cbBitResult+nYPos*nPitchResult+nXPos*4);
			COLORREF * pcrSource=(COLORREF *)(cbBitSource+nYPos*nPitchSource+nXPos*4);

			//混合处理
			if (*pcrSource!=crTransColor)
			{
				//结果颜色
				BYTE cbResultR=GetRValue(*pcrResult);
				BYTE cbResultG=GetGValue(*pcrResult);
				BYTE cbResultB=GetBValue(*pcrResult);

				//原图颜色
				BYTE cbSourceR=GetRValue(*pcrSource);
				BYTE cbSourceG=GetGValue(*pcrSource);
				BYTE cbSourceB=GetBValue(*pcrSource);

				//颜色混合
				cbResultR=(BYTE)(cbSourceR*fAlpha+cbResultR*(1.0-fAlpha));
				cbResultG=(BYTE)(cbSourceG*fAlpha+cbResultG*(1.0-fAlpha));
				cbResultB=(BYTE)(cbSourceB*fAlpha+cbResultB*(1.0-fAlpha));

				//颜色混合
				*pcrResult=RGB(cbResultR,cbResultG,cbResultB);
			}
		}
	}

	//绘画界面
	ImageResult.BitBlt(pDestDC->m_hDC,ImageRender.nXScreen,ImageRender.nYScreen);

	//释放对象
	ReleaseDC();
	ImageSource.ReleaseDC();
	ImageResult.ReleaseDC();

	return true;
}

//混合绘画
bool CBitImage::BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, COLORREF crTransColor, BYTE cbAlphaDepth)
{
	if ( CD2DEngine::GetCurrentWnd() && CD2DEngine::GetD2DEngine() )
	{
		return CD2DEngine::GetD2DEngine()->DrawImage(CD2DEngine::GetCurrentWnd(),m_strImageName,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cxSrc,cySrc, crTransColor, cbAlphaDepth);
	}

	//创建缓冲
	if ((cxDest!=cxSrc)||(cyDest!=cySrc))
	{
		//创建位图
		CBitImage ImageStretch;
		ImageStretch.Create(cxDest,cyDest,32);

		//拉伸位图
		HDC hStretchDC=ImageStretch.GetDC();
		StretchBlt(hStretchDC,0,0,cxDest,cyDest,xSrc,ySrc,cxSrc,cySrc,SRCCOPY);
		ImageStretch.BlendDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,0,0,crTransColor,cbAlphaDepth);

		//释放资源
		ImageStretch.ReleaseDC();
	}
	else
	{
		//普通调用
		BlendDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,xSrc,ySrc,crTransColor,cbAlphaDepth);
	}

	return true;
}

//矩形检查
bool CBitImage::IsRectIntersect(const CRect & rcRect, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//范围检测
	if ((nXPos>rcRect.right)||(nYPos>rcRect.bottom)) return false;
	if (((nXPos+nWidth)<rcRect.left)||((nYPos+nHeight)<rcRect.top)) return false;

	return true;
}

//区域计算
bool CBitImage::GetDrawImageArea(INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, CRect & rcDirty, tagImageRender & ImageRender)
{
	//相交矩形
	CRect rcIntersect;
	rcIntersect.top=__max(yDest,rcDirty.top);
	rcIntersect.left=__max(xDest,rcDirty.left);
	rcIntersect.right=__min((xDest+cxDest),rcDirty.right);
	rcIntersect.bottom=__min((yDest+cyDest),rcDirty.bottom);

	//渲染位置
	ImageRender.nYScreen=rcIntersect.top;
	ImageRender.nXScreen=rcIntersect.left;

	//渲染大小
	ImageRender.cxRender=rcIntersect.Width();
	ImageRender.cyRender=rcIntersect.Height();

	//图形索引
	ImageRender.nXImage=xSrc+ImageRender.nXScreen-xDest;
	ImageRender.nYImage=ySrc+ImageRender.nYScreen-yDest;

	return true;
}

//D2D自定义函数
bool CBitImage::D2DDrawCustomImage( CWnd* pWnd, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, DWORD* pImageData, INT nImageWidth, INT nImageHeight, BYTE cbAlpha /*= 255*/)
{
	if ( CD2DEngine::GetD2DEngine() )
	{
		return CD2DEngine::GetD2DEngine()->DrawCustomImage( pWnd, nXDest, nYDest, nDestWidth, nDestHeight, pImageData, nImageWidth, nImageHeight, cbAlpha );
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
