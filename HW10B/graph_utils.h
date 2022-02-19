#ifndef __GRAPH_UTILS_H__
#define __GRAPH_UTILS_H__

#include <stdio.h>
#include "dijkstra.h"

//loads dijkstra graph and nodes from file
int load_dijkstra(dijkstra_t *dij, const char *fname);

//saves dijkstra result to file
int save_dijkstra(dijkstra_t *dij, const char *fname);

//saves number to file in text format
void save_number_txt(FILE *file, int number);

//loads number from file in text format
int load_number_txt(FILE *file);

int get_nodes_count(FILE *file);

_Bool prepare_dijkstra(dijkstra_t *dij, int nodes_count);

#endif