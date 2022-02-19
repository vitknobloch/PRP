#ifndef __PQ_RHEAP_H__
#define __PQ_RHEAP_H__

#include "r_bucket.h"

typedef struct{
    int bucket_id;
    int bucket_index;
    int cost;
    int id;
} member_t;

typedef struct{
    int bucket_num;
    bucket_t **buckets;
    int member_count;
    member_t *members;
    int last_pop_index;
} radix_t;

/*
 * Function: pq_alloc
 * ----------------------------
 *   Return address of the newly created priority queue for storing up to the given size
 *   entries. The return value is pointer to void and the particular implementation
 *   of the priority queue can be hidden in the pq.c 
 *   It is supposed the pq_push() and pq_update() function will provide cost of the
 *   vertex with the 0 <= label <= (size-1)
 *
 *   size:  maximal number of the entries in priority queue 
 *
 *   returns: address to the newly allocated priority queue on success; NULL otherwise
 */
radix_t *pq_alloc(int size, int bucket_count);

/*
 * Function: pq_free
 * ----------------------------
 *   Deallocate all the associated memory of the given priority queue
 *   been push to the priority queue.
 *
 *   pq:  pointer to the priority queue allocated by the pq_alloc()
 *
 *   returns:
 */
void pq_free(radix_t *pq);

/*
 * Function: pq_push
 * ----------------------------
 *   Return true if the given label of the vertex with the associated cost has 
 *   been push to the priority queue. The given vertex label is supposed 
 *   to be within the range the 0 <= label <= (size-1), where
 *   the size is the maximal number of entries in the priority queue 
 *   provided to the pq_alloc()
 *
 *   pq:  pointer to the priority queue allocated by the pq_alloc()
 *   label: a vertex label with associated cost
 *   cost:  current cost of the vertex with the label 
 *
 *   returns: true on success; false otherwise
 */
_Bool pq_push(radix_t *pq, int label, int cost);

/*
 * Function: pq_update
 * ----------------------------
 *   Return true if a vertex with the given label is already in the priority queue
 *   and its cost has been updated  The given vertex label is supposed 
 *   to be within the range the 0 <= label <= (size-1), where
 *   the size is the maximal number of entries in the priority queue
 *   provided to the pq_alloc()
 *
 *   pq:  pointer to the priority queue allocated by the pq_alloc()
 *   label: a vertex label with associated cost
 *   cost:  new cost of the vertex with the given label 
 *
 *   returns: true on success; false otherwise
 */
_Bool pq_update(radix_t *pq, int label, int cost);

/*
 * Function: pq_pop
 * ----------------------------
 *   Return true if the label with the lowest cost has been 
 *   removed from the priority queue
 *
 *   pq:  pointer to the priority queue allocated by the pq_alloc()
 *   oLabel: pointer to int, where the label of the
 *           vertex with the lowest cost in the priority queue is stored
 *           on success
 *
 *   returns: true on success; false otherwise
 */
_Bool pq_pop(radix_t *pq, int *oLabel);


#endif

