/* See LICENSE for license details */

/*

Module: feed_view.c

Description:

    Iterator for rendered view of text and prompt on a single screen.

*/

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed_screen_iterator.h"

#include "feed_prompt_iterator.h"

#include "feed_text_iterator.h"

#include "feed_view.h"

#include "feed_list.h"

#include "feed_input.h"

#include "feed_utf8.h"

#include "feed_prompt.h"

#include "feed_text.h"

#include "feed_line.h"

#include "feed_glyph.h"

#include "feed_client.h"

static
char
feed_combo_iterator_get_prompt_glyph(
    struct feed_combo_iterator * const
        p_combo_iterator,
    unsigned long int const
        i_line_index,
    unsigned long int const
        i_glyph_index)
{
    char
        b_result;

    b_result =
        feed_prompt_iterator_set_index(
            &(
                p_combo_iterator->o_prompt_iterator),
            i_line_index,
            i_glyph_index);

    if (
        b_result)
    {
        p_combo_iterator->e_state =
            feed_view_state_prompt;
    }

    return
        b_result;

}

static
char
feed_combo_iterator_get_text_glyph(
    struct feed_combo_iterator * const
        p_combo_iterator,
    unsigned long int const
        i_line_index,
    unsigned long int const
        i_glyph_index)
{
    char
        b_result;

    b_result =
        feed_text_iterator_set_index(
            &(
                p_combo_iterator->o_text_iterator),
            i_line_index,
            i_glyph_index);

    if (
        b_result)
    {
        p_combo_iterator->e_state =
            feed_view_state_text;
    }

    return
        b_result;

}

static
char
feed_combo_iterator_next_prompt_glyph(
    struct feed_combo_iterator * const
        p_combo_iterator)
{
    char
        b_result;

    b_result =
        feed_prompt_iterator_next_glyph(
            &(
                p_combo_iterator->o_prompt_iterator));

    if (
        b_result)
    {
        p_combo_iterator->e_state =
            feed_view_state_prompt;
    }

    return
        b_result;

}

static
char
feed_combo_iterator_first_text_glyph(
    struct feed_combo_iterator * const
        p_combo_iterator)
{
    char
        b_result;

    b_result =
        feed_text_iterator_home_glyph(
            &(
                p_combo_iterator->o_text_iterator));

    if (
        b_result)
    {
        p_combo_iterator->e_state =
            feed_view_state_text;
    }

    return
        b_result;

}

static
char
feed_combo_iterator_next_line(
    struct feed_combo_iterator * const
        p_combo_iterator)
{
    char
        b_result;

    b_result =
        feed_text_iterator_next_line(
            &(
                p_combo_iterator->o_text_iterator));

    if (
        b_result)
    {
        p_combo_iterator->e_state =
            feed_view_state_text;
    }

    return
        b_result;

}

static
char
feed_combo_iterator_next_text_glyph(
    struct feed_combo_iterator * const
        p_combo_iterator)
{
    char
        b_result;

    b_result =
        feed_text_iterator_next_glyph(
            &(
                p_combo_iterator->o_text_iterator));

    if (
        b_result)
    {
        p_combo_iterator->e_state =
            feed_view_state_text;
    }

    return
        b_result;

}

static
char
feed_combo_iterator_first_prompt_glyph(
    struct feed_combo_iterator * const
        p_combo_iterator)
{
    char
        b_result;

    b_result =
        feed_prompt_iterator_set_line(
            &(
                p_combo_iterator->o_prompt_iterator),
            p_combo_iterator->o_text_iterator.i_line_index);

    if (
        b_result)
    {
        b_result =
            feed_prompt_iterator_first_glyph(
                &(
                    p_combo_iterator->o_prompt_iterator));

        if (
            b_result)
        {
            p_combo_iterator->e_state =
                feed_view_state_prompt;
        }
    }

    return
        b_result;

}

char
feed_combo_iterator_init(
    struct feed_combo_iterator * const
        p_combo_iterator,
    struct feed_client * const
        p_client)
{
    char
        b_result;

    memset(
        p_combo_iterator,
        0x00u,
        sizeof(
            *(p_combo_iterator)));

    p_combo_iterator->p_client =
        p_client;

    feed_text_iterator_init(
        p_client,
        feed_client_get_text(
            p_client),
        &(
            p_combo_iterator->o_text_iterator));

    feed_prompt_iterator_init(
        p_client,
        feed_client_get_prompt(
            p_client),
        &(
            p_combo_iterator->o_prompt_iterator));

    b_result =
        1;

    return
        b_result;

}

void
feed_combo_iterator_cleanup(
    struct feed_combo_iterator * const
        p_combo_iterator)
{
    feed_prompt_iterator_cleanup(
        &(
            p_combo_iterator->o_prompt_iterator));

    feed_text_iterator_cleanup(
        &(
            p_combo_iterator->o_text_iterator));

    p_combo_iterator->p_client =
        (struct feed_client *)(
            0);
}

char
feed_combo_iterator_head(
    struct feed_combo_iterator * const
        p_combo_iterator,
    struct feed_view_descriptor const * const
        p_view_descriptor)
{
    char
        b_result;

    p_combo_iterator->o_text_iterator.i_line_index =
        p_view_descriptor->i_line_index;

    p_combo_iterator->o_text_iterator.i_line_offset =
        p_view_descriptor->i_line_offset;

    p_combo_iterator->o_text_iterator.p_line =
        p_view_descriptor->p_line;

    if (feed_view_state_prompt == p_view_descriptor->e_state)
    {
        if (
            feed_combo_iterator_get_prompt_glyph(
                p_combo_iterator,
                p_view_descriptor->i_line_index,
                p_view_descriptor->i_glyph_index))
        {
            b_result =
                1;
        }
        else if (
            feed_combo_iterator_first_text_glyph(
                p_combo_iterator))
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
        if (
            feed_combo_iterator_get_text_glyph(
                p_combo_iterator,
                p_view_descriptor->i_line_index,
                p_view_descriptor->i_glyph_index))
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

    return
        b_result;

}

char
feed_combo_iterator_next(
    struct feed_combo_iterator * const
        p_combo_iterator)
{
    char
        b_result;

    if (feed_view_state_prompt == p_combo_iterator->e_state)
    {
        if (feed_combo_iterator_next_prompt_glyph(p_combo_iterator))
        {
            b_result = 1;
        }
        else if (feed_combo_iterator_first_text_glyph(p_combo_iterator))
        {
            b_result = 1;
        }
        else
        {
            b_result = 0;
        }
    }
    else if (feed_view_state_text == p_combo_iterator->e_state)
    {
        if (feed_combo_iterator_next_text_glyph(p_combo_iterator))
        {
            b_result = 1;
        }
        else
        {
            if (feed_combo_iterator_next_line(p_combo_iterator))
            {
                if (feed_combo_iterator_first_prompt_glyph(p_combo_iterator))
                {
                    b_result = 1;
                }
                else if (feed_combo_iterator_first_text_glyph(p_combo_iterator))
                {
                    b_result = 1;
                }
                else
                {
                    b_result = 0;
                }
            }
            else
            {
                b_result = 0;
            }
        }
    }
    else
    {
        b_result = 0;
    }

    return
        b_result;

}

char
feed_combo_iterator_query(
    struct feed_combo_iterator * const
        p_combo_iterator,
    struct feed_view_descriptor * const
        p_view_descriptor)
{
    char
        b_result;

    p_view_descriptor->e_state =
        p_combo_iterator->e_state;

    p_view_descriptor->p_line =
        p_combo_iterator->o_text_iterator.p_line;

    p_view_descriptor->i_line_index =
        p_combo_iterator->o_text_iterator.i_line_index;

    p_view_descriptor->i_line_offset =
        p_combo_iterator->o_text_iterator.i_line_offset;

    if (
        feed_view_state_prompt == p_combo_iterator->e_state)
    {
        p_view_descriptor->p_glyph =
            p_combo_iterator->o_prompt_iterator.p_glyph;

        p_view_descriptor->i_glyph_index =
            p_combo_iterator->o_prompt_iterator.i_glyph_index;

        p_view_descriptor->i_glyph_offset =
            0ul;
    }
    else if (
        feed_view_state_text == p_combo_iterator->e_state)
    {
        feed_text_iterator_validate(
            &(
                p_combo_iterator->o_text_iterator));

        p_view_descriptor->p_glyph =
            p_combo_iterator->o_text_iterator.p_glyph;

        p_view_descriptor->i_glyph_index =
            p_combo_iterator->o_text_iterator.i_glyph_index;

        p_view_descriptor->i_glyph_offset =
            p_combo_iterator->o_text_iterator.i_glyph_offset;
    }
    else
    {
        p_view_descriptor->p_glyph =
            (struct feed_glyph *)(
                0);

        p_view_descriptor->i_glyph_index =
            0ul;

        p_view_descriptor->i_glyph_offset =
            0ul;
    }

    b_result =
        1;

    return
        b_result;

}

static
char
feed_view_test(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    struct feed_glyph *
        p_glyph;

    if (
        feed_view_state_prompt == p_iterator->o_combo_iterator.e_state)
    {
        p_glyph =
            p_iterator->o_combo_iterator.o_prompt_iterator.p_glyph;
    }
    else
    {
        feed_text_iterator_validate(
            &(
                p_iterator->o_combo_iterator.o_text_iterator));

        p_glyph =
            p_iterator->o_combo_iterator.o_text_iterator.p_glyph;
    }

    if (p_glyph)
    {
        struct feed_screen_info *
            p_screen_info;

        p_screen_info =
            feed_client_get_screen_info(
                p_iterator->p_client);

        if (p_glyph->o_utf8_code.i_raw_len)
        {
            if (
                '\n' == p_glyph->o_utf8_code.a_raw[0u])
            {
                b_result =
                    feed_screen_iterator_test_newline(
                        p_screen_info,
                        &(
                            p_iterator->o_screen_iterator));
            }
            else if (
                '\t' == p_glyph->o_utf8_code.a_raw[0u])
            {
                b_result =
                    feed_screen_iterator_test_tab(
                        p_screen_info,
                        &(
                            p_iterator->o_screen_iterator));
            }
            else
            {
                unsigned short int i_glyph_width;

                i_glyph_width =
                    feed_glyph_get_visible_width(
                        p_glyph);

                b_result =
                    feed_screen_iterator_test_write(
                        p_screen_info,
                        &(
                            p_iterator->o_screen_iterator),
                        i_glyph_width);
            }
        }
        else
        {
            b_result =
                feed_screen_iterator_test_clear(
                    p_screen_info,
                    &(
                        p_iterator->o_screen_iterator));
        }
    }
    else
    {
        b_result =
            0;
    }

    if (!b_result)
    {
        p_iterator->b_screen_full =
            1;
    }

    return
        b_result;

}

char
feed_view_head(
    struct feed_view * const
        p_iterator,
    struct feed_view_descriptor const * const
        p_view_descriptor)
{
    char
        b_result;

    b_result =
        feed_combo_iterator_head(
            &(
                p_iterator->o_combo_iterator),
            p_view_descriptor);

    if (
        b_result)
    {
        struct feed_screen_info *
            p_screen_info;

        p_screen_info =
            feed_client_get_screen_info(
                p_iterator->p_client);

        feed_screen_iterator_init(
            p_screen_info,
            &(
                p_iterator->o_screen_iterator));

        p_iterator->b_screen_full =
            0;

        b_result =
            feed_view_test(
                p_iterator);
    }

    return
        b_result;

}

static
unsigned long int
feed_view_write(
    struct feed_view * const
        p_iterator)
{
    unsigned long int
        i_width;

    struct feed_glyph *
        p_glyph;

    if (
        feed_view_state_prompt == p_iterator->o_combo_iterator.e_state)
    {
        p_glyph =
            p_iterator->o_combo_iterator.o_prompt_iterator.p_glyph;
    }
    else
    {
        feed_text_iterator_validate(
            &(
                p_iterator->o_combo_iterator.o_text_iterator));

        p_glyph =
            p_iterator->o_combo_iterator.o_text_iterator.p_glyph;
    }

    if (p_glyph)
    {
        struct feed_screen_info *
            p_screen_info;

        p_screen_info =
            feed_client_get_screen_info(
                p_iterator->p_client);

        /* Advance screen position */
        if (p_glyph->o_utf8_code.i_raw_len)
        {
            if (
                '\n' == p_glyph->o_utf8_code.a_raw[0u])
            {
                /* Empty line, so do a newline */
                i_width =
                    feed_screen_iterator_newline(
                        p_screen_info,
                        &(
                            p_iterator->o_screen_iterator));
            }
            else if (
                '\t' == p_glyph->o_utf8_code.a_raw[0u])
            {
                i_width =
                    feed_screen_iterator_tab(
                        p_screen_info,
                        &(
                            p_iterator->o_screen_iterator));
            }
            else
            {
                i_width =
                    feed_glyph_get_visible_width(
                        p_glyph);

                feed_screen_iterator_write(
                    p_screen_info,
                    &(
                        p_iterator->o_screen_iterator),
                    i_width);
            }
        }
        else
        {
            i_width =
                feed_screen_iterator_clear(
                    p_screen_info,
                    &(
                        p_iterator->o_screen_iterator));
        }
    }

    return
        i_width;

}

/*

Notes:

    NEXT:

        - if state is prompt
            - if next prompt glyph
                - state is still prompt
            - else if first text glyph
                - state is text
            - else if is not last line
                - state is eol
            - else
                - state is eof
        - else if state is text
            - if next text glyph
                - state is still text
            - else if is not last line
                - state is eol
            - else
                - state is eof
        - else if state is eol
            - next line
            - if first prompt glyph
                - state is prompt
            - else if first text glyph
                - state is text
            - else if is not last line
                - state is eol
            - else
                - state is eof
        - else if state is eof
            - state is null

    PREV:

        - if state is text
            - if prev text glyph
                - state is still text
            - else if last prompt glyph
                - state is prompt
            - else if not top line
                - state is eol
            - else
                - state is eof
        - else if state is prompt
            - if prev prompt glyph
                - state is still prompt
            - else if not top line
                - state is eol
            - else
                - state is eof
        - else if state is eol
            - prev line
            - if goto last glyph of text
                - state is text
            - else if goto last glyph of prompt
                - state is prompt
            - else if not top line
                - state is eol
            - else
                - state is eof
        - else if state is eof
            - state is null

*/

char
feed_view_next(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    feed_view_write(
        p_iterator);

    b_result =
        feed_combo_iterator_next(
            &(
                p_iterator->o_combo_iterator));

    if (
        b_result)
    {
        b_result =
            feed_view_test(
                p_iterator);
    }

    return
        b_result;

}

char
feed_view_query(
    struct feed_view * const
        p_iterator,
    struct feed_view_descriptor * const
        p_view_descriptor)
{
    char
        b_result;

    b_result =
        feed_combo_iterator_query(
            &(
                p_iterator->o_combo_iterator),
            p_view_descriptor);

    return
        b_result;

}

void
feed_view_init(
    struct feed_view * const
        p_iterator,
    struct feed_client * const
        p_client)
{
    memset(
        p_iterator,
        0x00u,
        sizeof(
            *(p_iterator)));

    p_iterator->p_client =
        p_client;

    feed_combo_iterator_init(
        &(
            p_iterator->o_combo_iterator),
        p_client);

}

void
feed_view_cleanup(
    struct feed_view * const
        p_iterator)
{
    feed_combo_iterator_cleanup(
        &(
            p_iterator->o_combo_iterator));

    p_iterator->p_client =
        (struct feed_client *)(
            0);
}

/* end-of-file: feed_view.c */
