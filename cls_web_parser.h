#include "cls_web_css.h"
#include "cls_web_environment.h"
#include "cls_web_item.h"
#include <lib_import.h>

#define TAG_TEXT L"__hidden_tag__"

#pragma  once

#define RECORD_ENV_BEG cls_web_environment __pre_env = *env;
#define RECORD_ENV_END *env = __pre_env;
#define MAKE_CURRENT_BLOCK(block) env->parsing.cur_block = block;
#define ADD_TO_ITEMS(items,block) items.push_back(block);

class cls_web_parser
{
protected:
	cls_xml_wide html;

	map<wstring,int> map_acc;

	vector<cls_web_item*> items;
	wstring caption;
	wstring base_url;

	int max_width;
	int max_height;

public:

	static cls_html_parser_wide parser;
	map<wstring,cls_web_item*> map_id_items;

	virtual int get_height()
	{
		return max_height;
	}

	virtual int get_width()
	{
		return max_width;
	}

	virtual ~cls_web_parser()
	{
		clear_items();
	}

	void clear_items()
	{
		map_id_items.clear();
	
		vector<cls_web_item*>::iterator itor;
		for(itor=items.begin();itor!=items.end();itor++)
		{
			delete (*itor);
		}

		items.clear();
		caption = L"";
		base_url = L"";
	}

	void init()
	{
		static bool b_init = false;
		if (!b_init)
		{
			b_init = true;

			max_height = 10;
			max_width = 10;

			map_acc[L"html"] = 1;
			map_acc[L"body"] = 2;
			map_acc[L"__hidden_tag__"] = 3;
			map_acc[L"img"] = 4;
			map_acc[L"head"] = 5;
			map_acc[L"font"] = 6;
			map_acc[L"div"] = 7;
			map_acc[L"span"] = 8;
			map_acc[L"basefont"] = 9;
			map_acc[L"a"] = 10;
			map_acc[L"abbr"] = 11;
			map_acc[L"acronym"] = 12;
			map_acc[L"address"] = 13;
			map_acc[L"b"] = 14;
			map_acc[L"big"] = 15;
			map_acc[L"blockquote"] = 16;
			map_acc[L"bdo"] = 40;
			map_acc[L"br"] = 17;
			map_acc[L"caption"] = 18;
			map_acc[L"center"] = 19;
			map_acc[L"cite"] = 20;
			map_acc[L"code"] = 21;
			map_acc[L"del"] = 22;
			map_acc[L"em"] = 23;
			map_acc[L"h1"] = 24;
			map_acc[L"h2"] = 25;
			map_acc[L"h3"] = 26;
			map_acc[L"h4"] = 27;
			map_acc[L"h5"] = 28;
			map_acc[L"h6"] = 29;
			map_acc[L"i"] = 30;
			map_acc[L"ins"] = 31;
			map_acc[L"kbd"] = 32;
			map_acc[L"meta"] = 33;
			map_acc[L"object"] = 34;
			map_acc[L"q"] = 35;
			map_acc[L"s"] = 22;
			map_acc[L"small"] = 36;
			map_acc[L"strong"] = 14;
			map_acc[L"title"] = 18;
			map_acc[L"ul"] = 37;
			map_acc[L"dd"] = 41;
			map_acc[L"li"] = 38;
			map_acc[L"base"] = 39;
			map_acc[L"dl"] = 41;
			map_acc[L"menu"] = 42;
			map_acc[L"form"] = 43;
			map_acc[L"input"] = 44;
			map_acc[L"p"] = 45;
			map_acc[L"style"] = 46;
		}
	}

	cls_web_parser()
	{
		init();
	}

	int stream_unit_length(const wstring& src,int beg,int end)
	{
		int size_cnt = 0;
		for(int cnt=beg;cnt<end;cnt++)
		{
			if (src[cnt] > 256)
				size_cnt += 2;
			else
				size_cnt += 1;
		}

		return size_cnt/2 + size_cnt%2;
	}

	int stream_unit_count(const wstring& src,int beg,int size_wide)
	{
		size_wide *= 2;
		for(int cnt=beg;cnt<src.size();cnt++)
		{
			if (src[cnt] > 256)
			{
				if (size_wide >= 2)
					size_wide -= 2;
				else
					return cnt-beg;
			}
			else
			{
				if (size_wide >= 1)
					size_wide -= 1;
				else
					return cnt-beg;
			}
		}

		return src.size() - beg;
	}

	cls_web_item* stream_unit(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		int z_index = item_parent->position.z_index;

		int top = 0;
		int bottom = 0;
		int left = 0;
		int right = 0;

		web_css_position_layout position = position_static;
		if (env->parsing.cur_block->position.position == position_relative)
		{
			position = position_relative;
			top = env->parsing.cur_block->position.top;
			bottom = env->parsing.cur_block->position.bottom;
			left = env->parsing.cur_block->position.left;
			right = env->parsing.cur_block->position.right;
		}

		if (env->parsing.block_left == 0)
		{
			env->parsing.block_left += env->text.indent;
		}

		wstring & content = node->context;
		int len_word = stream_unit_length(content,0,content.size());
		int size_total = env->parsing.block_left + len_word * (env->font.size + env->text.letter_spacing);

		int len_parserd = 0;
		int size_valid = env->parsing.cur_block->rect.right -
			env->parsing.cur_block->get_right_space() - env->parsing.cur_block->get_left_space() -
			env->parsing.cur_block->rect.left;

		while(size_total > size_valid)
		{
			cls_web_text * text = env->make_web_item(web_text,node)->get_web_text();

			int size_left = size_valid - env->parsing.block_left;
			int size_wide = size_left / (env->font.size + env->text.letter_spacing);
			int num_cur_parsered_word = stream_unit_count(content,len_parserd,size_wide);

			text->content = content.substr(len_parserd,num_cur_parsered_word); 
			text->rect.top = env->parsing.block_top + env->parsing.cur_block->get_top_space();
			
			if (env->text.line_height == -1)
				text->rect.bottom = text->rect.top + env->font.size;
			else
				text->rect.bottom = text->rect.top + env->text.line_height;

			text->rect.left = env->parsing.block_left + env->parsing.cur_block->get_left_space() + env->parsing.cur_block->rect.left;
			text->rect.right = env->parsing.cur_block->rect.right - env->parsing.cur_block->get_right_space();

			if (item_sibling != null)
			{
				text->slibing = item_sibling;
				item_sibling = null;
			}

			item_parent->children.push_back(text);
			text->parent = item_parent;
			text->dock_parent = env->parsing.cur_block;

			text->position.position = position;
			text->position.top += top;
			text->position.left += left;
			text->position.right += right;
			text->position.bottom += bottom;
			text->position.z_index = z_index;

			items.push_back(text);

			size_total -= env->font.size * stream_unit_length(content,len_parserd,len_parserd+num_cur_parsered_word);
			len_parserd += num_cur_parsered_word;

			env->parsing.block_left = 0;
		}

		if (len_parserd < len_word)
		{
			cls_web_text *text = env->make_web_item(web_text,node)->get_web_text();

			int size_left = size_total - env->parsing.block_left;
			int size_wide = size_left / (env->font.size + env->text.letter_spacing);
			int num_cur_parsered_word = stream_unit_count(content,len_parserd,size_wide);

			text->content = content.substr(len_parserd,num_cur_parsered_word); 
			text->rect.top = env->parsing.block_top + env->parsing.cur_block->get_top_space();
			
			if (env->text.line_height == -1)
				text->rect.bottom = text->rect.top + env->font.size;
			else
				text->rect.bottom = text->rect.top + env->text.line_height;

			text->rect.left = env->parsing.block_left + env->parsing.cur_block->get_left_space() + env->parsing.cur_block->rect.left;
			text->rect.right = text->rect.left + size_wide * (env->font.size + env->text.letter_spacing);

			if (item_sibling != null)
			{
				text->slibing = item_sibling;
				item_sibling = null;
			}

			text->position.position = position;
			text->position.top += top;
			text->position.left += left;
			text->position.right += right;
			text->position.bottom += bottom;
			text->parent = item_parent;
			text->dock_parent = env->parsing.cur_block;
			text->position.z_index = z_index;

			items.push_back(text);

			size_total -= stream_unit_length(content,len_parserd,len_parserd+num_cur_parsered_word);
			len_parserd += num_cur_parsered_word;

			env->parsing.block_left += num_cur_parsered_word * (env->font.size + env->text.letter_spacing) + env->text.word_spacing;

			return text;
		}

		return null;
	}

	void block_judge_posititon_soild(cls_web_block* block,cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		block->position.z_index = max(item_parent->position.z_index,block->position.z_index);

		if(block->position.position == position_absolute)
		{
			items[0]->independent.push_back(block);
		}
		else if (block->position.element_float != float_none)
		{
			item_parent->independent.push_back(block);
		}
		
		if (env->parsing.cur_block->position.position == position_relative)
		{
			block->position.position = position_relative;
			block->position.left += env->parsing.cur_block->position.left;
			block->position.right += env->parsing.cur_block->position.right;
			block->position.top += env->parsing.cur_block->position.top;
			block->position.bottom += env->parsing.cur_block->position.bottom;
		}

		if (item_sibling != null && item_sibling->position.position != float_none)
		{
			item_parent->children.push_back(item_sibling);
		}
	}

	void block_judge_posititon_stream(cls_web_block* block,cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		block->position.z_index = max(item_parent->position.z_index,block->position.z_index);

		if(block->position.position == position_absolute)
		{
			items[0]->independent.push_back(block);
		}
		else if (block->position.element_float != float_none)
		{
			item_parent->independent.push_back(block);
		}
		
		if (env->parsing.cur_block->position.position == position_relative)
		{
			block->position.position = position_relative;
			block->position.left += env->parsing.cur_block->position.left;
			block->position.right += env->parsing.cur_block->position.right;
			block->position.top += env->parsing.cur_block->position.top;
			block->position.bottom += env->parsing.cur_block->position.bottom;
		}
	}

	void block_judge_soild_rect(cls_web_block*block,cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		block->parent = item_parent;
		block->slibing = null;
		block->dock_parent = env->parsing.cur_block;

		env->parsing.block_left = 0;
		env->parsing.block_top += env->text.line_height;

		if (block->position.position != position_absolute)
		{
			block->rect.top = 0;
			block->rect.left = env->parsing.block_left + env->parsing.cur_block->rect.left
				+ env->parsing.cur_block->get_left_space();
			block->rect.bottom = 0;
			block->rect.right = env->parsing.cur_block->rect.right 
				- env->parsing.cur_block->get_right_space();

			if (block->position.element_float == float_none)
				item_parent->children.push_back(block);
		}
		else if (block->position.position == position_absolute)
		{
			;
		}

		if (block->dimension.width != 0)
		{
			block->rect.left += env->parsing.block_left + env->parsing.cur_block->rect.left + env->parsing.cur_block->get_web_block()->get_left_space();
			env->parsing.block_left = 0;

			if (block->dimension.width < block->dimension.min_width)
			{
				block->rect.right = block->get_left_space() + block->rect.left 
					+ block->dimension.min_width + block->get_right_space();
			}
			else if (block->dimension.width > block->dimension.max_width)
			{
				block->rect.right = block->get_left_space() + block->rect.left + 
					block->dimension.max_width + block->get_right_space();
			}
			else
			{
				block->rect.right = block->get_left_space() + block->rect.left + 
					block->dimension.width + block->get_right_space();
			}
		}

		env->parsing.block_left = 0;
		env->parsing.block_top = 0;
	}

	cls_web_item* block_running_dump_soild(cls_web_block* block,cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		block->b_stream_block = false;

		cls_web_item * pre_item = null;
		vector<cls_xml_node_wide*>::iterator itor;
		for(itor=node->begin();itor!=node->end();itor++)
		{
			pre_item = run_unit(env,*itor,block,pre_item);
		}

		if (pre_item != null && pre_item->type == web_text)
		{
			pre_item->rect.right = block->rect.right - block->get_right_space();
		}

		return pre_item;
	}

	cls_web_item* block_soild_post(cls_web_block* block,cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		if (block->position.visible == true)
		{
			if (block->position.display != position_absolute &&
				block->position.element_float == float_none)
			{
				env->parsing.block_left = 0;
				env->parsing.block_top += env->text.line_height;

				return null;
			}
			else
			{
				return item_sibling;
			}
		}	
		else
		{
			return item_sibling;
		}
	}

	int get_sibling_length(cls_web_item* item)
	{
		int width = 0;
		while(item != null)
		{
			width += item->rect.get_width();
			item = item->slibing;
		}

		return width;
	}

	int get_sibling_left(cls_web_item* item)
	{
		int left = 10000000;
		while(item != null)
		{
			left = min(left,item->rect.left);
			item = item->slibing;
		}

		return left;
	}

	cls_web_item* block_running_dump_stream(cls_web_block* block,cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		cls_web_item* pre_item = null;

		block->b_stream_block = true;

		int max_final_left = 0;
		int max_final_width = 0;
		int child_of_parent = 0;

		if (block->position.display == display_inline_block || 
			block->position.display == display_inline_table)
		{
			pre_item = null;
			child_of_parent = block->children.size();

			vector<cls_xml_node_wide*>::iterator itor;
			for(itor=node->begin();itor!=node->end();itor++)
			{
				pre_item = run_unit(env,*itor,block,pre_item);
			}

			if (block->children.size() > child_of_parent || pre_item == null)
			{
				max_final_left = block->rect.left;
				max_final_width = max(max_final_width,block->rect.get_width());
			}
			else
			{
				max_final_left = max(max_final_left,get_sibling_left(pre_item));
				max_final_width = max(max_final_width,get_sibling_length(pre_item));
			}

			if (block->dimension.width != 0)
			{
				max_final_width = max(max_final_width,block->rect.get_width());
			}

			if (pre_item != null)
				block->children.push_back(pre_item);

			pre_item = block;
		}
		else if (block->type == web_input || block->type == web_media)
		{
			pre_item = item_sibling;
			child_of_parent = item_parent->children.size();

			vector<cls_xml_node_wide*>::iterator itor;
			for(itor=node->begin();itor!=node->end();itor++)
			{
				pre_item = run_unit(env,*itor,item_parent,pre_item);
			}

			if (item_parent->children.size() > child_of_parent || pre_item == null)
			{
				max_final_left = block->rect.left;
				max_final_width = max(max_final_width,block->rect.get_width());
			}
			else 
			{
				max_final_left = max(max_final_left,get_sibling_left(pre_item));
				max_final_width = max(max_final_width,get_sibling_length(pre_item));
			}

			return pre_item;
		}
		else if ( (block->position.element_float == float_none) && (block->position.display == display_inline || block->dimension.width == 0))
		{
			pre_item = item_sibling;
			child_of_parent = item_parent->children.size();

			vector<cls_xml_node_wide*>::iterator itor;
			for(itor=node->begin();itor!=node->end();itor++)
			{
				pre_item = run_unit(env,*itor,item_parent,pre_item);
			}

			if (item_parent->children.size() > child_of_parent || pre_item == null)
			{
				max_final_left = block->rect.left;
				max_final_width = max(max_final_width,block->rect.get_width());
			}
			else 
			{
				max_final_left = max(max_final_left,get_sibling_left(pre_item));
				max_final_width = max(max_final_width,get_sibling_length(pre_item));
			}

			block->rect.left = 1000000;
			block->rect.top = 1000000;
			block->rect.bottom = 0;
			block->rect.right = 0;

			return pre_item;
		}
		else if (block->position.element_float == float_left || 
				 block->position.element_float == float_right)
		{
			pre_item = null;
			child_of_parent = block->children.size();

			vector<cls_xml_node_wide*>::iterator itor;
			for(itor=node->begin();itor!=node->end();itor++)
			{
				pre_item = run_unit(env,*itor,block,pre_item);
			}

			if (block->children.size() > child_of_parent || pre_item == null)
			{
				max_final_left = block->rect.left;
				max_final_width = max(max_final_width,block->rect.get_width());
			}
			else 
			{
				max_final_left = max(max_final_left,get_sibling_left(pre_item));
				max_final_width = max(max_final_width,get_sibling_length(pre_item));
			}

			if (pre_item != null)
				block->children.push_back(pre_item);

			pre_item = item_sibling;
		}
		else
		{
			pre_item = null;
			child_of_parent = block->children.size();

			vector<cls_xml_node_wide*>::iterator itor;
			for(itor=node->begin();itor!=node->end();itor++)
			{
				pre_item = run_unit(env,*itor,block,pre_item);
			}

			if (block->children.size() > child_of_parent || pre_item == null)
			{
				max_final_left = block->rect.left;
			}
			else 
			{
				max_final_left = max(max_final_left,get_sibling_left(pre_item));
			}

			max_final_width = max(max_final_width,block->rect.get_width());

			if (pre_item != null)
				block->children.push_back(pre_item);

			pre_item = block;
		}

		block->rect.left = max_final_left;
		block->rect.right = block->rect.left + max_final_width;

		return pre_item;
	}

	void block_judge_stream_rect(cls_web_block*block,cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		if (block->position.position != position_absolute)
		{
			block->rect.top = 0;
			block->rect.left = env->parsing.cur_block->rect.left
				+ env->parsing.cur_block->get_left_space();
			block->rect.bottom = 0;
			block->rect.right = env->parsing.cur_block->rect.right 
				- env->parsing.cur_block->get_right_space();
		}
		else if (block->position.position == position_absolute)
		{
			;
		}

		if (block->position.display == display_inline_block ||
			block->position.display == display_inline_table)
		{
			if (block->dimension.width == 0)
			{
				block->rect.left = env->parsing.block_left + item_parent->rect.left + item_parent->get_web_block()->get_left_space();
				env->parsing.block_left = 0;
				block->rect.right = env->parsing.cur_block->rect.right 
					- env->parsing.cur_block->get_right_space();
			}
			else
			{
				block->rect.left = env->parsing.block_left + item_parent->rect.left + item_parent->get_web_block()->get_left_space();
				env->parsing.block_left = 0;

				if (block->dimension.width < block->dimension.min_width)
				{
					block->rect.right = block->get_left_space() + block->rect.left 
						+ block->dimension.min_width + block->get_right_space();
				}
				else if (block->dimension.width > block->dimension.max_width)
				{
					block->rect.right = block->get_left_space() + block->rect.left + 
						block->dimension.max_width + block->get_right_space();
				}
				else
				{
					block->rect.right = block->get_left_space() + block->rect.left + 
						block->dimension.width + block->get_right_space();
				}
			}

			block->slibing = item_sibling;
			block->parent = item_parent;
		}
		else if (block->dimension.width != 0 && (block->type == web_media || block->type == web_input || block->position.display != display_inline))
		{
			block->rect.left = env->parsing.block_left + item_parent->rect.left + item_parent->get_web_block()->get_left_space();
			env->parsing.block_left = 0;

			if (block->dimension.width < block->dimension.min_width)
			{
				block->rect.right = block->get_left_space() + block->rect.left 
					+ block->dimension.min_width + block->get_right_space();
			}
			else if (block->dimension.width > block->dimension.max_width)
			{
				block->rect.right = block->get_left_space() + block->rect.left + 
					block->dimension.max_width + block->get_right_space();
			}
			else
			{
				block->rect.right = block->get_left_space() + block->rect.left + 
					block->dimension.width + block->get_right_space();
			}

			if (block->rect.right > item_parent->rect.right - item_parent->get_web_block()->get_right_space())
			{
				if (item_sibling != null && item_sibling->position.element_float == float_none)
				{
					item_parent->children.push_back(item_sibling);
				}

				block->slibing = null;
				block->parent = item_parent;
			}
			else
			{
				block->slibing = item_sibling;
				block->parent = item_parent;
			}
		}
		else
		{
			block->slibing = item_sibling;
			block->parent = item_parent;
		}

		block->dock_parent = env->parsing.cur_block;
	}

	cls_web_item* block_stream_post(int left,cls_web_block* block,cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_pre,cls_web_item * item_sibling)
	{
		if (block->position.display == display_inline_table ||
			block->position.display == display_inline_block)
		{
			if (block->dimension.width != 0)
			{
				env->parsing.block_left = block->rect.right;
			}
			else
			{
				env->parsing.block_left = left;
				env->parsing.block_left += block->get_right_space() + block->get_left_space();
			}

			return item_pre;
		}
		else if (block->position.display != position_absolute &&
			block->position.element_float == float_none)
		{
			if (block->dimension.width != 0 && block->position.display != display_inline)
			{
				env->parsing.block_left = block->rect.right;
			}
			else
			{
				env->parsing.block_left = left;
				env->parsing.block_left += block->get_right_space() + block->get_left_space();
			}

			return item_pre;
		}
		else
		{
			return item_sibling;
		}
	}

	cls_web_item* block_div_span_soild(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_block * block = env->make_web_item(web_block,node)->get_web_block();

		if (block->position.display == display_inline ||
			block->position.display == display_inline_block ||
			block->position.display == display_inline_table)
		{
			delete block;
			
			return block_div_span_stream(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);
			return item_sibling;
		}

		block_judge_posititon_soild(block,env,node,item_parent,item_sibling);
		block_judge_soild_rect(block,env,node,item_parent,item_sibling);

		ADD_TO_ITEMS(items,block);
		MAKE_CURRENT_BLOCK(block);

		cls_web_item * pre_item = block_running_dump_soild(block,env,node,item_parent,item_sibling);
		
		if (pre_item != null)
			block->children.push_back(pre_item);

		RECORD_ENV_END

		return block_soild_post(block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_div_span_stream(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_block* block  = env->make_web_item(web_block,node)->get_web_block();

		if (block->position.display == display_block || 
			block->position.display == display_table)
		{
			delete block;

			return block_div_span_soild(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);
			return item_sibling;
		}

		block_judge_posititon_stream(block,env,node,item_parent,item_sibling);
		block_judge_stream_rect(block,env,node,item_parent,item_sibling);

		MAKE_CURRENT_BLOCK(block);
		ADD_TO_ITEMS(items,block);

		cls_web_item* pre_item = block_running_dump_stream(block,env,node,item_parent,item_sibling);

		int left  = env->parsing.block_left;
		
		RECORD_ENV_END

		return block_stream_post(left,block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_media_soild(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_media * block = env->make_web_item(web_media,node)->get_web_media();

		if (block->position.display == display_inline ||
			block->position.display == display_inline_block ||
			block->position.display == display_inline_table)
		{
			delete block;

			return block_media_stream(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
			block->url = node->get_attribute(L"src").val.val_str;
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
			block->dimension.width = node->get_attribute(L"width").val.val_int;
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
			block->dimension.height = node->get_attribute(L"height").val.val_int;
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);
			return item_sibling;
		}

		block_judge_posititon_soild(block,env,node,item_parent,item_sibling);
		block_judge_soild_rect(block,env,node,item_parent,item_sibling);

		ADD_TO_ITEMS(items,block);
		MAKE_CURRENT_BLOCK(block);

		cls_web_item * pre_item = block_running_dump_soild(block,env,node,item_parent,item_sibling);

		if (pre_item != null)
			block->children.push_back(pre_item);

		RECORD_ENV_END

		return block_soild_post(block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_media_stream(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_media* block  = env->make_web_item(web_media,node)->get_web_media();

		if (block->position.display == display_block || 
			block->position.display == display_table)
		{
			delete block;

			return block_media_soild(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
			block->url = node->get_attribute(L"src").val.val_str;
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
			block->dimension.width = node->get_attribute(L"width").val.val_int;
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
			block->dimension.height = node->get_attribute(L"height").val.val_int;
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);
			return item_sibling;
		}

		block_judge_posititon_stream(block,env,node,item_parent,item_sibling);
		block_judge_stream_rect(block,env,node,item_parent,item_sibling);

		MAKE_CURRENT_BLOCK(block);
		ADD_TO_ITEMS(items,block);

		cls_web_item* pre_item = block_running_dump_stream(block,env,node,block,item_sibling);

		int left  = env->parsing.block_left;

		RECORD_ENV_END

		return block_stream_post(left,block,env,node,block,item_sibling);
	}

	cls_web_item* block_list_soild(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_list * block = env->make_web_item(web_list,node)->get_web_list();

		if (block->position.display == display_inline ||
			block->position.display == display_inline_block ||
			block->position.display == display_inline_table)
		{
			delete block;

			return block_list_stream(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END
			
		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);
			return item_sibling;
		}

		block_judge_posititon_soild(block,env,node,item_parent,item_sibling);
		block_judge_soild_rect(block,env,node,item_parent,item_sibling);

		ADD_TO_ITEMS(items,block);
		MAKE_CURRENT_BLOCK(block);

		cls_web_item * pre_item = block_running_dump_soild(block,env,node,item_parent,item_sibling);

		if (pre_item != null)
			block->children.push_back(pre_item);

		RECORD_ENV_END

		return block_soild_post(block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_list_stream(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_list* block  = env->make_web_item(web_list,node)->get_web_list();

		if (block->position.display == display_block || 
			block->position.display == display_table)
		{
			delete block;

			return block_list_soild(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);
			return item_sibling;
		}

		block_judge_posititon_stream(block,env,node,item_parent,item_sibling);
		block_judge_stream_rect(block,env,node,item_parent,item_sibling);

		MAKE_CURRENT_BLOCK(block);
		ADD_TO_ITEMS(items,block);

		block->padding_top.size = 10;
		block->padding_right.size = 10;
		block->padding_left.size = 20;
		block->padding_bottom.size = 10;

		cls_web_item* pre_item = block_running_dump_stream(block,env,node,item_parent,item_sibling);

		int left  = env->parsing.block_left;

		RECORD_ENV_END

		return block_stream_post(left,block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_list_item_soild(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_list * block = env->make_web_item(web_list,node)->get_web_list();

		if (block->position.display == display_inline ||
			block->position.display == display_inline_block ||
			block->position.display == display_inline_table)
		{
			delete block;

			return block_list_item_stream(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END
			
		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);

			return item_sibling;
		}

		block_judge_posititon_soild(block,env,node,item_parent,item_sibling);
		block_judge_soild_rect(block,env,node,item_parent,item_sibling);

		if (block->list.image_url != L"")
		{
			cls_xml_node_wide * new_node = new cls_xml_node_wide;
			new_node->tag = L"img";
			new_node->set_attribute(L"src",block->list.image_url);
			new_node->set_attribute(L"width",(int)env->font.size);
			new_node->set_attribute(L"height",(int)env->font.size);

			node->children.insert(node->children.begin(),new_node);
		}
		else if (block->list.type != list_none)
		{
			cls_xml_node_wide * new_node = new cls_xml_node_wide;
			new_node->tag = L"__hidden_tag__";

			switch(block->list.type)
			{
			case list_circle:
				new_node->context = L"¡ñ";
				break;

			case list_disc:
				new_node->context = L"¡ð";
				break;

			case list_square:
				new_node->context = L"¡ö";
				break;
			}

			node->children.insert(node->children.begin(),new_node);
		}

		ADD_TO_ITEMS(items,block);
		MAKE_CURRENT_BLOCK(block);

		cls_web_item * pre_item = block_running_dump_soild(block,env,node,item_parent,item_sibling);

		if (pre_item != null)
			block->children.push_back(pre_item);

		RECORD_ENV_END

			return block_soild_post(block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_list_item_stream(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_list* block  = env->make_web_item(web_list,node)->get_web_list();

		if (block->position.display == display_block || 
			block->position.display == display_table)
		{
			delete block;

			return block_list_item_soild(env,node,item_parent,item_sibling);
		}


		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);

			return item_sibling;
		}

		block_judge_posititon_stream(block,env,node,item_parent,item_sibling);
		block_judge_stream_rect(block,env,node,item_parent,item_sibling);

		if (block->list.image_url != L"")
		{
			cls_xml_node_wide * new_node = new cls_xml_node_wide;
			new_node->tag = L"img";
			new_node->set_attribute(L"src",block->list.image_url);
			new_node->set_attribute(L"width",(int)env->font.size);
			new_node->set_attribute(L"height",(int)env->font.size);

			node->children.insert(node->children.begin(),new_node);
		}

		MAKE_CURRENT_BLOCK(block);
		ADD_TO_ITEMS(items,block);

		cls_web_item* pre_item = block_running_dump_stream(block,env,node,item_parent,item_sibling);

		int left  = env->parsing.block_left;

		RECORD_ENV_END

			return block_stream_post(left,block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_form_soild(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_form * block = env->make_web_item(web_form,node)->get_web_form();

		if (block->position.display == display_inline ||
			block->position.display == display_inline_block ||
			block->position.display == display_inline_table)
		{
			delete block;

			return block_form_stream(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);

			return item_sibling;
		}

		block_judge_posititon_soild(block,env,node,item_parent,item_sibling);
		block_judge_soild_rect(block,env,node,item_parent,item_sibling);

		ADD_TO_ITEMS(items,block);
		MAKE_CURRENT_BLOCK(block);

		cls_web_item * pre_item = block_running_dump_soild(block,env,node,item_parent,item_sibling);

		if (pre_item != null)
			block->children.push_back(pre_item);

		RECORD_ENV_END

			return block_soild_post(block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_form_stream(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_form* block  = env->make_web_item(web_form,node)->get_web_form();

		if (block->position.display == display_block || 
			block->position.display == display_table)
		{
			delete block;

			return block_form_soild(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			env->text.align = env->get_web_align(node->get_attribute(L"align").val.val_str);
		CLOSE_EXCEPTION_END

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);

			return item_sibling;
		}

		block_judge_posititon_stream(block,env,node,item_parent,item_sibling);
		block_judge_stream_rect(block,env,node,item_parent,item_sibling);

		MAKE_CURRENT_BLOCK(block);
		ADD_TO_ITEMS(items,block);

		cls_web_item* pre_item = block_running_dump_stream(block,env,node,item_parent,item_sibling);

		int left  = env->parsing.block_left;

		RECORD_ENV_END

		return block_stream_post(left,block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_input_soild(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		CLOSE_EXCEPTION_BEG
			env->text.align = env->get_web_align(node->get_attribute(L"align").val.val_str);
		CLOSE_EXCEPTION_END

		cls_web_input * block = env->make_web_item(web_input,node)->get_web_input();

		if (block->position.display == display_inline ||
			block->position.display == display_inline_block ||
			block->position.display == display_inline_table)
		{
			delete block;

			return block_div_span_stream(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);

			return item_sibling;
		}

		block_judge_posititon_soild(block,env,node,item_parent,item_sibling);
		block_judge_soild_rect(block,env,node,item_parent,item_sibling);

		ADD_TO_ITEMS(items,block);
		MAKE_CURRENT_BLOCK(block);

		cls_web_item * pre_item = block_running_dump_soild(block,env,node,item_parent,item_sibling);

		if (pre_item != null)
			block->children.push_back(pre_item);

		RECORD_ENV_END

			return block_soild_post(block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_input_stream(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_input * block  = env->make_web_item(web_input,node)->get_web_input();

		if (block->position.display == display_block || 
			block->position.display == display_table)
		{
			delete block;

			return block_div_span_soild(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			delete block;
			return item_sibling;
		}

		block_judge_posititon_stream(block,env,node,item_parent,item_sibling);
		block_judge_stream_rect(block,env,node,item_parent,item_sibling);

		MAKE_CURRENT_BLOCK(block);
		ADD_TO_ITEMS(items,block);

		cls_web_item* pre_item = block_running_dump_stream(block,env,node,item_parent,item_sibling);

		int left  = env->parsing.block_left;

		RECORD_ENV_END

			return block_stream_post(left,block,env,node,block,item_sibling);
	}

	cls_web_item* block_quto_soild(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_block * block = env->make_web_item(web_block,node)->get_web_block();

		if(block->position.display == display_none)
		{
			delete block;
			return item_sibling;
		}

		if (block->position.display == display_inline ||
			block->position.display == display_inline_block ||
			block->position.display == display_inline_table)
		{
			delete block;

			return block_div_span_stream(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);

			return item_sibling;
		}

		block_judge_posititon_soild(block,env,node,item_parent,item_sibling);
		block_judge_soild_rect(block,env,node,item_parent,item_sibling);

		ADD_TO_ITEMS(items,block);
		MAKE_CURRENT_BLOCK(block);

		block->margin_left.size = 20;
		block->margin_right.size = 20;
		block->margin_top.size = 10;
		block->margin_bottom.size = 10;

		cls_web_item * pre_item = block_running_dump_soild(block,env,node,item_parent,item_sibling);

		if (pre_item != null)
			block->children.push_back(pre_item);

		RECORD_ENV_END

			return block_soild_post(block,env,node,pre_item,item_sibling);
	}

	cls_web_item* block_quto_stream(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		RECORD_ENV_BEG

		cls_web_block* block  = env->make_web_item(web_block,node)->get_web_block();

		if (block->position.display == display_block || 
			block->position.display == display_table)
		{
			delete block;

			return block_div_span_soild(env,node,item_parent,item_sibling);
		}

		CLOSE_EXCEPTION_BEG
			map_id_items.insert(make_pair(node->get_attribute(L"id").val.val_str,block));
		CLOSE_EXCEPTION_END

		if(block->position.display == display_none)
		{
			ADD_TO_ITEMS(items,block);

			return item_sibling;
		}

		block_judge_posititon_stream(block,env,node,item_parent,item_sibling);
		block_judge_stream_rect(block,env,node,item_parent,item_sibling);

		MAKE_CURRENT_BLOCK(block);
		ADD_TO_ITEMS(items,block);

		cls_web_item* pre_item = block_running_dump_stream(block,env,node,item_parent,item_sibling);

		int left  = env->parsing.block_left;

		RECORD_ENV_END

			return block_stream_post(left,block,env,node,pre_item,item_sibling);
	}

	cls_web_item* text_style(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		web_css_text text = env->text;
		web_css_font font = env->font;

		env->create_style(node,null);

		switch(map_acc[node->tag])
		{
		case 14:
			{
				env->font.weight = 800;
			}
			break;

		case 15:
			{
				env->font.size *= 1.5;
			}
			break;

		case 19:
			{
				env->text.align = align_center;
			}
			break;

		case 20:
			{
				env->font.style = font_italic;
			}
			break;

		case 22:
			{
				env->text.decoration = decoration_line_throught;
			}
			break;

		case 23:
			{
				env->font.size *= 1.1;
				env->font.weight = 900;
			}
			break;

		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
			{
				int cnt = 30 - map_acc[node->tag];

				env->font.size *= (1.1 + 0.15*cnt);
				env->text.align = align_center;
			}
			break;

		case 30:
			{
				env->font.style = font_italic;
			}
			break;

		case 31:
			{
				env->text.decoration = decoration_underline;
			}
			break;

		case 36:
			{
				env->font.size *= 0.8;
			}
			break;
		}

		cls_web_item * pre_item = item_sibling;
		vector<cls_xml_node_wide*>::iterator itor;
		for(itor=node->begin();itor!=node->end();itor++)
		{
			pre_item = run_unit(env,*itor,item_parent,pre_item);
		}

		env->text = text;
		env->font = font;

		return pre_item;
	}

	cls_web_item* run_unit(cls_web_environment* env,cls_xml_node_wide*node,
		cls_web_item* item_parent,cls_web_item * item_sibling)
	{
		switch(map_acc[node->tag])
		{
		case 1:
			{
				cls_web_item * pre_item = null;
				vector<cls_xml_node_wide*>::iterator itor;
				for(itor=node->begin();itor!=node->end();itor++)
				{
					pre_item = run_unit(env,*itor,null,pre_item);
				}

				return null;
			}
			break;

		case 2:
			{
				cls_web_block* block = env->make_web_item(web_block,node)->get_web_block();

				env->parsing.cur_block = block;

				CLOSE_EXCEPTION_BEG
					env->parsing.cur_block->background.color = node->get_attribute(L"bgcolor").val.val_int;
				CLOSE_EXCEPTION_END

				CLOSE_EXCEPTION_BEG
					env->tag.link_color = node->get_attribute(L"link").val.val_int;
				CLOSE_EXCEPTION_END

				CLOSE_EXCEPTION_BEG
					env->text.color = node->get_attribute(L"text").val.val_int;
				CLOSE_EXCEPTION_END

				CLOSE_EXCEPTION_BEG
					env->tag.unlink_color = node->get_attribute(L"v_link").val.val_int;
				CLOSE_EXCEPTION_END

				block->parent = null;
				block->slibing = null;
				block->rect.right = env->parsing.width;
				block->rect.left = 0;
				block->rect.top = 0;
				block->rect.bottom = env->parsing.heihgt;

				items.push_back(block);

				cls_web_item * pre_item = null;
				vector<cls_xml_node_wide*>::iterator itor;
				for(itor=node->begin();itor!=node->end();itor++)
				{
					pre_item = run_unit(env,*itor,block,pre_item);
				}

				if (pre_item != null)
				{
					block->children.push_back(pre_item);
				}
			
				return null;
			}
			break;

		case 3:
			{
				return stream_unit(env,node,item_parent,item_sibling);
			}
			break;

		case 4:
			{
				return block_media_stream(env,node,item_parent,item_sibling);
			}
			break;

		case 5:
			{
				vector<cls_xml_node_wide*>::iterator itor;
				for(itor=node->begin();itor!=node->end();itor++)
				{
					run_unit(env,*itor,null,null);
				}
			}
			break;

		case 6:
			{
				web_css_font font = env->font;
				web_css_text text = env->text;

				env->create_style(node,null);

				CLOSE_EXCEPTION_BEG
					env->font.family = node->get_attribute(L"face").val.val_str;
				CLOSE_EXCEPTION_END

				CLOSE_EXCEPTION_BEG
					env->font.size = node->get_attribute(L"size").val.val_int;
				CLOSE_EXCEPTION_END

				CLOSE_EXCEPTION_BEG
					env->text.color = env->get_color(node->get_attribute(L"color").val.val_str);
				CLOSE_EXCEPTION_END

				cls_web_item * pre_item = item_sibling;
				vector<cls_xml_node_wide*>::iterator itor;
				for(itor=node->begin();itor!=node->end();itor++)
				{
					pre_item = run_unit(env,*itor,item_parent,pre_item);
				}

				env->font = font;
				env->text = text;

				return pre_item;
			}
			break;

		case 7:
			{
				return block_div_span_soild(env,node,item_parent,item_sibling);
			}
			break;

		case 8:
			{
				return block_div_span_stream(env,node,item_parent,item_sibling);
			}
			break;

		case 9:
			{
				CLOSE_EXCEPTION_BEG
					env->font.family = node->get_attribute(L"face").val.val_str;
				CLOSE_EXCEPTION_END

				CLOSE_EXCEPTION_BEG
					env->text.color = env->get_color(node->get_attribute(L"color").val.val_str);
				CLOSE_EXCEPTION_END

				CLOSE_EXCEPTION_BEG
					env->font.size = node->get_attribute(L"size").val.val_int;
				CLOSE_EXCEPTION_END
			}
			break;

		case 10:
			{
				web_css_font font = env->font;
				web_css_text text = env->text;

				env->text.color = env->tag.link_color;
				env->text.decoration = decoration_underline;

				env->create_style(node,null);

				cls_web_item * pre_item = item_sibling;
				vector<cls_xml_node_wide*>::iterator itor;
				for(itor=node->begin();itor!=node->end();itor++)
				{
					pre_item = run_unit(env,*itor,item_parent,pre_item);
				}

				env->font = font;
				env->text = text;

				return pre_item;
			}
			break;

		case 11:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 12:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 13:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 14:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 15:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 16:
			{
				return block_quto_soild(env,node,item_parent,item_sibling);
			}
			break;

		case 17:
			{
				if (item_sibling != 0)
					item_parent->children.push_back(item_sibling);

				env->parsing.block_left = 0;
				return null;
			}
			break;

		case 18:
			{
				CLOSE_EXCEPTION_BEG
					caption = node->get_attribute(L"value").val.val_str;
				CLOSE_EXCEPTION_END

				return item_sibling;
			}
			break;

		case 19:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 20:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 21:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 22:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 23:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 24:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 25:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 26:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 27:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 28:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 29:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 30:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 31:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 32:
			{
				//TODO;
			}
			break;

		case 33:
			{
				//TODO;
			}
			break;

		case 34:
			{
				return block_media_stream(env,node,item_parent,item_sibling);
			}
			break;

		case 35:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 36:
			{
				return text_style(env,node,item_parent,item_sibling);
			}
			break;

		case 37:
			{
				return block_list_soild(env,node,item_parent,item_sibling);
			}
			break;

		case 38:
			{
				return block_list_item_soild(env,node,item_parent,item_sibling);
			}
			break;

		case 39:
			{
				CLOSE_EXCEPTION_BEG
					base_url = node->get_attribute(L"url").val.val_str;
				CLOSE_EXCEPTION_END

				return item_sibling;
			}
		case 41:
			{
				return block_list_soild(env,node,item_parent,item_sibling);
			}
			break;

		case 42:
			{
				return block_list_soild(env,node,item_parent,item_sibling);
			}
			break;

		case 43:
			{
				return block_form_soild(env,node,item_parent,item_sibling);
			}
			break;

		case 44:
			{
				return block_input_soild(env,node,item_parent,item_sibling);
			}
			break;

		case 45:
			{
				return block_div_span_stream(env,node,item_parent,item_sibling);
			}
			break;

		case 46:
			{
				env->make_style_tag(node);

				return null;
			}
			break;
		}
	}

	int make_up_lines(cls_web_block* block)
	{
		int acc_height = block->rect.top + block->get_top_space();

		vector<cls_web_item*>::iterator itor;

		for(itor=block->children.begin();itor != block->children.end();++itor)
		{
			int cur_height = 0;

			cls_web_item * search_list = (*itor);
			while(search_list != null)
			{
				if (search_list->position.position == position_absolute ||
					search_list->position.element_float != float_none)
				{
					;
				}
				else if (search_list->type != web_text)
				{
					int block_height = make_up_blocks(search_list->get_web_block(),acc_height);
					cur_height = max(cur_height,block_height);
				}
				else
				{
					cur_height = max(cur_height,search_list->rect.get_height());
				}

				search_list = search_list->slibing;
			}

			search_list = (*itor);

			while(search_list != null)
			{
				if (search_list->position.position == position_absolute ||
					search_list->position.element_float != float_none)
				{
					;
				}
				else if (search_list->position.position == position_relative)
				{
					search_list->rect.top = acc_height + search_list->position.top - search_list->position.bottom;
					search_list->rect.left +=  search_list->position.left - search_list->position.right;
					search_list->rect.right +=  search_list->position.left - search_list->position.right;

					max_width = max(max_width,search_list->rect.right);				
					search_list->rect.bottom = acc_height + cur_height - search_list->position.bottom + search_list->position.top;
					max_height = max(max_height,search_list->rect.bottom);
				}
				else
				{
					search_list->rect.top = acc_height;

					max_width = max(max_width,search_list->rect.right);				
					search_list->rect.bottom = acc_height + cur_height;

					max_height = max(max_height,search_list->rect.bottom);
				}

				if (search_list->dock_parent != search_list->parent && search_list->dock_parent->b_stream_block == true)
				{
					search_list->dock_parent->rect.left = min(search_list->rect.left - search_list->dock_parent->get_web_block()->get_left_space(),search_list->dock_parent->rect.left);
					search_list->dock_parent->rect.right = max(search_list->rect.right + search_list->dock_parent->get_web_block()->get_right_space(),search_list->dock_parent->rect.right);
					search_list->dock_parent->rect.top = min(search_list->rect.top - search_list->dock_parent->get_web_block()->get_top_space(),search_list->dock_parent->rect.top);
					search_list->dock_parent->rect.bottom = max(search_list->rect.bottom + search_list->dock_parent->get_web_block()->get_bottom_space(),search_list->dock_parent->rect.bottom);
				}

				search_list = search_list->slibing;
			}

			acc_height += cur_height;
		}

		for(itor=block->independent.begin();itor!=block->independent.end();itor++)
		{
			(*itor)->rect.left += block->backup_1;
			(*itor)->rect.right += block->backup_2;

			int width = (*itor)->rect.get_width();

			if ((*itor)->position.position == position_absolute)
			{
				(*itor)->rect.top = block->rect.top + 
					(*itor)->position.top + block->get_top_space();

				(*itor)->backup_1 = (*itor)->rect.left;
				(*itor)->backup_2 = (*itor)->rect.right;

				(*itor)->rect.left = block->rect.left + (*itor)->position.left;
				(*itor)->rect.right = max((*itor)->rect.left + width, block->rect.right - block->get_right_space() - (*itor)->position.right);

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;

				int height = make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);

				(*itor)->rect.bottom = max((*itor)->rect.top + height,(*itor)->position.bottom);
			}
			
			if ((*itor)->position.element_float == float_left)
			{
				(*itor)->rect.top = block->rect.top; 
				
				(*itor)->rect.left = block->rect.left + block->get_left_space() 
					+ ((*itor)->position.position == position_relative?(*itor)->position.left - (*itor)->position.right:0);

				(*itor)->rect.right = (*itor)->rect.left + width ;

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;

				make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);
			}
			else if ((*itor)->position.element_float == float_right)
			{
				(*itor)->rect.top = block->rect.top;

				(*itor)->backup_1 = (*itor)->rect.left;
				(*itor)->backup_2 = (*itor)->rect.right;

				(*itor)->rect.right = block->rect.right - block->get_right_space()
					+ ((*itor)->position.position == position_relative?(*itor)->position.left - (*itor)->position.right:0);

				(*itor)->rect.left = (*itor)->rect.right - width ;

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;

				make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);
			}

			max_width = max(max_width,(*itor)->rect.right);
			max_height = max(max_height,(*itor)->rect.bottom);
		}

		return acc_height - block->rect.top - block->get_top_space();
	}

	int make_up_lines_float(cls_web_block* block)
	{
		int acc_height = block->rect.top + block->get_top_space();

		vector<cls_web_item*>::iterator itor;

		for(itor=block->children.begin();itor != block->children.end();++itor)
		{
			int cur_height = 0;

			cls_web_item * search_list = (*itor);
			while(search_list != null)
			{
				if (search_list->position.position == position_absolute ||
					search_list->position.element_float != float_none)
				{
					;
				}
				else if (search_list->type != web_text)
				{
					int block_height = make_up_blocks_float_inner(block,search_list->get_web_block(),acc_height);
					cur_height = max(cur_height,block_height);
				}
				else
				{
					cur_height = max(cur_height,search_list->rect.get_height());
				}

				search_list = search_list->slibing;
			}

			search_list = (*itor);

			while(search_list != null)
			{
				int width = search_list->rect.get_width();

				if (search_list->position.position == position_absolute ||
					search_list->position.element_float != float_none)
				{
					;
				}
				else if (search_list->position.position == position_relative)
				{
					search_list->rect.top = acc_height;
					search_list->rect.right += block->backup_2;
					search_list->rect.left += block->backup_1;

					max_width = max(max_width,search_list->rect.right);				
					search_list->rect.bottom = acc_height + cur_height - search_list->position.bottom + search_list->position.top;
		
					max_height = max(max_height,search_list->rect.bottom);
				}
				else
				{
					search_list->rect.top = acc_height;
					search_list->rect.right += block->backup_2;
					search_list->rect.left += block->backup_1;

					max_width = max(max_width,search_list->rect.right);				
					search_list->rect.bottom = acc_height + cur_height;

					max_height = max(max_height,search_list->rect.bottom);
				}

				if (search_list->dock_parent != search_list->parent && search_list->dock_parent->b_stream_block == true)
				{
					search_list->dock_parent->rect.left = min(search_list->rect.left - search_list->dock_parent->get_web_block()->get_left_space(),search_list->dock_parent->rect.left);
					search_list->dock_parent->rect.right = max(search_list->rect.right + search_list->dock_parent->get_web_block()->get_right_space(),search_list->dock_parent->rect.right);
					search_list->dock_parent->rect.top = min(search_list->rect.top - search_list->dock_parent->get_web_block()->get_top_space(),search_list->dock_parent->rect.top);
					search_list->dock_parent->rect.bottom = max(search_list->rect.bottom + search_list->dock_parent->get_web_block()->get_bottom_space(),search_list->dock_parent->rect.bottom);
				}

				search_list = search_list->slibing;
			}

			acc_height += cur_height;
		}

		for(itor=block->independent.begin();itor!=block->independent.end();itor++)
		{
			(*itor)->rect.left += block->backup_1;
			(*itor)->rect.right += block->backup_2;

			int width = (*itor)->rect.get_width();

			if ((*itor)->position.position == position_absolute)
			{
				(*itor)->rect.top = block->rect.top + (*itor)->position.top;

				(*itor)->backup_1 = (*itor)->rect.left;
				(*itor)->backup_2 = (*itor)->rect.right;

				(*itor)->rect.left = block->rect.left + (*itor)->position.left;
				(*itor)->rect.right = max((*itor)->rect.left + width, (*itor)->position.right);

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;


				int height = make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);

				(*itor)->rect.bottom = max((*itor)->rect.top + height,(*itor)->position.bottom);
			}
		
			if ((*itor)->position.element_float == float_left)
			{
				(*itor)->rect.top = block->rect.top;

				(*itor)->backup_1 = (*itor)->rect.left;
				(*itor)->backup_2 = (*itor)->rect.right;

				(*itor)->rect.left = block->rect.left + block->get_left_space() 
					+ ((*itor)->position.position == position_relative?(*itor)->position.left - (*itor)->position.right:0);

				(*itor)->rect.right = (*itor)->rect.left + width ;

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;

				make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);
			}
			else if ((*itor)->position.element_float == float_right)
			{
				(*itor)->rect.top = block->rect.top;

				(*itor)->backup_1 = (*itor)->rect.left;
				(*itor)->backup_2 = (*itor)->rect.right;

				(*itor)->rect.right = block->rect.right - block->get_right_space()
					+ ((*itor)->position.position == position_relative?(*itor)->position.left - (*itor)->position.right:0);

				(*itor)->rect.left = (*itor)->rect.right - width ;

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;

				make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);
			}

			max_width = max(max_width,(*itor)->rect.right);
			max_height = max(max_height,(*itor)->rect.bottom);
		}

		return acc_height - block->rect.top - block->get_top_space();
	}

	int make_up_blocks(cls_web_block*block,int top)
	{
		int height_org = block->dimension.height;
		block->rect.top = top;

		int height = 0;
		
		if ((block->position.element_float == float_none ||
			block->position.element_float == float_left) &&
			block->position.position != position_absolute)
		{
			height = make_up_lines(block);
		}
		else
		{
			height = make_up_lines_float(block);
		}

		if (block->type != web_media && block->type != web_input)
		{
			block->rect.bottom = top + height + block->get_top_space() + block->get_bottom_space();
		
			max_height = max(max_height,block->rect.bottom);
			max_width = max(max_width,block->rect.right);

			return block->rect.get_height();
		}
		else
		{
			block->rect.bottom = top + height_org + block->get_top_space() + block->get_bottom_space();
			
			max_height = max(max_height,block->rect.bottom);
			max_width = max(max_width,block->rect.right);

			return height_org + block->get_top_space() + block->get_bottom_space();
		}
	}

	int make_up_lines_float_inner(cls_web_block* block)
	{
		int acc_height = block->rect.top + block->get_top_space();

		vector<cls_web_item*>::iterator itor;

		for(itor=block->children.begin();itor != block->children.end();++itor)
		{
			int cur_height = 0;

			cls_web_item * search_list = (*itor);
			while(search_list != null)
			{
				if (search_list->position.position == position_absolute ||
					search_list->position.element_float != float_none)
				{
					;
				}
				else if (search_list->type != web_text)
				{
					int block_height = make_up_blocks_float_inner(block,search_list->get_web_block(),acc_height);
					cur_height = max(cur_height,block_height);
				}
				else
				{
					cur_height = max(cur_height,search_list->rect.get_height());
				}

				search_list = search_list->slibing;
			}

			search_list = (*itor);

			while(search_list != null)
			{
				int width = search_list->rect.get_width();

				if (search_list->position.position == position_absolute ||
					search_list->position.element_float != float_none)
				{
					;
				}
				else if (search_list->position.position == position_relative)
				{
					search_list->rect.top = acc_height;
					search_list->rect.left += block->backup_1;
					search_list->rect.right += block->backup_2;

					max_width = max(max_width,search_list->rect.right);				
					search_list->rect.bottom = acc_height + cur_height - search_list->position.bottom + search_list->position.top;

					max_height = max(max_height,search_list->rect.bottom);
				}
				else
				{
					search_list->rect.top = acc_height;
					search_list->rect.left += block->backup_1;
					search_list->rect.right += block->backup_2;

					max_width = max(max_width,search_list->rect.right);				
					search_list->rect.bottom = acc_height + cur_height;

					max_height = max(max_height,search_list->rect.bottom);
				}

				if ((search_list->dock_parent != search_list->parent && search_list->parent->b_stream_block == true))
				{
					search_list->dock_parent->rect.left = min(search_list->rect.left - search_list->dock_parent->get_web_block()->get_left_space(),search_list->dock_parent->rect.left);
					search_list->dock_parent->rect.right = max(search_list->rect.right + search_list->dock_parent->get_web_block()->get_right_space(),search_list->dock_parent->rect.right);
					search_list->dock_parent->rect.top = min(search_list->rect.top - search_list->dock_parent->get_web_block()->get_top_space(),search_list->dock_parent->rect.top);
					search_list->dock_parent->rect.bottom = max(search_list->rect.bottom + search_list->dock_parent->get_web_block()->get_bottom_space(),search_list->dock_parent->rect.bottom);
				}

				search_list = search_list->slibing;
			}

			acc_height += cur_height;
		}

		for(itor=block->independent.begin();itor!=block->independent.end();itor++)
		{
			(*itor)->rect.left += block->backup_1;
			(*itor)->rect.right += block->backup_2;

			int width = (*itor)->rect.get_width();

			if ((*itor)->position.position == position_absolute)
			{
				(*itor)->rect.top = block->rect.top + (*itor)->position.top;

				(*itor)->backup_1 = (*itor)->rect.left;
				(*itor)->backup_2 = (*itor)->rect.right;

				(*itor)->rect.left = block->rect.left + (*itor)->position.left;
				(*itor)->rect.right = max((*itor)->rect.left + width, (*itor)->position.right);

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;

				int height = make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);

				(*itor)->rect.bottom = max((*itor)->rect.top + height,(*itor)->position.bottom);
			}

			if ((*itor)->position.element_float == float_left)
			{
				(*itor)->rect.top = block->rect.top;

				(*itor)->backup_1 = (*itor)->rect.left;
				(*itor)->backup_2 = (*itor)->rect.right;

				(*itor)->rect.left = block->rect.left + block->get_left_space()
					+ ((*itor)->position.position == position_relative?(*itor)->position.left - (*itor)->position.right:0);

				(*itor)->rect.right = (*itor)->rect.left + width;

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;

				make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);
			}
			else if ((*itor)->position.element_float == float_right)
			{
				(*itor)->rect.top = block->rect.top;

				(*itor)->backup_1 = (*itor)->rect.left;
				(*itor)->backup_2 = (*itor)->rect.right;

				(*itor)->rect.right = block->rect.right - block->get_right_space()
					+ ((*itor)->position.position == position_relative?(*itor)->position.left - (*itor)->position.right:0);

				(*itor)->rect.left = (*itor)->rect.right - width;

				(*itor)->backup_1 = (*itor)->rect.left - (*itor)->backup_1;
				(*itor)->backup_2 = (*itor)->rect.right - (*itor)->backup_2;

				make_up_blocks((*itor)->get_web_block(),(*itor)->rect.top);
			}

			max_width = max(max_width,(*itor)->rect.right);
			max_height = max(max_height,(*itor)->rect.bottom);
		}

		return acc_height - block->rect.top - block->get_top_space();
	}

	int make_up_blocks_float_inner(cls_web_block* parent,cls_web_block*block,int top)
	{
		int width = block->rect.get_width();
		int height_org = block->dimension.height;

		block->rect.top = top;

		block->backup_1 = parent->backup_1;
		block->backup_2 = parent->backup_2;

		int height = 0;
		height = make_up_lines_float_inner(block);

		if (block->type != web_media && block->type != web_input)
		{
			block->rect.bottom = top + height + block->get_top_space() + block->get_bottom_space();

			max_height = max(max_height,block->rect.bottom);
			max_width = max(max_width,block->rect.right);

			return block->rect.get_height();
		}
		else
		{
			block->rect.bottom = top + height_org + block->get_top_space() + block->get_bottom_space();

			max_height = max(max_height,block->rect.bottom);
			max_width = max(max_width,block->rect.right);

			return height_org + block->get_top_space() + block->get_bottom_space();
		}
	}

	void make_lines_extra(cls_web_block* block)
	{
		vector<cls_web_item*>::iterator itor;
		for(itor=block->children.begin();itor!=block->children.end();++itor)
		{
			if ((*itor) == null)
				continue;

			if ((*itor)->type != web_text && 
				(*itor)->type != web_input && 
				(*itor)->type != web_media)
			{
				make_lines_extra((*itor)->get_web_block());
			}
			else if ((*itor)->type != web_input)
			{
				(*itor)->rect.right = block->rect.right - block->get_right_space();
			}
		}
	}

	void run(wstring str_filename,cls_web_environment& env)
	{
		clear_items();

		parser.run(str_filename);
		html = parser.get_xml();

		run_unit(&env,html.root,null,null);	
		make_up_blocks(items[0]->get_web_block(),0);

		items[0]->rect.right = env.parsing.width;
		items[0]->rect.bottom = env.parsing.heihgt;
	}

	void run_refresh(cls_web_environment& env)
	{
		clear_items();

		run_unit(&env,html.root,null,null);	
		make_up_blocks(items[0]->get_web_block(),0);

		items[0]->rect.right = env.parsing.width;
		items[0]->rect.bottom = env.parsing.heihgt;
	}

	cls_xml_wide* get_html()
	{
		return &html;
	}

	vector<cls_web_item*>& get_output()
	{
		return items;
	}

	int get_max_height()
	{
		return max_height;
	}

	int get_max_width()
	{
		return  max_width;
	}

	wstring& get_caption()
	{
		return caption;
	}

	wstring& get_base_url()
	{
		return base_url;
	}
};