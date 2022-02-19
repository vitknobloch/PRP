#ifndef __LOAD_SIMPLE_H__
#define __LOAD_SIMPLE_H__

#include "graph.h"

int load_graph_simple(const char *fname, graph_t *g);
int set_graph_simple(int e, int graph_edges[][3], graph_t *g);
int load_graph(const char *fname, graph_t *g);

#endif
