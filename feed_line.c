/* See LICENSE for license details */

/*

Module: feed_line.c

Description:

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_line.h"

#include "feed_utf8.h"

#include "feed_glyph.h"

#include "feed_object.h"

#include "feed_buf.h"

#include "feed_input.h"

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
        0ul;

    p_line->i_width =
        0ul;

    p_line->i_raw_len =
        0ul;

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
            p_line->p_client,
            p_glyph);

        p_iterator =
            p_next;

    }

    p_line->i_glyph_count =
        0u;

    p_line->i_width =
        0ul;

    p_line->i_raw_len =
        0ul;

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
    struct feed_client * const
        p_client =
        p_line->p_client;

    feed_object_destroy(
        p_client,
        (void *)(
            p_line),
        &(
            feed_line_cleanup_cb));
}

void
feed_line_append_glyph(
    struct feed_line * const
        p_line,
    struct feed_glyph * const
        p_glyph)
{
    feed_line_insert_glyph_before(
        p_line,
        p_glyph,
        (struct feed_glyph *)(
            0));
}

void
feed_line_append_utf8_code(
    struct feed_line * const
        p_line,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    struct feed_client * const
        p_client =
            p_line->p_client;

    /* Create a character */
    /* Set char information */
    struct feed_glyph * const
        p_glyph =
        feed_glyph_create(
            p_client,
            p_utf8_code);

    if (
        p_glyph)
    {
        /* Store the char into the list */
        feed_line_append_glyph(
            p_line,
            p_glyph);

    }
}

void
feed_line_write_utf8_code(
    struct feed_line * const
        p_line,
    struct feed_utf8_code const * const
        p_utf8_code,
    unsigned long int const
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
            p_utf8_code);

    if (
        p_glyph)
    {
        struct feed_glyph *
            p_current_glyph;

        p_current_glyph =
            feed_line_get_glyph(
                p_line,
                i_glyph_index);

        feed_line_insert_glyph_before(
            p_line,
            p_glyph,
            p_current_glyph);
    }
}

struct feed_glyph *
feed_line_get_glyph(
    struct feed_line * const
        p_line,
    unsigned long int const
        i_glyph_index)
{
    struct feed_glyph *
        p_glyph;

    if (
        i_glyph_index >= p_line->i_glyph_count)
    {
        p_glyph =
            (struct feed_glyph *)(
                0);
    }
    else if (
        0u == i_glyph_index)
    {
        p_glyph =
            (struct feed_glyph *)(
                p_line->o_glyphs.p_next);
    }
    else if (
        i_glyph_index == (p_line->i_glyph_count - 1u))
    {
        p_glyph =
            (struct feed_glyph *)(
                p_line->o_glyphs.p_prev);
    }
    else
    {
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
    }

    return
        p_glyph;

}

struct feed_glyph *
feed_line_get_glyph_and_offset(
    struct feed_line * const
        p_line,
    unsigned long int const
        i_glyph_index,
    unsigned long int * const
        p_raw_offset)
{
    struct feed_glyph *
        p_glyph;

    unsigned long int
        i_raw_offset;

    i_raw_offset =
        0ul;

    if (
        i_glyph_index >= p_line->i_glyph_count)
    {
        p_glyph =
            (struct feed_glyph *)(
                0);
    }
    else if (
        0u == i_glyph_index)
    {
        p_glyph =
            (struct feed_glyph *)(
                p_line->o_glyphs.p_next);
    }
    else
    {
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
            p_glyph =
                (struct feed_glyph *)(
                    p_glyph_iterator);

            i_raw_offset +=
                p_glyph->o_utf8_code.i_raw_len;

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
    }

    *(
        p_raw_offset) =
        i_raw_offset;

    return
        p_glyph;

} /* feed_line_get_glyph_and_offset() */

unsigned long int
feed_line_length(
    struct feed_line * const
        p_line)
{
    return
        p_line->i_raw_len;

} /* feed_line_length() */

void
feed_line_save(
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
            p_glyph->o_utf8_code.a_raw,
            p_glyph->o_utf8_code.a_raw + p_glyph->o_utf8_code.i_raw_len);

        p_iterator =
            p_iterator->p_next;
    }

} /* feed_line_save() */

/* Create a char and add to prompt line */
static
void
feed_line_set_callback(
    void * const
        p_context,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    if (
        p_context)
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_context);

        feed_line_write_utf8_code(
            p_line,
            p_utf8_code,
            p_line->i_glyph_count);
    }

} /* feed_line_set_callback() */

char
feed_line_set(
    struct feed_line * const
        p_line,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    char
        b_result;

    struct feed_utf8_parser
        o_utf8_parser;

    if (
        feed_utf8_parser_init(
            &(
                o_utf8_parser)))
    {
        unsigned int
            i_data_iterator;

        i_data_iterator =
            0u;

        b_result =
            1;

        while (
            b_result
            && (
                i_data_iterator
                < i_data_length))
        {
            int
                i_result;

            struct feed_utf8_code
                o_utf8_code;

            i_result =
                feed_utf8_parser_write(
                    &(
                        o_utf8_parser),
                    p_data[i_data_iterator],
                    &(
                        o_utf8_code));

            if (
                0
                <= i_result)
            {
                if (
                    0
                    < i_result)
                {
                    feed_line_set_callback(
                        p_line,
                        &(
                            o_utf8_code));
                }

                i_data_iterator ++;
            }
            else
            {
                b_result =
                    0;
            }
        }

        feed_utf8_parser_cleanup(
            &(
                o_utf8_parser));
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

void
feed_line_remove_glyph(
    struct feed_line * const
        p_line,
    struct feed_glyph * const
        p_glyph)
{
    feed_list_join(
        &(
            p_glyph->o_list),
        &(
            p_glyph->o_list));

    {
        unsigned char
            i_glyph_width;

        i_glyph_width =
            feed_glyph_get_visible_width(
                p_glyph);

        if (p_line->i_width > i_glyph_width)
        {
            p_line->i_width -= i_glyph_width;
        }
        else
        {
            p_line->i_width = 0ul;
        }
    }

    {
        unsigned char
            i_glyph_len;

        i_glyph_len =
            p_glyph->o_utf8_code.i_raw_len;

        if (p_line->i_raw_len > i_glyph_len)
        {
            p_line->i_raw_len -= i_glyph_len;
        }
        else
        {
            p_line->i_raw_len = 0ul;
        }
    }

    if (p_line->i_glyph_count)
    {
        p_line->i_glyph_count --;
    }
}

void
feed_line_clear(
    struct feed_line * const
        p_line)
{
    while (p_line->i_glyph_count)
    {
        struct feed_glyph *
            p_glyph;

        p_glyph =
            (struct feed_glyph *)(
                p_line->o_glyphs.p_next);

        feed_line_remove_glyph(
            p_line,
            p_glyph);

        feed_glyph_destroy(
            p_line->p_client,
            p_glyph);
    }
}

void
feed_line_insert_glyph_before(
    struct feed_line * const
        p_line,
    struct feed_glyph * const
        p_glyph_new,
    struct feed_glyph * const
        p_glyph_existing)
{
    if (p_glyph_existing)
    {
        feed_list_join(
            &(
                p_glyph_new->o_list),
            &(
                p_glyph_existing->o_list));
    }
    else
    {
        feed_list_join(
            &(
                p_glyph_new->o_list),
            &(
                p_line->o_glyphs));
    }

    p_line->i_width +=
        feed_glyph_get_visible_width(
            p_glyph_new);

    p_line->i_raw_len +=
        p_glyph_new->o_utf8_code.i_raw_len;

    p_line->i_glyph_count ++;
}

char
feed_line_index_to_offset(
    struct feed_line * const
        p_line,
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
        i_glyph_iterator;

    struct feed_list *
        p_glyph_iterator;

    i_offset =
        0ul;

    i_glyph_iterator =
        0ul;

    p_glyph_iterator =
        p_line->o_glyphs.p_next;

    while (
        (i_glyph_iterator < i_glyph_index)
        && (p_glyph_iterator != &(p_line->o_glyphs)))
    {
        struct feed_glyph *
            p_glyph;

        p_glyph =
            (struct feed_glyph *)(
                p_glyph_iterator);

        i_offset +=
            p_glyph->o_utf8_code.i_raw_len;

        i_glyph_iterator ++;

        p_glyph_iterator =
            p_glyph_iterator->p_next;
    }

    *(
        p_offset) =
        i_offset;

    b_result =
        1;

    return
        b_result;

}

char
feed_line_offset_to_index(
    struct feed_line * const
        p_line,
    unsigned long int const
        i_offset,
    unsigned long int * const
        p_glyph_index)
{
    char
        b_result;

    unsigned long int
        i_offset_iterator;

    unsigned long int
        i_glyph_index;

    struct feed_list *
        p_glyph_iterator;

    i_offset_iterator =
        0ul;

    i_glyph_index =
        0ul;

    p_glyph_iterator =
        p_line->o_glyphs.p_next;

    while (
        (
            i_offset_iterator < i_offset)
        && (
           p_glyph_iterator != &(p_line->o_glyphs)))
    {
        struct feed_glyph *
            p_glyph;

        p_glyph =
            (struct feed_glyph *)(
                p_glyph_iterator);

        i_offset_iterator += p_glyph->o_utf8_code.i_raw_len;

        i_glyph_index ++;

        p_glyph_iterator =
            p_glyph_iterator->p_next;
    }

    *(
        p_glyph_index) =
        i_glyph_index;

    b_result =
        1;

    return
        b_result;

}

/* end-of-file: feed_line.c */
