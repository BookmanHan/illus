#include <lib_import.h>
#include "cls_web_render.h"
#include "cls_web_environment.h"

#pragma  once

class cls_web_point
{
public:
	int x;
	int y;
};

enum web_event
{
	event_click,
	event_change,
	event_select,
	event_load,
	event_unload,
	event_mouseover,
	event_mouseout,
	event_abort,
	event_after_update,
	event_before_update,
	event_blur,
	event_bounce,
	event_db_click,
	event_error,
	event_finish,
	event_foucus,
	event_help,
	event_key_press,
	event_key_up,
	event_key_down,
	event_mouse_down,
	event_mouse_move,
	event_mouse_over,
	event_mouse_up,
	event_move,
	event_ready_state_change,
	event_reset,
	event_resize,
	event_scroll,
	event_start,
	event_submit,
};

class cls_web_event_param
{
public:
	web_event type;
	cls_web_point pt;
	cls_xml_wide *html;
};

class cls_web_event_return
{
public:
	bool pass_next;
};

class cls_web_event
{
public:
	virtual cls_web_event_return on_click(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_change(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_select(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_load(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_mouse_over(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_mouse_out(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_mouse_down(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_abort(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_after_update(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_before_update(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_blur(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_bounce(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_db_click(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_error(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_finish(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_focus(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_help(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_key_press(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_key_up(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_key_down(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_mouse_up(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_move(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_ready_state_change(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_reset(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_resize(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_scroll(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_submit(cls_web_item* item,cls_web_event_param& param) = 0;
	virtual cls_web_event_return on_unload(cls_web_item* item,cls_web_event_param& param) = 0;
};

class cls_web_event_base
	:public cls_web_event
{
public:


	virtual cls_web_event_return on_after_update( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_error( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_key_up( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_help( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_mouse_out( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_db_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_bounce( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_blur( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_key_press( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_abort( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_before_update( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_move( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_ready_state_change( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_click( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_submit( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_load( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_focus( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_scroll( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_select( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_change( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_reset( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_mouse_over( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_resize( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_mouse_up( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_key_down( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_mouse_down( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_unload( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual cls_web_event_return on_finish( cls_web_item* item,cls_web_event_param& param ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}
};

class cls_web_rect
{
public:
	int top;
	int left;
	int right;
	int bottom;

	cls_web_rect(cls_rect & rect)
	{
		top = rect.top;
		left = rect.left;
		right = rect.right;
		bottom = rect.bottom;
	}
};

class cls_web_event_manager
{
protected:
	cls_web_render * render;
	vector<cls_web_item*>* items;

public:
	cls_web_render *get_render()
	{
		return render;
	}

	void set_render(cls_web_render* render)
	{
		this->render = render;
		items = &(render->get_items());
	}

	cls_web_item* get_item_by_id(wstring& id)
	{
		return render->map_id_item[id];
	}

	cls_web_item* get_item_by_id(wchar_t id[])
	{
		wstring wid(id);
		return render->map_id_item[wid];
	}

	void set_event(wstring id,cls_web_event* new_event)
	{
		if (render->map_id_item.find(id) != render->map_id_item.end())
		{
			if (render->map_id_item[id]->events != null)
			{
				delete render->map_id_item[id]->events;
			}

			render->map_id_item[id]->events = new_event;
		}
		else
		{
			delete new_event;
		}
	}

	vector<cls_web_item*> find_items_by_point(cls_web_point point)
	{
		vector<cls_web_item*> re;
		map<wstring,cls_web_item*>::reverse_iterator itor;

		for(itor=render->map_id_item.rbegin();itor!=render->map_id_item.rend();++itor)
		{
			if (itor->second->rect.pt_in_rect(point.x,point.y))
			{
				re.push_back(itor->second);
			}
		}

		return re;
	}

	void fire(web_event event_type,cls_web_point point)
	{
		vector<cls_web_item*> fire_items = find_items_by_point(point);
		vector<cls_web_item*>::iterator itor;

		bool flag_refresh = false;

		switch(event_type)
		{
		case event_click:
			{
				cls_web_event_param param;
				param.pt = point;
				param.type = event_click;
				param.html = render->get_html();

				for(itor=fire_items.begin();itor!=fire_items.end();itor++)
				{
					try
					{
						if ((*itor)->events != null)
						{
							flag_refresh = true;
							if ((*itor)->events->on_click(*itor,param).pass_next == false)
							{
								break;
							}
						}
					}
					catch (cls_exception &exc)
					{
						;
					}
					catch (exception except)
					{
						;
					}	
				}
			}
			break;
		}

		if (flag_refresh == true)	
			render->refresh();
	}
};