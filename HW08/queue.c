#include "queue.h"

/*Allocates memory of given size and return pointer*/
void *allocate_memory(int size);
/*Reallocates memory at given pointer to block of size of new_size*/
void reallocate_memory(void **ptr, int new_size);
/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr);
/*Move queue to smaller block if convenient*/
void shrink_queue(queue_t *queue);
void realloc_ptr_arr(queue_t *queue, int new_capacity);
void extend_queue(queue_t *queue);

/* creates a new queue with a given size */
queue_t *create_queue(int capacity)
{
    queue_t *new_queue = allocate_memory(sizeof(queue_t));
    (*new_queue).ptr_arr = allocate_memory(sizeof(void*) * capacity);
    (*new_queue).initial_capacity = capacity;
    (*new_queue).capacity = capacity;
    (*new_queue).head_index = 0;
    (*new_queue).q_len = 0;

    return new_queue;
}

/* deletes the queue and all allocated memory */
void delete_queue(queue_t *queue){
    deallocate_memory((void**)&((*queue).ptr_arr));
    deallocate_memory((void **)&queue);
}

/*
 * inserts a reference to the element into the queue
 * returns: true on success; false otherwise
 */
bool push_to_queue(queue_t *queue, void *data){
    
    extend_queue(queue); //extends queue to double size if possible

    int tail_index = ((*queue).head_index + (*queue).q_len) % (*queue).capacity;

    (*queue).ptr_arr[tail_index] = data;
    (*queue).q_len++;

    return true;
}

/*
 * gets the first element from the queue and removes it from the queue
 * returns: the first element on success; NULL otherwise
 */
void *pop_from_queue(queue_t *queue){
    if((*queue).q_len < 1) //if queue is empty
        return NULL;

    void *ret = (*queue).ptr_arr[(*queue).head_index];
    (*queue).head_index = ((*queue).head_index + 1) % (*queue).capacity;
    (*queue).q_len--;

    shrink_queue(queue); //shrinks queque if it's convenient

    return ret;
}

/*
 * Moves ptr_arr to a smaller location if the number of stored
 * elements is lower than 1/3 of the queue_size.
 */
void shrink_queue(queue_t *queue){
    //only shrink if queue can fit in loacion of 1/3 the current

    if((*queue).q_len > (*queue).capacity / 3)
        return;

    int new_capacity = (*queue).capacity / 2;

    //Don't shrink to less than the initial size
    if(new_capacity < (*queue).initial_capacity)
        new_capacity = (*queue).initial_capacity;

    realloc_ptr_arr(queue, new_capacity);
}

/* Moves ptr_arr to a double sized location if the current queue is full*/
void extend_queue(queue_t *queue){
    //only extend if the queue cannot fit another element
    if ((*queue).q_len < (*queue).capacity)
        return;

    int new_capacity = (*queue).q_len * 2;
    
    realloc_ptr_arr(queue, new_capacity);
}

/* Moves pointers from ptr_arr to array of size new_capacity*/
void realloc_ptr_arr(queue_t *queue, int new_capacity){

    void **new_ptr_arr = allocate_memory(sizeof(void *) * new_capacity);

    for (int i = 0; i < (*queue).q_len; i++)
    {
        int index = ((*queue).head_index + i) % (*queue).capacity;
        new_ptr_arr[i] = (*queue).ptr_arr[index];
    }

    deallocate_memory((void **)&((*queue).ptr_arr));
    (*queue).ptr_arr = new_ptr_arr;
    (*queue).capacity = new_capacity;
    (*queue).head_index = 0;
}

/*
 * gets idx-th element from the queue, i.e., it returns the element that 
 * will be popped after idx calls of the pop_from_queue()
 * returns: the idx-th element on success; NULL otherwise
 */
void *get_from_queue(queue_t *queue, int idx){
    if(idx < 0 || idx >= (*queue).q_len)
        return NULL;
    
    int index = ((*queue).head_index + idx) % (*queue).capacity;
    return (*queue).ptr_arr[index];
}

/* gets number of stored elements */
int get_queue_size(queue_t *queue){
    return (*queue).q_len;
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
