#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph.h"
#include "my_malloc.h"

#define INITIAL_EDGE_CAPACITY 1000
#define MAX_ROW_LEN 100


graph_t *allocate_graph()
{
   graph_t *graph = allocate_memory(sizeof(graph_t));
   graph->edge_count = 0;
   graph->edge_capacity = INITIAL_EDGE_CAPACITY;
   graph->from = allocate_memory(sizeof(int) * INITIAL_EDGE_CAPACITY);
   graph->to = allocate_memory(sizeof(int) * INITIAL_EDGE_CAPACITY);
   graph->cost = allocate_memory(sizeof(int) * INITIAL_EDGE_CAPACITY);

   return graph;
}


graph_t *allocate_graph_sized(int capacity)
{
   graph_t *graph = allocate_memory(sizeof(graph_t));
   graph->edge_count = 0;
   graph->edge_capacity = capacity;
   graph->from = allocate_memory(sizeof(int) * capacity);
   graph->to = allocate_memory(sizeof(int) * capacity);
   graph->cost = allocate_memory(sizeof(int) * capacity);

   return graph;
}


void free_graph(graph_t **graph){
   deallocate_memory((void **)&(**graph).from);
   deallocate_memory((void **)&(**graph).to);
   deallocate_memory((void **)&(**graph).cost);
   deallocate_memory((void **)graph);
}


unsigned long get_buff_size(int edge_capacity){
   //limit buffer size to save RAM with small graphs
   if(edge_capacity < 1000){
      return 1024; //1KB buffer for small graphs
   }
   if(edge_capacity < 10000){
      return 1024 * 512;
   }
   if(edge_capacity < 100000){
      return 1024 * 1024;
   }
   return 4 * 1024 * 1024; //4MB buffer for large graphs
}


void load_txt(const char *fname, graph_t *graph){
   FILE *file = fopen(fname, "rt");
   if (file == NULL)
   {
      free_graph(&graph);
      fprintf(stderr, "Error opening file.\n");
      exit(-1);
   }

   //prepare buffer for chunks of the file
   unsigned long file_size = get_file_size(file, graph);
   unsigned long buffer_size = get_buff_size(graph->edge_capacity);
   char *buff = allocate_memory(buffer_size + 1);
   unsigned long last_loaded_index = 0;

   //decrease buffer_size if the is less file left than the original buffer_size
   buffer_size = file_size - last_loaded_index > buffer_size ? 
               buffer_size : file_size - last_loaded_index;

   //load buffer
   get_buff(file, buff, last_loaded_index, buffer_size);
   last_loaded_index += buffer_size;

   int index = 0;
   while(buff[index] != '\0')
   {  //read from buffer until the file ends

      if (graph->edge_count >= graph->edge_capacity)
      {
         enlarge_graph(graph, graph->edge_count * 2);
      }

      //insert edges one by one
      load_num(buff, &index, &graph->from[graph->edge_count]);
      index++;
      load_num(buff, &index, &graph->to[graph->edge_count]);
      index++;
      load_num(buff, &index, &graph->cost[graph->edge_count]);
      index++;

      graph->edge_count += 1;

      //check if new buffer has to be loaded
      if (index > buffer_size - MAX_ROW_LEN && last_loaded_index != file_size)
      {
         //load again any parts that has not yet been read
         last_loaded_index -= buffer_size - index;
         int offset = index - buffer_size;

         //decrease buffer_size if the is less file left than the original buffer_size
         buffer_size = file_size - last_loaded_index > buffer_size ? 
                                 buffer_size : file_size - last_loaded_index;

         //load buffer
         get_buff(file, buff, offset, buffer_size);
         last_loaded_index += buffer_size;
         index = 0;
      }
   }

   //close file
   if (fclose(file) == EOF)
   {
      fprintf(stderr, "Error closing file\n");
   }

   deallocate_memory((void**)&buff); //free buffer
}


void load_num(const char *buff, int *index, int *out){
   *out = 0;
   //read number digit by digit while there isn't non-digit character
   while((buff[*index] <= '9') && (buff[*index] >= '0')){
      *out = *out *10 + buff[*index] - '0';

      *index += 1; //move 'buffer cursor'
   }
}


void get_buff(FILE *file, char *buff, int offset, unsigned long size){
   fseek(file, offset, SEEK_CUR); //adjust cursor for ofset
   assert(fread(buff, size, 1, file) == 1); //load from file to buffer
   buff[size] = '\0'; //end buffer with '\0'
}


long get_file_size(FILE *file, graph_t *graph)
{
   fseek(file, 0, SEEK_END); //set cursor to end of file
   long size = ftell(file); //save cursor position

   set_graph_size_estimate(file, graph); //scale graph

   return size;
}


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


void set_graph_size_estimate(FILE *file, graph_t *graph){
   fseek(file, -2, SEEK_END); //set cursor to last non-empty line of file

   //search for start of the line
   while (fgetc(file) != '\n')
   {
      fseek(file, -2, SEEK_CUR);
   }

   //get the countof nodes of the graph (from node of the last edge)
   int count = load_number_file(file) + 1;
   rewind(file); //reset cursor to start of file
   enlarge_graph(graph, (int)(count * 3.2f)); //scale fraph to fit estimated number of edges
}

/*saves graph to file in a slow and save way (debug function) */
void print_debug(const graph_t *const graph, const char *fname){
   FILE *f = fopen(fname, "w");
   for(int i = 0; i < graph->edge_count; i++){
      fprintf(f, "%d %d %d\n", graph->from[i], graph->to[i], graph->cost[i]);
   }
   fclose(f);
}

int save_num(char *buff, int start_index, int num){
   //save -1 if that's the number
   if(num < 0){
      buff[start_index++] = '-';
      num *= -1;
   }
   //get individual digits in reversed order from number
   char digits[20];
   int counter = 0;
   while (num > 9)
   {
      digits[counter++] = '0' + (num % 10);
      num /= 10;
   }
   digits[counter] = '0' + num;

   //write digits in correct order
   while (counter >= 0)
   {
      buff[start_index++] = digits[counter--];
   }

   return start_index;
}


void save_txt(const graph_t *const graph, const char *fname)
{
   FILE *file = fopen(fname, "w");
   if (file == NULL)
   {
      fprintf(stderr, "Error writing file\n");
      return;
   }

   //prepare buffer for chunks of file
   unsigned long buffer_size = get_buff_size(graph->edge_capacity);
   int row_chunk_size = buffer_size / MAX_ROW_LEN; //number of lines to fit in one buffer
   char *buff = allocate_memory(buffer_size);
   int start_index = 0;

   //write edges one by one
   for (int i = 0; i < (*graph).edge_count; i++)
   {
      start_index = save_num(buff, start_index, graph->from[i]);
      buff[start_index++] = ' ';
      start_index = save_num(buff, start_index, graph->to[i]);
      buff[start_index++] = ' ';
      start_index = save_num(buff, start_index, graph->cost[i]);
      buff[start_index++] = '\n';

      //save buffer to file if full and start filling it from start
      if(i % row_chunk_size == 0){
         fwrite(buff, start_index, 1, file);
         start_index = 0;
      }         
   }

   //save the rest to file
   fwrite(buff, start_index, 1, file);
   deallocate_memory((void**)&buff);

   //close file
   if (fclose(file) == EOF)
   {
      fprintf(stderr, "Error closing file\n");
   }
}


void enlarge_graph(graph_t *graph, int new_capacity){
   graph->edge_capacity = new_capacity;
   reallocate_memory((void**)&(graph->from), sizeof(int) * new_capacity);
   reallocate_memory((void **)&(graph->to), sizeof(int) * new_capacity);
   reallocate_memory((void **)&(graph->cost), sizeof(int) * new_capacity);
}


void load_bin(const char *fname, graph_t *graph){
   //open file
   FILE *file = fopen(fname, "rb");
   if (file == NULL)
   {
      free_graph(&graph);
      fprintf(stderr, "Error opening file.\n");
      exit(-1);
   }

   //get the number of nodes from file
   int num_nodes;
   fseek(file, -12, SEEK_END);
   if(fread(&num_nodes, 4, 1, file) != 1){
      printf("Error estimating graph size");
   }else
   {
      enlarge_graph(graph, (int)(num_nodes * 3.2f));
   }   
   rewind(file); //reset cursor
   
   //pointer to location to save next edge
   int read_n = 1;
   while (read_n > 0)
   {
      //reallocate graph edge list if necessary
      if (graph->edge_count == graph->edge_capacity)
      {
         enlarge_graph(graph, graph->edge_capacity * 2);
      }

      //read whole edge_t at a time until EOF
      int edge[3];
      read_n = fread(edge, sizeof(int), 3, file); //load directly to edges list
      graph->from[graph->edge_count] = edge[0];
      graph->to[graph->edge_count] = edge[1];
      graph->cost[graph->edge_count] = edge[2];
      if (read_n != 3)
      {
         if (ferror(file))
         { //terminate in case of error
            fprintf(stderr, "Error reading file.\n");
            free_graph(&graph);
            fclose(file);
            exit(-1);
         }
         break; //at the end of file
      }

      graph->edge_count++;
      
   }

   //close file
   if (fclose(file) == EOF)
   {
      fprintf(stderr, "Error closing file\n");
   }
}


void save_bin(const graph_t *const graph, const char *fname){
   //open file
   FILE *file = fopen(fname, "w");
   if (file == NULL)
   {
      fprintf(stderr, "Error opening file\n");
      return;
   }

   //write the all the edges from graph edge list to binary file
   for(int i = 0; i < graph->edge_count; i++){
      const int edge[3] = {graph->from[i], graph->to[i], graph->cost[i]};
      const int w = fwrite(edge, sizeof(int), 3, file);
      //print error message if not all the edges were saved
      if (w != 3)
      {
         fprintf(stderr, "Error writing file\n");
      }
   }
   

   //close file
   if (fclose(file) == EOF)
   {
      fprintf(stderr, "Error closing file\n");
   }
}

