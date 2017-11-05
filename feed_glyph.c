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

#include "feed_utf8.h"

#include "feed_buf.h"

#include "feed_object.h"

static
void
feed_glyph_write_code(
    struct feed_glyph * const
        p_glyph,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    memcpy(
        p_glyph->a_raw,
        p_utf8_code->a_raw,
        p_utf8_code->i_raw_len);

    p_glyph->i_raw_length =
        p_utf8_code->i_raw_len;

    if (p_utf8_code->i_code < 32ul)
    {
        p_glyph->a_visible[0u] =
            '^';

        p_glyph->a_visible[1u] =
            (unsigned char)(
                '@' + p_utf8_code->i_code);

        p_glyph->i_visible_length =
            2u;

        p_glyph->i_visible_width =
            2u;
    }
    else if (p_utf8_code->i_code == 127ul)
    {
        p_glyph->a_visible[0u] =
            '^';

        p_glyph->a_visible[1u] =
            '?';

        p_glyph->i_visible_length =
            2u;

        p_glyph->i_visible_width =
            2u;
    }
    else
    {
        memcpy(
            p_glyph->a_visible,
            p_utf8_code->a_raw,
            p_utf8_code->i_raw_len);

        p_glyph->i_visible_length =
            p_utf8_code->i_raw_len;

        p_glyph->i_visible_width =
            1u;
    }

} /* feed_glyph_write_code() */


static
char
feed_glyph_init(
    struct feed_glyph * const
        p_glyph,
    struct feed_client * const
        p_client,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    char
        b_result;

    feed_list_init(
        &(
            p_glyph->o_list));

    p_glyph->p_client =
        p_client;

    p_glyph->i_raw_length =
        0u;

    p_glyph->i_visible_length =
        0u;

    feed_glyph_write_code(
        p_glyph,
        p_utf8_code);

    b_result =
        1;

    return
        b_result;

} /* feed_glyph_init() */


static
char
feed_glyph_init_cb(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_glyph * const
        p_glyph =
        (struct feed_glyph *)(
            p_object);

    struct feed_utf8_code const * const
        p_utf8_code =
        (struct feed_utf8_code const *)(
            p_descriptor);

    b_result =
        feed_glyph_init(
            p_glyph,
            p_client,
            p_utf8_code);

    return
        b_result;

} /* feed_glyph_init_cb() */


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


static
void
feed_glyph_cleanup_cb(
    void * const
        p_object)
{
    struct feed_glyph * const
        p_glyph =
        (struct feed_glyph *)(
            p_object);

    feed_glyph_cleanup(
        p_glyph);

}


struct feed_glyph *
feed_glyph_create(
    struct feed_client * const
        p_client,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    struct feed_glyph *
        p_glyph;

    p_glyph =
        (struct feed_glyph *)(
            feed_object_create(
                p_client,
                (unsigned int)(
                    sizeof(
                        struct feed_glyph)),
                &(
                    feed_glyph_init_cb),
                (void const *)(
                    p_utf8_code)));

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
        struct feed_client *
            p_client;

        p_client =
            p_glyph->p_client;

        if (
            p_client)
        {
            feed_object_destroy(
                p_client,
                (void *)(
                    p_glyph),
                &(
                    feed_glyph_cleanup_cb));

        }
    }

} /* feed_glyph_destroy() */

/* end-of-file: feed_glyph.c */
