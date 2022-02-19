#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "my_malloc.h"
#include "graph_utils.h"

#define INITIAL_EDGE_CAPACITY 1000

graph_t *allocate_graph()
{
    graph_t *graph = allocate_memory(sizeof(graph_t));
    graph->edge_count = 0;
    graph->edge_capacity = INITIAL_EDGE_CAPACITY;
    graph->edges = allocate_memory(sizeof(edge_t) * INITIAL_EDGE_CAPACITY);

    return graph;
}

graph_t * allocate_graph_sized(int init_capacity){
    graph_t *graph = allocate_memory(sizeof(graph_t));
    graph->edge_count = 0;
    graph->edge_capacity = init_capacity;
    graph->edges = allocate_memory(sizeof(edge_t) * init_capacity);

    return graph;
}

void free_graph(graph_t **graph)
{
    deallocate_memory((void **)&(**graph).edges);
    deallocate_memory((void **)graph);
}

void load_txt(const char *fname, graph_t *graph)
{
    //open file
    FILE *file = fopen(fname, "r");
    if (file == NULL)
    {
        free_graph(&graph);
        fprintf(stderr, "Error opening file.\n");
        exit(-1);
    }

    //load edges from txt file until EOF
    int num = load_number_txt(file);
    while (num != -1)
    {
        edge_t edge;
        edge.from = num;
        num = load_number_txt(file);
        edge.to = num;
        num = load_number_txt(file);
        edge.cost = num;
        num = load_number_txt(file);
        insert_edge(graph, &edge);
    }

    //close stream
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error closing file\n");
    }
}

/*Adds an edge to graph*/
void insert_edge(graph_t *graph, edge_t const *edge)
{
    //reallocate graph edge list if necessary
    if (graph->edge_count == graph->edge_capacity)
    {
        enlarge_graph(graph, graph->edge_capacity*2);
    }
    //Add edge
    graph->edges[graph->edge_count] = *edge;
    graph->edge_count++;
}

void enlarge_graph(graph_t *graph, int new_capacity){
    graph->edge_capacity = new_capacity;
    reallocate_memory((void **)&(graph->edges),
                      sizeof(edge_t) * graph->edge_capacity);
}

/* Save the graph to the text file. */
void save_txt(const graph_t *const graph, const char *fname)
{
    //open file
    FILE *file = fopen(fname, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error writing file\n");
        return;
    }

    //write edges one by one
    for (int i = 0; i < graph->edge_count; i++)
    {
        const edge_t edge = (*graph).edges[i];
        save_number_txt(file, edge.from);
        fputc(' ', file);
        save_number_txt(file, edge.to);
        fputc(' ', file);
        save_number_txt(file, edge.cost);
        fputc('\n', file);
    }

    //close file
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error closing file\n");
    }
}



/*Prints the edges to standart output*/
void print_graph(graph_t *g)
{
    fprintf(stderr, "Graph has %d edges and %d edges are allocated\n", g->edge_count, g->edge_capacity);
    edge_t *e = g->edges;
    for (int i = 0; i < g->edge_count; ++i, ++e)
    {
        printf("%d %d %d\n", e->from, e->to, e->cost);
    }
}