#ifndef _UIUtils_H_
#define _UIUtils_H_
#include <atlimage.h>

/**
 * 对齐方式
 */
#define ALIGN_LEFT     0
#define ALIGN_TOP      0
#define ALIGN_RIGHT    0x1
#define ALIGN_BOTTOM   0x2
#define ALIGN_VCENTER  0x4
#define ALIGN_HCENTER  0x8
#define ALIGN_CENTER   (ALIGN_HCENTER | ALIGN_VCENTER)

namespace UIUtils
{
	//////////////////////////////////////////////////////////////////////////
	// 功能函数
	template<typename T>
		inline void AlignResetPosition(T &x, T &y, T w, T h, unsigned int align)
	{
		if (align & ALIGN_HCENTER)
		{
			x -= w / 2;
		}
		else if (align & ALIGN_RIGHT)
		{
			x -= w;
		}

		if (align & ALIGN_VCENTER)
		{
			y -= h / 2;
		}
		else if (align & ALIGN_BOTTOM)
		{
			y -= h;
		}

		x = (T)(int)x;
		y = (T)(int)y;
	} // AlignResetPosition

	template<typename T>
		inline void AlignInsideResetPosition(T &x, T &y, T w, T h, unsigned int align)
	{
		if (align & ALIGN_HCENTER)
		{
			x += w / 2;
		}
		else if (align & ALIGN_RIGHT)
		{
			x += w;
		}

		if (align & ALIGN_VCENTER)
		{
			y += h / 2;
		}
		else if (align & ALIGN_BOTTOM)
		{
			y += h;
		}

		x = (T)(int)x;
		y = (T)(int)y;
	} // AlignInsideResetPosition

	//////////////////////////////////////////////////////////////////////////
	// 重建缓冲
	inline void RebuildImage(CImage& img, const CRect& rc)
	{
		if (img.IsNull())
		{
			img.Create(rc.Width(),rc.Height(),32);
		}
		else
		{
			if (img.GetWidth() != rc.Width() || img.GetHeight() != rc.Height())
			{
				img.Destroy();
				img.Create(rc.Width(),rc.Height(),32);
			}
		}
	} // RebuildImage
} // UIUtils

#endif // _UIUtils_H_