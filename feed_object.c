/* See LICENSE for license details */

#include "feed_os.h"

#include "feed_object.h"

#include "feed_client.h"

#include "feed_heap.h"

void *
feed_object_create(
    struct feed_client * const
        p_client,
    unsigned long int const
        i_buf_len,
    char (*p_init_cb)(
        void * const
            p_buf,
        struct feed_client * const
            p_client,
        void const * const
            p_descriptor),
    void const * const
        p_descriptor)
{
    void *
        p_buf;

    if (
        p_client)
    {
        struct feed_heap *
            p_heap;

        p_heap =
            feed_client_get_heap(
                p_client);

        if (
            p_heap)
        {
            p_buf =
                feed_heap_alloc(
                    p_heap,
                    i_buf_len);

            if (
                p_buf)
            {
                if (
                    (*p_init_cb)(
                        p_buf,
                        p_client,
                        p_descriptor))
                {
                }
                else
                {
                    feed_heap_free(
                        p_heap,
                        p_buf);

                    p_buf =
                        (void *)(
                            0);
                }
            }
        }
        else
        {
            p_buf =
                (void *)(
                    0);
        }
    }
    else
    {
        p_buf =
            (void *)(
                0);
    }

    return
        p_buf;

}

void
feed_object_destroy(
    struct feed_client * const
        p_client,
    void * const
        p_buf,
    void (*p_cleanup_cb)(
        void * const
            p_buf))
{
    if (p_client)
    {
        struct feed_heap *
            p_heap;

        p_heap =
            feed_client_get_heap(
                p_client);

        if (
            p_heap)
        {
            if (p_buf)
            {
                (*p_cleanup_cb)(
                    p_buf);

                feed_heap_free(
                    p_heap,
                    p_buf);
            }
        }
    }
}

