#pragma once
#include "SkinControlHead.h"
#include "..\图像控件\PngImage.h"
#include <functional>

class SKIN_CONTROL_CLASS PngButton
{
public:
	PngButton(void);
	virtual ~PngButton(void);

public:
	void create(CWnd* p_wnd);
	void set_png(HINSTANCE h_instance, const wchar_t* resource_name);
	void set_position(int t_left, int t_top);

	__declspec(property(get = get_enable, put = set_enable)) bool enable;
	bool get_enable(void) const { return m_enable; }
	void set_enable(bool value)
	{
		if (m_enable != value)
		{
			m_enable = value;
			m_is_hover = false;
			m_is_pushed = false;
		}
	}

	__declspec(property(get = get_visible, put = set_visible)) bool visible;
	bool get_visible(void) const { return m_visible; }
	void set_visible(bool value)
	{
		if (m_visible != value)
		{
			m_visible = value;
			m_is_hover = false;
			m_is_pushed = false;
		}
	}

	__declspec(property(get = get_left)) int left;
	int get_left(void) const { return m_left; }

	__declspec(property(get = get_top)) int top;
	int get_top(void) const { return m_top; }

	__declspec(property(get = get_right)) int right;
	int get_right(void) const { return m_right; }

	__declspec(property(get = get_bottom)) int bottom;
	int get_bottom(void) const { return m_bottom; }

	__declspec(property(get = get_width)) int width;
	int get_width(void) const { return m_width; }

	__declspec(property(get = get_height)) int height;
	int get_height(void) const { return m_height; }

	__declspec(property(get = get_show_text, put = set_show_text)) bool show_text;
	bool get_show_text(void) const { return m_show_text; }
	void set_show_text(bool value) { m_show_text = value; }

	__declspec(property(get = get_text, put = set_text)) CString text;
	CString get_text(void) const { return m_text; }
	void set_text(CString value) { m_text = value; }

	__declspec(property(get = get_text_offset, put = set_text_offset)) CPoint text_offset;
	CPoint get_text_offset(void) const { return m_text_offset; }
	void set_text_offset(CPoint value) { m_text_offset = value; }

	// 事件。
public:
	void mouse_move(CPoint point);
	void left_button_down(void);
	void left_button_up(void);

	// 回调。
public:
	std::function<void(void)>	on_click;

	// 绘制。
public:
	void draw(CDC* p_dc);

private:
	void draw(void);
	bool is_in_area(CPoint point);

	static void __stdcall timer_proc(HWND, UINT, UINT_PTR timer_id, DWORD);

private:
	CWnd* m_p_wnd;

	bool m_show_text;
	CString m_text;
	CDFontEx m_font;
	CPoint m_text_offset;

	// PNG 图像。
	CPngImage m_png_image;
	
	// 按钮位置。
	int m_left, m_top, m_right, m_bottom, m_width, m_height;
	// 按钮可见性及是否启用。
	bool m_visible, m_enable;
	bool m_is_hover, m_is_pushed;

	UINT_PTR m_timer_id;
	int m_frame_index;
};
