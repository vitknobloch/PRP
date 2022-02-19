#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "graph_utils.h"
#include "graph.h"
#include "my_malloc.h"
#include "dijkstra.h"

#define MAX_ROW_LEN 100

/*Returns file size in bytes*/
static unsigned long get_file_size_utils(FILE *file);
/*Returns number of nodes of graph stored in file*/
static int get_nodes_count(FILE *file);


_Bool prepare_dijkstra(dijkstra_t *dij, int nodes_count)
{
    //clear dijkstra if it contains any data
    if (dij->graph)
    {
        free_graph(&dij->graph);
        deallocate_memory((void **)&dij->graph);
    }
    deallocate_memory((void **)&dij->nodes);

    //allocate new structures with predicted size
    dij->nodes = allocate_memory(sizeof(node_t) * nodes_count);
    dij->node_capacity = nodes_count;
    dij->graph = allocate_graph_sized((int)(nodes_count * 3.2f));

    //check for error
    if (!dij->graph)
    {
        free(dij);
        dij = NULL;
        return 0;
    }

    return 1;
}


static unsigned long get_file_size_utils(FILE *file)
{
    fseek(file, 0, SEEK_END); //set cursor to end of file
    long size = ftell(file); //save cursor position
    rewind(file); //reset cursor to start of file
    return size;
}


static int get_nodes_count(FILE *file)
{
    //go to last non-empty line of file
    fseek(file, -2, SEEK_END);
    //search for start of this line
    while (fgetc(file) != '\n')
    {
        fseek(file, -2, SEEK_CUR);
    }
    //load label of last node of the graph
    int count = load_number_file(file);
    //reset cursor to start of file
    rewind(file);
    //return number of nodes of the graph
    return count + 1;
}


int load_dijkstra(dijkstra_t *dij, const char *fname){
    //open file
    FILE *file = fopen(fname, "r");
    if (file == NULL)
    {
        free_graph(&dij->graph);
        fprintf(stderr, "Error opening file.\n");
        exit(-1);
    }
    
    //scale dijkstra
    prepare_dijkstra(dij, get_nodes_count(file));

    //prepare buffer for chunks of the file
    unsigned long file_size = get_file_size_utils(file);
    unsigned long buffer_size = get_buff_size(dij->graph->edge_capacity);
    char *buff = allocate_memory(buffer_size + 1);
    unsigned long last_loaded_index = 0;

    //decrease buffer_size if the is less file left than the original buffer_size
    buffer_size = file_size - last_loaded_index > buffer_size ? 
                                    buffer_size : file_size - last_loaded_index;

    //load buffer
    get_buff(file, buff, last_loaded_index, buffer_size);
    last_loaded_index += buffer_size;

    int index = 0;
    int node_num = -1;
    while (buff[index] != '\0')
    {   //read from buffer until the file ends

        //read edge from buffer
        if (dij->graph->edge_count >= dij->graph->edge_capacity)
        {
            enlarge_graph(dij->graph, dij->graph->edge_count * 2);
        }

        //insert edges one by one
        load_num(buff, &index, &dij->graph->from[dij->graph->edge_count]);
        index++;
        load_num(buff, &index, &dij->graph->to[dij->graph->edge_count]);
        index++;
        load_num(buff, &index, &dij->graph->cost[dij->graph->edge_count]);
        index++;

        const int from = dij->graph->from[dij->graph->edge_count];

        //insert nodes into dijkstra (even if there is no edge from them)
        if (from > node_num)
        {
            for (int i = node_num + 1; i <= from; i++)
            {
                node_t node;
                node.cost = -1;
                node.parent = -1;
                node.edge_start = dij->graph->edge_count;
                node.edge_count = 0;
                dijkstra_insert_node(dij, &node);
            }
            node_num = from;
        }
        (dij->nodes[from]).edge_count++; //update number of edges from current node
        dij->graph->edge_count += 1;
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

    //free buffer
    deallocate_memory((void**)&buff);

    //close file
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error closing file\n");
    }

    return node_num;
}


int save_dijkstra(dijkstra_t *dij, const char *fname)
{
    //open file
    FILE *file = fopen(fname, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error writing file\n");
        return 1;
    }

    //prepare buffer for chunks of the file
    unsigned long buffer_size = get_buff_size(dij->graph->edge_capacity);
    int row_chunk_size = buffer_size / MAX_ROW_LEN; //number of rows to fit one buffer
    char *buff = allocate_memory(buffer_size);
    int start_index = 0;

    //save results in text format to buffer
    for (int i = 0; i < dij->num_nodes; i++)
    {
        const node_t node = dij->nodes[i];
        start_index = save_num(buff, start_index, i);
        buff[start_index++] = ' ';
        start_index = save_num(buff, start_index, node.cost);
        buff[start_index++] = ' ';
        start_index = save_num(buff, start_index, node.parent);
        buff[start_index++] = '\n';

        //save buffer to file when full, and start filling it from start
        if (i % row_chunk_size == 0)
        {
            fwrite(buff, start_index, 1, file);
            start_index = 0;
        }
    }

    //save the rest of file
    fwrite(buff, start_index, 1, file);
    deallocate_memory((void **)&buff); //free buffer

    //close file
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error closing file\n");
        return 1;
    }

    return 0;
}

