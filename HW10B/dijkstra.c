#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "dijkstra.h"
#include "graph_utils.h"
#include "graph.h"
#include "my_malloc.h"
#include "heap.h"

#define INIT_NODES_CAPACITY 1000

void *dijkstra_init(void){
    dijkstra_t *dij = allocate_memory(sizeof(dijkstra_t));
    dij->start_node = -1;
    dij->num_nodes = 0;
    dij->node_capacity = INIT_NODES_CAPACITY;
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
    dij->nodes[dij->num_nodes] = *node;
    dij->num_nodes++;
}

void enlarge_nodes(dijkstra_t *dijkstra, int new_capacity){
    dijkstra->node_capacity = new_capacity;
    reallocate_memory((void **)&(dijkstra->nodes),
                      sizeof(node_t) * new_capacity);
}


_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra){
    dijkstra_t *dij = (dijkstra_t *)dijkstra;
    prepare_dijkstra(dij , edges[e - 1][0]);
    int nodes_num = -1;
    for(int i = 0; i<e; i++){
        edge_t edge;
        edge.from = edges[i][0];
        edge.to = edges[i][1];
        edge.cost = edges[i][2];
        if(edges[i][0] > nodes_num){
            for(int j = nodes_num +1; j <= edges[i][0]; j++){
                node_t node;
                node.parent = -1;
                node.edge_start = i;
                node.edge_count = 0;
                node.cost = -1;
                dijkstra_insert_node(dij, &node);
            }
            nodes_num = edges[i][0];
        }
        dij->nodes[i].edge_count++;
        insert_edge(dij->graph, &edge);
    }
    return 1;
}

heap_t *create_root(){
    heap_t *root = allocate_memory(sizeof(heap_t));
    root->cost = -1;
    root->rank = 0;
    root->label = -1;
    root->marked = false;
    root->min_child = NULL;
    root->next = root;
    root->prev = root;
    root->parent = NULL;
    
    return root;
}

heap_t *create_heap_nodes(int label, int num_nodes){
    heap_t *h_nodes = allocate_memory(sizeof(heap_t) * num_nodes);
    
    h_nodes[label].cost = 0;
    h_nodes[label].rank = 0;
    h_nodes[label].label = label;
    h_nodes[label].marked = false;
    h_nodes[label].min_child = NULL;

    return h_nodes;
}

_Bool dijkstra_solve(void *dijkstra, int label){
    dijkstra_t *dij = (dijkstra_t*)dijkstra;
    heap_t *root = create_root();

    heap_t *h_nodes = create_heap_nodes(label, dij->num_nodes);
    dij->nodes[label].cost = 0;

    heap_push(root, &h_nodes[label]);

    int rank_list_len = (int)log2(dij->num_nodes) + 1;
    heap_t **rank_list = allocate_memory(sizeof(heap_t*) * rank_list_len);
    memset(rank_list, 0, sizeof(heap_t*) * rank_list_len);

    heap_t *curr_node = heap_pop(root, rank_list, rank_list_len);
    while(curr_node != NULL){
        int curr_label = curr_node->label;
        //printf("POP: %d %d %d\n", curr_node->label, dij->nodes[curr_label].cost, dij->nodes[curr_label].parent);
        if(curr_label == 86553){
            printf("%d\n", h_nodes[73935].cost);
            printf("POP: %d %d %d\n", curr_node->label, dij->nodes[curr_label].cost, dij->nodes[curr_label].parent);
        }
        for (int i = 0; i < dij->nodes[curr_label].edge_count; i++)
        {
            edge_t edge = dij->graph->edges[dij->nodes[curr_label].edge_start + i];
            if(dij->nodes[edge.to].cost == -1){
                h_nodes[edge.to].cost = dij->nodes[edge.from].cost + edge.cost;
                h_nodes[edge.to].rank = 0;
                h_nodes[edge.to].label = edge.to;
                h_nodes[edge.to].marked = false;
                h_nodes[edge.to].min_child = NULL;
                dij->nodes[edge.to].parent = edge.from;
                dij->nodes[edge.to].cost = dij->nodes[edge.from].cost + edge.cost;
                heap_push(root, &h_nodes[edge.to]);
            }
            else if (dij->nodes[edge.to].cost > dij->nodes[edge.from].cost + edge.cost)
            {
                dij->nodes[edge.to].parent = edge.from;
                dij->nodes[edge.to].cost = dij->nodes[edge.from].cost + edge.cost;
                heap_update(&h_nodes[edge.to], dij->nodes[edge.from].cost + edge.cost, root);
            }
        }
        curr_node = heap_pop(root, rank_list, rank_list_len);
    }
    deallocate_memory((void**)&h_nodes);
    deallocate_memory((void**)&rank_list);
    deallocate_memory((void**)&root);
    return true;
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