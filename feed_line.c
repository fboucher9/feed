/* See LICENSE for license details */

/*

Module: feed_line.c

Description:

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_line.h"

#include "feed_glyph.h"

#include "feed_client.h"

#include "feed_heap.h"

static
void
feed_line_init(
    struct feed_line * const
        p_line,
    struct feed_client * const
        p_client)
{
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

struct feed_line *
feed_line_create(
    struct feed_client * const
        p_client)
{
    struct feed_line *
        p_line;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_line =
        (struct feed_line *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_line)));

    if (
        p_line)
    {
        feed_line_init(
            p_line,
            p_client);
    }

    return
        p_line;

}

void
feed_line_destroy(
    struct feed_line * const
        p_line)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_line->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_line_cleanup(
        p_line);

    feed_heap_free(
        p_heap,
        (void *)(
            p_line));

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

/* end-of-file: feed_line.c */
