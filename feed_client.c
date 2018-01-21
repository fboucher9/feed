/* See LICENCE for license details */

#include "feed_os.h"

#include "feed_client.h"

static
unsigned char
g_feed_client_null_object[1u] =
{
    'Z'
};

static
struct feed_heap * const
g_feed_heap_null_ptr =
(struct feed_heap *)(
    g_feed_client_null_object);

static
struct feed_screen_info * const
g_feed_screen_info_null_ptr =
(struct feed_screen_info *)(
    g_feed_client_null_object);

static
struct feed_prompt * const
g_feed_prompt_null_ptr =
(struct feed_prompt *)(
    g_feed_client_null_object);

static
struct feed_text * const
g_feed_text_null_ptr =
(struct feed_text *)(
    g_feed_client_null_object);

static
struct feed_device * const
g_feed_device_null_ptr =
(struct feed_device *)(
    g_feed_client_null_object);

char
feed_client_init(
    struct feed_client * const
        p_client)
{
    char
        b_result;

    p_client->p_heap =
        g_feed_heap_null_ptr;

    p_client->p_screen_info =
        g_feed_screen_info_null_ptr;

    p_client->p_prompt =
        g_feed_prompt_null_ptr;

    p_client->p_text =
        g_feed_text_null_ptr;

    p_client->p_device =
        g_feed_device_null_ptr;

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
        g_feed_heap_null_ptr;

    p_client->p_screen_info =
        g_feed_screen_info_null_ptr;

    p_client->p_prompt =
        g_feed_prompt_null_ptr;

    p_client->p_text =
        g_feed_text_null_ptr;

    p_client->p_device =
        g_feed_device_null_ptr;

} /* feed_client_cleanup() */

void
feed_client_set_heap(
    struct feed_client * const
        p_client,
    struct feed_heap * const
        p_heap)
{
    p_client->p_heap =
        p_heap;

} /* feed_client_set_heap() */

void
feed_client_reset_heap(
    struct feed_client * const
        p_client)
{
    p_client->p_heap =
        g_feed_heap_null_ptr;

} /* feed_client_reset_heap() */

struct feed_heap *
feed_client_get_heap(
    struct feed_client * const
        p_client)
{
    return
        p_client->p_heap;

} /* feed_client_get_heap() */

struct feed_screen_info *
feed_client_get_screen_info(
    struct feed_client * const
        p_client)
{
    return
        p_client->p_screen_info;

} /* feed_client_get_screen_info() */

void
feed_client_set_device(
    struct feed_client * const
        p_client,
    struct feed_device * const
        p_device)
{
    p_client->p_device =
        p_device;

} /* feed_client_set_device() */

void
feed_client_reset_device(
    struct feed_client * const
        p_client)
{
    p_client->p_device =
        g_feed_device_null_ptr;

} /* feed_client_reset_device() */

struct feed_device *
feed_client_get_device(
    struct feed_client * const
        p_client)
{
    return
        p_client->p_device;

} /* feed_client_get_device() */

/* end-of-file: feed_client.c */
