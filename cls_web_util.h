#include <lib_import.h>
#include "cls_web_render.h"
#include "cls_web_environment.h"

class cls_web_util
{
public:

	static cls_xml_node_wide * trans_html_to_node(wstring str)
	{
		str = wstring(L"<xml>") + str + wstring(L"</xml>");

		cls_web_parser::parser.run_raw(str);
		return cls_web_parser::parser.get_xml().root;
	}

	static void modify_style(cls_web_item*item,wstring name,wstring value)
	{
		if (item == null)
			return ;

		CLOSE_EXCEPTION_BEG
		
		cls_web_environment::css_unit.run(item->node->get_attribute(L"style").val.val_str);

		vector<pair<wstring,wstring> >& style = cls_web_environment::css_unit.get_output();
		vector<pair<wstring,wstring> >::iterator itor;

		for(itor=style.begin();itor != style.end();++itor)
		{
			if (itor->first == name)
			{
				itor->second = value;

				set_style(item,style);

				wstring str_style;
				vector<pair<wstring,wstring> >::iterator itor;
				for(itor=style.begin();itor!=style.end();itor++)
				{
					str_style += itor->first + L":" + itor->second + L";";
				}

				item->node->set_attribute(L"style",str_style);

				return;
			}
		}

		style.push_back(make_pair(name,value));

		wstring str_style;
		for(itor=style.begin();itor!=style.end();itor++)
		{
			str_style += itor->first + L":" + itor->second + L";";
		}

		item->node->set_attribute(L"style",str_style);
		return;

		CLOSE_EXCEPTION_END

		item->node->set_attribute(L"style",name + L":" + value + L";");
	}

	static wstring get_style(cls_web_item* item , wstring name)
	{
		CLOSE_EXCEPTION_BEG

		cls_web_environment::css_unit.run(item->node->get_attribute(L"style").val.val_str);

		vector<pair<wstring,wstring> >& style = cls_web_environment::css_unit.get_output();
		vector<pair<wstring,wstring> >::iterator itor;

		for(itor=style.begin();itor!=style.end();++itor)
		{
			if (itor->first == name)
			{
				return itor->second;
			}
		}

		return L"";

		CLOSE_EXCEPTION_END
	}

	static vector<pair<wstring,wstring> > get_style(cls_web_item* item)
	{
		CLOSE_EXCEPTION_BEG
			cls_web_environment::css_unit.run(item->node->get_attribute(L"style").val.val_str);
		CLOSE_EXCEPTION_END

			return cls_web_environment::css_unit.get_output();
	}

	static vector<pair<wstring,wstring> > get_style(cls_xml_node_wide* item)
	{
		CLOSE_EXCEPTION_BEG
			cls_web_environment::css_unit.run(item->get_attribute(L"style").val.val_str);
		CLOSE_EXCEPTION_END

			return cls_web_environment::css_unit.get_output();
	}

	static void set_style(cls_web_item *item,vector<pair<wstring,wstring> >& style)
	{
		wstring str_style;
		vector<pair<wstring,wstring> >::iterator itor;
		for(itor=style.begin();itor!=style.end();itor++)
		{
			str_style += itor->first + L":" + itor->second + L";";
		}

		item->node->set_attribute(L"style",str_style);
	}

	static cls_web_event_return return_continue()
	{
		cls_web_event_return er;
		er.pass_next = true;

		return er;
	}

	static cls_web_event_return return_stop()
	{
		cls_web_event_return er;
		er.pass_next = false;

		return er;
	}
};
