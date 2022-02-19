#include <stdio.h>
#include <stdlib.h>

#include "graph.c"

int main(int argc, char *argv[]){
    graph_t *graph = allocate_graph();
    load_txt("graph.txt", graph);
    save_bin(graph, "loaded.bin");

    free_graph(&graph);
    graph = allocate_graph();
    load_bin("loaded.bin", graph);
    save_txt(graph, "loaded.txt");

    free_graph(&graph);
    return 0;
}