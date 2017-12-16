/* See LICENCE for license details */

/*

Module: feed_heap.h

Description:
    Memory manager.

*/

#if defined(INC_FEED_HEAP_H)
#error include once
#endif /* #if defined(INC_FEED_HEAP_H) */

#define INC_FEED_HEAP_H

struct feed_heap;

struct feed_heap *
feed_heap_create(void);

void
feed_heap_destroy(
    struct feed_heap * const
        p_heap);

void *
feed_heap_alloc(
    struct feed_heap * const
        p_heap,
    unsigned long int const
        i_buffer_length);

void
feed_heap_free(
    struct feed_heap * const
        p_heap,
    void * const
        p_buffer);

void *
feed_heap_realloc(
    struct feed_heap * const
        p_heap,
    void * const
        p_buffer,
    unsigned long int const
        i_buffer_length);

/* end-of-file: feed_heap.h */
