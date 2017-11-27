/* See LICENCE for license details */

/*

Module: feed_client.h

Description:

    Execution context.  Provide access to basic services such as heap manager
    and logging system.

*/

struct feed_heap;

struct feed_tty;

struct feed_client
{
    struct feed_heap *
        p_heap;

    struct feed_tty *
        p_tty;

    struct feed_screen_info *
        p_screen_info;

}; /* struct feed_client */

char
feed_client_init(
    struct feed_client * const
        p_client);

void
feed_client_cleanup(
    struct feed_client * const
        p_client);

struct feed_heap *
feed_client_get_heap(
    struct feed_client * const
        p_client);

struct feed_tty *
feed_client_get_tty(
    struct feed_client * const
        p_client);

struct feed_screen_info *
feed_client_get_screen_info(
    struct feed_client * const
        p_client);

/* end-of-file: feed_client.h */
