#include "cls_web_render.h"
#include "cls_web_event.h"
#include "web_interface.h"
#include "cls_web_util.h"

class page_blog_support
	:public cls_web_event_base
{
public:
	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		cls_xml_node_wide* nd_snum = illus_get_em()->get_item_by_id(L"support_num")->node;

		nd_snum->children[0]->context = 
			get_wstr_from_type<int>(get_type_from_wstring<int>(nd_snum->children[0]->context) + 1);

		return cls_web_util::return_continue();
	}
};

class page_blog_profile
	:public cls_web_event_base
{
public:
	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		cls_web_util::modify_style(item,L"background-color",L"#EEEEEE");

		return cls_web_util::return_continue();
	}
};

class page_blog_reply
	:public cls_web_event_base
{
public:
	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		cls_web_item* item_reply_list = illus_get_em()->get_item_by_id(L"reply_list");

		cls_xml_node_wide * node = new cls_xml_node_wide;
		cls_xml_node_wide * node_span = new cls_xml_node_wide;
		cls_xml_node_wide * node_text = new cls_xml_node_wide;
		cls_xml_node_wide * node_span_text = new cls_xml_node_wide;

		node->tag = L"div";
		node->set_attribute(L"style",L"font-size:18px; color:#000000; width:75%; margin-bottom:4px;");

		node_span->tag = L"span";
		node_span_text->tag = TAG_TEXT;
		node_span_text->context = 
			wstring(L"#") + get_wstr_from_type<int>(item_reply_list->node->children.size() + 1);
		node_span->set_attribute(L"style",L"width:40px;");
		node_span->children.push_back(node_span_text);

		node_text->tag = TAG_TEXT;
		node_text->context = 
			illus_get_em()->get_item_by_id(L"reply_content")->get_web_input()->content;

		node->children.push_back(node_span);
		node->children.push_back(node_text);

		item_reply_list->node->children.push_back(node);

		return cls_web_util::return_continue();
	}
};

class page_blog_reply_more
	:public cls_web_event_base
{
public:
	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		cls_web_item* item_reply_list = illus_get_em()->get_item_by_id(L"reply_list");

		wstring wstr_no = L"#" + get_wstr_from_type<int>(item_reply_list->node->children.size() + 1);
		wstring wstr_content = illus_get_em()->get_item_by_id(L"reply_content")->node->context;
		wstring wstr = L"<div style=\"font-size:18px; color:#000000; width:75%; margin-bottom:4px;\"><span style=\"width:40px;\">" + wstr_no + L"</span>" + wstr_content + L"</div>";

		cls_xml_node_wide * node = cls_web_util::trans_html_to_node(wstr);

		item_reply_list->node->children.push_back(node->children[0]);

		return cls_web_util::return_continue();
	}
};

class page_blog_mark
	:public cls_web_event_base
{
public:
	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		cls_web_item * item_result_mark = illus_get_em()->get_item_by_id(L"result-mark");
		cls_web_util::modify_style(item_result_mark,L"display",L"block");

		return cls_web_util::return_continue();
	}
};

class page_blog_hide_mark
	:public cls_web_event_base
{
public:
	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		cls_web_item * item_result_mark = illus_get_em()->get_item_by_id(L"result-mark");
		cls_web_util::modify_style(item_result_mark,L"display",L"none");

		return cls_web_util::return_continue();
	}
};

class page_blog_space
	:public cls_web_event_base
{
public:
	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		illus_get_em()->get_render()->run(L"D:\\fruite\\illus\\illus_excosite.html");

		return cls_web_util::return_continue();
	}
};

class page_blog_exit
	:public cls_web_event_base
{
public:
	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		exit(0);

		return cls_web_util::return_continue();
	}
};

void illus_main()
{
	illus_get_em()->set_event(L"support",new page_blog_support);
	illus_get_em()->set_event(L"profile",new page_blog_profile);
	illus_get_em()->set_event(L"reply",new page_blog_reply_more);
	illus_get_em()->set_event(L"mark",new page_blog_mark);
	illus_get_em()->set_event(L"result-mark",new page_blog_hide_mark);
	illus_get_em()->set_event(L"space",new page_blog_space);
	illus_get_em()->set_event(L"exit",new page_blog_exit);
}

void illus_init()
{
	;
}