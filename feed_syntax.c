/* See LICENSE for license details */

/*

Module: feed_syntax.c

Description:

    Syntax state machine.

*/

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed.h"

#include "feed_syntax.h"

#include "feed_list.h"

#include "feed_utf8.h"

#include "feed_glyph.h"

char
feed_syntax_parser_init(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_client * const
        p_client,
    enum feed_syntax const
        e_syntax)
{
    char
        b_result;

    p_syntax_parser->p_client =
        p_client;

    p_syntax_parser->e_syntax =
        e_syntax;

    p_syntax_parser->i_raw_len =
        0u;

    p_syntax_parser->i_input_read_index =
        0u;

    p_syntax_parser->i_input_write_index =
        0u;

    p_syntax_parser->i_input_count =
        0u;

    p_syntax_parser->i_output_read_index =
        0u;

    p_syntax_parser->i_output_write_index =
        0u;

    p_syntax_parser->i_output_count =
        0u;

    b_result =
        1;

    return
        b_result;

} /* feed_syntax_parser_init() */

void
feed_syntax_parser_cleanup(
    struct feed_syntax_parser * const
        p_syntax_parser)
{
    (void)(
        p_syntax_parser);

} /* feed_syntax_parser_cleanup() */

#if 0
static
char
feed_syntax_parser_move_right(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_text_iterator * const
        p_text_iterator)
{
    char
        b_result;

    if (
        feed_text_iterator_next_glyph(
            p_syntax_parser->p_text,
            p_text_iterator))
    {
        b_result =
            1;
    }
    else
    {
        b_result =
            feed_text_iterator_next_line(
                p_syntax_parser->p_text,
                p_text_iterator);

        if (
            b_result)
        {
            b_result =
                feed_text_iterator_home_glyph(
                    p_syntax_parser->p_text,
                    p_text_iterator);
        }
    }
}
#endif

#if 0
char
feed_syntax_parser_next(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_glyph * * const
        pp_glyph,
    enum feed_syntax * const
        p_syntax)
{
    char
        b_result;

    (void)(
        p_syntax_parser);
    (void)(
        pp_glyph);
    (void)(
        p_syntax);

    /* Get the next glyph */
    if (
        feed_syntax_parser_move_right(
            p_syntax_parser,
            p_syntax_parser->p_text_iterator))
    {
        unsigned char
            c;

        /* Detect current glyph */
        feed_text_iterator_validate(
            p_syntax_parser->p_text,
            p_syntax_parser->p_text_iterator);

        if (
            p_syntax_parser->p_text_iterator->p_glyph->i_raw_len)
        {
            c =
                p_syntax_parser->p_text_iterator->p_glyph->a_raw[0u];
        }
        else
        {
            c =
                '\n';
        }

        /* Detect beginning of line */
        if (
            feed_syntax_newline
            == p_syntax_parser->e_syntax)
        {
            /* Detect preprocessor */
            if (
                (' ' == c)
                || ('\t' == c))
            {
                p_syntax_parser->e_syntax =
                    feed_syntax_indentation;
            }
            else if (
                '#' == c)
            {
                p_syntax_parser->e_syntax =
                    feed_syntax_preprocessor;
            }
            else if (
                ('-' == c)
                || ('+' == c)
                || ('=' == c)
                || ('|' == c)
                || ('<' == c)
                || ('>' == c)
                || (',' == c)
                || ('.' == c)
                || ('/' == c)
                || ('?' == c)
                || (';' == c)
                || (':' == c)
                || ('[' == c)
                || (']' == c)
                || ('{' == c)
                || ('}' == c)
                || ('!' == c)
                || ('~' == c)
                || ('%' == c)
                || ('^' == c)
                || ('&' == c)
                || ('*' == c)
                || ('(' == c)
                || (')' == c))
            {
            }
            else
            {
            }
        }
        else if (
            feed_syntax_indentation
            == p_syntax_parser->e_syntax)
        {
            /* Detect preprocessor */
            if (
                '#' == c)
            {
            }
        }
        else if (
            feed_syntax_whitespace
            == p_syntax_parser->e_syntax)
        {
        }
        else if (
            feed_syntax_operator
            == p_syntax_parser->e_syntax)
        {
        }
        else if (
            feed_syntax_preprocessor
            == p_syntax_parser->e_syntax)
        {
        }
        else
        {
        }

        /* Detect beginning of word */

        /* Setup a preview text iterator */
        struct feed_text_iterator
            o_preview;

        o_preview =
            *(
                p_syntax_parser->p_text_iterator);

    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

} /* feed_syntax_parser_next() */
#endif

/*

*/
char
feed_syntax_parser_is_ready(
    struct feed_syntax_parser * const
        p_syntax_parser)
{
    char
        b_result;

    if (
        p_syntax_parser->i_output_count)
    {
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

} /* feed_syntax_parser_is_ready() */

/*

*/
char
feed_syntax_parser_write(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_glyph * const
        p_glyph)
{
    char
        b_result;

#if 0
    if (
        p_syntax_parser->i_input_count < 128u)
    {
        unsigned char
            i_glyph_raw_len;

        i_glyph_raw_len =
            p_glyph->o_utf8_code.i_raw_len;

        if (
            (p_syntax_parser->i_raw_len + i_glyph_raw_len) <= 1024u)
        {
            memcpy(
                p_syntax_parser->a_raw + p_syntax_parser->i_raw_len,
                p_glyph->o_utf8_code.a_raw,
                i_glyph_raw_len);

            p_syntax_parser->a_input[p_syntax_parser->i_input_write_index] =
                p_glyph;

            p_syntax_parser->i_input_write_index ++;

            if (p_syntax_parser->i_input_write_index >= 128u)
            {
                p_syntax_parser->i_input_write_index = 0u;
            }

            p_syntax_parser->i_input_count ++;

            p_syntax_parser->i_raw_len += i_glyph_raw_len;

            /* Trigger state machine... */

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
#endif

    /* Store glyph directly into output queue */
    if (
        p_syntax_parser->i_output_count < 128u)
    {
        p_syntax_parser->a_output[p_syntax_parser->i_output_write_index] =
            p_glyph;

        p_syntax_parser->a_syntax[p_syntax_parser->i_output_write_index] =
            feed_syntax_default;

        p_syntax_parser->i_output_write_index ++;

        if (p_syntax_parser->i_output_write_index >= 128u)
        {
            p_syntax_parser->i_output_write_index = 0u;
        }

        p_syntax_parser->i_output_count ++;

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

} /* feed_syntax_parser_write() */

/*

*/
char
feed_syntax_parser_read(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_glyph * * const
        pp_glyph,
    enum feed_syntax * const
        p_syntax)
{
    char
        b_result;

    if (
        p_syntax_parser->i_output_count)
    {
        *(
            pp_glyph) =
            p_syntax_parser->a_output[p_syntax_parser->i_output_read_index];

        *(
            p_syntax) =
            p_syntax_parser->a_syntax[p_syntax_parser->i_output_read_index];

        p_syntax_parser->i_output_read_index ++;

        if (p_syntax_parser->i_output_read_index >= 128u)
        {
            p_syntax_parser->i_output_read_index = 0u;
        }

        p_syntax_parser->i_output_count --;

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

} /* feed_syntax_parser_read() */

/* end-of-file: feed_syntax.c */
