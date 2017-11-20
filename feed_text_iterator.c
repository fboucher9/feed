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
        && (p_text_iterator->i_glyph_index < p_text_iterator->p_line->i_glyph_count))
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

    if (p_text_iterator->p_line)
    {
        b_result =
            feed_text_iterator_set_glyph(
                p_text,
                p_text_iterator,
                p_text_iterator->p_line->i_glyph_count);
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

    struct feed_text_iterator
        o_next_line;

    feed_text_iterator_end_glyph(
        p_text,
        p_text_iterator);

    o_next_line =
        *(p_text_iterator);

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

        while (
            o_next_line.p_line->i_glyph_count)
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

/* end-of-file: feed_text_iterator.c */
