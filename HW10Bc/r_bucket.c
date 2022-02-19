#include <stdlib.h>
#include "r_bucket.h"
#include "pq_rheap.h"
#include "my_malloc.h"

bucket_t *bucket_alloc(int capacity){
    bucket_t *bucket = allocate_memory(sizeof(bucket_t)); //alloc bucket struct
    bucket->member_capacity = capacity;
    bucket->member_count = 0;
    //alloc field for member ids
    bucket->member_ids = allocate_memory(sizeof(int) * capacity);
    return bucket;
}


int bucket_push(bucket_t *bucket, int member_id){
    //reallocate member_ids if necessary
    if(bucket->member_count >= bucket->member_capacity){
        bucket->member_capacity *= 2;
        reallocate_memory((void**)&bucket->member_ids, sizeof(int) * bucket->member_capacity);
    }

    bucket->member_ids[bucket->member_count] = member_id; //add new member

    return bucket->member_count++; //return the new members index
}


int bucket_pop_min(bucket_t *bucket, void *_pq){
    if(bucket->member_count == 0){ // return -1 if empty
        return -1;
    }

    radix_t *pq = (radix_t*)_pq;

    //go through every member's cost and find minimum
    int min_index = 0;
    //blah
    for(int i = 1; i < bucket->member_count; i++){
        if (pq->members[bucket->member_ids[i]].cost < 
                                pq->members[bucket->member_ids[min_index]].cost)
        {
            min_index = i;
        }
    }
    int min_id = bucket->member_ids[min_index];

    bucket->member_count -= 1;

    //Replace the popped member with last member and update the index in the member
    bucket->member_ids[min_index] = bucket->member_ids[bucket->member_count];
    pq->members[bucket->member_ids[min_index]].bucket_index = min_index;

    return min_id; //return label of popped member
}


int bucket_pop_index(bucket_t *bucket, void *_pq, int index)
{
    if(index >= bucket->member_count || index < 0){ // return -1 if empty
        return -1;
    }

    radix_t *pq = (radix_t *)_pq;
    int id = bucket->member_ids[index]; //label of popped member
    bucket->member_count -= 1;

    //replace the popped member with last member and update last members index in pq->members
    bucket->member_ids[index] = bucket->member_ids[bucket->member_count];
    pq->members[bucket->member_ids[index]].bucket_index = index;

    return id;
}


void bucket_free(bucket_t **_bucket){
    deallocate_memory((void**)&((*_bucket)->member_ids)); //free member_ids
    deallocate_memory((void**)_bucket); //free bucket
    *_bucket = NULL;
}

