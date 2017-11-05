/* See LICENSE for license details */

/*

Module: feed_glyph.c

Description:

    Object to store a single glyph, which represents an array of bytes
    in raw file or an array of bytes to display onto terminal.

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_utf8.h"

#include "feed_glyph.h"

#include "feed_buf.h"

#include "feed_object.h"

static
char
feed_glyph_init(
    struct feed_glyph * const
        p_glyph,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    char
        b_result;

    feed_list_init(
        &(
            p_glyph->o_list));

    p_glyph->o_utf8_code =
        *(
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

    (void)(
        p_client);

    b_result =
        feed_glyph_init(
            p_glyph,
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
    struct feed_client * const
        p_client,
    struct feed_glyph * const
        p_glyph)
{
    if (p_glyph)
    {
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


unsigned char
feed_glyph_render_visible(
    struct feed_glyph const * const
        p_glyph,
    unsigned char * const
        a_visible)
{
    unsigned char
        i_visible_length;

    if (p_glyph->o_utf8_code.a_raw[0u] < 32ul)
    {
        a_visible[0u] =
            '^';

        a_visible[1u] =
            (unsigned char)(
                '@' + p_glyph->o_utf8_code.a_raw[0u]);

        i_visible_length =
            2u;
    }
    else if (p_glyph->o_utf8_code.a_raw[0u] == 127ul)
    {
        a_visible[0u] =
            '^';

        a_visible[1u] =
            '?';

        i_visible_length =
            2u;
    }
    else
    {
        memcpy(
            a_visible,
            p_glyph->o_utf8_code.a_raw,
            p_glyph->o_utf8_code.i_raw_len);

        i_visible_length =
            p_glyph->o_utf8_code.i_raw_len;
    }

    return
        i_visible_length;

}

unsigned char
feed_glyph_get_visible_width(
    struct feed_glyph const * const
        p_glyph)
{
    return
        (
            (
                p_glyph->o_utf8_code.a_raw[0u] < 32ul)
            || (
                p_glyph->o_utf8_code.a_raw[0u] == 127ul)
            ? 2u
            : 1u);
}

/* end-of-file: feed_glyph.c */
