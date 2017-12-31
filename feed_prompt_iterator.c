/* See LICENSE for license details */

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed_prompt_iterator.h"

#include "feed_prompt.h"

#include "feed_list.h"

#include "feed_line.h"

#include "feed_utf8.h"

#include "feed_glyph.h"

char
feed_prompt_iterator_init(
    struct feed_client * const
        p_client,
    struct feed_prompt * const
        p_prompt,
    struct feed_prompt_iterator * const
        p_iterator)
{
    char
        b_result;

    p_iterator->p_client =
        p_client;

    p_iterator->p_prompt =
        p_prompt;

    p_iterator->p_line =
        (struct feed_line *)(
            0);

    p_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    p_iterator->i_line_index =
        0ul;

    p_iterator->i_glyph_index =
        0ul;

    b_result =
        1;

    return
        b_result;

}

void
feed_prompt_iterator_cleanup(
    struct feed_prompt_iterator * const
        p_iterator)
{
    p_iterator->p_client =
        (struct feed_client *)(
            0);

    p_iterator->p_prompt =
        (struct feed_prompt *)(
            0);

    p_iterator->p_line =
        (struct feed_line *)(
            0);

    p_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    p_iterator->i_line_index =
        0ul;

    p_iterator->i_glyph_index =
        0ul;

}

static
char
feed_prompt_iterator_validate_line(
    struct feed_prompt_iterator * const
        p_iterator)
{
    char
        b_result;

    if (
        !(
            p_iterator->p_line))
    {
        if (
            0ul == p_iterator->i_line_index)
        {
            p_iterator->p_line =
                feed_prompt_get1(
                    p_iterator->p_prompt);
        }
        else
        {
            p_iterator->p_line =
                feed_prompt_get2(
                    p_iterator->p_prompt);
        }

        if (
            p_iterator->p_line)
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
            1;
    }

    return
        b_result;

}

static
char
feed_prompt_iterator_validate_glyph(
    struct feed_prompt_iterator * const
        p_iterator)
{
    char
        b_result;

    if (
        !(
            p_iterator->p_glyph))
    {
        b_result =
            feed_prompt_iterator_validate_line(
                p_iterator);

        if (
            b_result)
        {
            p_iterator->p_glyph =
                feed_line_get_glyph(
                    p_iterator->p_line,
                    p_iterator->i_glyph_index);

            if (
                p_iterator->p_glyph)
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
    }
    else
    {
        b_result =
            1;
    }

    return
        b_result;

}

char
feed_prompt_iterator_set_line(
    struct feed_prompt_iterator * const
        p_iterator,
    unsigned long int const
        i_line_index)
{
    char
        b_result;

    p_iterator->i_line_index =
        i_line_index;

    p_iterator->i_glyph_index =
        0ul;

    p_iterator->p_line =
        (struct feed_line *)(
            0);

    p_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    b_result =
        feed_prompt_iterator_validate_glyph(
            p_iterator);

    return
        b_result;

}

char
feed_prompt_iterator_set_glyph(
    struct feed_prompt_iterator * const
        p_iterator,
    unsigned long int const
        i_glyph_index)
{
    char
        b_result;

    p_iterator->i_glyph_index =
        i_glyph_index;

    p_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    b_result =
        feed_prompt_iterator_validate_glyph(
            p_iterator);

    return
        b_result;

}

char
feed_prompt_iterator_set_index(
    struct feed_prompt_iterator * const
        p_iterator,
    unsigned long int const
        i_line_index,
    unsigned long int const
        i_glyph_index)
{
    char
        b_result;

    p_iterator->i_line_index =
        i_line_index;

    p_iterator->i_glyph_index =
        i_glyph_index;

    p_iterator->p_line =
        (struct feed_line *)(
            0);

    p_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    b_result =
        feed_prompt_iterator_validate_glyph(
            p_iterator);

    return
        b_result;

}

char
feed_prompt_iterator_first_glyph(
    struct feed_prompt_iterator * const
        p_iterator)
{
    char
        b_result;

    b_result =
        feed_prompt_iterator_set_glyph(
            p_iterator,
            0ul);

    return
        b_result;

}

char
feed_prompt_iterator_last_glyph(
    struct feed_prompt_iterator * const
        p_iterator)
{
    char
        b_result;

    b_result =
        feed_prompt_iterator_validate_line(
            p_iterator);

    if (
        b_result)
    {
        b_result =
            feed_prompt_iterator_set_glyph(
                p_iterator,
                p_iterator->p_line->i_glyph_count - 1u);
    }

    return
        b_result;

}

char
feed_prompt_iterator_next_glyph(
    struct feed_prompt_iterator * const
        p_iterator)
{
    char
        b_result;

    b_result =
        feed_prompt_iterator_validate_glyph(
            p_iterator);

    if (
        b_result)
    {
        if (
            p_iterator->p_glyph->o_list.p_next != &(p_iterator->p_line->o_glyphs))
        {
            p_iterator->i_glyph_index ++;

            p_iterator->p_glyph =
                (struct feed_glyph *)(
                    p_iterator->p_glyph->o_list.p_next);

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

char
feed_prompt_iterator_prev_glyph(
    struct feed_prompt_iterator * const
        p_iterator)
{
    char
        b_result;

    b_result =
        feed_prompt_iterator_validate_glyph(
            p_iterator);

    if (
        b_result)
    {
        if (
            p_iterator->p_glyph->o_list.p_prev != &(p_iterator->p_line->o_glyphs))
        {
            p_iterator->i_glyph_index ++;

            p_iterator->p_glyph =
                (struct feed_glyph *)(
                    p_iterator->p_glyph->o_list.p_prev);

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

