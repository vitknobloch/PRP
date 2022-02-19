#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>

/* Struct with data about one graph*/
typedef struct
{
   int edge_count;
   int edge_capacity;
   int *from;
   int *to;
   int *cost;
} graph_t;

/* Allocate a new graph and return a pointer to it. */
graph_t *allocate_graph();

/*Allocate a new graph with given edge capacity and return pointer to it*/
graph_t *allocate_graph_sized(int capacity);

/* Free all allocated memory and set the pointer to the graph to NULL. */
void free_graph(graph_t **graph);

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph);
/* Save the graph to the text file. */
void save_txt(const graph_t *const graph, const char *fname);

/*Reallocate the graph edges field to fit 'new_capacity' edges*/
void enlarge_graph(graph_t *graph, int new_capacity);

/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph);

/* Save the graph to the binary file. */
void save_bin(const graph_t *const graph, const char *fname);

/**
 * @brief Load text from file to the buffer
 * 
 * @param file file to read from
 * @param buff buffer to load to
 * @param offset position of first unread char relative to cursor
 * @param size number of chars to load from file
 */
void get_buff(FILE *file, char *buff, int offset, unsigned long size);

/*Returns the size of file in bytes and sets graph to predicted size*/
long get_file_size(FILE *file, graph_t *graph);

/*Returns buffer size according to predicted number of edges of the graph*/
unsigned long get_buff_size(int edge_capacity);

/*Loads a number directly from the file*/
int load_number_file(FILE *file);

/*loads number from buffer and saves it to '*out' */
void load_num(const char *buff, int *index, int *out);

/*saves number to buffer*/
int save_num(char *buff, int start_index, int num);

/**
 * @brief Set the graph size to estimated size
 * 
 * @param file text file of the graph
 * @param graph graph to set
 */
void set_graph_size_estimate(FILE *file, graph_t *graph);

#endif // __GRAPH_H__

