#include "StdAfx.h"
#include "PngButton.h"
#include <map>

typedef std::map<UINT_PTR, PngButton*> TimerMap;
TimerMap	g_timer_map;

// 图像类型（从左到右）。
enum
{
	BUTTON_NORMAL,
	BUTTON_HOVER_0,
	BUTTON_HOVER_1,
	BUTTON_PUSHED,
	BUTTON_DISABLED,
	BUTTON_IMAGE_COUNT
};

PngButton::PngButton(void)
	: m_p_wnd(NULL)
	, m_enable(true)
	, m_visible(true)
	, m_is_hover(false)
	, m_is_pushed(false)
	, m_left(0)
	, m_top(0)
	, m_right(0)
	, m_bottom(0)
	, m_width(0)
	, m_height(0)
	, m_show_text(false)
	, on_click(nullptr)	
	, m_timer_id(0)
	, m_frame_index(0)
{}

PngButton::~PngButton(void)
{
	m_font.DeleteFont();
}

void PngButton::create(CWnd* p_wnd)
{
	m_p_wnd = p_wnd;

	m_font.CreateFont(m_p_wnd, TEXT("宋体"), 12, 400);
}

void PngButton::set_png(HINSTANCE h_instance, const wchar_t* resource_name)
{
	m_png_image.LoadImage(h_instance, resource_name);

	m_width = m_png_image.GetWidth() / BUTTON_IMAGE_COUNT;
	m_height = m_png_image.GetHeight();

	m_right = m_left + m_width;
	m_bottom = m_top + m_height;
}

void PngButton::set_position(int t_left, int t_top)
{
	m_left = t_left;
	m_top = t_top;

	m_right = m_left + m_width;
	m_bottom = m_top + m_height;
}

void PngButton::mouse_move(CPoint point)
{
	if (!m_visible || !m_enable)
		return;

	if (m_is_hover)
	{
		if (!is_in_area(point))
		{
			m_is_hover = false;

			draw();
		}
	}
	else
	{
		if (is_in_area(point))
		{
			m_is_hover = true;

			if (!m_is_pushed)
			{
				m_frame_index = 1;
				m_timer_id = SetTimer(nullptr, 0, 50, timer_proc);
				g_timer_map.insert(std::make_pair(m_timer_id, this));
			}

			draw();
		}
	}
}

void PngButton::left_button_down(void)
{
	if (!m_visible || !m_enable)
		return;

	if (m_is_hover && !m_is_pushed)
	{
		m_is_pushed = true;
		draw();
	}
}

void PngButton::left_button_up(void)
{
	if (!m_visible || !m_enable)
		return;

	if (m_is_pushed)
	{
		m_is_pushed = false;
		draw();

		if (m_is_hover)
		{
			if (on_click)
				on_click();
		}
	}
}

void PngButton::draw(CDC* p_dc)
{
	if ((!m_visible || !m_enable || !m_is_hover || !m_is_pushed) && m_timer_id != 0)
	{
		KillTimer(nullptr, m_timer_id);
		m_timer_id = 0;
	}

	if (!m_visible)
		return;

	if (m_enable)
	{
		if (m_is_hover)
		{
			if (m_is_pushed)
				m_png_image.DrawImage(p_dc, m_left, m_top, m_width, m_height, m_width * BUTTON_PUSHED, 0);
			else
			{
				// 底图。
				m_png_image.DrawImage(p_dc, m_left, m_top, m_width, m_height, m_width * BUTTON_NORMAL, 0);
				// 发光。
				m_png_image.AlphaDrawImage(p_dc, m_left, m_top, m_width, m_height, m_width * BUTTON_HOVER_0, 0, m_frame_index * 255 / 8);
			}
		}
		else
			m_png_image.DrawImage(p_dc, m_left, m_top, m_width, m_height, m_width * BUTTON_NORMAL, 0);
	}
	else
		m_png_image.DrawImage(p_dc, m_left, m_top, m_width, m_height, m_width * BUTTON_DISABLED, 0);

	if (m_show_text)
	{
		CRect rect(m_left + m_text_offset.x, m_top + m_text_offset.y, m_right, m_bottom);
		m_font.DrawText(p_dc, m_text, rect, RGB(0, 0, 0), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	}
}

void PngButton::draw(void)
{
	RECT rect = { m_left, m_top, m_right, m_bottom };
	m_p_wnd->InvalidateRect(&rect);
}

bool PngButton::is_in_area(CPoint point)
{
	return point.x >= m_left &&
		point.x <= m_right &&
		point.y >= m_top &&
		point.y <= m_bottom;
}

void PngButton::timer_proc(HWND, UINT, UINT_PTR timer_id, DWORD)
{
	auto pos = g_timer_map.find(timer_id);
	if (pos != g_timer_map.end())
	{
		if (pos->second->m_frame_index < 8)
			pos->second->m_frame_index++;
		else
		{
			KillTimer(NULL, timer_id);
			pos->second->m_timer_id = 0;
		}

		pos->second->draw();
	}
}