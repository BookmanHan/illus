#include "cls_web_css.h"
#include "cls_web_item.h"
#include "cls_parser_css_style.h"

#pragma  once

#define CLOSE_EXCEPTION_BEG try{
#define CLOSE_EXCEPTION_END }catch(cls_exception& exc){;}

class cls_web_environment
{
public:
	struct __tag_pasring
	{
		int block_left;
		int block_top;
		int width;
		int heihgt;
		cls_web_block* cur_block;

		__tag_pasring()
		{
			block_left = 0;
			block_top = 0;
			width = 0;
			heihgt = 0;
			cur_block = null;
		}

	} parsing;

	struct 
	{
		web_color link_color;
		web_color unlink_color;
	} tag;

	web_css_font font;
	web_css_text text;

	static cls_parser_css_unit css_unit;
	static cls_parser_css_style css_style;
	static map<wstring,int> map_style;
	
	map<wstring,wstring> map_style_tag;

	cls_web_environment()
	{
		map_style_tag.clear();

		parsing.cur_block = null;
		
		static bool b_static_init = false;
		if (b_static_init == false)
		{
			b_static_init = true;
			init_map_style();
		}
	}

	static void init_map_style()
	{
		map_style[L"background-attachment"] = 1;
		map_style[L"background-color"] = 2;
		map_style[L"background-image"] = 3;
		map_style[L"background-position"] = 4;
		map_style[L"background-repeat"] = 5;
		map_style[L"border-bottom"] = 6;
		map_style[L"border-bottom-color"] = 7;
		map_style[L"border-bottom-style"] = 8;
		map_style[L"border-bottom-width"] = 9;
		map_style[L"border-color"] = 10;
		map_style[L"border-left"] = 11;
		map_style[L"border-left-color"] = 12;
		map_style[L"border-left-style"] = 13;
		map_style[L"border-left-width"] = 14;
		map_style[L"border-right"] = 15;
		map_style[L"border-right-color"] = 16;
		map_style[L"border-right-style"] = 17;
		map_style[L"border-right-width"] = 18;
		map_style[L"border-style"] = 19;
		map_style[L"border-top"] = 20;
		map_style[L"border-top-color"] = 21;
		map_style[L"border-top-style"] = 22;
		map_style[L"border-top-width"] = 23;
		map_style[L"border-width"] = 24;
		map_style[L"outline"] = 25;
		map_style[L"outline-color"] = 26;
		map_style[L"outline-style"] = 27;
		map_style[L"outline-width"] = 28;
		map_style[L"color"] = 29;
		map_style[L"direction"] = 30;
		map_style[L"letter-spacing"] = 31;
		map_style[L"line-height"] = 32;
		map_style[L"text-align"] = 33;
		map_style[L"text-decoration"] = 34;
		map_style[L"text-indent"] = 35;
		map_style[L"text-transform"] = 36;
		map_style[L"unicode-bidi"] = 37;
		map_style[L"white-space"] = 38;
		map_style[L"word-spacing"] = 39;
		map_style[L"font"] = 40;
		map_style[L"font-family"] = 41;
		map_style[L"font-size"] = 42;
		map_style[L"font-stretch"] = 43;
		map_style[L"font-style"] = 44;
		map_style[L"font-variant"] = 45;
		map_style[L"font-weight"] = 46;
		map_style[L"margin"] = 47;
		map_style[L"margin-bottom"] = 48;
		map_style[L"margin-left"] = 49;
		map_style[L"margin-right"] = 50;
		map_style[L"margin-top"] = 51;
		map_style[L"padding"] = 52;
		map_style[L"padding-bottom"] = 53;
		map_style[L"padding-left"] = 54;
		map_style[L"padding-right"] = 55;
		map_style[L"padding-top"] = 56;
		map_style[L"list-style-image"] = 58;
		map_style[L"list-style-position"] = 59;
		map_style[L"list-style-type"] = 60;
		map_style[L"height"] = 70;
		map_style[L"max-height"] = 71;
		map_style[L"max-width"] = 72;
		map_style[L"min-height"] = 73;
		map_style[L"min-width"] = 74;
		map_style[L"width"] = 75;
		map_style[L"bottom"] = 76;
		map_style[L"display"] = 77;
		map_style[L"float"] = 78;
		map_style[L"left"] = 79;
		map_style[L"position"] = 80;
		map_style[L"right"] = 81;
		map_style[L"top"] = 82;
		map_style[L"vertical-align"] = 83;
		map_style[L"visibility"] = 84;
		map_style[L"z-index"] = 85;
	}

	void make_style_tag(cls_xml_node_wide * node)
	{
		if (node->children.size() > 0)
		{
			wstring wstr;
			vector<cls_xml_node_wide*>::iterator itor_children;
			for(itor_children=node->children.begin();itor_children!=node->children.end();++itor_children)
			{
				wstr += (*itor_children)->context;
			}

			css_style.run(wstr);
			vector<pair<vector<wstring>,wstring> > & re = css_style.get_output();

			vector<pair<vector<wstring>,wstring> >::iterator itor;
			vector<wstring>::iterator itor_inner;

			for(itor = re.begin(); itor != re.end();itor++)
			{
				for(itor_inner=itor->first.begin();itor_inner != itor->first.end();itor_inner++)
				{
					map_style_tag[*itor_inner] = itor->second;
				}
			}
		}
	}

	int get_color(const wstring& str_in)
	{
		wstring str = str_in;
		if (str[0] == '#')
		{
			wstring str_ana = str.substr(1,str.size()-1);

			if (str_ana.size() == 6)
			{
				for(int cnt=1;cnt<=6;cnt++)
				{
					str[cnt] = str[cnt] <= '9' && str[cnt] >= '0' ? str[cnt] - '0' : str[cnt] - 'A' + 10;
				}

				int r = (str[1])*16 + str[2];
				int g = (str[3])*16 + str[4];
				int b = (str[5])*16 + str[6];

				return r*1000000 + g*1000 + b;
			}
			else if (str_ana.size() >= 3)
			{
				for(int cnt=1;cnt<=3;cnt++)
				{
					str[cnt] = str[cnt] <= '9' && str[cnt] >= '0' ? str[cnt] - '0' : str[cnt] - 'A' + 10;
				}

				int r = (str[1])*16 + str[1];
				int g = (str[2])*16 + str[2];
				int b = (str[3])*16 + str[3];

				return INNER_COLOR(r,g,b);
			}
		}
		else
		{
			return get_type_from_wstring<int>(str);
		}
	}

	//没有支持绝对长度定位。
	int get_size(const wstring& str,bool b_width=true)
	{
		wchar_t wc = str[str.size()-1];
		if (wc >= '0' && wc <= '9')
		{
			return get_type_from_wstring<double>(str);
		}
		else if (wc == '%')
		{
			if (b_width == true)
				return get_type_from_wstring<double>(str.substr(0,str.size()-1))*parsing.cur_block->rect.get_width()/100;
			else
				return get_type_from_wstring<double>(str.substr(0,str.size()-1))*parsing.heihgt/100;
		}
		else
		{
			wstring post_fix = str.substr(str.size()-3,2);
			wstring pre_fix = str.substr(0,str.size()-2);

			int size = get_type_from_wstring<int>(pre_fix);

			if (post_fix == L"px")
			{	
				return size;
			}
			else if (post_fix == L"em")
			{
				return font.size*size;
			}
			else if (post_fix == L"ex")
			{
				return font.size*size/2;
			}
			else 
			{
				return size;
			}
		}
	}

	web_css_display get_display(const wstring& str)
	{
		if (str == L"none")
			return display_none;
		if (str == L"block")
			return display_block;
		if (str == L"inline")
			return display_inline;
		if (str == L"inline-block")
			return display_inline_block;
		if (str == L"run-in")
			return display_run_in;
		if (str == L"table")
			return display_table;
		if (str == L"inline-table")
			return display_inline_table;

		return display_inline;
	}

	web_css_position_layout get_position(const wstring& str)
	{
		if (str == L"relative")
			return position_relative;
		else if (str == L"absolute")
			return position_absolute;
		else if (str == L"fixed")
			return position_fixed;
		else if (str == L"static")
			return position_static;

		return position_static;
	}

	web_input_type get_input_type(const wstring& str)
	{
		if (str == L"button")
			return input_button;
		if (str == L"checkbox")
			return input_checkbox;
		if (str == L"file")
			return input_file;
		if (str == L"hidden")
			return input_hidden;
		if (str == L"image")
			return input_image;
		if (str == L"password")
			return input_password;
		if (str == L"radio")
			return input_radio;
		if (str == L"reset")
			return input_reset;
		if (str == L"submit")
			return input_submit;
		if (str == L"text")
			return input_text;

		return input_text;
	}

	web_css_align get_web_align(const wstring& str)
	{
		if (str == L"bottom")
			return align_bottom;
		if (str == L"center")
			return align_center;
		if (str == L"justify")
			return align_justify;
		if (str == L"left")
			return align_left;
		if (str == L"right")
			return align_right;
		if (str == L"top")
			return align_top;
		if (str == L"vcenter")
			return align_vcenter;

		return text.align;
	}

	web_css_background_attatchment get_background_attatchment(const wstring& str)
	{
		if (str == L"fixed")
			return attatchment_fixed;
		else
			return attatchment_scroll;
	}

	web_css_background_repeated get_background_repeated(const wstring& str)
	{
		if (str == L"no_repeat")
			return repeated_no_repeat;
		if (str == L"repeat")
			return repeated_repeat;
		if (str == L"repeat-x")
			return repeated_repeat_x;
		if (str == L"repeat_y")
			return repeated_repeat_y;

		if (parsing.cur_block != null)
			return parsing.cur_block->background.repeat;
		else
			return repeated_no_repeat;
	}

	web_css_border_style get_border_style(const wstring& str)
	{
		if (str == L"dashed")
			return border_dashed;
		if (str == L"dotted")
			return border_dotted;
		if (str == L"double")
			return border_double_;
		if (str == L"groove")
			return border_groove;
		if (str == L"hidden")
			return border_hidden;
		if (str == L"none")
			return border_none;
		if (str == L"outset")
			return border_outset;
		if (str == L"ridge")
			return border_ridge;
		if (str == L"soild")
			return border_soild;

		return border_none;
	}

	web_css_font_style get_font_style(const wstring& str)
	{
		if (str == L"italic")
			return font_italic;
		if (str == L"normal")
			return font_normal;
		if (str == L"oblique")
			return font_oblique;

		return font.style;
	}

	web_css_list_type get_list_style(const wstring& str)
	{
		if (str == L"circle")
			return list_circle;
		if (str == L"none")
			return list_none;
		if (str == L"square")
			return list_square;
		if (str == L"disc")
			return list_disc;

		return list_disc;
	}

	web_css_text_decoration get_text_decoration(const wstring& str)
	{
		if (str == L"blink")
			return decoration_blink;
		if (str == L"line-thought")
			return decoration_line_throught;
		if (str == L"none")
			return decoration_normal_d;
		if (str == L"overline")
			return decoration_orverline;
		if (str == L"underline")
			return decoration_underline;

		return text.decoration;
	}

	web_css_text_direction get_text_direction(const wstring& str)
	{
		if (str == L"ltor")
			return direction_ltor;
		else
			return direction_rtol;
	}

	web_css_float get_float(const wstring& str)
	{
		if (str == L"left")
			return web_css_float::float_left;
		else if (str == L"right")
			return web_css_float::float_right;

		return web_css_float::float_none;
	}

	cls_web_item * make_web_item(web_item type,cls_xml_node_wide*node)
	{
		switch(type)
		{
		case web_text:
			{
				cls_web_text * item = new cls_web_text;
				if (node != null)
				{
					create_style(node,item);
				}

				item->node = node;
				item->font = font;
				item->text = text;

				return item;
			}
			break;

		case web_block:
			{
				cls_web_block* item = new cls_web_block;

				if (node != null)
				{
					create_style(node,item);
				}

				if (item->position.position == position_absolute)
				{
					item->rect.left = item->position.left;
					item->rect.right = parsing.cur_block->rect.right - parsing.cur_block->get_right_space() -
						item->position.right;
					item->rect.top = item->position.top;
					item->rect.bottom = parsing.cur_block->rect.bottom - parsing.cur_block->get_bottom_space() -
						item->position.bottom;
				}

				item->node = node;

				return item;
			}
			break;

		case web_media:
			{
				cls_web_media* item = new cls_web_media;

				if (node != null)
				{
					create_style(node,item);
				}

				if (item->position.position ==position_absolute)
				{
					item->rect.left = item->position.left;
					item->rect.right = parsing.cur_block->rect.right - parsing.cur_block->get_right_space() -
						item->position.right;
					item->rect.top = item->position.top;
					item->rect.bottom = parsing.cur_block->rect.bottom - parsing.cur_block->get_bottom_space() -
						item->position.bottom;
				}

				item->node = node;

				return item;
			}
			break; 

		case web_list:
			{
				cls_web_list* item = new cls_web_list;

				if (node != null)
				{
					create_style(node,item);
				}

				if (item->position.position == position_absolute)
				{
					item->rect.left = item->position.left;
					item->rect.right = parsing.cur_block->rect.right - parsing.cur_block->get_right_space() -
						item->position.right;
					item->rect.top = item->position.top;
					item->rect.bottom = parsing.cur_block->rect.bottom - parsing.cur_block->get_bottom_space() -
						item->position.bottom;
				}

				item->node = node;

				return item;
			}
			break; 

		case web_form:
			{
				cls_web_form* item = new cls_web_form;

				if (node != null)
				{
					create_style(node,item);
				}

				if (item->position.position == position_absolute)
				{
					item->rect.left = item->position.left;
					item->rect.right = parsing.cur_block->rect.right - parsing.cur_block->get_right_space() -
						item->position.right;
					item->rect.top = item->position.top;
					item->rect.bottom = parsing.cur_block->rect.bottom - parsing.cur_block->get_bottom_space() -
						item->position.bottom;
				}

				item->node = node;

				return item;
			}
			break; 

		case web_input:
			{
				cls_web_input* item = new cls_web_input;

				if (node != null)
				{
					create_style(node,item);
				}

				CLOSE_EXCEPTION_BEG
					item->input_type = get_input_type(node->get_attribute(L"type").val.val_str);
					
					if (item->dimension.width == 0)
						item->dimension.width = 100;
					
					if (item->dimension.height == 0)
						item->dimension.height = 30;
					
					item->content = node->get_attribute(L"value").val.val_str;
				CLOSE_EXCEPTION_END

				if (item->position.position == position_absolute)
				{
					item->rect.left = item->position.left;
					item->rect.right = parsing.cur_block->rect.right - parsing.cur_block->get_right_space() -
						item->position.right;
					item->rect.top = item->position.top;
					item->rect.bottom = parsing.cur_block->rect.bottom - parsing.cur_block->get_bottom_space() -
						item->position.bottom;
				}

				item->node = node;

				return item;
			}
			break; 
		}
	}

	void create_style(cls_xml_node_wide* node,cls_web_item *item)
	{
		wstring id;
		wstring style;
		wstring class_name;

		CLOSE_EXCEPTION_BEG
			class_name = node->get_attribute(L"class").val.val_str;
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
			id = node->get_attribute(L"id").val.val_str;
		CLOSE_EXCEPTION_END

		if (map_style_tag.find(node->tag) != map_style_tag.end())
		{
			style += map_style_tag[node->tag];
		}

		if (map_style_tag.find(L"#" + id) != map_style_tag.end())
		{
			style += map_style_tag[L"#" + id];
		}
		
		if (map_style_tag.find(L"." + class_name) != map_style_tag.end())
		{
			style += map_style_tag[L"." + class_name];
		}

		CLOSE_EXCEPTION_BEG
			style += node->get_attribute(L"style").val.val_str;
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
		{
			css_unit.run(style);
			vector<pair<wstring,wstring> >& re = css_unit.get_output();
			vector<pair<wstring,wstring> >::iterator itor;

			for(itor=re.begin();itor!=re.end();itor++)
			{
				CLOSE_EXCEPTION_BEG

				switch(map_style[itor->first])
				{
				case 1:
					item->get_web_block()->background.attatchment = get_background_attatchment(itor->second);
					break;

				case 2:
					item->get_web_block()->background.color = get_color(itor->second);
					break;

				case 3:
					item->get_web_block()->background.image = itor->second;
					break;

				case 4:
					//TODO;
					break;

				case 5:
					item->get_web_block()->background.repeat = get_background_repeated(itor->second);
					break;

				case 6:
					{
						if (itor->second[0] == '#')
						{
							item->get_web_block()->border_bottom.color = get_color(itor->second);
						}
						else if (isdigit(itor->second[0]))
						{
							item->get_web_block()->border_bottom.width  = get_size(itor->second);
						}
						else
						{
							item->get_web_block()->border_bottom.style = get_border_style(itor->second);
						}
					}
					break;

				case 7:
					{
						item->get_web_block()->border_bottom.color = get_color(itor->second);
					}
					break;

				case 8:
					{
						item->get_web_block()->border_bottom.style = get_border_style(itor->second);
					}
					break;

				case 9:
					{
						item->get_web_block()->border_bottom.width = get_size(itor->second);
					}
					break;

				case 10:
					{
						item->get_web_block()->border_bottom.color = get_color(itor->second);
						item->get_web_block()->border_top.color = get_color(itor->second);
						item->get_web_block()->border_left.color = get_color(itor->second);
						item->get_web_block()->border_right.color = get_color(itor->second);
					}
					break;

				case 11:
					{
						if (itor->second[0] == '#')
						{
							item->get_web_block()->border_left.color = get_color(itor->second);
						}
						else if (isdigit(itor->second[0]))
						{
							item->get_web_block()->border_left.width  = get_size(itor->second);
						}
						else
						{
							item->get_web_block()->border_left.style = get_border_style(itor->second);
						}
					}
					break;

				case 12:
					{
						item->get_web_block()->border_left.color = get_color(itor->second);
					}
					break;

				case 13:
					{
						item->get_web_block()->border_left.style = get_border_style(itor->second);
					}
					break;

				case 14:
					{
						item->get_web_block()->border_left.width = get_size(itor->second);
					}
					break;


				case 15:
					{
						if (itor->second[0] == '#')
						{
							item->get_web_block()->border_right.color = get_color(itor->second);
						}
						else if (isdigit(itor->second[0]))
						{
							item->get_web_block()->border_right.width  = get_size(itor->second);
						}
						else
						{
							item->get_web_block()->border_right.style = get_border_style(itor->second);
						}
					}
					break;

				case 16:
					{
						item->get_web_block()->border_right.color = get_color(itor->second);
					}
					break;

				case 17:
					{
						item->get_web_block()->border_right.style = get_border_style(itor->second);
					}
					break;

				case 18:
					{
						item->get_web_block()->border_right.width = get_size(itor->second);
					}
					break;

				case 19:
					{
						item->get_web_block()->border_bottom.style = get_border_style(itor->second);
						item->get_web_block()->border_top.style = get_border_style(itor->second);
						item->get_web_block()->border_left.style = get_border_style(itor->second);
						item->get_web_block()->border_right.style = get_border_style(itor->second);
					}
					break;

				case 20:
					{
						if (itor->second[0] == '#')
						{
							item->get_web_block()->border_top.color = get_color(itor->second);
						}
						else if (isdigit(itor->second[0]))
						{
							item->get_web_block()->border_top.width  = get_size(itor->second,false);
						}
						else
						{
							item->get_web_block()->border_top.style = get_border_style(itor->second);
						}
					}
					break;

				case 21:
					{
						item->get_web_block()->border_top.color = get_color(itor->second);
					}
					break;

				case 22:
					{
						item->get_web_block()->border_top.style = get_border_style(itor->second);
					}
					break;

				case 23:
					{
						item->get_web_block()->border_top.width = get_size(itor->second,false);
					}
					break;

				case 24:
					{
						item->get_web_block()->border_bottom.width = get_size(itor->second,false);
						item->get_web_block()->border_top.width = get_size(itor->second,false);
						item->get_web_block()->border_left.width = get_size(itor->second);
						item->get_web_block()->border_right.width = get_size(itor->second);
					}
					break;

				case 25:
				case 26:
				case 27:
				case 28:
					{
						//TODO
					}
					break;

				case 29:
					{
						text.color = get_color(itor->second);
					}
					break;

				case 30:
					{
						text.direction = get_text_direction(itor->second);
					}
					break;

				case 31:
					{
						text.letter_spacing = get_size(itor->second);
					}
					break;

				case 32:
					{
						text.line_height = get_size(itor->second);
					}
					break;

				case 33:
					{
						text.align = get_web_align(itor->second);
					}
					break;

				case 34:
					{
						text.decoration = get_text_decoration(itor->second);
					}
					break;

				case 35:
					{
						text.indent = get_size(itor->second);
					}
					break;

				case 36:
					{
						//TODO
					}
					break;

				case 37:
					{
						//TODO
					}
					break;

				case 38:
					{
						text.while_space = get_size(itor->second);
					}
					break;

				case 39:
					{
						text.word_spacing = get_size(itor->second);
					}
					break;

				case 40:
					{
						if (itor->second == L"normal" || itor->second == L"italic" || itor->second == L"oblique")
						{
							font.style = get_font_style(itor->second);
						}
						else if (isdigit(itor->second[itor->second.size()-1]))
						{
							font.weight = get_size(itor->second);
						}
						else if (isdigit(itor->second[0]))
						{
							font.size = get_size(itor->second);
						}
						else
						{
							font.family = itor->second;
						}
					}
					break;

				case 41:
					{
						font.family = itor->second;
					}
					break;

				case 42:
					{
						font.size = get_size(itor->second);
					}
					break;

				case 43:
					{
						//TODO;
					}
					break;

				case 44:
					{
						font.style = get_font_style(itor->second);
					}
					break;

				case 45:
					{
						//TODO;
					}
					break;

				case 46:
					{
						font.weight = get_size(itor->second);
					}
					break;

				case 47:
					{
						if (itor == re.end()-1 || itor->first != (itor+1)->first)
						{
							item->get_web_block()->margin_bottom.size = get_size(itor->second,false);
							item->get_web_block()->margin_left.size = get_size(itor->second);
							item->get_web_block()->margin_right.size = get_size(itor->second);
							item->get_web_block()->margin_top.size = get_size(itor->second,false);
						}
						else
						{
							item->get_web_block()->margin_left.size = get_size(itor->second);
							itor++;
							item->get_web_block()->margin_top.size = get_size(itor->second,false);
							itor++;
							item->get_web_block()->margin_right.size = get_size(itor->second);
							itor++;
							item->get_web_block()->margin_bottom.size = get_size(itor->second,false);
						}
					}
					break;

				case 48:
					{
						item->get_web_block()->margin_bottom.size = get_size(itor->second,false);
					}
					break;

				case 49:
					{
						item->get_web_block()->margin_left.size = get_size(itor->second);
					}
					break;

				case 50:
					{
						item->get_web_block()->margin_right.size = get_size(itor->second);
					}
					break;

				case 51:
					{
						item->get_web_block()->margin_top.size = get_size(itor->second,false);
					}
					break;

				case 52:
					{
						if (itor == re.end()-1 || itor->first != (itor+1)->first)
						{
							item->get_web_block()->padding_bottom.size = get_size(itor->second,false);
							item->get_web_block()->padding_left.size = get_size(itor->second);
							item->get_web_block()->padding_right.size = get_size(itor->second);
							item->get_web_block()->padding_top.size = get_size(itor->second,false);
						}
						else
						{
							item->get_web_block()->padding_left.size = get_size(itor->second);
							itor++;
							item->get_web_block()->padding_top.size = get_size(itor->second,false);
							itor++;
							item->get_web_block()->padding_right.size = get_size(itor->second);
							itor++;
							item->get_web_block()->padding_bottom.size = get_size(itor->second,false);
						}
					}
					break;

				case 53:
					{
						item->get_web_block()->padding_bottom.size = get_size(itor->second,false);
					}
					break;

				case 54:
					{
						item->get_web_block()->padding_left.size = get_size(itor->second);
					}
					break;

				case 55:
					{
						item->get_web_block()->padding_right.size = get_size(itor->second);
					}
					break;

				case 56:
					{
						item->get_web_block()->padding_top.size = get_size(itor->second,false);
					}
					break;

				case 58:
					{
						item->get_web_list()->list.image_url = itor->second;
					}
					break;

				case 59:
					{
						item->get_web_list()->list.inside = itor->second == L"inside"?true:false;
					}
					break;

				case 60:
					{
						item->get_web_list()->list.type = get_list_style(itor->second);
					}
					break;

				case 70:
					{
						item->get_web_block()->dimension.height = get_size(itor->second,false);
					}
					break;

				case 71:
					{
						item->get_web_block()->dimension.max_height = get_size(itor->second,false);
					}
					break;

				case 72:
					{
						item->get_web_block()->dimension.max_width = get_size(itor->second);
					}
					break;

				case 73:
					{
						item->get_web_block()->dimension.min_height = get_size(itor->second,false);
					}
					break;

				case 74:
					{
						item->get_web_block()->dimension.min_width = get_size(itor->second);
					}
					break;

				case 75:
					{
						item->get_web_block()->dimension.width = get_size(itor->second);
					}
					break;

				case 76:
					{
						item->position.bottom = get_size(itor->second,false);
					}
					break;

				case 77:
					{
						item->position.display = get_display(itor->second);
					}
					break;

				case 78:
					{
						item->position.element_float = get_float(itor->second);
					}
					break;

				case 79:
					{
						item->position.left = get_size(itor->second);
					}
					break;

				case 80:
					{
						item->position.position = get_position(itor->second);
					}
					break;

				case 81:
					{
						item->position.right = get_size(itor->second);
					}
					break;

				case 82:
					{
						item->position.top = get_size(itor->second,false);
					}

				case 83:
					{
						item->position.align = get_web_align(itor->second);
					}
					break;

				case 84:
					{
						item->position.visible = itor->second == L"true"?true:false;
					}
					break;

				case 85:
					{
						item->position.z_index = get_type_from_wstring<int>(itor->second);
					}
					break;
				}

				CLOSE_EXCEPTION_END
			}
		}
		CLOSE_EXCEPTION_END
	}
};