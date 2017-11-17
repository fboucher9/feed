/* See LICENCE for license details */

/*

Module: feed_heap.c

Description:

    Memory manager.

*/

/* Precompiled header file */
#include "feed_os.h"

/* Compile-time configuration options */
#include "feed_cfg.h"

/* Heap */
#include "feed_heap.h"

/*

Structure: feed_heap

Description:

    State and statistics for memory manager.

*/
struct feed_heap
{
    signed long int
        i_leak_count;

    unsigned long int
        i_stat_total_alloc;

    unsigned long int
        i_stat_total_free;

    unsigned long int
        i_stat_total_alloc_error;

    unsigned long int
        i_stat_total_free_error;

};

/*

Function: feed_heap_create

Description:

    Create a heap object.

*/
struct feed_heap *
feed_heap_create(void)
{
    struct feed_heap *
        p_heap;

    p_heap =
        (struct feed_heap *)(
            malloc(
                sizeof(
                    struct feed_heap)));

    if (
        p_heap)
    {
        p_heap->i_leak_count =
            0l;

        p_heap->i_stat_total_alloc =
            0ul;

        p_heap->i_stat_total_free =
            0ul;

        p_heap->i_stat_total_alloc_error =
            0ul;

        p_heap->i_stat_total_free_error =
            0ul;

    }

    return
        p_heap;

} /* feed_heap_create() */

/*

Function: feed_heap_destroy

Description:

    Destroy a heap object that was created by feed_heap_create.

*/
void
feed_heap_destroy(
    struct feed_heap * const
        p_heap)
{
#if defined(FEED_CFG_DEBUG)
    if (
        p_heap->i_leak_count)
    {
        printf(
            "*** detected %ld memory leaks ***\n",
            p_heap->i_leak_count);
    }
    else
    {
        printf(
            "*** no memory leaks ***\n");
    }
#endif /* #if defined(FEED_CFG_DEBUG) */

    free(
        (void *)(
            p_heap));

} /* feed_heap_destroy() */

/*

Function: feed_heap_alloc

Description:

    Allocate memory

*/
void *
feed_heap_alloc(
    struct feed_heap * const
        p_heap,
    unsigned long int const
        i_buffer_length)
{
    void *
        p_buffer;

    if (
        i_buffer_length)
    {
        p_buffer =
            malloc(
                (size_t)(
                    i_buffer_length));

        if (
            p_buffer)
        {
            p_heap->i_leak_count ++;

            p_heap->i_stat_total_alloc ++;
        }
        else
        {
            p_heap->i_stat_total_alloc_error ++;
        }
    }
    else
    {
        p_heap->i_stat_total_alloc_error ++;

        p_buffer =
            (void *)(
                0);
    }

    return
        p_buffer;

} /* feed_heap_alloc() */

/*

Function: feed_heap_free

Description:

    Free memory that was allocated by feed_heap_alloc

*/
void
feed_heap_free(
    struct feed_heap * const
        p_heap,
    void * const
        p_buffer)
{
    if (
        p_buffer)
    {
        free(
            p_buffer);

        p_heap->i_stat_total_free ++;

        p_heap->i_leak_count --;
    }
    else
    {
        p_heap->i_stat_total_free_error ++;
    }

} /* feed_heap_free() */

/* end-of-file: feed_heap.c */
