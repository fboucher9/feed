/* See LICENSE for license details */

/*

Module: feed_utf8.h

Description:

    Parser for utf8 strings.

*/

struct feed_utf8_code
{
    unsigned long int
        i_code;

    unsigned char
        i_raw_len;

    unsigned char
        a_raw[7u];

};

struct feed_utf8_parser
{
    struct feed_utf8_code
        o_event;

    unsigned char
        i_count;

    unsigned char
        uc_padding[7u];

};

char
feed_utf8_parser_init(
    struct feed_utf8_parser * const
        p_utf8_parser);

void
feed_utf8_parser_cleanup(
    struct feed_utf8_parser * const
        p_utf8_parser);

int
feed_utf8_parser_write(
    struct feed_utf8_parser * const
        p_utf8_parser,
    unsigned char const
        c_data,
    struct feed_utf8_code * const
        p_utf8_code);

