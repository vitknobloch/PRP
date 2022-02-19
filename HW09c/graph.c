#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph.h"

#define INITIAL_EDGE_CAPACITY 1000
#define MAX_ROW_LEN 100
#define MAX_BUFFER_SIZE 4 * 1024 * 1024

void save_buffer(FILE *file, char *str, const int len);
void load_num(const char *buff, int *index, int *out);
void set_graph_size_estimate(FILE *file, graph_t *graph);
/*Allocates memory of given size and return pointer*/
void *allocate_memory(int size);
/*Reallocates memory at given pointer to block of size of new_size*/
void reallocate_memory(void **ptr, int new_size);
/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr);

/* Allocate a new graph and return a reference to it. */
graph_t *allocate_graph()
{
   graph_t *graph = allocate_memory(sizeof(graph_t));
   graph->edge_count = 0;
   graph->edge_capacity = INITIAL_EDGE_CAPACITY;
   graph->edges = allocate_memory(sizeof(edge_t) * INITIAL_EDGE_CAPACITY);

   return graph;
}

/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph){
   deallocate_memory((void **)&(**graph).edges);
   deallocate_memory((void **)graph);
}

unsigned long get_buff_size(int edge_capacity){
   if(edge_capacity < 1000){
      return 1024;
   }
   if(edge_capacity < 10000){
      return 1024 * 512;
   }
   if(edge_capacity < 100000){
      return 1024 * 1024;
   }
   return 4 * 1024 * 1024;
}

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph){
   FILE *file = fopen(fname, "rt");
   edge_t edge;
   unsigned long file_size = get_file_size(file, graph);

   //unsigned long buffer_size = MAX_BUFFER_SIZE > file_size ? MAX_BUFFER_SIZE : file_size;
   unsigned long buffer_size = get_buff_size(graph->edge_capacity);

   char *buff = allocate_memory(buffer_size + 1);
   unsigned long last_loaded_index = 0;
   
   
   buffer_size = file_size - last_loaded_index > buffer_size ? 
               buffer_size : file_size - last_loaded_index;
   get_buff(file, buff, last_loaded_index, buffer_size);
   last_loaded_index += buffer_size;
   int index = 0;

   while(buff[index] != '\0'){
      load_num(buff, &index, &edge.from);
      index++;
      load_num(buff, &index, &edge.to);
      index++;
      load_num(buff, &index, &edge.cost);
      index++;
      insert_edge(graph, &edge);

      if(index > buffer_size - 100 && last_loaded_index != file_size){
         last_loaded_index -= buffer_size - index;
         int offset = index - buffer_size;
         buffer_size = file_size - last_loaded_index > buffer_size ? 
                        buffer_size : file_size - last_loaded_index;
         get_buff(file, buff, offset, buffer_size);
         last_loaded_index += buffer_size;
         index = 0;
      }
   }
   

   fclose(file);
   deallocate_memory((void**)&buff);
}

/*load one integer from buffer string and save to out adress*/
void load_num(const char *buff, int *index, int *out){
   *out = 0;
   while((buff[*index] <= '9') && (buff[*index] >= '0')){
      *out = *out *10 + buff[*index] - '0';

      *index += 1;
   }
}

/*loads buffer string from file*/
void get_buff(FILE *file, char *buff, int offset, unsigned long size){
   //printf("Loading BUFFER\n");
   //printf("%d\n", offset);
   fseek(file, offset, SEEK_CUR);
   assert(fread(buff, size, 1, file) == 1);
   buff[size] = '\0';
}

long get_file_size(FILE *file, graph_t *graph)
{
   fseek(file, 0, SEEK_END);
   long size = ftell(file);

   fseek(file, -2, SEEK_END);
   set_graph_size_estimate(file, graph);
   return size;
}

//loads number from file in text format
int load_number_file(FILE *file)
{
   int result = 0;
   int c = fgetc(file);
   //load digit by digit
   while (c >= '0' && c <= '9')
   {
      result *= 10;
      result += c - '0';
      c = fgetc(file);
   }

   //return -1 on EOF
   if (c == EOF)
      return -1;
   return result;
}

/**
 * @brief Set the graph size to estimated size
 * 
 * @param file text file of the graph
 * @param graph graph to set
 */
void set_graph_size_estimate(FILE *file, graph_t *graph){
   fseek(file, -2, SEEK_END);
   while (fgetc(file) != '\n')
   {
      fseek(file, -2, SEEK_CUR);
   }
   int count = load_number_file(file) + 1;
   rewind(file);
   enlarge_graph(graph, (int)(count * 3.2f));
}

/*saves graph to file in a save way*/
void print_debug(const graph_t *const graph, const char *fname){
   FILE *f = fopen(fname, "w");
   for(int i = 0; i < graph->edge_count; i++){
      fprintf(f, "%d %d %d\n", graph->edges[i].from, graph->edges[i].to, graph->edges[i].cost);
   }
   fclose(f);
}

int save_num(char *buff, int start_index, int num){
   //get individual digits in reversed order from number
   char digits[20];
   int counter = 0;
   while (num > 9)
   {
      digits[counter] = '0' + (num % 10);
      num /= 10;
      counter++;
   }
   digits[counter] = '0' + num;

   //write digits in correct order
   while (counter >= 0)
   {
      buff[start_index++] = digits[counter];
      counter--;
   }
   return start_index;
}


/* Save the graph to the text file. */
void save_txt(const graph_t *const graph, const char *fname)
{
   FILE *file = fopen(fname, "w");
   if (file == NULL)
   {
      fprintf(stderr, "Error writing file\n");
      return;
   }

   unsigned long buffer_size = get_buff_size(graph->edge_capacity);
   int row_chunk_size = buffer_size / MAX_ROW_LEN;
   char *buff = allocate_memory(buffer_size);
   int start_index = 0;
   //write edges one by one
   for (int i = 0; i < (*graph).edge_count; i++)
   {
      const edge_t edge = graph->edges[i];
      start_index = save_num(buff, start_index, edge.from);
      buff[start_index++] = ' ';
      start_index = save_num(buff, start_index, edge.to);
      buff[start_index++] = ' ';
      start_index = save_num(buff, start_index, edge.cost);
      buff[start_index++] = '\n';
      if(i % row_chunk_size == 0){
         fwrite(buff, start_index, 1, file);
         start_index = 0;
      }         
   }
   fwrite(buff, start_index, 1, file);
   deallocate_memory((void**)&buff);

   //close file
   if (fclose(file) == EOF)
   {
      fprintf(stderr, "Error closing file\n");
   }
}

/*Adds an edge to graph*/
void insert_edge(graph_t *graph, edge_t const *edge){
   if(graph->edge_count >= graph->edge_capacity){
      enlarge_graph(graph, graph->edge_count * 2);
   }
   graph->edges[graph->edge_count++] = *edge;
}

void enlarge_graph(graph_t *graph, int new_capacity){
   graph->edge_capacity = new_capacity;
   reallocate_memory((void**)&(graph->edges), sizeof(edge_t) * new_capacity);
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

   int num_nodes;
   fseek(file, -12, SEEK_END);
   if(fread(&num_nodes, 4, 1, file) != 1){
      printf("Error estimating graph size");
   }else
   {
      enlarge_graph(graph, (int)(num_nodes * 3.2f));
   }   
   rewind(file);
   

   edge_t *ptr = graph->edges;
   int read_n = 1;
   while (read_n > 0)
   {
      //reallocate graph edge list if necessary
      if (graph->edge_count == graph->edge_capacity)
      {
         graph->edge_capacity *= 2;
         reallocate_memory((void **)&(graph->edges),
                           sizeof(edge_t) * graph->edge_capacity);
         ptr = graph->edges + graph->edge_count;
      }

      //read whole edge_t at a time until EOF
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
      ptr++;
      graph->edge_count++;
   }

   //close file
   if (fclose(file) == EOF)
   {
      fprintf(stderr, "Error closing file\n");
   }
}
/* Save the graph to the binary file. */
void save_bin(const graph_t *const graph, const char *fname){
   //open file
   FILE *file = fopen(fname, "w");
   if (file == NULL)
   {
      fprintf(stderr, "Error opening file\n");
      return;
   }

   //write the all the edges from graph edge list to binary file
   int i = fwrite(graph->edges, sizeof(edge_t), graph->edge_count, file);
   //print error message if not all the edges were saved
   if (i != graph->edge_count)
   {
      fprintf(stderr, "Error writing file\n");
   }

   //close file
   if (fclose(file) == EOF)
   {
      fprintf(stderr, "Error closing file\n");
   }
}

/*Allocates memory of given size and return pointer*/
void *allocate_memory(int size)
{
   void *ptr = malloc(size);
   if (ptr == NULL)
   {
      fprintf(stderr, "Error allocating memory\n");
      exit(-1);
   }
   return ptr;
}
/*Reallocates memory at given pointer to block of size of new_size*/
void reallocate_memory(void **ptr, int new_size)
{
   *ptr = realloc(*ptr, new_size);
   if (*ptr == NULL)
   {
      fprintf(stderr, "Error re-allocating memory\n");
      exit(-1);
   }
}
/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr)
{
   if (*ptr != NULL)
   {
      free(*ptr);
      *ptr = NULL;
   }
}
