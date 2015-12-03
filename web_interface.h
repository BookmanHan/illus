#include "cls_web_render.h"
#include "cls_web_event.h"

void illus_main();
void illus_init();

extern wstring wstr_cur_page;

cls_web_event_manager* illus_get_em();
void illus_set_cur_page(wstring wstr_url);