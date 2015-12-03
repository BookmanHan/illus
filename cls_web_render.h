#include <lib_import.h>
#include "cls_web_parser.h"
#pragma  once

class cls_web_render
{
protected:
	cls_web_parser core;
	cls_web_item * cur_item;
	bool b_out_of_time;

public:

	virtual void draw_line(int l,int t,int r,int b,int color,int width,web_css_border_style style) = 0;
	virtual void draw_rectangle(int l,int t,int r,int b,int color) = 0;
	virtual void draw_point(int x,int y,int color) = 0;
	virtual void draw_text(const wstring &str, int l,int t,int r,int b,web_css_align align) = 0;
	virtual void draw_picture(const wstring&str, int l,int t,int r,int b,web_css_background_repeated repeated) = 0;
	virtual void make_input(const wstring&str,int l,int t,int r,int b,web_input_type type,const web_css_font& font,const web_css_text& text) = 0;
	virtual void set_font_and_text(const web_css_font& font,const web_css_text& text) = 0;
	virtual void error(string& str) = 0;
	virtual void init(cls_web_environment* env) = 0;

public:
	cls_web_render()
	{
		b_out_of_time = false;
	}

	map<wstring,cls_web_item*> map_id_item;

	cls_xml_wide* get_html()
	{
		return core.get_html();
	}

	void run(const wstring& str)
	{
		cls_web_environment env;
		init(&env);

		try
		{
			core.run(str,env);
		}
		catch (cls_exception &exc)
		{
			error(exc.display());
		}

		map_id_item = core.map_id_items;
	}

	virtual wstring& get_base_url()
	{
		return core.get_base_url();
	}

	virtual wstring& get_caption()
	{
		return core.get_caption();
	}

	virtual int get_height()
	{
		return core.get_height();
	}

	virtual int get_width()
	{
		return core.get_width();
	}

	vector<cls_web_item*>& get_items()
	{
		return core.get_output();
	}

	void refresh()
	{
		cls_web_environment env;
		init(&env);

		b_out_of_time = true;

		try
		{
			core.run_refresh(env);
		}
		catch (cls_exception &exc)
		{
			error(exc.display());
		}

		map_id_item = core.map_id_items;
	}

	bool check_out_of_date()
	{
		return b_out_of_time;
	}

	void render()
	{
		b_out_of_time = false;

		vector<cls_web_item*>& items = core.get_output();

		stable_sort(items.begin(),items.end(),cmp_pointer_item);

		vector<cls_web_item*>::iterator itor;
		for(itor= items.begin();itor !=  items.end();itor++)
		{
			cur_item = (*itor);

			if (cur_item->position.display == display_none)
				continue;

			switch((*itor)->type)
			{
			case web_form:
			case web_list:
			case web_block:
				{
					cls_web_block * block = (cls_web_block*)(*itor);

					draw_rectangle(block->rect.left + block->margin_left.size,
						block->rect.top + block->margin_top.size,
						block->rect.right - block->margin_right.size,
						block->rect.bottom - block->margin_bottom.size
						,block->background.color);

					draw_picture(block->background.image,block->rect.left,block->rect.top,block->rect.right,
						block->rect.bottom,block->background.repeat);

					int width = block->rect.get_width() - block->get_right_space() - block->get_left_space();
					int height = block->rect.get_height() - block->get_top_space() - block->get_bottom_space();

					int cord_1 = block->rect.top  + block->margin_top.size + block->border_top.width/2;
					int cord_3 = block->rect.left + block->margin_left.size + block->border_left.width/2;
					int cord_2 = block->rect.bottom - block->margin_bottom.size - block->border_bottom.width/2;
					int cord_4 = block->rect.right - block->margin_right.size- block->border_right.width/2;

					draw_line(cord_3,cord_1,cord_3,cord_2,block->border_left.color,block->border_left.width,
						block->border_left.style);

					draw_line(cord_4,cord_1,cord_4,cord_2,block->border_right.color,block->border_right.width,
						block->border_right.style);

					draw_line(cord_3,cord_1,cord_4,cord_1,block->border_top.color,block->border_top.width,
						block->border_top.style);

					draw_line(cord_3,cord_2,cord_4,cord_2,block->border_bottom.color,block->border_bottom.width,
						block->border_bottom.style);

				}
				break;

			case web_text:
				{
					cls_web_text *text = (cls_web_text*)(*itor);

					set_font_and_text(text->font,text->text);

					draw_text(text->content,text->rect.left,text->rect.top,
						text->rect.right,text->rect.bottom,text->text.align);
				}
				break;

			case web_media:
				{
					cls_web_media *media = (cls_web_media*)(*itor);
					cls_web_block *block = media;

					draw_rectangle(block->rect.left,block->rect.top,block->rect.right,
						block->rect.bottom,block->background.color);

					draw_picture(block->background.image,block->rect.left,block->rect.top,block->rect.right,
						block->rect.bottom,block->background.repeat);

					int width = block->rect.get_width() - block->get_right_space() - block->get_left_space();
					int height = block->rect.get_height() - block->get_top_space() - block->get_bottom_space();

					int cord_1 = block->rect.top  + block->margin_top.size + block->border_top.width/2;
					int cord_3 = block->rect.left + block->margin_left.size + block->border_left.width/2;
					int cord_2 = block->rect.bottom - block->margin_bottom.size - block->border_bottom.width/2;
					int cord_4 = block->rect.right - block->margin_right.size- block->border_right.width/2;

					draw_line(cord_3,cord_1,cord_3,cord_2,block->border_left.color,block->border_left.width,
						block->border_left.style);

					draw_line(cord_4,cord_1,cord_4,cord_2,block->border_right.color,block->border_right.width,
						block->border_right.style);

					draw_line(cord_3,cord_1,cord_4,cord_1,block->border_top.color,block->border_top.width,
						block->border_top.style);

					draw_line(cord_3,cord_2,cord_4,cord_2,block->border_bottom.color,block->border_bottom.width,
						block->border_bottom.style);

					int real_left = media->rect.left + media->get_left_space();
					int real_right = media->rect.right - media->get_right_space();
					int real_top = 0;
					int real_bottom = 0;
					if (media->dimension.height + media->get_top_space() + media->get_bottom_space() >= media->rect.get_height())
					{
						real_top = media->rect.top + media->get_top_space();
						real_bottom = media->rect.bottom - media->get_bottom_space();
					}
					else
					{
						int der_height = media->rect.get_height() - media->get_top_space() - media->get_bottom_space() - media->dimension.height;
						real_top = media->rect.top + media->get_top_space() + der_height/2;
						real_bottom = media->rect.bottom - media->get_bottom_space() - der_height /2;
					}

					draw_picture(media->url,real_left,real_top,real_right,real_bottom,repeated_no_repeat);
				}
				break;

			case web_input:
				{
					cls_web_input* item = (cls_web_input*)(*itor);

					cls_web_block *block = item;

					draw_rectangle(block->rect.left,block->rect.top,block->rect.right,
						block->rect.bottom,block->background.color);
					draw_picture(block->background.image,block->rect.left,block->rect.top,block->rect.right,
						block->rect.bottom,block->background.repeat);

					int width = block->rect.get_width() - block->get_right_space() - block->get_left_space();
					int height = block->rect.get_height() - block->get_top_space() - block->get_bottom_space();

					int cord_1 = block->rect.top  + block->margin_top.size + block->border_top.width/2;
					int cord_3 = block->rect.left + block->margin_left.size + block->border_left.width/2;
					int cord_2 = block->rect.bottom - block->margin_bottom.size - block->border_bottom.width/2;
					int cord_4 = block->rect.right - block->margin_right.size- block->border_right.width/2;

					draw_line(cord_3,cord_1,cord_3,cord_2,block->border_left.color,block->border_left.width,
						block->border_left.style);

					draw_line(cord_4,cord_1,cord_4,cord_2,block->border_right.color,block->border_right.width,
						block->border_right.style);

					draw_line(cord_3,cord_1,cord_4,cord_1,block->border_top.color,block->border_top.width,
						block->border_top.style);

					draw_line(cord_3,cord_2,cord_4,cord_2,block->border_bottom.color,block->border_bottom.width,
						block->border_bottom.style);

					make_input(item->content,item->rect.left,item->rect.top,
						item->rect.right,item->rect.bottom,item->input_type,item->font,item->text);
				}
				break;
			}
		}
	}
};