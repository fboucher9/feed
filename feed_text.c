/* See LICENSE for license details */

/*

Module: feed_text.c

Description:

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_utf8.h"

#include "feed_input.h"

#include "feed_text.h"

#include "feed_line.h"

#include "feed_object.h"

#include "feed_buf.h"

#include "feed_glyph.h"

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

    feed_utf8_parser_init(
        &(
            p_text->o_utf8_parser));

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
            /* insert a dummy glyph at end of document */
            {
                struct feed_utf8_code
                    o_dummy_eof;

                o_dummy_eof.i_raw_len =
                    0u;

                o_dummy_eof.a_raw[0u] =
                    '\000';

                feed_line_append_utf8_code(
                    p_line,
                    &(
                        o_dummy_eof));
            }

            feed_text_insert_line_tail(
                p_text,
                p_line);

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
char
feed_text_init_cb(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_text * const
        p_text =
        (struct feed_text *)(
            p_object);

    (void)(
        p_descriptor);

    b_result =
        feed_text_init(
            p_text,
            p_client);

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
    while (
        p_text->i_line_count)
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_text->o_lines.p_next);

        feed_text_remove_line(
            p_text,
            p_line);

        feed_line_destroy(
            p_line);
    }

    feed_utf8_parser_cleanup(
        &(
            p_text->o_utf8_parser));
} /* feed_text_cleanup() */

static
void
feed_text_cleanup_cb(
    void * const
        p_object)
{
    struct feed_text * const
        p_text =
        (struct feed_text *)(
            p_object);

    feed_text_cleanup(
        p_text);

}

struct feed_text *
feed_text_create(
    struct feed_client * const
        p_client)
{
    struct feed_text *
        p_text;

    p_text =
        (struct feed_text *)(
            feed_object_create(
                p_client,
                sizeof(
                    struct feed_text),
                &(
                    feed_text_init_cb),
                (void const *)(
                    0)));

    return
        p_text;

}

void
feed_text_destroy(
    struct feed_text * const
        p_text)
{
    if (
        p_text)
    {
        struct feed_client * const
            p_client =
            p_text->p_client;

        if (
            p_client)
        {
            feed_object_destroy(
                p_client,
                (void *)(
                    p_text),
                &(
                    feed_text_cleanup_cb));
        }
    }
}

struct feed_line *
feed_text_get_line(
    struct feed_text const * const
        p_text,
    unsigned long int const
        i_line_index)
{
    struct feed_line *
        p_line;

    if (
        i_line_index >= p_text->i_line_count)
    {
        p_line =
            NULL;
    }
    else if (
        0u == i_line_index)
    {
        p_line =
            (struct feed_line *)(
                p_text->o_lines.p_next);
    }
    else if (
        i_line_index == (p_text->i_line_count - 1u))
    {
        p_line =
            (struct feed_line *)(
                p_text->o_lines.p_prev);
    }
    else
    {
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
    }

    return
        p_line;

}

void
feed_text_write_utf8_code(
    struct feed_text * const
        p_text,
    struct feed_utf8_code const * const
        p_utf8_code,
    unsigned long int const
        i_line_index,
    unsigned long int const
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
        feed_line_write_utf8_code(
            p_line,
            p_utf8_code,
            i_glyph_index);
    }

}

unsigned long int
feed_text_length(
    struct feed_text * const
        p_text)
{
    unsigned long int
        i_buf_len;

    struct feed_list *
        p_iterator;

    unsigned long int
        i_line_iterator;

    i_buf_len =
        0u;

    i_line_iterator =
        0u;

    p_iterator =
        p_text->o_lines.p_next;

    while (
        p_iterator
        != &(p_text->o_lines))
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_iterator);

        i_buf_len +=
            feed_line_length(
                p_line);

        i_line_iterator ++;

        p_iterator =
            p_iterator->p_next;
    }

    return
        i_buf_len;

}

void
feed_text_save(
    struct feed_text * const
        p_text,
    struct feed_buf * const
        p_buf)
{
    struct feed_list *
        p_iterator;

    unsigned int
        i_line_iterator;

    p_iterator =
        p_text->o_lines.p_next;

    i_line_iterator =
        0u;

    while (
        p_iterator
        != &(p_text->o_lines))
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_iterator);

        feed_line_save(
            p_line,
            p_buf);

        i_line_iterator ++;

        p_iterator =
            p_iterator->p_next;
    }

}

void
feed_text_remove_line(
    struct feed_text * const
        p_text,
    struct feed_line * const
        p_line)
{
    feed_list_join(
        &(
            p_line->o_list),
        &(
            p_line->o_list));

    p_text->i_line_count --;
}

void
feed_text_insert_line_after(
    struct feed_text * const
        p_text,
    struct feed_line * const
        p_line_existing,
    struct feed_line * const
        p_line_new)
{
    feed_list_join(
        &(
            p_line_existing->o_list),
        &(
            p_line_new->o_list));

    p_text->i_line_count ++;
}

void
feed_text_insert_line_tail(
    struct feed_text * const
        p_text,
    struct feed_line * const
        p_line_new)
{
    feed_list_join(
        &(
            p_line_new->o_list),
        &(
            p_text->o_lines));

    p_text->i_line_count ++;
}

void
feed_text_clear(
    struct feed_text * const
        p_text)
{
    /* Delete all the lines */
    while (
        p_text->i_line_count > 1u)
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_text->o_lines.p_next);

        feed_text_remove_line(
            p_text,
            p_line);

        feed_line_destroy(
            p_line);
    }

    /* Clear the first line */
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_text->o_lines.p_next);

        feed_line_clear(
            p_line);
    }
}

char
feed_text_index_to_offset(
    struct feed_text * const
        p_text,
    unsigned long int const
        i_line_index,
    unsigned long int const
        i_glyph_index,
    unsigned long int * const
        p_offset)
{
    char
        b_result;

    unsigned long int
        i_offset;

    unsigned long int
        i_line_iterator;

    struct feed_list *
        p_line_iterator;

    b_result =
        0;

    i_line_iterator =
        0ul;

    p_line_iterator =
        p_text->o_lines.p_next;

    i_offset =
        0ul;

    while (
        (!b_result)
        && (
            p_line_iterator != &(p_text->o_lines))
        && (
            i_line_iterator <= i_line_index))
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_line_iterator);

        if (
            i_line_iterator < i_line_index)
        {
            i_offset += feed_line_length(p_line);

            i_line_iterator ++;

            p_line_iterator =
                p_line_iterator->p_next;
        }
        else
        {
            unsigned long int
                i_glyph_offset;

            if (
                feed_line_index_to_offset(
                    p_line,
                    i_glyph_index,
                    &(
                        i_glyph_offset)))
            {
                i_offset += i_glyph_offset;

                *(p_offset) =
                    i_offset;

                b_result =
                    1;
            }
        }

    }

    return
        b_result;

}

char
feed_text_offset_to_index(
    struct feed_text * const
        p_text,
    unsigned long int const
        i_offset,
    unsigned long int * const
        p_line_index,
    unsigned long int * const
        p_glyph_index)
{
    char
        b_result;

    unsigned long int
        i_raw_iterator;

    unsigned long int
        i_line_index;

    unsigned long int
        i_glyph_index;

    struct feed_list *
        p_line_iterator;

    b_result =
        0;

    i_raw_iterator =
        0ul;

    i_line_index =
        0ul;

    i_glyph_index =
        0ul;

    p_line_iterator =
        p_text->o_lines.p_next;

    while (
        !b_result
        && (i_line_index < p_text->i_line_count))
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_line_iterator);

        if (
            (
                i_raw_iterator <= i_offset)
            && (
                (i_raw_iterator + p_line->i_raw_len) > i_offset))
        {
            if (
                feed_line_offset_to_index(
                    p_line,
                    (i_offset - i_raw_iterator),
                    &(i_glyph_index)))
            {
                *(
                    p_line_index) =
                    i_line_index;

                *(
                    p_glyph_index) =
                    i_glyph_index;

                b_result =
                    1;
            }
        }
        else
        {
            i_raw_iterator +=
                p_line->i_raw_len;

            i_line_index ++;

            p_line_iterator =
                p_line_iterator->p_next;
        }
    }

    return
        b_result;

}

void
feed_text_consume(
    struct feed_text * const
        p_text,
    struct feed_buf * const
        p_buf)
{
    char
        b_more;

    b_more =
        1;

    while (b_more)
    {
        struct feed_line *
            p_line;

        p_line =
            feed_text_get_line(
                p_text,
                0ul);

        if (
            p_line)
        {
            struct feed_glyph *
                p_glyph;

            p_glyph =
                feed_line_get_glyph(
                    p_line,
                    0ul);

            if (
                p_glyph)
            {
                if (
                    p_glyph->o_utf8_code.i_raw_len)
                {
                    if (
                        feed_buf_write_character_array(
                            p_buf,
                            p_glyph->o_utf8_code.a_raw,
                            p_glyph->o_utf8_code.i_raw_len))
                    {
                        feed_line_remove_glyph(
                            p_line,
                            p_glyph);

                        feed_glyph_destroy(
                            p_text->p_client,
                            p_glyph);
                    }
                    else
                    {
                        b_more =
                            0;
                    }
                }
                else
                {
                    b_more =
                        0;
                }
            }
            else
            {
                if (p_text->i_line_count > 1ul)
                {
                    feed_text_remove_line(
                        p_text,
                        p_line);

                    feed_line_destroy(
                        p_line);
                }
                else
                {
                    b_more =
                        0;
                }
            }
        }
        else
        {
            b_more =
                0;
        }
    }
}

/* end-of-file: feed_text.c */
