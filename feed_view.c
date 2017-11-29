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
void
feed_view_use_prompt_iterator(
    struct feed_view * const
        p_iterator)
{
    p_iterator->i_glyph_index =
        p_iterator->o_prompt_iterator.i_glyph_index;

    p_iterator->p_glyph =
        p_iterator->o_prompt_iterator.p_glyph;

}

static
char
feed_view_get_prompt_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    if (p_iterator->p_document_line)
    {
        b_result =
            feed_prompt_iterator_set_index(
                p_iterator->p_client->p_prompt,
                &(
                    p_iterator->o_prompt_iterator),
                p_iterator->i_line_index,
                p_iterator->i_glyph_index);

        if (
            b_result)
        {
            feed_view_use_prompt_iterator(
                p_iterator);
        }
    }
    else
    {
        b_result =
            0;
    }

    if (!b_result)
    {
        p_iterator->p_glyph =
            NULL;

    }

    return
        b_result;

}

static
char
feed_view_get_text_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    if (p_iterator->p_document_line)
    {
        p_iterator->p_glyph =
            feed_line_get_glyph(
                p_iterator->p_document_line,
                p_iterator->i_glyph_index);

        if (p_iterator->p_glyph)
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

    if (!b_result)
    {
        p_iterator->p_glyph = NULL;
    }

    return
        b_result;

}

static
char
feed_view_next_prompt_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    p_iterator->i_glyph_index ++;

    if (p_iterator->p_document_line)
    {
        b_result =
            feed_prompt_iterator_next_glyph(
                p_iterator->p_client->p_prompt,
                &(
                    p_iterator->o_prompt_iterator));

        if (
            b_result)
        {
            feed_view_use_prompt_iterator(
                p_iterator);
        }
    }
    else
    {
        b_result =
            0;
    }

    if (!b_result)
    {
        p_iterator->p_glyph = NULL;
    }

    return
        b_result;

}

static
char
feed_view_prev_prompt_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    if (p_iterator->i_glyph_index)
    {
        p_iterator->i_glyph_index --;

        if (p_iterator->p_document_line)
        {
            b_result =
                feed_prompt_iterator_prev_glyph(
                    p_iterator->p_client->p_prompt,
                    &(
                        p_iterator->o_prompt_iterator));

            if (
                b_result)
            {
                feed_view_use_prompt_iterator(
                    p_iterator);
            }
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

    if (!b_result)
    {
        p_iterator->p_glyph = NULL;
    }

    return
        b_result;

}


static
char
feed_view_first_text_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    p_iterator->i_glyph_index =
        0u;

    if (p_iterator->p_document_line)
    {
        if (p_iterator->p_document_line->o_glyphs.p_next != &(p_iterator->p_document_line->o_glyphs))
        {
            p_iterator->p_glyph =
                (struct feed_glyph *)(
                    p_iterator->p_document_line->o_glyphs.p_next);

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

    if (!b_result)
    {
        p_iterator->p_glyph =
            NULL;
    }

    return
        b_result;

}

static
char
feed_view_last_text_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    p_iterator->i_glyph_index =
        0u;

    if (p_iterator->p_document_line
        && (p_iterator->p_document_line->o_glyphs.p_prev != &(p_iterator->p_document_line->o_glyphs)))
    {
        p_iterator->i_glyph_index =
            (p_iterator->p_document_line->i_glyph_count - 1u);

        p_iterator->p_glyph =
            (struct feed_glyph *)(
                p_iterator->p_document_line->o_glyphs.p_prev);

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

    if (!b_result)
    {
        p_iterator->p_glyph =
            NULL;
    }

    return
        b_result;

}

static
char
feed_view_next_line(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    p_iterator->i_line_index ++;

    p_iterator->i_glyph_index = 0u;

    p_iterator->p_glyph = NULL;

    if (p_iterator->p_document_line)
    {
        if (p_iterator->p_document_line->o_list.p_next != &(p_iterator->p_client->p_text->o_lines))
        {
            p_iterator->p_document_line =
                (struct feed_line *)(
                    p_iterator->p_document_line->o_list.p_next);

            b_result =
                1;
        }
        else
        {
            p_iterator->p_document_line = NULL;

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

static
char
feed_view_prev_line(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    if (p_iterator->i_line_index)
    {
        p_iterator->i_line_index --;

        p_iterator->i_glyph_index = 0u;

        p_iterator->p_glyph = NULL;

        if (p_iterator->p_document_line)
        {
            if (p_iterator->p_document_line->o_list.p_prev != &(p_iterator->p_client->p_text->o_lines))
            {
                p_iterator->p_document_line =
                    (struct feed_line *)(
                        p_iterator->p_document_line->o_list.p_prev);

                b_result =
                    1;
            }
            else
            {
                p_iterator->p_document_line = NULL;

                b_result =
                    0;
            }
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

static
char
feed_view_next_text_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    p_iterator->i_glyph_index ++;

    if (p_iterator->p_document_line)
    {
        if (p_iterator->p_glyph)
        {
            if (p_iterator->p_glyph->o_list.p_next != &(p_iterator->p_document_line->o_glyphs))
            {
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

    if (!b_result)
    {
        p_iterator->p_glyph = NULL;
    }

    return
        b_result;

}

static
char
feed_view_prev_text_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    if (p_iterator->i_glyph_index)
    {
        p_iterator->i_glyph_index --;

        if (p_iterator->p_document_line
            && p_iterator->p_glyph
            && (p_iterator->p_glyph->o_list.p_prev != &(p_iterator->p_document_line->o_glyphs)))
        {
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
    else
    {
        b_result =
            0;
    }

    if (!b_result)
    {
        p_iterator->p_glyph = NULL;
    }

    return
        b_result;

}

static
char
feed_view_first_prompt_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    p_iterator->i_glyph_index = 0u;

    p_iterator->p_glyph = NULL;

    if (p_iterator->p_document_line)
    {
        b_result =
            feed_prompt_iterator_set_index(
                p_iterator->p_client->p_prompt,
                &(
                    p_iterator->o_prompt_iterator),
                p_iterator->i_line_index,
                p_iterator->i_glyph_index);

        if (
            b_result)
        {
            feed_view_use_prompt_iterator(
                p_iterator);
        }
    }
    else
    {
        b_result =
            0;
    }

    if (!b_result)
    {
        p_iterator->p_glyph = NULL;
    }

    return
        b_result;

}

static
char
feed_view_last_prompt_glyph(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    p_iterator->i_glyph_index = 0u;

    if (p_iterator->p_document_line)
    {
        b_result =
            feed_prompt_iterator_set_line(
                p_iterator->p_client->p_prompt,
                &(
                    p_iterator->o_prompt_iterator),
                p_iterator->i_line_index);

        if (
            b_result)
        {
            b_result =
                feed_prompt_iterator_last_glyph(
                    p_iterator->p_client->p_prompt,
                    &(
                        p_iterator->o_prompt_iterator));

            if (
                b_result)
            {
                feed_view_use_prompt_iterator(
                    p_iterator);
            }
        }
    }
    else
    {
        b_result =
            0;
    }

    if (!b_result)
    {
        p_iterator->p_glyph = NULL;
    }

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

    p_glyph =
        p_iterator->p_glyph;

    if (p_glyph)
    {
        if (p_glyph->o_utf8_code.i_raw_len)
        {
            if (
                '\n' == p_glyph->o_utf8_code.a_raw[0u])
            {
                b_result =
                    feed_screen_iterator_test_newline(
                        p_iterator->p_client->p_screen_info,
                        &(
                            p_iterator->o_screen_iterator));
            }
            else if (
                '\t' == p_glyph->o_utf8_code.a_raw[0u])
            {
                b_result =
                    feed_screen_iterator_test_tab(
                        p_iterator->p_client->p_screen_info,
                        &(
                            p_iterator->o_screen_iterator));
            }
            else
            {
                unsigned short int i_glyph_width;

                i_glyph_width =
                    feed_glyph_get_visible_width(
                        p_iterator->p_glyph);

                b_result =
                    feed_screen_iterator_test_write(
                        p_iterator->p_client->p_screen_info,
                        &(
                            p_iterator->o_screen_iterator),
                        i_glyph_width);
            }
        }
        else
        {
            b_result =
                feed_screen_iterator_test_clear(
                    p_iterator->p_client->p_screen_info,
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
    struct feed_line * const
        p_page_line,
    unsigned long int const
        i_page_line_index,
    unsigned long int const
        i_page_glyph_index,
    enum feed_view_state const
        e_page_state)
{
    p_iterator->i_line_index =
        i_page_line_index;

    p_iterator->i_glyph_index =
        i_page_glyph_index;

    if (feed_view_state_prompt == e_page_state)
    {
        p_iterator->e_state =
            feed_view_state_prompt;
    }
    else
    {
        p_iterator->e_state =
            feed_view_state_text;
    }

    p_iterator->p_glyph =
        NULL;

    if (p_page_line)
    {
        p_iterator->p_document_line =
            p_page_line;
    }
    else
    {
        p_iterator->p_document_line =
            feed_text_get_line(
                p_iterator->p_client->p_text,
                p_iterator->i_line_index);
    }

    if (p_iterator->p_document_line)
    {
        if (feed_view_state_prompt == p_iterator->e_state)
        {
            if (
                feed_view_get_prompt_glyph(
                    p_iterator))
            {
                p_iterator->e_state = feed_view_state_prompt;
            }
            else if (
                feed_view_first_text_glyph(
                    p_iterator))
            {
                p_iterator->e_state = feed_view_state_text;
            }
            else
            {
                p_iterator->e_state = feed_view_state_null;
            }
        }
        else if (
            feed_view_state_text == p_iterator->e_state)
        {
            if (
                feed_view_get_text_glyph(
                    p_iterator))
            {
                p_iterator->e_state = feed_view_state_text;
            }
            else
            {
                p_iterator->e_state = feed_view_state_null;
            }
        }
        else
        {
            p_iterator->e_state = feed_view_state_null;
        }
    }
    else
    {
        p_iterator->i_line_index =
            p_iterator->p_client->p_text->i_line_count;

        p_iterator->i_glyph_index =
            0u;

        p_iterator->p_glyph =
            NULL;

        p_iterator->e_state =
            feed_view_state_null;
    }

    feed_screen_iterator_init(
        p_iterator->p_client->p_screen_info,
        &(
            p_iterator->o_screen_iterator));

    p_iterator->b_screen_full =
        0;

    return
        feed_view_test(
            p_iterator);

}

/*

    TEXT PROMPT EOL
    TEXT PROMPT EOF NULL

*/
char
feed_view_tail(
    struct feed_view * const
        p_iterator,
    struct feed_line * const
        p_page_line,
    unsigned long int const
        i_page_line_index,
    unsigned long int const
        i_page_glyph_index,
    enum feed_view_state const
        e_page_state)
{
    p_iterator->i_line_index =
        i_page_line_index;

    p_iterator->i_glyph_index =
        i_page_glyph_index;

    if (feed_view_state_prompt == e_page_state)
    {
        p_iterator->e_state =
            feed_view_state_prompt;
    }
    else
    {
        p_iterator->e_state =
            feed_view_state_text;
    }

    p_iterator->p_glyph =
        NULL;

    p_iterator->p_document_line =
        NULL;

    if (p_page_line)
    {
        p_iterator->p_document_line =
            p_page_line;
    }
    else
    {
        p_iterator->p_document_line =
            feed_text_get_line(
                p_iterator->p_client->p_text,
                p_iterator->i_line_index);
    }

    if (!(p_iterator->p_document_line))
    {
        /* we are at end of file */
        if (p_iterator->p_client->p_text->i_line_count)
        {
            p_iterator->i_line_index =
                (p_iterator->p_client->p_text->i_line_count - 1ul);

            p_iterator->p_document_line =
                feed_text_get_line(
                    p_iterator->p_client->p_text,
                    p_iterator->i_line_index);

            if (p_iterator->p_document_line)
            {
                if (p_iterator->p_document_line->i_glyph_count)
                {
                    p_iterator->i_glyph_index =
                        (p_iterator->p_document_line->i_glyph_count - 1ul);

                    p_iterator->e_state =
                        feed_view_state_text;
                }
                else
                {
                    p_iterator->e_state =
                        feed_view_state_null;
                }
            }
            else
            {
                p_iterator->e_state =
                    feed_view_state_null;
            }
        }
        else
        {
            p_iterator->e_state =
                feed_view_state_null;
        }
    }

    if (p_iterator->p_document_line)
    {
        if (feed_view_state_text == p_iterator->e_state)
        {
            if (
                feed_view_get_text_glyph(
                    p_iterator))
            {
                p_iterator->e_state =
                    feed_view_state_text;
            }
            else if (
                feed_view_last_prompt_glyph(
                    p_iterator))
            {
                p_iterator->e_state =
                    feed_view_state_prompt;
            }
            else
            {
                if (feed_view_prev_line(
                        p_iterator))
                {
                    if (
                        feed_view_last_text_glyph(
                            p_iterator))
                    {
                        p_iterator->e_state =
                            feed_view_state_text;
                    }
                    else
                    {
                        p_iterator->e_state =
                            feed_view_state_null;
                    }
                }
                else
                {
                    p_iterator->e_state =
                        feed_view_state_null;
                }
            }
        }
        else if (feed_view_state_prompt == p_iterator->e_state)
        {
            if (
                feed_view_get_prompt_glyph(
                    p_iterator))
            {
                p_iterator->e_state =
                    feed_view_state_prompt;
            }
            else
            {
                if (feed_view_prev_line(
                        p_iterator))
                {
                    if (
                        feed_view_last_text_glyph(
                            p_iterator))
                    {
                        p_iterator->e_state =
                            feed_view_state_text;
                    }
                    else
                    {
                        p_iterator->e_state =
                            feed_view_state_null;
                    }
                }
                else
                {
                    p_iterator->e_state =
                        feed_view_state_null;
                }
            }
        }
    }
    else
    {
        p_iterator->i_line_index =
            p_iterator->p_client->p_text->i_line_count;

        p_iterator->i_glyph_index =
            0u;

        p_iterator->p_glyph =
            NULL;

        p_iterator->e_state =
            feed_view_state_null;
    }

    feed_screen_iterator_init(
        p_iterator->p_client->p_screen_info,
        &(
            p_iterator->o_screen_iterator));

    p_iterator->b_screen_full =
        0;

    return
        feed_view_test(
            p_iterator);

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

    p_glyph =
        p_iterator->p_glyph;

    if (p_glyph)
    {
        /* Advance screen position */
        if (p_glyph->o_utf8_code.i_raw_len)
        {
            if (
                '\n' == p_glyph->o_utf8_code.a_raw[0u])
            {
                /* Empty line, so do a newline */
                i_width =
                    feed_screen_iterator_newline(
                        p_iterator->p_client->p_screen_info,
                        &(
                            p_iterator->o_screen_iterator));
            }
            else if (
                '\t' == p_glyph->o_utf8_code.a_raw[0u])
            {
                i_width =
                    feed_screen_iterator_tab(
                        p_iterator->p_client->p_screen_info,
                        &(
                            p_iterator->o_screen_iterator));
            }
            else
            {
                i_width =
                    feed_glyph_get_visible_width(
                        p_iterator->p_glyph);

                feed_screen_iterator_write(
                    p_iterator->p_client->p_screen_info,
                    &(
                        p_iterator->o_screen_iterator),
                    i_width);
            }
        }
        else
        {
            i_width =
                feed_screen_iterator_clear(
                    p_iterator->p_client->p_screen_info,
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

    if (feed_view_state_prompt == p_iterator->e_state)
    {
        if (feed_view_next_prompt_glyph(p_iterator))
        {
            p_iterator->e_state = feed_view_state_prompt;
        }
        else if (feed_view_first_text_glyph(p_iterator))
        {
            p_iterator->e_state = feed_view_state_text;
        }
        else
        {
            p_iterator->e_state = feed_view_state_null;
        }

        b_result = 1;
    }
    else if (feed_view_state_text == p_iterator->e_state)
    {
        if (feed_view_next_text_glyph(p_iterator))
        {
            p_iterator->e_state = feed_view_state_text;
        }
        else
        {
            if (feed_view_next_line(p_iterator))
            {
                if (feed_view_first_prompt_glyph(p_iterator))
                {
                    p_iterator->e_state = feed_view_state_prompt;
                }
                else if (feed_view_first_text_glyph(p_iterator))
                {
                    p_iterator->e_state = feed_view_state_text;
                }
                else
                {
                    p_iterator->e_state = feed_view_state_null;
                }
            }
            else
            {
                p_iterator->e_state = feed_view_state_null;
            }
        }

        b_result = 1;
    }
    else
    {
        b_result = 0;
    }

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

/*

Function: feed_view_prev

Description:

    Move to previous character in document and in screen.

Comments:

    TEXT PROMPT EOL
    TEXT PROMPT EOF NULL

*/
char
feed_view_prev(
    struct feed_view * const
        p_iterator)
{
    char
        b_result;

    feed_view_write(
        p_iterator);

    if (feed_view_state_text == p_iterator->e_state)
    {
        if (
            feed_view_prev_text_glyph(
                p_iterator))
        {
            p_iterator->e_state = feed_view_state_text;
        }
        else if (
            feed_view_last_prompt_glyph(
                p_iterator))
        {
            p_iterator->e_state = feed_view_state_prompt;
        }
        else
        {
            if (
                feed_view_prev_line(
                    p_iterator))
            {
                if (
                    feed_view_last_text_glyph(
                        p_iterator))
                {
                    p_iterator->e_state = feed_view_state_text;
                }
                else
                {
                    p_iterator->e_state = feed_view_state_null;
                }
            }
            else
            {
                p_iterator->e_state = feed_view_state_null;
            }
        }

        b_result =
            1;
    }
    else if (
        feed_view_state_prompt == p_iterator->e_state)
    {
        if (
            feed_view_prev_prompt_glyph(
                p_iterator))
        {
            p_iterator->e_state = feed_view_state_prompt;
        }
        else
        {
            if (
                feed_view_prev_line(
                    p_iterator))
            {
                if (
                    feed_view_last_text_glyph(
                        p_iterator))
                {
                    p_iterator->e_state = feed_view_state_text;
                }
                else
                {
                    p_iterator->e_state = feed_view_state_null;
                }
            }
            else
            {
                p_iterator->e_state = feed_view_state_null;
            }
        }

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

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

}

void
feed_view_cleanup(
    struct feed_view * const
        p_iterator)
{
    (void)(
        p_iterator);
}

/* end-of-file: feed_view.c */
