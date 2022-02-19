#include <stdio.h>

#include "graph.h"

int load_graph_simple(const char *fname, graph_t *g) 
{
   int c = 0;
   int exit = 0;
   FILE *f = fopen(fname, "r");
   while (f && !exit) {
      if (g->num_edges == g->capacity) {
         enlarge_graph(g);
      }
      edge_t *e = g->edges + g->num_edges;
      while (g->num_edges < g->capacity) {
         int r = fscanf(f, "%d %d %d\n", &(e->from), &(e->to), &(e->cost));
         if (r == 3) {
            g->num_edges += 1;
            c += 1;
            e += 1;
         } else {
            exit = 1;
            break;
         }
      }
   }
   if (f) {
      fclose(f);
   }
   return c;
}

int set_graph_simple(int e, int graph_edges[][3], graph_t *g)
{
   for(int i = 0; i < e; i++)
   {
      if (g->num_edges == g->capacity)
      {
         enlarge_graph(g);
      }
      edge_t *edge = g->edges + g->num_edges;
      edge->from = graph_edges[i][0];
      edge->to = graph_edges[i][1];
      edge->cost = graph_edges[i][2];

      g->num_edges++;
   }
   
   return e;
}

int load_graph(const char *fname, graph_t *graph)
{
   load_txt(fname, graph);
   return graph->num_edges;
}
