/* See LICENSE for license details */

/*

Module: feed_utf8.c

Description:

    Parser for utf8 strings.

*/

#include "feed_os.h"

#include "feed_utf8.h"

char
feed_utf8_parser_init(
    struct feed_utf8_parser * const
        p_utf8_parser)
{
    char
        b_result;

    p_utf8_parser->o_event.i_raw_len =
        0u;

    p_utf8_parser->i_count =
        0u;

    b_result =
        1;

    return
        b_result;

}

void
feed_utf8_parser_cleanup(
    struct feed_utf8_parser * const
        p_utf8_parser)
{
    (void)(
        p_utf8_parser);

}

int
feed_utf8_parser_write(
    struct feed_utf8_parser * const
        p_utf8_parser,
    unsigned char const
        c_data,
    struct feed_utf8_code * const
        p_utf8_code)
{
    int
        i_result;

    if (p_utf8_parser->i_count)
    {
        p_utf8_parser->o_event.a_raw[p_utf8_parser->o_event.i_raw_len] =
            c_data;

        p_utf8_parser->o_event.i_raw_len ++;

        if (0x80u == (c_data & 0xC0u))
        {
            if (
                p_utf8_parser->o_event.i_raw_len
                == p_utf8_parser->i_count)
            {
                /* Notify */
                *(p_utf8_code) =
                    p_utf8_parser->o_event;

                p_utf8_parser->i_count =
                    0u;

                i_result =
                    1;
            }
            else
            {
                i_result =
                    0;
            }
        }
        else
        {
            p_utf8_parser->i_count =
                0u;

            i_result =
                -1;
        }
    }
    else
    {
        p_utf8_parser->o_event.i_raw_len =
            1u;

        p_utf8_parser->o_event.a_raw[0u] =
            c_data;

        if (0xC0u <= c_data)
        {
            i_result =
                0;

            if (0xC0u == (c_data & 0xE0u))
            {
                p_utf8_parser->i_count =
                    2u;
            }
            else if (0xE0u == (c_data & 0xF0u))
            {
                p_utf8_parser->i_count =
                    3u;
            }
            else if (0xF0u == (c_data & 0xF8u))
            {
                p_utf8_parser->i_count =
                    4u;
            }
            else if (0xF8u == (c_data & 0xFCu))
            {
                p_utf8_parser->i_count =
                    5u;
            }
            else if (0xFCu == (c_data & 0xFEu))
            {
                p_utf8_parser->i_count =
                    6u;
            }
            else
            {
                i_result =
                    -1;
            }
        }
        else
        {
            *(p_utf8_code) =
                p_utf8_parser->o_event;

            i_result =
                1;
        }
    }

    return
        i_result;

} /* feed_utf8_parser_write() */

unsigned long int
feed_utf8_decode(
    struct feed_utf8_code const * const
        p_utf8_code)
{
    unsigned long int
        i_code;

    unsigned char
        c;

    c =
        p_utf8_code->a_raw[0u];

    if (c < 0xC0u)
    {
        i_code =
            (unsigned long int)(
                c);
    }
    else
    {
        unsigned char
            i;

        if (c < 0xE0u)
        {
            i_code =
                (unsigned long int)(
                    c & 0x1Fu);
        }
        else if (c < 0xF0u)
        {
            i_code =
                (unsigned long int)(
                    c & 0x0Fu);
        }
        else if (c < 0xF8u)
        {
            i_code =
                (unsigned long int)(
                    c & 0x07u);
        }
        else if (c < 0xFCu)
        {
            i_code =
                (unsigned long int)(
                    c & 0x03u);
        }
        else if (c < 0xFEu)
        {
            i_code =
                (unsigned long int)(
                    c & 0x01u);
        }
        else
        {
            i_code =
                0ul;
        }

        for (
            i = 1u;
            i < p_utf8_code->i_raw_len;
            i ++)
        {
            c =
                p_utf8_code->a_raw[i];

            i_code =
                (unsigned long int)(
                    (i_code << 6u)
                    | (unsigned long int)(
                        c & 0x3Fu));
        }
    }

    return
        i_code;

}

#if 0
/*
7 = 7
6+5 = 11
6+6+4 = 16
6+6+6+3 = 21
6+6+6+6+2 = 26
6+6+6+6+6+1 = 31
*/
void
feed_utf8_encode(
    struct feed_utf8_code * const
        p_utf8_code,
    unsigned long int const
        i_code)
{
    if (
        i_code <= 0x7Ful)
    {
    }
    else if (
        i_code <= 0x7FFul)
    {
    }
    else if (
        i_code <= 0xFFFFul)
    {
    }
    else if (
        i_code <= 0x1FFFFFul)
    {
    }
    else if (
        i_code <= 0x3FFFFFFul)
    {
    }
    else if (
        i_code <= 0x7FFFFFFFul)
    {
    }
}
#endif

/* end-of-file: feed_utf8.c */
