#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>

#include "heap.h"
#include "my_malloc.h"

void heap_push(heap_t *heap, heap_t *new_node){
    new_node->parent = heap;
    heap->rank++;

    if(!(heap->min_child)){        
        heap->min_child = new_node;
        new_node->parent = heap;
        new_node->next = new_node;
        new_node->prev = new_node;        
    }
    else{        
        new_node->next = heap->min_child->next;
        new_node->prev = heap->min_child;
        new_node->prev->next = new_node;
        new_node->next->prev = new_node;
        if(new_node->cost < heap->min_child->cost){
            heap->min_child = new_node;
        }
    }
}

void rank_list_add(heap_t **curr_p, heap_t **rank_list){
    heap_t *curr = *curr_p;
    while(rank_list[(curr)->rank] != NULL){
        int index = (curr)->rank;
        heap_t *other = rank_list[index];
        if(other == curr){
            return;
        }
        if((curr)->cost < other->cost){
            heap_erase_child((curr)->parent, other);
            heap_push(curr, other);
        }
        else{
            heap_erase_child((curr)->parent, curr);
            heap_push(other, curr);
            curr = other;
            *curr_p = other;
        }
        rank_list[index] = NULL;        
    }    
    rank_list[(curr)->rank] = curr;
}

void pop_raise_children(heap_t *heap){
    if(!heap->min_child){
        return;
    }
    
    heap->parent->rank += heap->rank;

    heap->min_child->prev->next = heap->parent->min_child->next;
    heap->parent->min_child->next->prev = heap->min_child->prev;
    heap->min_child->prev = heap->parent->min_child;
    heap->parent->min_child->next = heap->min_child;

    heap_t *curr = heap->min_child;
    for (int i = 0; i < heap->rank; i++)
    {
        curr->parent = heap->parent;
        curr = curr->next;
    }
}

void print_heap(heap_t *heap, int tab){
    if(!heap->parent){
        printf("ROOT min: %d\n", heap->min_child->label);
    }
    else{
        printf("HEAP %d |%d|: parent %d, min-ch %d", heap->label, heap->cost, heap->parent->label, heap->min_child ? heap->min_child->label: -1);
        printf(", prev %d, next %d, deg %d\n", heap->prev->label, heap->next->label, heap->rank);
    }
    
    
    if(heap->min_child){
        heap_t* curr = heap->min_child;
        while(curr->next != heap->min_child){
            if(curr->parent == curr){
                printf("CHYBA\n");
                break;
            }
            for(int i = 0; i < tab; i++){
                printf("\t");
            }
            
            print_heap(curr, tab +1);
            curr = curr->next;
        }
        for (int i = 0; i < tab; i++)
        {
            printf("\t");
        }
        print_heap(curr, tab +1);
    }

    if(tab == 1){
        sleep(4);
    }
}

heap_t *heap_pop(heap_t *heap, heap_t **rank_list, int rank_list_len){
    if(!(heap->min_child)){
        return NULL;
    }
    heap_t *ret = heap->min_child;
    rank_list[ret->rank] = NULL;
    pop_raise_children(ret);
    heap_erase_child(heap, ret);

    if(!heap->min_child){
        return ret;
    }

    heap_t *curr = heap->min_child->next;
    while(curr != heap->min_child){
        rank_list_add(&curr, rank_list);
        if(curr->cost < heap->min_child->cost){
            heap->min_child = curr;
        }
        curr = curr->next;
    }

    return ret;
}

heap_t *heap_set_min(heap_t *heap){
    if (!heap->min_child)
    {
        return NULL;
    }

    heap_t *init = heap->min_child;
    heap_t *curr = init->next;

    while (curr != init)
    {
        if (curr->cost < heap->min_child->cost)
        {
            heap->min_child = curr;
        }
        curr = curr->next;
    }

    return heap->min_child;
}

void heap_erase_child(heap_t *heap, heap_t *child){

    if(heap->min_child == child){
       if(child->next == child){
           heap->min_child = NULL;
           heap->rank = 0;
           return;
       }
       else{
           heap->min_child = child->next;
       }
    }
    child->next->prev = child->prev;
    child->prev->next = child->next;
    heap->rank--;
    
}

void heap_update(heap_t *node, int new_cost, heap_t *root){
    node->cost = new_cost;

    if(node->parent != root && new_cost < node->parent->cost){  
        heap_t *parent = node->parent;      
        heap_erase_child(node->parent, node);
        heap_push(root, node);
        heap_mark(parent, root);
    }
}

void heap_mark(heap_t *heap, heap_t *root){
    if(heap->parent == root){
        return;
    }
    
    if(heap->marked){
        heap_erase_child(heap->parent, heap);
        heap_mark(heap->parent, root);
        heap_push(root, heap);
        heap->marked = false;
        return;
    }

    heap->marked = true;
}
