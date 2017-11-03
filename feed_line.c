/* See LICENSE for license details */

/*

Module: feed_line.c

Description:

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_line.h"

#include "feed_glyph.h"

#include "feed_object.h"

#include "feed_buf.h"

static
char
feed_line_init(
    struct feed_line * const
        p_line,
    struct feed_client * const
        p_client)
{
    char
        b_result;

    feed_list_init(
        &(
            p_line->o_list));

    p_line->p_client =
        p_client;

    feed_list_init(
        &(
            p_line->o_glyphs));

    p_line->i_glyph_count =
        0u;

    b_result =
        1;

    return
        b_result;

}

static
char
feed_line_init_cb(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_line * const
        p_line =
        (struct feed_line *)(
            p_object);

    (void)(
        p_descriptor);

    b_result =
        feed_line_init(
            p_line,
            p_client);

    return
        b_result;

}

static
void
feed_line_reset(
    struct feed_line * const
        p_line)
{
    struct feed_list *
        p_iterator;

    struct feed_list *
        p_next;

    struct feed_glyph *
        p_glyph;

    /* Free all characters... */
    p_iterator =
        p_line->o_glyphs.p_next;

    while (
        p_iterator != &(p_line->o_glyphs))
    {
        p_next =
            p_iterator->p_next;

        p_glyph =
            (struct feed_glyph *)(
                p_iterator);

        feed_glyph_destroy(
            p_glyph);

        p_iterator =
            p_next;

    }

    p_line->i_glyph_count =
        0u;

}

static
void
feed_line_cleanup(
    struct feed_line * const
        p_line)
{
    feed_list_join(
        &(
            p_line->o_list),
        &(
            p_line->o_list));

    /* Free all characters... */
    feed_line_reset(
        p_line);

}

static
void
feed_line_cleanup_cb(
    void * const
        p_object)
{
    struct feed_line * const
        p_line =
        (struct feed_line *)(
            p_object);

    feed_line_cleanup(
        p_line);

}

struct feed_line *
feed_line_create(
    struct feed_client * const
        p_client)
{
    struct feed_line *
        p_line;

    p_line =
        (struct feed_line *)(
            feed_object_create(
                p_client,
                sizeof(
                    struct feed_line),
                &(
                    feed_line_init_cb),
                (void const *)(
                    0)));

    return
        p_line;

}

void
feed_line_destroy(
    struct feed_line * const
        p_line)
{
    if (
        p_line)
    {
        struct feed_client * const
            p_client =
            p_line->p_client;

        if (
            p_client)
        {
            feed_object_destroy(
                p_client,
                (void *)(
                    p_line),
                &(
                    feed_line_cleanup_cb));
        }
    }
}

void
feed_line_write_event(
    struct feed_line * const
        p_line,
    struct feed_event const * const
        p_event,
    unsigned int const
        i_glyph_index)
{
    struct feed_client *
        p_client;

    struct feed_glyph *
        p_glyph;

    p_client =
        p_line->p_client;

    /* Create a character */
    /* Set char information */
    p_glyph =
        feed_glyph_create(
            p_client,
            p_event);

    if (
        p_glyph)
    {
        struct feed_glyph *
            p_current_glyph;

        p_current_glyph =
            feed_line_get_glyph(
                p_line,
                i_glyph_index);

        if (
            p_current_glyph)
        {
            feed_list_join(
                &(
                    p_glyph->o_list),
                &(
                    p_current_glyph->o_list));
        }
        else
        {
            /* Store the char into the list */
            feed_list_join(
                &(
                    p_glyph->o_list),
                &(
                    p_line->o_glyphs));
        }

        p_line->i_glyph_count ++;
    }
}

struct feed_glyph *
feed_line_get_glyph(
    struct feed_line * const
        p_line,
    unsigned int const
        i_glyph_index)
{
    struct feed_glyph *
        p_glyph;

    struct feed_list *
        p_glyph_iterator;

    unsigned int
        i_glyph_iterator;

    i_glyph_iterator =
        0u;

    p_glyph_iterator =
        p_line->o_glyphs.p_next;

    while (
        (
            i_glyph_iterator < i_glyph_index)
        && (
            p_glyph_iterator != &(p_line->o_glyphs)))
    {
        i_glyph_iterator ++;

        p_glyph_iterator =
            p_glyph_iterator->p_next;
    }

    if (
        p_glyph_iterator != &(p_line->o_glyphs))
    {
        p_glyph =
            (struct feed_glyph *)(
                p_glyph_iterator);
    }
    else
    {
        p_glyph =
            (struct feed_glyph *)(
                0);
    }

    return
        p_glyph;

}

unsigned int
feed_line_get_raw_length(
    struct feed_line * const
        p_line)
{
    unsigned int
        i_buf_len;

    struct feed_list *
        p_iterator;

    i_buf_len =
        0u;

    p_iterator =
        p_line->o_glyphs.p_next;

    while (
        p_iterator
        != &(p_line->o_glyphs))
    {
        struct feed_glyph *
            p_glyph;

        p_glyph =
            (struct feed_glyph *)(
                p_iterator);

        i_buf_len +=
            p_glyph->i_raw_length;

        p_iterator =
            p_iterator->p_next;
    }

    /* Add one for newline */
    i_buf_len ++;

    return
        i_buf_len;

} /* feed_line_get_raw_length() */

void
feed_line_get_raw_buffer(
    struct feed_line * const
        p_line,
    struct feed_buf * const
        p_buf)
{
    struct feed_list *
        p_iterator;

    p_iterator =
        p_line->o_glyphs.p_next;

    while (
        p_iterator
        != &(p_line->o_glyphs))
    {
        struct feed_glyph *
            p_glyph;

        p_glyph =
            (struct feed_glyph *)(
                p_iterator);

        feed_buf_write_character_array(
            p_buf,
            p_glyph->a_raw,
            p_glyph->i_raw_length);

        p_iterator =
            p_iterator->p_next;
    }

    feed_buf_write_character(
        p_buf,
        '\n');

} /* feed_line_get_raw_buffer() */

/* end-of-file: feed_line.c */
