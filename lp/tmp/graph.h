#ifndef PROTO_GRAPH_H_
#define PROTO_GRAPH_H_

#include <cstdlib>
#include <cstdio>
#include <functional>
#include "plugin.h"
using namespace std;

extern Graph<plugin> plugin_graph;

void init_plugin_graph();
void show_plugin_graph();

#endif /* PROTO_GRAPH_H_ */
