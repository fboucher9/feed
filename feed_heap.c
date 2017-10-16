/* See LICENCE for license details */

#include "feed_os.h"

#include "feed_heap.h"

#include "feed_client.h"

struct feed_heap
{
    struct feed_client *
        p_client;

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

struct feed_heap *
feed_heap_create(
    struct feed_client * const
        p_client)
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
        p_heap->p_client =
            p_client;

        p_client->p_heap =
            p_heap;

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

void
feed_heap_destroy(
    struct feed_heap * const
        p_heap)
{
    if (
        p_heap->i_leak_count)
    {
        fprintf(
            stderr,
            "*** detected %ld memory leaks ***\n",
            p_heap->i_leak_count);
    }
    else
    {
        fprintf(
            stderr,
            "*** no memory leaks ***\n");
    }

    p_heap->p_client->p_heap =
        (struct feed_heap *)(
            0);

    free(
        (void *)(
            p_heap));

} /* feed_heap_destroy() */

void *
feed_heap_alloc(
    struct feed_heap * const
        p_heap,
    unsigned int const
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
