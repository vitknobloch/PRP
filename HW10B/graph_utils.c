#include <stdlib.h>
#include <stdio.h>

#include "graph_utils.h"
#include "graph.h"
#include "my_malloc.h"

//loads dijkstra graph and nodes from file
int load_dijkstra(dijkstra_t *dij, const char *fname){
    //open file
    FILE *file = fopen(fname, "r");
    if (file == NULL)
    {
        free_graph(&dij->graph);
        fprintf(stderr, "Error opening file.\n");
        exit(-1);
    }

    int nodes_count = get_nodes_count(file);
    prepare_dijkstra(dij, nodes_count);

    //load edges from txt file until EOF
    int node_num = -1;
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

        if(edge.from > node_num){
            for (int i = node_num + 1; i <= edge.from; i++)
            {
                node_t node;
                node.cost = -1;
                node.parent = -1;
                node.edge_start = dij->graph->edge_count;
                node.edge_count = 0;
                dijkstra_insert_node(dij, &node);
            }
            node_num = edge.from;
        }        
        dij->nodes[edge.from].edge_count++;
        insert_edge(dij->graph, &edge);        
        
    }

    //close stream
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error closing file\n");
    }

    return node_num;
}

_Bool prepare_dijkstra(dijkstra_t *dij, int nodes_count){    
    if (dij->graph)
    {
        free_graph(&dij->graph);
        deallocate_memory((void**)&dij->graph);
    }    
    deallocate_memory((void**)&dij->nodes);
    dij->nodes = allocate_memory(sizeof(node_t) * nodes_count);
    dij->node_capacity = nodes_count;
    dij->graph = allocate_graph_sized(nodes_count * 4);
    if (!dij->graph)
    {
        free(dij);
        dij = NULL;
        return 0;
    }
    return 1;
}

//saves dijkstra result to file
int save_dijkstra(dijkstra_t *dij, const char *fname)
{
    //open file
    FILE *file = fopen(fname, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error writing file\n");
        return 1;
    }

    //write edges one by one
    for (int i = 0; i < dij->num_nodes; i++)
    {
        const node_t node = dij->nodes[i];
        save_number_txt(file, i);
        fputc(' ', file);
        save_number_txt(file, node.cost);
        fputc(' ', file);
        save_number_txt(file, node.parent);
        fputc('\n', file);
    }

    //close file
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error closing file\n");
        return 1;
    }

    return 0;
}

//saves number to file in text format
void save_number_txt(FILE *file, int number){
    if(number < 0){
        number *= -1;
        fputc('-', file);
    }

    //get individual digits in reversed order from number
    char digits[20];
    int counter = 0;
    while (number > 9)
    {
        digits[counter] = '0' + (number % 10);
        number /= 10;
        counter++;
    }
    digits[counter] = '0' + number;

    //write digits in correct order
    while (counter >= 0)
    {
        fputc(digits[counter], file);
        counter--;
    }
}

//loads number from file in text format
int load_number_txt(FILE *file){
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

int get_nodes_count(FILE *file){
    fseek(file, -2, SEEK_END);
    while(fgetc(file) != '\n'){
        fseek(file, -2, SEEK_CUR);
    }
    int count = load_number_txt(file);
    rewind(file);
    return count + 1;
}