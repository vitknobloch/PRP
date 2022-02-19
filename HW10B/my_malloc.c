#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"

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