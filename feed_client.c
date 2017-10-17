/* See LICENCE for license details */

#include "feed_os.h"

#include "feed_client.h"

char
feed_client_init(
    struct feed_client * const
        p_client)
{
    char
        b_result;

    p_client->p_heap =
        (struct feed_heap *)(
            0);

    b_result =
        1;

    return
        b_result;

} /* feed_client_init() */

void
feed_client_cleanup(
    struct feed_client * const
        p_client)
{
    p_client->p_heap =
        (struct feed_heap *)(
            0);

} /* feed_client_cleanup() */

struct feed_heap *
feed_client_get_heap(
    struct feed_client * const
        p_client)
{
    return
        p_client->p_heap;

} /* feed_client_get_heap() */

/* end-of-file: feed_client.c */