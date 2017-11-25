/* See LICENSE for license details */

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed_text_iterator.h"

#include "feed_list.h"

#include "feed_input.h"

#include "feed_utf8.h"

#include "feed_text.h"

#include "feed_line.h"

#include "feed_glyph.h"

#include "feed_buf.h"

void
feed_text_iterator_init(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    (void)(
        p_text);

    p_text_iterator->p_line =
        (struct feed_line *)(
            0);

    p_text_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    p_text_iterator->i_line_index =
        0ul;

    p_text_iterator->i_glyph_index =
        0ul;

}

void
feed_text_iterator_cleanup(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    (void)(
        p_text);

    p_text_iterator->p_line =
        (struct feed_line *)(
            0);

    p_text_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    p_text_iterator->i_line_index =
        0ul;

    p_text_iterator->i_glyph_index =
        0ul;

}

static
void
feed_text_iterator_invalidate_line(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    (void)(
        p_text);

    p_text_iterator->p_line =
        (struct feed_line *)(
            0);

}

static
void
feed_text_iterator_invalidate_glyph(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    (void)(
        p_text);

    p_text_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

}

void
feed_text_iterator_invalidate(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    (void)(
        p_text);

    feed_text_iterator_invalidate_line(
        p_text,
        p_text_iterator);

    feed_text_iterator_invalidate_glyph(
        p_text,
        p_text_iterator);

}

static
void
feed_text_iterator_validate_line(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    if (!(p_text_iterator->p_line))
    {
        p_text_iterator->p_line =
            feed_text_get_line(
                p_text,
                p_text_iterator->i_line_index);
    }

}

static
void
feed_text_iterator_validate_glyph(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    feed_text_iterator_validate_line(
        p_text,
        p_text_iterator);

    if (p_text_iterator->p_line)
    {
        if (!(p_text_iterator->p_glyph))
        {
            p_text_iterator->p_glyph =
                feed_line_get_glyph(
                    p_text_iterator->p_line,
                    p_text_iterator->i_glyph_index);
        }
    }
}

void
feed_text_iterator_validate(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    feed_text_iterator_validate_glyph(
        p_text,
        p_text_iterator);

}

char
feed_text_iterator_set_line(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_line_index)
{
    char
        b_result;

    p_text_iterator->i_line_index =
        i_line_index;

    feed_text_iterator_invalidate(
        p_text,
        p_text_iterator);

    b_result =
        1;

    return
        b_result;

}

char
feed_text_iterator_next_line(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    char
        b_result;

    if ((p_text_iterator->i_line_index + 1u) < p_text->i_line_count)
    {
        feed_text_iterator_validate_line(
            p_text,
            p_text_iterator);

        if (p_text_iterator->p_line
            && (p_text_iterator->p_line->o_list.p_next != &(p_text->o_lines)))
        {
            p_text_iterator->i_line_index ++;

            p_text_iterator->p_line =
                (struct feed_line *)(
                    p_text_iterator->p_line->o_list.p_next);

            feed_text_iterator_invalidate_glyph(
                p_text,
                p_text_iterator);

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_text_iterator_prev_line(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    char
        b_result;

    if (p_text_iterator->i_line_index > 0u)
    {
        feed_text_iterator_validate_line(
            p_text,
            p_text_iterator);

        if (p_text_iterator->p_line
            && (p_text_iterator->p_line->o_list.p_prev != &(p_text->o_lines)))
        {
            p_text_iterator->i_line_index --;

            p_text_iterator->p_line =
                (struct feed_line *)(
                    p_text_iterator->p_line->o_list.p_prev);

            /* Invalidate the glyph ptr cache */
            /* Try to stay at same glyph index */
            feed_text_iterator_invalidate_glyph(
                p_text,
                p_text_iterator);

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_text_iterator_home_line(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    return
        feed_text_iterator_set_line(
            p_text,
            p_text_iterator,
            0ul);
}

char
feed_text_iterator_end_line(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    return
        feed_text_iterator_set_line(
            p_text,
            p_text_iterator,
            p_text->i_line_count - 1ul);
}

char
feed_text_iterator_set_glyph(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_glyph_index)
{
    char
        b_result;

    p_text_iterator->i_glyph_index =
        i_glyph_index;

    feed_text_iterator_invalidate_glyph(
        p_text,
        p_text_iterator);

    b_result =
        1;

    return
        b_result;

}

char
feed_text_iterator_next_glyph(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    char
        b_result;

    feed_text_iterator_validate(
        p_text,
        p_text_iterator);

    if (p_text_iterator->p_line
        && p_text_iterator->p_glyph
        && ((p_text_iterator->i_glyph_index + 1u) < p_text_iterator->p_line->i_glyph_count))
    {
        if (p_text_iterator->p_glyph->o_list.p_next != &(p_text_iterator->p_line->o_glyphs))
        {
            p_text_iterator->i_glyph_index ++;

            p_text_iterator->p_glyph =
                (struct feed_glyph *)(
                    p_text_iterator->p_glyph->o_list.p_next);

            b_result =
                1;
        }
        else
        {
            p_text_iterator->i_glyph_index ++;

            p_text_iterator->p_glyph =
                (struct feed_glyph *)(
                    0);

            b_result =
                1;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_text_iterator_prev_glyph(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    char
        b_result;

    feed_text_iterator_validate(
        p_text,
        p_text_iterator);

    if (p_text_iterator->p_line
        && (p_text_iterator->i_glyph_index > 0u))
    {
        if (p_text_iterator->p_glyph)
        {
            if (p_text_iterator->p_glyph->o_list.p_prev != &(p_text_iterator->p_line->o_glyphs))
            {
                p_text_iterator->i_glyph_index --;

                p_text_iterator->p_glyph =
                    (struct feed_glyph *)(
                        p_text_iterator->p_glyph->o_list.p_prev);

                b_result =
                    1;
            }
            else
            {
                b_result =
                    0;
            }
        }
        else
        {
            if (p_text_iterator->p_line->o_glyphs.p_prev != &(p_text_iterator->p_line->o_glyphs))
            {
                p_text_iterator->i_glyph_index --;

                p_text_iterator->p_glyph =
                    (struct feed_glyph *)(
                        p_text_iterator->p_line->o_glyphs.p_prev);

                b_result =
                    1;
            }
            else
            {
                b_result =
                    0;
            }
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_text_iterator_home_glyph(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    return
        feed_text_iterator_set_glyph(
            p_text,
            p_text_iterator,
            0ul);

}

char
feed_text_iterator_end_glyph(
    struct feed_text const * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    char
        b_result;

    feed_text_iterator_validate_line(
        p_text,
        p_text_iterator);

    if (
        p_text_iterator->p_line
        && p_text_iterator->p_line->i_glyph_count)
    {
        b_result =
            feed_text_iterator_set_glyph(
                p_text,
                p_text_iterator,
                p_text_iterator->p_line->i_glyph_count - 1u);
    }
    else
    {
        b_result =
            feed_text_iterator_home_glyph(
                p_text,
                p_text_iterator);
    }

    return
        b_result;

}

struct feed_glyph *
feed_text_iterator_remove_glyph(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    struct feed_glyph *
        p_glyph;

    feed_text_iterator_validate(
        p_text,
        p_text_iterator);

    if (p_text_iterator->p_line
        && p_text_iterator->p_glyph)
    {
        p_glyph =
            p_text_iterator->p_glyph;

        if (p_glyph->o_list.p_next != &(p_text_iterator->p_line->o_glyphs))
        {
            p_text_iterator->p_glyph =
                (struct feed_glyph *)(
                    p_glyph->o_list.p_next);
        }
        else
        {
            p_text_iterator->p_glyph =
                (struct feed_glyph *)(
                    0);
        }

        /* Remove the selected char from the linked list */
        feed_line_remove_glyph(
            p_text_iterator->p_line,
            p_glyph);

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

void
feed_text_iterator_append_glyph(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    struct feed_glyph * const
        p_glyph)
{
    feed_text_iterator_validate_line(
        p_text,
        p_text_iterator);

    if (p_text_iterator->p_line)
    {
        feed_line_append_glyph(
            p_text_iterator->p_line,
            p_glyph);
    }
}

void
feed_text_iterator_write_glyph(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    struct feed_glyph * const
        p_glyph)
{
    feed_text_iterator_validate(
        p_text,
        p_text_iterator);

    feed_line_insert_glyph_before(
        p_text_iterator->p_line,
        p_glyph,
        p_text_iterator->p_glyph);

    p_text_iterator->i_glyph_index ++;
}

char
feed_text_iterator_join_lines(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    char
        b_result;

    char
        b_more;

    struct feed_text_iterator
        o_next_line;

    feed_text_iterator_validate(
        p_text,
        p_text_iterator);

    o_next_line =
        *(p_text_iterator);

    if (
        p_text_iterator->p_line)
    {
        p_text_iterator->i_glyph_index =
            p_text_iterator->p_line->i_glyph_count;
    }
    else
    {
        p_text_iterator->i_glyph_index =
            0u;
    }

    if (
        feed_text_iterator_next_line(
            p_text,
            &(
                o_next_line)))
    {
        feed_text_iterator_home_glyph(
            p_text,
            &(
                o_next_line));

        feed_text_iterator_validate_line(
            p_text,
            &(
                o_next_line));

        b_more =
            1;

        while (
            b_more
            && o_next_line.p_line->i_glyph_count)
        {
            struct feed_glyph *
                p_glyph;

            p_glyph =
                feed_text_iterator_remove_glyph(
                    p_text,
                    &(
                        o_next_line));

            if (
                p_glyph)
            {
                feed_text_iterator_append_glyph(
                    p_text,
                    p_text_iterator,
                    p_glyph);
            }
            else
            {
                b_more =
                    0;
            }
        }

        feed_text_remove_line(
            p_text,
            o_next_line.p_line);

        feed_line_destroy(
            o_next_line.p_line);

        feed_text_iterator_invalidate_glyph(
            p_text,
            p_text_iterator);

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_text_iterator_set_index(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_line_index,
    unsigned long int const
        i_glyph_index)
{
    char
        b_result;

    p_text_iterator->i_line_index =
        i_line_index;

    p_text_iterator->i_glyph_index =
        i_glyph_index;

    feed_text_iterator_invalidate(
        p_text,
        p_text_iterator);

    b_result =
        1;

    return
        b_result;

}

char
feed_text_iterator_set_offset(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_offset)
{
    char
        b_result;

    unsigned long int
        i_line_index;

    unsigned long int
        i_glyph_index;

    if (
        feed_text_offset_to_index(
            p_text,
            i_offset,
            &(
                i_line_index),
            &(
                i_glyph_index)))
    {
        if (
            feed_text_iterator_set_index(
                p_text,
                p_text_iterator,
                i_line_index,
                i_glyph_index))
        {
            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

} /* feed_text_iterator_set_offset() */

void
feed_text_iterator_save(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    struct feed_buf * const
        p_buf)
{
    char
        b_more;

    b_more =
        1;

    while (
        b_more)
    {
        feed_text_iterator_validate(
            p_text,
            p_text_iterator);

        if (
            p_text_iterator->p_glyph)
        {
            if (
                feed_buf_write_character_array(
                    p_buf,
                    p_text_iterator->p_glyph->o_utf8_code.a_raw,
                    p_text_iterator->p_glyph->o_utf8_code.i_raw_len))
            {
            }
            else
            {
                b_more =
                    0;
            }
        }

        if (
            feed_text_iterator_next_glyph(
                p_text,
                p_text_iterator))
        {
        }
        else
        {
            if (
                feed_text_iterator_next_line(
                    p_text,
                    p_text_iterator))
            {
                feed_text_iterator_home_glyph(
                    p_text,
                    p_text_iterator);
            }
            else
            {
                b_more =
                    0;
            }
        }
    }

} /* feed_text_iterator_save() */

unsigned long int
feed_text_iterator_get_offset(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    unsigned long int
        i_offset;

    if (
        feed_text_index_to_offset(
            p_text,
            p_text_iterator->i_line_index,
            p_text_iterator->i_glyph_index,
            &(
                i_offset)))
    {
    }
    else
    {
        i_offset =
            0ul;
    }

    return
        i_offset;

}

void
feed_text_iterator_delete_region(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_region_length)
{
    char
        b_more;

    unsigned long int
        i_region_iterator;

    /* Delete characters until reach region length */
    b_more =
        1;

    i_region_iterator =
        0ul;

    while (
        b_more
        && (
            i_region_iterator < i_region_length))
    {
        feed_text_iterator_validate(
            p_text,
            p_text_iterator);

        if (p_text_iterator->p_glyph)
        {
            struct feed_glyph *
                p_glyph;

            p_glyph =
                feed_text_iterator_remove_glyph(
                    p_text,
                    p_text_iterator);

            if (
                p_glyph)
            {
                i_region_iterator +=
                    p_glyph->o_utf8_code.i_raw_len;

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
            if (
                feed_text_iterator_join_lines(
                    p_text,
                    p_text_iterator))
            {
                i_region_iterator ++;
            }
            else
            {
                b_more =
                    0;
            }
        }
    }
}

void
feed_text_iterator_load(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    struct feed_utf8_parser
        o_utf8_parser;

    (void)(
        p_text);
    (void)(
        p_text_iterator);

    if (
        feed_utf8_parser_init(
            &(
                o_utf8_parser)))
    {
        char
            b_more;

        unsigned long int
            i_data_iterator;

        b_more =
            1;

        i_data_iterator =
            0ul;

        while (
            b_more
            && (
                i_data_iterator
                < i_data_length))
        {
            struct feed_utf8_code
                o_utf8_code;

            int
                i_result;

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
                    if ('\n' == o_utf8_code.a_raw[0u])
                    {
                        feed_text_iterator_insert_newline(
                            p_text,
                            p_text_iterator);
                    }
                    else
                    {
                        feed_text_iterator_insert_code(
                            p_text,
                            p_text_iterator,
                            &(
                                o_utf8_code));
                    }
                }

                i_data_iterator ++;
            }
            else
            {
                b_more =
                    0;
            }
        }

        feed_utf8_parser_cleanup(
            &(
                o_utf8_parser));

    }
}

char
feed_text_iterator_insert_newline(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    char
        b_result;

    {
        struct feed_utf8_code
            o_utf8_code;

        o_utf8_code.i_raw_len =
            1u;

        o_utf8_code.a_raw[0u] =
            '\n';

        feed_text_iterator_insert_code(
            p_text,
            p_text_iterator,
            &(
                o_utf8_code));
    }

    feed_text_iterator_validate(
        p_text,
        p_text_iterator);

    if (
        p_text_iterator->p_line)
    {
        struct feed_glyph *
            p_glyph;

        struct feed_line *
            p_line_down;

        if (p_text_iterator->p_glyph)
        {
            p_glyph =
                p_text_iterator->p_glyph;
        }
        else
        {
            p_glyph =
                feed_line_get_glyph(
                    p_text_iterator->p_line,
                    p_text_iterator->i_glyph_index);
        }

        p_line_down =
            feed_line_create(
                p_text->p_client);

        if (
            p_line_down)
        {
            feed_text_insert_line_after(
                p_text,
                p_text_iterator->p_line,
                p_line_down);

            if (p_glyph)
            {
                /* Transfer characters from this line to next */
                while (
                    &(
                        p_glyph->o_list)
                    != &(
                        p_text_iterator->p_line->o_glyphs))
                {
                    struct feed_glyph *
                        p_glyph_next;

                    p_glyph_next =
                        (struct feed_glyph *)(
                            p_glyph->o_list.p_next);

                    feed_line_remove_glyph(
                        p_text_iterator->p_line,
                        p_glyph);

                    feed_line_append_glyph(
                        p_line_down,
                        p_glyph);

                    p_glyph =
                        p_glyph_next;
                }
            }

            p_text_iterator->i_line_index ++;

            p_text_iterator->p_line = p_line_down;

            feed_text_iterator_home_glyph(
                p_text,
                p_text_iterator);

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_text_iterator_insert_code(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    char
        b_result;

    struct feed_glyph *
        p_glyph;

    p_glyph =
        feed_glyph_create(
            p_text->p_client,
            p_utf8_code);

    if (
        p_glyph)
    {
        feed_text_iterator_write_glyph(
            p_text,
            p_text_iterator,
            p_glyph);

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

/* end-of-file: feed_text_iterator.c */
