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

    p_utf8_parser->o_event.i_code =
        0ul;

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
            p_utf8_parser->o_event.i_code =
                (unsigned long int)(
                    (
                        p_utf8_parser->o_event.i_code << 6u)
                    | (unsigned long int)(
                        c_data & 0x3Fu));

            if (
                p_utf8_parser->o_event.i_raw_len
                == p_utf8_parser->i_count)
            {
                /* Notify */
                *(p_utf8_code) =
                    p_utf8_parser->o_event;

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

        p_utf8_parser->o_event.i_code =
            0ul;

        if (0xC0u <= c_data)
        {
            i_result =
                0;

            if (0xC0u == (c_data & 0xE0u))
            {
                p_utf8_parser->o_event.i_code =
                    (unsigned long int)(
                        c_data & 0x1Fu);

                p_utf8_parser->i_count =
                    2u;
            }
            else if (0xE0u == (c_data & 0xF0u))
            {
                p_utf8_parser->o_event.i_code =
                    (unsigned long int)(
                        c_data & 0x0Fu);

                p_utf8_parser->i_count =
                    3u;
            }
            else if (0xF0u == (c_data & 0xF8u))
            {
                p_utf8_parser->o_event.i_code =
                    (unsigned long int)(
                        c_data & 0x07u);

                p_utf8_parser->i_count =
                    4u;
            }
            else if (0xF8u == (c_data & 0xFCu))
            {
                p_utf8_parser->o_event.i_code =
                    (unsigned long int)(
                        c_data & 0x03u);

                p_utf8_parser->i_count =
                    5u;
            }
            else if (0xFCu == (c_data & 0xFEu))
            {
                p_utf8_parser->o_event.i_code =
                    (unsigned long int)(
                        c_data & 0x01u);

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
            p_utf8_parser->o_event.i_code =
                (unsigned long int)(
                    c_data);

            *(p_utf8_code) =
                p_utf8_parser->o_event;

            i_result =
                1;
        }
    }

    return
        i_result;

} /* feed_utf8_parser_write() */

/* end-of-file: feed_utf8.c */
