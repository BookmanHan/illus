#include <lib_import.h>

class cls_parser_css_style
	:public cls_parser_wide<vector<wstring> >
{
protected:
	vector<pair<vector<wstring>,wstring > > mmap_attributes;

public:

	cls_parser_css_style()
	{		
		vector<string> v_product;
		v_product.push_back("#start#@#css_list#");
		v_product.push_back("#css_list#@#css_list##css_unit#");
		v_product.push_back("#css_list#@#css_unit#");
		v_product.push_back("#pre_list#@$var$");
		v_product.push_back("#pre_list#@#pre_list#$var$");
		v_product.push_back("#css_unit#@#pre_list#${$$var$$}$");

		yacc.add_product(v_product);
		yacc.run();

		vector<cls_lalr_table_item> table;
		yacc.get_output(table);

		frame = new cls_lalr_frame_wide<vector<wstring> >(table,yacc.info,yacc.get_kernel_size(),this);
	}

	virtual wstring operator()(const wstring& item)
	{
		if (item == L"__inner_end")
		{
			return L"__inner_end";
		}
		else if (check_alpha(item[0]))
		{
			return wstring(L"var");
		}
		else
		{
			return item;
		}
	}

	virtual void operator()(vector<cls_lalr_frame_item_wide<vector<wstring> > > & now,
		int next_status,
		cls_lalr_frame_item_wide<vector<wstring> > &re,
		vector<cls_lalr_frame_item_wide<vector<wstring> > >& stack)
	{
		next_status -= 10000;

		switch(next_status)
		{
		case 3:
			{
				re.user_data.push_back(now[1].str_pre);
			}
			break;

		case 4:
			{
				re.user_data = now[1].user_data;
				re.user_data.push_back(now[2].str_pre);
			}
			break;

		case 5:
			{
				mmap_attributes.push_back(make_pair(now[1].user_data,now[3].str_pre));
			}
			break;
		}
	}

	virtual bool check_alpha( const wchar_t c ) 
	{
		return  c!= ' ' && c != '\t' && c != '{' && c != '}';
	}

	virtual bool check_digit( const wchar_t c ) 
	{
		return false;
	}

	virtual vector<wstring> * run( const wstring& str_context ) 
	{
		mmap_attributes.clear();

		vector<wstring> tokens_pre;
		spilt(str_context,tokens_pre);

		vector<wstring> tokens;
		vector<wstring>::iterator itor;
		for(itor=tokens_pre.begin();itor!=tokens_pre.end();itor++)
		{
			wstring str_post = alg_wstring_trim(*itor);
			if (str_post == L"")
				continue;
			else
				tokens.push_back(str_post);
		}

		frame->run_map(tokens);

		return null;
	}

	virtual vector<pair<vector<wstring>,wstring> > & get_output()
	{
		return mmap_attributes;
	}
};