#include "cls_web_environment.h"
#include "cls_web_parser.h"
#include "web_interface.h"

cls_html_parser_wide cls_web_parser::parser;
cls_parser_css_unit cls_web_environment::css_unit;
map<wstring,int> cls_web_environment::map_style;
cls_parser_css_style cls_web_environment::css_style;

wstring wstr_cur_page;

cls_web_event_manager* illus_get_em()
{
	static cls_web_event_manager em;

	return &em;
}

void illus_set_cur_page(wstring wstr_url)
{
	wstr_cur_page = wstr_url;
}