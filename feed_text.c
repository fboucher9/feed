/* See LICENSE for license details */

/*

Module: feed_text.c

Description:

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_input.h"

#include "feed_text.h"

#include "feed_line.h"

#include "feed_client.h"

#include "feed_heap.h"

static
char
feed_text_init(
    struct feed_text * const
        p_text,
    struct feed_client * const
        p_client)
{
    char
        b_result;

    memset(
        p_text,
        0x00u,
        sizeof(
            struct feed_text));

    p_text->p_client =
        p_client;

    feed_list_init(
        &(
            p_text->o_lines));

    p_text->i_line_count =
        0;

    {
        struct feed_line *
            p_line;

        /* Create a line */
        p_line =
            feed_line_create(
                p_client);

        /* Store the line into the list */
        if (p_line)
        {
            feed_list_join(
                &(
                    p_line->o_list),
                &(
                    p_text->o_lines));

            p_text->i_line_count ++;

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }

    return
        b_result;

}


static
void
feed_text_cleanup(
    struct feed_text * const
        p_text)
{
    /* Delete all the lines */
    struct feed_list *
        p_iterator;

    p_iterator =
        p_text->o_lines.p_next;

    while (
        p_iterator
        != &(
            p_text->o_lines))
    {
        struct feed_list *
            p_next;

        struct feed_line *
            p_line;

        p_next =
            p_iterator->p_next;

        p_line =
            (struct feed_line *)(
                p_iterator);

        feed_line_destroy(
            p_line);

        p_iterator =
            p_next;

    }

} /* feed_text_cleanup() */


struct feed_text *
feed_text_create(
    struct feed_client * const
        p_client)
{
    struct feed_text *
        p_text;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_text =
        (struct feed_text *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_text)));

    if (
        p_text)
    {
        if (
            feed_text_init(
                p_text,
                p_client))
        {
        }
        else
        {
            feed_heap_free(
                p_heap,
                (void *)(
                    p_text));

            p_text =
                (struct feed_text *)(
                    0);
        }
    }

    return
        p_text;

}


void
feed_text_destroy(
    struct feed_text * const
        p_text)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_text->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_text_cleanup(
        p_text);

    feed_heap_free(
        p_heap,
        (void *)(
            p_text));

}

struct feed_line *
feed_text_get_line(
    struct feed_text * const
        p_text,
    unsigned int const
        i_line_index)
{
    struct feed_line *
        p_line;

    struct feed_list *
        p_iterator;

    unsigned int
        i_line_iterator;

    i_line_iterator =
        0u;

    p_iterator =
        p_text->o_lines.p_next;

    while (
        (
            i_line_iterator < i_line_index)
        && (
            p_iterator != &(p_text->o_lines)))
    {
        i_line_iterator ++;

        p_iterator =
            p_iterator->p_next;
    }

    if (
        p_iterator != &(p_text->o_lines))
    {
        p_line =
            (struct feed_line *)(
                p_iterator);
    }
    else
    {
        p_line =
            (struct feed_line *)(
                0);
    }

    return
        p_line;

}

void
feed_text_write_event(
    struct feed_text * const
        p_text,
    struct feed_event const * const
        p_event,
    unsigned int const
        i_line_index,
    unsigned int const
        i_glyph_index)
{
    struct feed_line *
        p_line;

    p_line =
        feed_text_get_line(
            p_text,
            i_line_index);

    if (
        p_line)
    {
        feed_line_write_event(
            p_line,
            p_event,
            i_glyph_index);
    }

}

/* end-of-file: feed_text.c */
