#include <lib_import.h>
#include "cls_web_render.h"
#include <Windows.h>
#pragma  once

class cls_web_render_win32
	:public cls_web_render
{
public:
	int id_beg;

	HDC dc;
	HWND wnd;
	map<HWND,cls_web_item*> v_inputs;
	map<int,HWND> map_id_hwnd;

	COLORREF trans_color_to_rgb(int color)
	{
		int r = color/1000000;
		int g = (color%1000000)/1000;
		int b = (color%1000);

		return RGB(r,g,b);
	}

public:

	void set_param(HDC a_dc,HWND a_wnd)
	{
		SetBkMode(a_dc,TRANSPARENT);

		dc = a_dc;
		wnd = a_wnd;
	}

	virtual void render()
	{
		id_beg = 1;

		map<HWND,cls_web_item*>::iterator itor;
		for(itor=v_inputs.begin();itor!=v_inputs.end();itor++)
		{
			DestroyWindow(itor->first);
		}

		v_inputs.clear();

		cls_web_render::render();
	}

	virtual void draw_rectangle( int l,int t,int r,int b,int color ) 
	{
		if (color == 1000000000)
			return;

		HBRUSH brush;
		LOGBRUSH log_brush;
		log_brush.lbColor = trans_color_to_rgb(color);
		log_brush.lbHatch = null;
		log_brush.lbStyle = BS_SOLID;

		brush = CreateBrushIndirect(&log_brush);
		HGDIOBJ obj_pre = SelectObject(dc,brush);

		SelectObject(dc,GetStockObject(NULL_PEN));
		Rectangle(dc,l,t,r,b);
		SelectObject(dc,obj_pre);
	}

	virtual void init( cls_web_environment* env ) 
	{
		RECT rect;
		GetClientRect(wnd,&rect);
		env->parsing.width = rect.right - rect.top;
		env->parsing.heihgt = rect.bottom - rect.top;
	}

	virtual void draw_line( int l,int t,int r,int b,int color,int width,web_css_border_style style ) 
	{
		if (style == border_none || style == border_hidden || color == 1000000000)
			return;

		LOGPEN lp;
		lp.lopnColor = trans_color_to_rgb(color);
		lp.lopnWidth.x = width;
		lp.lopnWidth.y = width;
		lp.lopnStyle = 0;

		switch(style)
		{
		case border_dashed:
			lp.lopnStyle |= PS_DASH;
			break;
		case border_dotted:
			lp.lopnStyle |= PS_DOT;
			break;
		case border_double_:
			lp.lopnStyle |= PS_DASHDOTDOT;
			break;
		case border_groove:
			lp.lopnColor = RGB(127,127,127);
			lp.lopnStyle |= PS_SOLID;
			break;
		case border_inset:
			lp.lopnColor = RGB(200,200,200);
			lp.lopnStyle |= PS_SOLID;
			break;
		case border_outset:
			lp.lopnColor = RGB(50,50,50);
			lp.lopnStyle |= PS_SOLID;
			break;
		case border_soild:
			lp.lopnStyle |= PS_SOLID;
			break;
		case border_ridge:
			lp.lopnStyle |= PS_SOLID;
			break;
		}

		HPEN pen;
		pen = CreatePenIndirect(&lp);

		SelectObject(dc,pen);

		MoveToEx(dc,l,t,null);
		LineTo(dc,r,b);
	}

	virtual void draw_picture( const wstring&str, int l,int t,int r,int b,web_css_background_repeated repeated ) 
	{
		HBITMAP h_bmp = (HBITMAP) LoadImage(NULL,str.c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE |LR_CREATEDIBSECTION);

		if (h_bmp != null)
		{
			BITMAP bmp;
			GetObject(h_bmp,sizeof(BITMAP),(void*)&bmp);
			HDC h_compatible_dc = CreateCompatibleDC(dc);
			SelectObject(h_compatible_dc,h_bmp);
			SetStretchBltMode(dc,HALFTONE);
			StretchBlt(dc,l,t,r-l,b-t,h_compatible_dc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			ReleaseDC(wnd,h_compatible_dc);
			DeleteObject(h_bmp);
		}
	}

	virtual void draw_text( const wstring &str, int l,int t,int r,int b,web_css_align align ) 
	{
		RECT rect;
		rect.left = l;
		rect.right = r;
		rect.top = t;
		rect.bottom = b;

		switch(align)
		{
		case align_bottom:
			DrawText(dc,str.c_str(),str.length(),&rect,DT_SINGLELINE | DT_BOTTOM);
			break;
		case align_center:
			DrawText(dc,str.c_str(),str.length(),&rect,DT_SINGLELINE | DT_BOTTOM | DT_CENTER);
			break;
		case align_justify:
			DrawText(dc,str.c_str(),str.length(),&rect,DT_SINGLELINE | DT_BOTTOM | DT_CENTER);
			break;
		case align_left:
			DrawText(dc,str.c_str(),str.length(),&rect,DT_SINGLELINE | DT_BOTTOM | DT_LEFT);
			break;
		case align_right:
			DrawText(dc,str.c_str(),str.length(),&rect,DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
			break;
		case align_top:
			DrawText(dc,str.c_str(),str.length(),&rect,DT_SINGLELINE | DT_BOTTOM | DT_TOP);
			break;
		case align_vcenter:
			DrawText(dc,str.c_str(),str.length(),&rect,DT_SINGLELINE | DT_BOTTOM | DT_VCENTER);
			break;
		}
	}

	virtual void draw_point( int x,int y,int color ) 
	{
		SetPixel(dc,x,y,trans_color_to_rgb(color));
	}

	virtual void set_font_and_text( const web_css_font& font,const web_css_text& text ) 
	{
		LOGFONT lf;
		wcscpy(lf.lfFaceName,font.family.c_str());
		lf.lfWidth = (font.size-1)/2;
		lf.lfWeight = font.weight;
		lf.lfHeight = font.size;
		lf.lfItalic = (font.style == font_italic)?true:false;
		lf.lfUnderline = (text.decoration == decoration_underline)?true:false;
		lf.lfStrikeOut = (text.decoration == decoration_line_throught)?true:false;
		lf.lfPitchAndFamily = VARIABLE_PITCH | FF_MODERN;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfCharSet = CHINESEBIG5_CHARSET;
		lf.lfOutPrecision = OUT_TT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = PROOF_QUALITY;
	
		HFONT h_font;
		h_font = CreateFontIndirect(&lf);
		SelectObject(dc,h_font);

		SetTextColor(dc,trans_color_to_rgb(text.color));
	}

	virtual void error( string& str ) 
	{
		MessageBoxA(NULL,str.c_str(),"HTML解析错误",MB_ICONERROR);
	}

	virtual void make_input( const wstring&str,int l,int t,int r,int b,web_input_type type,const web_css_font& font,const web_css_text& text) 
	{
		HWND h_input = null;
		switch(type)
		{
		case input_button:
			h_input = CreateWindow(TEXT("button"),str.c_str(),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			break;

		case input_submit:
			if (str.size() != 0)
				h_input = CreateWindow(TEXT("button"),str.c_str(),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			else
				h_input = CreateWindow(TEXT("button"),L"提交",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			break;

		case input_reset:
			if (str.size() != 0)
				h_input = CreateWindow(TEXT("button"),str.c_str(),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			else
				h_input = CreateWindow(TEXT("button"),L"重置",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			break;

		case input_checkbox:
			h_input = CreateWindow(TEXT("button"),str.c_str(),WS_CHILD | WS_VISIBLE | BS_CHECKBOX,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			break;

		case input_radio:
			h_input = CreateWindow(TEXT("button"),str.c_str(),WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON ,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			break;

		case input_text:
			h_input = CreateWindow(TEXT("edit"),str.c_str(),WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT ,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			break;

		case input_password:
			h_input = CreateWindow(TEXT("edit"),str.c_str(),WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_PASSWORD,l,t,r-l,b-t,wnd,(HMENU)id_beg,null,null);
			break;
		}

		id_beg++;

		if (h_input != null)
		{	
			SetWindowText(h_input,cur_item->node->context.c_str());
			SetTextColor(GetDC(h_input),trans_color_to_rgb(text.color));
			UpdateWindow(h_input);
			v_inputs.insert(make_pair(h_input,cur_item));
			map_id_hwnd.insert(make_pair(id_beg-1,h_input));
		}
	}
};