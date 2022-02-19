#include <stdlib.h>
#include <math.h>

#include "dijkstra.h"
#include "graph_utils.h"
#include "graph.h"
#include "my_malloc.h"
#include "pq_rheap.h"

void *dijkstra_init(){
    dijkstra_t *dij = allocate_memory(sizeof(dijkstra_t));
    dij->start_node = -1;
    dij->num_nodes = 0;
    dij->node_capacity = 0;
    dij->longest_edge = 0;
    dij->nodes = NULL;
    dij->graph = NULL;
    return dij;
}


_Bool dijkstra_load_graph(const char *filename, void *dijkstra){
    return load_dijkstra((dijkstra_t *)dijkstra, filename) > 0;
}


void dijkstra_insert_node(void *dijkstra, node_t const *node){
    dijkstra_t *dij = (dijkstra_t*)dijkstra;

    //reallocate graph edge list if necessary
    if (dij->node_capacity == dij->num_nodes)
    {
        enlarge_nodes(dij, dij->node_capacity * 2);
    }

    //Add edge
    dij->nodes[dij->num_nodes++] = *node;
}


void enlarge_nodes(dijkstra_t *dijkstra, int new_capacity){
    dijkstra->node_capacity = new_capacity;
    reallocate_memory((void **)&(dijkstra->nodes), sizeof(node_t) * new_capacity);
}


_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra){
    dijkstra_t *dij = (dijkstra_t *)dijkstra;
    prepare_dijkstra(dij , edges[e - 1][0]);
    int nodes_num = -1;

    //insert every edge to dijkstra's graph
    for(int i = 0; i<e; i++){
        dij->graph->from[i] = edges[i][0];
        dij->graph->to[i] = edges[i][1];
        dij->graph->cost[i] = edges[i][2];

        //insert nodes even when there is no edge from them
        if(edges[i][0] > nodes_num){
            for(int j = nodes_num +1; j <= edges[i][0]; j++){
                node_t node;
                node.parent = -1;
                node.edge_start = i; //index of first edge from this node
                node.edge_count = 0;
                node.cost = -1;
                dijkstra_insert_node(dij, &node);
            }
            nodes_num = edges[i][0];
        }
        //update number of edges from this node
        dij->nodes[i].edge_count++;
    }
    return 1;
}


_Bool dijkstra_solve(void *dijkstra, int label){
    dijkstra_t *dij = (dijkstra_t *)dijkstra;

    //check if label is a node of graph
    if (!dij || label < 0 || label >= dij->num_nodes)
    {
        return 0;
    }
    dij->start_node = label;

    //initialize radix heap based priority queue
    radix_t *pq = pq_alloc(dij->num_nodes, (int)(log2(dij->num_nodes + 1)) + 1);

    dij->nodes[label].cost = 0L; // initialize the starting node
    pq_push(pq, label, 0L);

    int cur_label;
    float div = 0;
    while (pq_pop(pq, &cur_label)) //while there are nodes in the queue
    {
        node_t *cur = &(dij->nodes[cur_label]);
        //go through the popped node's edges
        for (int i = 0; i < cur->edge_count; ++i)
        {                                                             
            const int edge_index = cur->edge_start + i;
            node_t *to = &(dij->nodes[dij->graph->to[edge_index]]);
            const int cost = cur->cost + dij->graph->cost[edge_index]; //calculate cost through this node
            if (to->cost == -1L)
            { // the node to has not been visited yet
                to->cost = cost;
                to->parent = cur_label;
                pq_push(pq, dij->graph->to[edge_index], cost);
            }
            else if (cost < to->cost)
            { // already relaxed check if we can make a shortcut to child node via the current node
                to->cost = cost;
                to->parent = cur_label;
                pq_update(pq, dij->graph->to[edge_index], cost);
            }
        } // end all children of the cur node;
        if(cur->cost / dij->longest_edge > div){
            div = cur->cost / dij->longest_edge;
        }
    }     // end pq_is_empty

    printf("Poměr: %f", div);
    pq_free(pq); // release the memory

    return 1;
}


_Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3]){
    dijkstra_t *dij = (dijkstra_t*)dijkstra;

    for(int i = 0; i < dij->num_nodes; i++){
        solution[i][0] = i;
        solution[i][1] = dij->nodes[i].cost;
        solution[i][2] = dij->nodes[i].parent;
    }
    return 1;
}


_Bool dijkstra_save_path(const void *dijkstra, const char *filename){
    return save_dijkstra((dijkstra_t *)dijkstra, filename) == 0;
}


void dijkstra_free(void *dijkstra){
    dijkstra_t *dij = (dijkstra_t *)dijkstra;
    deallocate_memory((void**)&dij->nodes);
    free_graph(&dij->graph);
    deallocate_memory(&dijkstra);
}

