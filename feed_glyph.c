/* See LICENSE for license details */

/*

Module: feed_glyph.c

Description:

    Object to store a single glyph, which represents an array of bytes
    in raw file or an array of bytes to display onto terminal.

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_glyph.h"

#include "feed_input.h"

#include "feed_buf.h"

#include "feed_client.h"

#include "feed_heap.h"

static
void
feed_glyph_write_event(
    struct feed_glyph * const
        p_glyph,
    struct feed_event const * const
        p_event)
{
    memcpy(
        p_glyph->a_raw,
        p_event->a_raw,
        p_event->i_raw_len);

    p_glyph->i_raw_length =
        p_event->i_raw_len;

    if (
        FEED_EVENT_KEY_FLAG & p_event->i_code)
    {
        struct feed_buf
            o_buf;

        memset(
            p_glyph->a_visible,
            0u,
            sizeof(
                p_glyph->a_visible));

        p_glyph->a_visible[0u] =
            '<';

        feed_buf_init(
            &(
                o_buf),
            p_glyph->a_visible + 1u,
            sizeof(
                p_glyph->a_visible) - 1u);

        feed_input_print(
            p_event,
            &(
                o_buf));

        p_glyph->a_visible[1u + o_buf.i_len] =
            '>';

        p_glyph->i_visible_length =
            (unsigned char)(
                o_buf.i_len + 2u);

        p_glyph->i_visible_width =
            (unsigned char)(
                o_buf.i_len + 2u);
    }
    else
    {
        memcpy(
            p_glyph->a_visible,
            p_event->a_raw,
            p_event->i_raw_len);

        p_glyph->i_visible_length =
            p_event->i_raw_len;

        p_glyph->i_visible_width =
            1u;

    }

} /* feed_glyph_write_event() */


static
void
feed_glyph_init(
    struct feed_glyph * const
        p_glyph,
    struct feed_client * const
        p_client,
    struct feed_event const * const
        p_event)
{
    feed_list_init(
        &(
            p_glyph->o_list));

    p_glyph->p_client =
        p_client;

    p_glyph->i_raw_length =
        0u;

    p_glyph->i_visible_length =
        0u;

    p_glyph->i_visible_width =
        0u;

    feed_glyph_write_event(
        p_glyph,
        p_event);

} /* feed_glyph_init() */


static
void
feed_glyph_cleanup(
    struct feed_glyph * const
        p_glyph)
{
    feed_list_join(
        &(
            p_glyph->o_list),
        &(
            p_glyph->o_list));

} /* feed_glyph_cleanup() */


struct feed_glyph *
feed_glyph_create(
    struct feed_client * const
        p_client,
    struct feed_event const * const
        p_event)
{
    struct feed_glyph *
        p_glyph;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_glyph =
        (struct feed_glyph *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_glyph)));

    if (
        p_glyph)
    {
        feed_glyph_init(
            p_glyph,
            p_client,
            p_event);
    }

    return
        p_glyph;

} /* feed_glyph_create() */


void
feed_glyph_destroy(
    struct feed_glyph * const
        p_glyph)
{
    if (p_glyph)
    {
        struct feed_heap *
            p_heap;

        {
            struct feed_client *
                p_client;

            p_client =
                p_glyph->p_client;

            p_heap =
                feed_client_get_heap(
                    p_client);
        }

        feed_glyph_cleanup(
            p_glyph);

        feed_heap_free(
            p_heap,
            (void *)(
                p_glyph));

    }

} /* feed_glyph_destroy() */

/* end-of-file: feed_glyph.c */