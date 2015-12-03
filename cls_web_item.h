#include "cls_web_css.h"
#pragma  once

enum web_item
{
	web_text,
	web_media,
	web_block,
	web_form,
	web_input,
	web_list,
};

enum web_input_type
{
	input_button,
	input_checkbox,
	input_file,
	input_hidden,
	input_image,
	input_password,
	input_radio,
	input_reset,
	input_submit,
	input_text,
};

class cls_web_block;
class cls_web_form;
class cls_web_input;
class cls_web_item;
class cls_web_list;
class cls_web_media;
class cls_web_text;
class cls_web_event;

class cls_web_item
{
protected:
	cls_web_item()
	{
		backup_1 = 0;
		backup_2 = 0;
		rect.left = 0;
		rect.right = 0;
		rect.bottom = 0;
		rect.top = 0;

		node = null;
		slibing = null;
		parent = null;
		events = null;

		b_stream_block = false;
	}

public:

	bool operator < (const cls_web_item& that) const
	{
		return position.z_index < that.position.z_index;
	}

	virtual ~cls_web_item()
	{
		if (events != null)
			delete events;
	}

	web_item type;

	bool b_stream_block;

	int backup_1;
	int backup_2;

	cls_web_event * events;
	cls_xml_node_wide *node;
	cls_web_item *slibing;
	cls_web_item *parent;
	cls_web_item *dock_parent;

	vector<cls_web_item*> children;
	vector<cls_web_item*> independent;

	cls_rect rect;
	web_css_position position;

	cls_web_block * get_web_block()
	{
		if (type != web_block && type != web_list && type != web_media
			&& type != web_form && type != web_input)
			throw cls_exception_execution("type error.");

		return (cls_web_block*)this;
	}

	cls_web_text * get_web_text()
	{
		if (type != web_text)
			throw cls_exception_execution("type error.");

		return (cls_web_text*)this;
	}

	cls_web_media * get_web_media()
	{
		if (type != web_media)
			throw cls_exception_execution("type error.");

		return (cls_web_media*)this;
	}

	cls_web_form * get_web_form()
	{
		if (type != web_form)
			throw cls_exception_execution("type error.");

		return (cls_web_form*)this;
	}

	cls_web_input * get_web_input()
	{
		if (type != web_input)
			throw cls_exception_execution("type error.");

		return (cls_web_input*)this;
	}

	cls_web_list* get_web_list()
	{
		if (type != web_list)
			throw cls_exception_execution("type error.");

		return (cls_web_list*)this;
	}
};

class cls_web_text
	:public cls_web_item
{
public:
	web_css_font font;
	web_css_text text;

	wstring content;

	cls_web_text()
	{
		content.clear();
		type = web_text;
	}
};

class cls_web_block
	:public cls_web_item
{
public:

	web_css_background background;
	web_css_border 
		border_left,border_right,border_top,border_bottom;
	
	web_css_dimension dimension;
	web_css_margin_padding margin_left,margin_top,margin_bottom,
		margin_right,padding_top,padding_bottom,padding_left,padding_right;

	cls_web_block()
	{
		type = web_block;
	}

	int get_left_space()
	{
		return margin_left.size + padding_left.size + border_left.width;
	}

	int get_top_space()
	{
		return margin_top.size + padding_top.size + border_top.width;
	}

	int get_right_space()
	{
		return margin_right.size + padding_right.size + border_right.width;
	}

	int get_bottom_space()
	{
		return margin_bottom.size + padding_bottom.size + border_bottom.width;
	}
};

class cls_web_list
	:public cls_web_block
{
public:
	web_css_list list;

	cls_web_list()
	{
		type = web_list;
	}
};

class cls_web_media
	:public cls_web_block
{
public:
	wstring url;

	cls_web_media()
	{
		type = web_media;
	}
};

class cls_web_form
	:public cls_web_block
{
public:
	cls_web_form()
	{
		type = web_form;
	}
};

class cls_web_input
	:public cls_web_block
{
public:
	web_input_type input_type;
	wstring content;

	web_css_font font;
	web_css_text text;

	cls_web_input()
	{
		type = web_input;
	}
};

inline
bool cmp_pointer_item(const cls_web_item* src,const cls_web_item* des)
{
	return src->position.z_index < des->position.z_index;
}