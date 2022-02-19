#include <stdio.h>
#include <stdlib.h>
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
/*Prints the graph to stdout (debug function)*/
void print_graph(const graph_t *const graph);
/*Loads one number from txt file*/
int load_number_txt(FILE *file);
/*Saves one number to txt file*/
void save_number_txt(FILE *file, const int number);

/* Allocate a new graph and return a reference to it. */
graph_t *allocate_graph()
{
   graph_t *graph = allocate_memory(sizeof(graph_t));
   (*graph).edge_count = 0;
   (*graph).edge_capacity = INITIAL_EDGE_CAPACITY;
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
      edge.start_node = num;
      num = load_number_txt(file);
      edge.end_node = num;
      num = load_number_txt(file);
      edge.weight = num;
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

   edge_t *ptr = (*graph).edges;
   int read_n = 1;
   while (read_n > 0){
      //reallocate graph edge list if necessary
      if ((*graph).edge_count == (*graph).edge_capacity){
         (*graph).edge_capacity *= 2;
         reallocate_memory((void **)&((*graph).edges),
                           sizeof(edge_t) * (*graph).edge_capacity);
         ptr = (*graph).edges + (*graph).edge_count;
      }  

      //read whole edge_t at a time until EOF
      read_n = fread(ptr, sizeof(edge_t), 1, file);
      if (read_n != 1){
         if(ferror(file)){ //terminate in case of error
            fprintf(stderr, "Error reading file.\n");
            free_graph(&graph);
            fclose(file);
            exit(-1);
         }
         break;         
      }
      ptr++;
      (*graph).edge_count++;
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
   if((*graph).edge_count == (*graph).edge_capacity){
      (*graph).edge_capacity *= 2;
      reallocate_memory((void **)&((*graph).edges), 
                        sizeof(edge_t) * (*graph).edge_capacity);
   }
   //Add edge
   (*graph).edges[(*graph).edge_count] = *edge;
   (*graph).edge_count++;
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
   for (int i = 0; i < (*graph).edge_count; i++){
      const edge_t edge = (*graph).edges[i];
      save_number_txt(file, edge.start_node);
      fputc(' ', file);
      save_number_txt(file, edge.end_node);
      fputc(' ', file);
      save_number_txt(file, edge.weight);
      fputc('\n', file);
   }

   //close file
   if(fclose(file)==EOF){
      fprintf(stderr, "Error closing file\n");
   }
}

/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname){
   //open file
   FILE *file = fopen(fname, "w");
   if (file == NULL)
   {
      fprintf(stderr, "Error opening file\n");
      return;
   }

   //write the all the edges from graph edge list to binary file
   int i = fwrite((*graph).edges, sizeof(edge_t), (*graph).edge_count, file);
   //print error message if not all the edges were saved
   if(i != (*graph).edge_count){
      fprintf(stderr, "Error writing file\n");
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

/*Prints the edges to standart output*/
void print_graph(const graph_t * const graph){
   //print each edge on new line
   for (int i = 0; i < (*graph).edge_count; i++)
   {
      const edge_t edge = (*graph).edges[i];
      printf("%d %d %d\n", edge.start_node, edge.end_node, edge.weight);
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

