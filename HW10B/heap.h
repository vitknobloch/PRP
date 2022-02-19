#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>

typedef struct heap_t
{
    struct heap_t *min_child;
    struct heap_t *parent;
    struct heap_t *next;
    struct heap_t *prev;
    int rank;
    bool marked;
    int cost;
    int label;
} heap_t;


void heap_push(heap_t *heap, heap_t *new_node);

heap_t *heap_pop(heap_t *heap, heap_t **deg_list, int deg_list_len);

void heap_update(heap_t *node, int new_cost, heap_t *root);

void heap_mark(heap_t *heap, heap_t *root);

void rank_list_add(heap_t **curr_p, heap_t **deg_list);

heap_t *heap_set_min(heap_t *heap);

void heap_set_degree(heap_t *heap);

void heap_erase_child(heap_t *heap, heap_t *child);

void print_heap(heap_t *heap, int tab);

#endif