/* See LICENSE for license details */

/*

*/

struct feed_client;

void *
feed_object_create(
    struct feed_client * const
        p_client,
    unsigned int const
        i_buf_len,
    char (*p_init_cb)(
        void * const
            p_object,
        struct feed_client * const
            p_client,
        void const * const
            p_descriptor),
    void const * const
        p_descriptor);

void
feed_object_destroy(
    struct feed_client * const
        p_client,
    void * const
        p_object,
    void (*p_cleanup_cb)(
        void * const
            p_object));

