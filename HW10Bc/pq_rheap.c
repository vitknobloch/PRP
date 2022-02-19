#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>

#include "pq_rheap.h"
#include "r_bucket.h"
#include "my_malloc.h"

static void set_ranges(int *ranges, int min, int max, int count);

void print_ranges(int *ranges, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%6d ", ranges[i]);
    }
    printf("\n");
}

radix_t *pq_alloc(int size, int bucket_count)
{
    radix_t *pq = allocate_memory(sizeof(radix_t)); //allocate pq structure

    //allocate member field
    pq->member_count = size;
    pq->members = allocate_memory(sizeof(member_t) * size);

    //allocate field of pointers to buckets
    pq->bucket_num = bucket_count;
    pq->buckets = allocate_memory(sizeof(bucket_t*) * bucket_count);
    pq->ranges = allocate_memory(sizeof(int) * (bucket_count + 1));

    //allocate the buckets with increasing sizes
    for (int i = 0; i < pq->bucket_num; i++){
        int bucket_size;
        int max = (int)log2(size) + 1;
        if (i < max)
        {
            bucket_size = (1 << i);
        }
        else
        {
            bucket_size = max + 1;
        }

        pq->buckets[i] = bucket_alloc(bucket_size);
    }

    set_ranges(pq->ranges, 0, INT_MAX, bucket_count);
    pq->ranges[bucket_count] = INT_MAX;

    return pq; //return pointer to alloc'd structure
}

void pq_free(radix_t *pq){
    //free members field
    deallocate_memory((void**)&pq->members);

    //free all of bucket's memory
    for(int i = 0; i < pq->bucket_num; i++){
        bucket_free(&pq->buckets[i]);
    }

    //free field of pointers to buckets
    deallocate_memory((void**)&pq->buckets);
    deallocate_memory((void**)&pq->ranges);

    deallocate_memory((void**)&pq);//free pq structure
}

_Bool pq_push(radix_t *pq, int label, int cost){
    //find the diff_bit between last-popped nodes cost and pushed nodes cost
    int bucket_id = -1;

    //printf("PUSH %6d COST %ld\n", label, cost);

    for(int i = 1; i <= pq->bucket_num; i++){
        if(pq->ranges[i] > cost){
            bucket_id = i-1;
            break;
        }
    }

    if(bucket_id == -1){
        printf("PUSH ERROR %6d COST %d\n", label, cost);
        return 0;
    }

    //fill information to correct members index
    pq->members[label].id = label;
    pq->members[label].cost = cost;
    pq->members[label].bucket_id = bucket_id;
    //push to correct bucket
    pq->members[label].bucket_index = bucket_push(pq->buckets[bucket_id], label);

    return 1;
}

_Bool pq_update(radix_t *pq, int label, long cost){
    
    //if the bucket is still the same, only update the cost
    if( cost > pq->ranges[pq->members[label].bucket_id]){
        pq->members[label].cost = cost;
        return 1;
    }

    //else pop member from pq
    if(-1 != bucket_pop_index(pq->buckets[pq->members[label].bucket_id], pq, 
                                            pq->members[label].bucket_index)){
        //push it with new cost
        pq_push(pq, label, cost);
        return 1;
    }
    else{
        return 0;
    }

}

_Bool pq_pop(radix_t *pq, int *oLabel)
{
    int i;
    for(i = 0; i < pq->bucket_num; i++){ //go through buckets
        *oLabel = bucket_pop_min(pq->buckets[i], pq);
        if(*oLabel != -1){ //break when found non-empty bucket
            break;
        }
    }

    if(*oLabel == -1){ //return false if all buckets were empty        
        return 0;
    }

    if(i > 0){
        /*if the non-empty bucket wasn't the first one 
        (all costs in first bucket are the same)
        take all remaining members of the bucket and push them angain 
        (they will have different diff_bit with last_popped)*/
        set_ranges(pq->ranges, pq->members[*oLabel].cost, pq->ranges[i+1], i+1);
        int mem_count = pq->buckets[i]->member_count;
        pq->buckets[i]->member_count = 0; //virtually empty the bucket
        for(int m = 0; m < mem_count; m++){
            pq_push(pq, pq->buckets[i]->member_ids[m], 
                            pq->members[pq->buckets[i]->member_ids[m]].cost);
        }
    }

    return 1;
}


static void set_ranges(int *ranges, int min, int max, int count)
{
    if (count == 1)
    {
        ranges[0] = min;
        ranges[1] = max;
        return;
    }
    
    double range_size = log2(max - min) / (count - 1);
    ranges[0] = min;

    for (int i = 0; i < count - 1; i++)
    {
        ranges[i + 1] = min + (long)round(pow(2, i * range_size));
    }

}
