

#include "graph.h"

Graph<plugin> plugin_graph;

//proto_info_list中通过协议名来寻找
struct find_plugin_info: public binary_function<plugin_info, const char *, bool> {
	bool operator()(plugin_info pi, const char *p_name) const {
		if (strcmp(pi.plug_in.proto_name, p_name) == 0)
			return true;
		return false;
	}
};

//p_表示plugin，pi_表示plugin_info
//建立proto_graph并建立连接
void init_plugin_graph() {
	//add plugin到graph
	DEBUG_BEGIN();
	vector<plugin_info>::iterator begin = plugin_info_list.begin();
	cout << "plugin size:" << plugin_info_list.size() << endl;
	for (; begin != plugin_info_list.end(); begin++) {
		plugin_graph.add(begin->plug_in);
		DEBUG("%s%s","pluin_info_vector add:",begin->plug_in.proto_name);
	}

	//link各个plugin
	begin = plugin_info_list.begin();
	for (; begin != plugin_info_list.end(); begin++) {
		plugin& p = begin->plug_in;
		const char *p_lower_name = p.lower_proto_name;
		vector<plugin_info>::iterator pi_lower_itr;
		pi_lower_itr = find_if(plugin_info_list.begin(), plugin_info_list.end(),
				bind2nd(find_plugin_info(), p_lower_name));
		if (pi_lower_itr != plugin_info_list.end()) {
			plugin& p_lower = pi_lower_itr->plug_in;
			plugin_graph.link(p_lower,p);
			DEBUG("%s%s%s%s","proto_graph link:",p_lower.proto_name," ==> ",p.proto_name);
		}
	}
	DEBUG_END();
}

static void print_plugin_info(plugin& plug_in) {
	cout << plug_in.proto_name;
}
void show_plugin_graph() {
	DEBUG_BEGIN();
	plugin_graph.debug(print_plugin_info);
	DEBUG_END();
}
