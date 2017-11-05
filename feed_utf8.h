/* See LICENSE for license details */

/*

Module: feed_utf8.h

Description:

    Parser for utf8 strings.

*/

/* Reverse include guard */
#if defined(INC_FEED_UTF8_H)
#error include feed_utf8.h once
#endif /* #if defined(INC_FEED_UTF8_H) */

#define INC_FEED_UTF8_H

struct feed_utf8_code
{
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

/* --- Interface --- */

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

unsigned long int
feed_utf8_decode(
    struct feed_utf8_code const * const
        p_utf8_code);

void
feed_utf8_encode(
    struct feed_utf8_code * const
        p_utf8_code,
    unsigned long int const
        i_code);

/* end-of-file: feed_utf8.h */
