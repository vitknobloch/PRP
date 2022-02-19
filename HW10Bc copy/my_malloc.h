#ifndef __MY_MALLOC_H__
#define __MY_MALLOC_H__


/*Allocates memory of given size and return pointer*/
void *allocate_memory(int size);
/*Reallocates memory at given pointer to block of size of new_size*/
void reallocate_memory(void **ptr, int new_size);
/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr);

#endif //__MY_MALLOC_H__

