#ifndef __GRAPH_H__
#define __GRAPH_H__

/* Struct with data of individual edge*/
typedef struct {
   int from;
   int to;
   int cost;
} edge_t;

/* Struct with data about one graph*/
typedef struct {
   int num_edges;
   int capacity;
   edge_t *edges;
} graph_t;

/* Allocate a new graph and return a reference to it. */
graph_t* allocate_graph();
/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph);

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph);
/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph);

/* Save the graph to the text file. */
void save_txt(const graph_t * const graph, const char *fname);
/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname);

void print_graph(graph_t *g);

graph_t *enlarge_graph(graph_t *g);

#endif // __GRAPH_H__
