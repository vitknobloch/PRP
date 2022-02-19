#ifndef __GRAPH_UTILS_H__
#define __GRAPH_UTILS_H__

#include <stdio.h>
#include "dijkstra.h"

/**
 * @brief Fills dijkstra structure with data loaded from text file 'fname'
 * 
 * @param dij pointer to dijkstra instance to be filled
 * @param fname filename of the graph-file (string)
 * @return number of nodes of the loaded graph
 */
int load_dijkstra(dijkstra_t *dij, const char *fname);

/**
 * @brief Saves result of dijkstra algorithm stored in dijkstra structure 
 * to text file
 * 
 * @param dij pointer to dijkstra structure with data to save
 * @param fname filename of text file to save the result to (string)
 * @return int - 0 default, 1 - error writing to file
 */
int save_dijkstra(dijkstra_t *dij, const char *fname);

/**
 * @brief Scales the dijkstra node capacity and dijkstra's graph capacity to 
 * prevent excessive reallocation
 * 
 * @param dij dijkstra structure to scale
 * @param nodes_count predicted number of nodes of loaded graph
 * @return _Bool - true on success, false otherwise
 */
_Bool prepare_dijkstra(dijkstra_t *dij, int nodes_count);

#endif

