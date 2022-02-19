#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>

#include "graph.h"

#define INITIAL_EDGE_CAPACITY 1000

    /*Allocates memory of given size and return pointer*/
    void *allocate_memory(int size);
/*Reallocates memory at given pointer to block of size of new_size*/
void reallocate_memory(void **ptr, int new_size);
/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr);
/*Adds and edge to graph*/
void insert_edge(graph_t *graph, edge_t const *edge);
/*Loads one number from txt file*/
int load_number_txt(FILE *file);
/*Saves one number to txt file*/
void save_number_txt(FILE *file, const int number);

_Bool is_little_endian();

/* Allocate a new graph and return a reference to it. */
graph_t *allocate_graph()
{
   graph_t *graph = allocate_memory(sizeof(graph_t));
   (*graph).num_edges = 0;
   (*graph).capacity = INITIAL_EDGE_CAPACITY;
   (*graph).edges = allocate_memory(sizeof(edge_t) * INITIAL_EDGE_CAPACITY);

   return graph;
}

/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph){
   deallocate_memory((void**)&(**graph).edges);
   deallocate_memory((void**)graph);
}

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph){
   //open file
   FILE *file = fopen(fname, "r");
   if(file == NULL){
      free_graph(&graph);
      fprintf(stderr, "Error opening file.\n");
      exit(-1);
   }
   
   //load edges from txt file until EOF
   int num = load_number_txt(file);
   while(num != -1){
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
   if (fclose(file) == EOF){
      fprintf(stderr, "Error closing file\n");
   }
}

/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph){
   //open file
   FILE *file = fopen(fname, "rb");
   if (file == NULL)
   {
      free_graph(&graph);
      fprintf(stderr, "Error opening file.\n");
      exit(-1);
   }

   _Bool is_little = is_little_endian();
   edge_t *ptr = (*graph).edges;
   int read_n = 1;
   while (read_n > 0){
      //reallocate graph edge list if necessary
      if ((*graph).num_edges == (*graph).capacity){
         (*graph).capacity *= 2;
         reallocate_memory((void **)&((*graph).edges),
                           sizeof(edge_t) * (*graph).capacity);
         ptr = (*graph).edges + (*graph).num_edges;
      }  

      //read whole edge_t at a time until EOF
      if(is_little){
         int vals[3];
         read_n = fread(vals, sizeof(int), 3, file);
         ptr->from = ntohl(vals[0]);
         ptr->to = ntohl(vals[1]);
         ptr->cost = ntohl(vals[2]);
         if (read_n != 3)
         {
            if (ferror(file))
            { //terminate in case of error
               fprintf(stderr, "Error reading file.\n");
               free_graph(&graph);
               fclose(file);
               exit(-1);
            }
            break;
         }
      }else{
         read_n = fread(ptr, sizeof(edge_t), 1, file);
         if (read_n != 1)
         {
            if (ferror(file))
            { //terminate in case of error
               fprintf(stderr, "Error reading file.\n");
               free_graph(&graph);
               fclose(file);
               exit(-1);
            }
            break;
         }
      }
      
      
      ptr++;
      (*graph).num_edges++;
   }

   //close file
   if(fclose(file) == EOF){
      fprintf(stderr, "Error closing file\n");
   }
}

/* Loads one number from txt file*/
int load_number_txt(FILE *file){   
   int result = 0;
   int c = fgetc(file);
   //load digit by digit
   while (c >= '0' && c <= '9'){
      result *= 10;
      result += c-'0';
      c = fgetc(file);
   }

   //return -1 on EOF
   if(c == EOF)
      return -1;
   return result;
}

/*Adds an edge to graph*/
void insert_edge(graph_t *graph, edge_t const *edge){
   //reallocate graph edge list if necessary
   if((*graph).num_edges == (*graph).capacity){
      (*graph).capacity *= 2;
      reallocate_memory((void **)&((*graph).edges), 
                        sizeof(edge_t) * (*graph).capacity);
   }
   //Add edge
   (*graph).edges[(*graph).num_edges] = *edge;
   (*graph).num_edges++;
}

/* Save the graph to the text file. */
void save_txt(const graph_t * const graph, const char *fname){
   //open file
   FILE *file = fopen(fname, "w");
   if(file == NULL){
      fprintf(stderr, "Error writing file\n");
      return;
   }

   //write edges one by one
   for (int i = 0; i < (*graph).num_edges; i++){
      const edge_t edge = (*graph).edges[i];
      save_number_txt(file, edge.from);
      fputc(' ', file);
      save_number_txt(file, edge.to);
      fputc(' ', file);
      save_number_txt(file, edge.cost);
      fputc('\n', file);
   }

   //close file
   if(fclose(file)==EOF){
      fprintf(stderr, "Error closing file\n");
   }
}

/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname){
   _Bool is_little = is_little_endian();
   //open file
   FILE *file = fopen(fname, "w");
   if (file == NULL)
   {
      fprintf(stderr, "Error opening file\n");
      return;
   }

   int vals[3];
   for (int i = 0; i < graph->num_edges; i++)
   {
      if(is_little){
         vals[0] = htonl(graph->edges[i].from);
         vals[1] = htonl(graph->edges[i].to);
         vals[2] = htonl(graph->edges[i].cost);
      }
      else{
         vals[0] = graph->edges[i].from;
         vals[1] = graph->edges[i].to;
         vals[2] = graph->edges[i].cost;
      }

      //write the all the edges from graph edge list to binary file
      int i = fwrite(vals, sizeof(int), 3, file);
      //print error message if not all the edges were saved
      if (i != 3)
      {
         fprintf(stderr, "Error writing file\n");
         break;
      }
   }

   //close file
   if (fclose(file) == EOF)
   {
      fprintf(stderr, "Error closing file\n");
   }
}

/*Writes one number in text file*/
void save_number_txt(FILE *file, int number){
   //get individual digits in reversed order from number
   char digits[20];
   int counter = 0;
   while(number > 9){
      digits[counter] = '0' + (number % 10);
      number /= 10;
      counter ++;
   }
   digits[counter] = '0' + number;

   //write digits in correct order
   while(counter >= 0){
      fputc(digits[counter], file);
      counter--;
   }
}

_Bool is_little_endian()
{
   unsigned int i = 1;
   char *c = (char *)&i;
   return (int)*c;
}

graph_t *enlarge_graph(graph_t *g)
{
   assert(g != NULL);
   int n = g->capacity == 0 ? INITIAL_EDGE_CAPACITY : g->capacity * 2; /* double the memory */

   edge_t *e = allocate_memory(n * sizeof(edge_t));
   memcpy(e, g->edges, g->num_edges * sizeof(edge_t));
   free(g->edges);
   g->edges = e;
   g->capacity = n;
   return g;
}

/*Prints the edges to standart output*/
void print_graph(graph_t *g)
{
   assert(g != NULL);
   fprintf(stderr, "Graph has %d edges and %d edges are allocated\n", g->num_edges, g->capacity);
   edge_t *e = g->edges;
   for (int i = 0; i < g->num_edges; ++i, ++e)
   {
      printf("%d %d %d\n", e->from, e->to, e->cost);
   }
}

/*Allocates memory of given size and return pointer*/
void *allocate_memory(int size){
   void *ptr = malloc(size);
   if (ptr == NULL){
      fprintf(stderr, "Error allocating memory\n");
      exit(-1);
   }
   return ptr;
}

/*Reallocates memory at given pointer to block of size of new_size*/
void reallocate_memory(void **ptr, int new_size){
   *ptr = realloc(*ptr, new_size);
   if (*ptr == NULL)
   {
      fprintf(stderr, "Error re-allocating memory\n");
      exit(-1);
   }
}

/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr){
   if(*ptr != NULL){
      free(*ptr);
      *ptr = NULL;
   }
}

