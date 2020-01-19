#ifndef BIT_IMAGE_HEAD_FILE
#define BIT_IMAGE_HEAD_FILE

#pragma once

#include "WHImageHead.h"

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//渲染信息
struct tagImageRender
{
	INT								nXImage;							//图片位置
	INT								nYImage;							//图片位置
	INT								nXScreen;							//渲染位置
	INT								nYScreen;							//渲染位置
	INT								cxRender;							//渲染尺寸
	INT								cyRender;							//渲染尺寸
};

//////////////////////////////////////////////////////////////////////////////////

//位图对象
class WH_IMAGE_CLASS CBitImage : public CImage
{
	//变量定义
protected:
	TCHAR							m_strImageName[MAX_PATH];			//图片名称

	//函数定义
public:
	//构造函数
	CBitImage();
	//析构函数
	virtual ~CBitImage();

	//创建函数
public:
	//创建资源
	void LoadFromResource( HINSTANCE hInstance, UINT nIDResource );
	//创建资源
	void LoadFromResource( HINSTANCE hInstance, LPCTSTR pszResourceName );
	//从加密文件中创建资源
	void LoadFromResourceDes( HINSTANCE hInstance, LPCTSTR pszResourceName );
	//从加密文件中创建资源
	void LoadFromResourceDes( HINSTANCE hInstance, UINT nIDResource );

	//区域函数
public:
	//创建区域
	bool CreateImageRegion(CRgn & ImageRgn, COLORREF crTransColor);
	//创建区域
	bool CreateImageRegion(CRgn & ImageRgn, INT nXSrcPos, INT nYSrcPos, INT nSrcWidth, INT nSrcHeight, COLORREF crTransColor);

	//一般绘画
public:
	//一般绘画
	BOOL BitBlt( HDC hDestDC, int xDest, int yDest, DWORD dwROP = SRCCOPY );
	//一般绘画
	BOOL BitBlt( HDC hDestDC, const POINT& pointDest, DWORD dwROP = SRCCOPY );
	//一般绘画
	BOOL BitBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP = SRCCOPY );
	//一般绘画
	BOOL BitBlt( HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, DWORD dwROP = SRCCOPY );

	//绘画图片
public:
	//平铺绘画
	bool DrawImageTile(CDC * pDC, CRect & rcDestRect);
	//平铺绘画
	bool DrawImageTile(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//透明绘画
public:
	//透明绘画
	bool TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor);
	//透明绘画
	bool TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor);
	//透明绘画
	bool TransDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, COLORREF crTransColor);

	//透明绘画
public:
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth);
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth);
	//混合绘画
	bool AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth);

	//混合绘画
public:
	//混合绘画
	bool BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, COLORREF crTransColor, BYTE cbAlphaDepth);
	//混合绘画
	bool BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor, BYTE cbAlphaDepth);
	//混合绘画
	bool BlendDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, COLORREF crTransColor, BYTE cbAlphaDepth);

	//辅助函数
public:
	//矩形检查
	bool IsRectIntersect(const CRect & rcRect, INT nXPos, INT nYPos, INT nWidth, INT nHeight);
	//区域计算
	bool GetDrawImageArea(INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, CRect & rcDirty, tagImageRender & ImageRender);

	//D2D自定义函数
public:
	static bool D2DDrawCustomImage( CWnd* pWnd, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, DWORD* pImageData, INT nImageWidth, INT nImageHeight, BYTE cbAlpha = 255);

};

//////////////////////////////////////////////////////////////////////////////////

#endif