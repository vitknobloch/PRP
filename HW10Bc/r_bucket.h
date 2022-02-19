#ifndef __R_BUCKET_H__
#define __R_BUCKET_H__

typedef struct{
    int member_capacity;
    int member_count;
    int *member_ids;
} bucket_t;

/**
 * @brief Allocate structure for bucket and all dynamic fields it contains
 * 
 * @param capacity number of members the bucket can initialy have
 * @return Pointer to the newly alloc'd structure
 */
bucket_t * bucket_alloc(int capacity);

/**
 * @brief Add member to bucket
 * 
 * @param bucket busket to push to
 * @param member_id label of node to push
 * @return int - index of the new_member in bucket
 */
int bucket_push(bucket_t *bucket, int member_id);

/**
 * @brief Return label of minimal member of the bucket and erase the member from bucket
 * 
 * @param bucket bucket to pop from
 * @param _pq pointer to pq_rheap which the bucket is member of
 * @return int - label of popped member
 */
int bucket_pop_min(bucket_t *bucket, void *_pq);

/**
 * @brief Return label of member of the bucket in the bucket and erase it from bucket
 * 
 * @param bucket bucket to pop from
 * @param _pq pointer to pq_rheap which the bucket is member of
 * @param index index in bucket->member_ids field to pop
 * @return int - label of popped member
 */
int bucket_pop_index(bucket_t *bucket, void *_pq, int index);

/**
 * @brief Release all memory allocated for the bucket
 * 
 * @param _bucket pointer to pointer to the bucket to free
 */
void bucket_free(bucket_t **_bucket);


#endif

