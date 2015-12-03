#include <lib_import.h>
#pragma  once

#define COLOR_TRANSPARENT 1000000000
#define INNER_COLOR(r,g,b) (r*1000000 + g*100 + b)
#define GET_INNER_COLOR_R(color) (color/1000000)
#define GET_INNER_COLOR_G(color) (color%1000000/1000)
#define GET_INNER_COLOR_B(color) (color%1000)

typedef unsigned web_color;

enum web_css_background_attatchment
{
	attatchment_scroll,
	attatchment_fixed,
};

enum web_css_background_repeated
{
	repeated_repeat,
	repeated_repeat_x,
	repeated_repeat_y,
	repeated_no_repeat,
};

enum web_css_border_style
{
	border_none,
	border_hidden,
	border_dotted,
	border_dashed,
	border_soild,
	border_double_,
	border_groove,
	border_ridge,
	border_inset,
	border_outset,
};

enum web_css_text_direction
{
	direction_ltor,
	direction_rtol,
};

enum web_css_align
{
	align_left,
	align_right,
	align_center,
	align_justify,
	align_bottom,
	align_top,
	align_vcenter,
};

enum web_css_text_decoration
{
	decoration_normal_d,
	decoration_underline,
	decoration_orverline,
	decoration_line_throught,
	decoration_blink,
};

enum web_css_font_style
{
	font_normal,
	font_italic,
	font_oblique,
};

enum web_css_list_type
{
	list_none,
	list_disc,
	list_circle,
	list_square,
};

enum web_css_display
{
	display_none,
	display_block,
	display_inline,
	display_inline_block,
	display_list_item,
	display_run_in,
	display_table,
	display_inline_table,
};

enum web_css_position_layout
{
	position_relative,
	position_absolute,
	position_fixed,
	position_static,
};

enum web_css_float
{
	float_left,
	float_right,
	float_none,
};

enum web_css_text_transform
{
	ttransform_none,
	ttransform_capitalize,
	ttransform_uppercase,
	ttransform_lowercase,
};

enum web_css_font_stretch
{
	stretch_normal,
	stretch_wider,
	stretch_narrower,
	stretch_ultra_condensed,
	stretch_extra_condensed,
	stretch_condensed,
	stretch_semi_condensed,
	stretch_semi_expanded,
	stretch_expanded,
	stretch_extra_expanded,
	stretch_ultra_expanded,
};

enum web_css_vertical_align
{
	baseline,
	sub,
	super,
	top,
	text_top,
	middle,
	bottom,
	text_bottom,
	length,
};

struct web_css_background
{
	web_css_background_attatchment attatchment;
	web_color color;
	wstring image;
	cls_point<int> position;
	web_css_background_repeated repeat;

	web_css_background()
	{
		attatchment = attatchment_fixed;
		color = COLOR_TRANSPARENT;
		image = L"";
	}
};

struct web_css_border
{
	web_color color;
	web_css_border_style style;
	int width;

	web_css_border()
	{
		color = COLOR_TRANSPARENT;
		style = border_none;
		width = 0;
	}
};

struct web_css_text
{
	web_color color;
	web_css_text_direction direction;
	int letter_spacing;
	int line_height;
	web_css_align align;
	web_css_text_decoration decoration;
	int indent;
	web_css_text_transform transform;
	int while_space;
	int word_spacing;

	web_css_text()
	{
		align = align_bottom;
		color = INNER_COLOR(0,0,0);
		direction = direction_ltor;
		letter_spacing = 0;
		line_height =  -1;
		indent = 0;
		while_space = 0;
		word_spacing = 0;
	}
} ;

struct web_css_font
{
	wstring family;
	int size;
	web_css_font_stretch stretch;
	web_css_font_style style;
	int weight;

	web_css_font()
	{
		family = L"Î¢ÈíÑÅºÚ";
		size = 24;
		style = font_normal;
		weight = 400;
	}
};

struct web_css_margin_padding
{
	int size;

	web_css_margin_padding()
	{
		size = 0;
	}

} ;

struct web_css_list
{
	web_css_list_type type;
	wstring image_url;
	bool inside;

	web_css_list()
	{
		type = list_disc;
		image_url = L"";
		inside = true;
	}
};

struct web_css_dimension
{
	int height;
	int width;
	int max_height;
	int min_height;
	int min_width;
	int max_width;

	web_css_dimension()
	{
		height = 0;
		width = 0;
		max_height = 10000;
		max_width = 10000;
		min_height = 0;
		min_width = 0;
	}
};

struct web_css_position
{
	int bottom;
	int top;
	int left;
	int right;

	cls_rect clip;

	bool visible;
	int z_index;
	web_css_display display;
	web_css_position_layout position;
	web_css_float element_float;
	web_css_float element_clear;
	web_css_align align;

	web_css_position()
	{
		bottom = 0;
		left = 0;
		top = 0;
		right = 0;

		align = align_bottom;
		visible = true;
		z_index = 0;
		display = display_run_in;
		position = position_static;

		element_float = float_none;
		element_clear = float_none;
	}
};
