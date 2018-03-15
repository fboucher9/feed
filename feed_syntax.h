/* See LICENSE for license details */

/*

Module: feed_syntax.h

Description:

    Syntax state machine.

*/

/* Reverse include guard */
#if defined(INC_FEED_SYNTAX_H)
#error include feed_syntax.h once
#endif /* #if defined(INC_FEED_SYNTAX_H) */

#define INC_FEED_SYNTAX_H

/* Header file dependencies */
#if !defined(INC_FEED_H)
#error include feed.h before
#endif /* #if !defined(INC_FEED_H) */

struct feed_client;

struct feed_glyph;

/*

Structure: feed_syntax_parser

Description:

    Syntax state machine.

*/
struct feed_syntax_parser
{
    struct feed_client *
        p_client;

    void *
        pv_padding[1u];

    /* -- */

    /* Current state */
    enum feed_syntax
        e_syntax;

    unsigned int
        i_raw_len;

    unsigned int
        i_input_read_index;

    unsigned int
        i_input_write_index;

    /* -- */

    unsigned int
        i_input_count;

    unsigned int
        i_output_read_index;

    unsigned int
        i_output_write_index;

    unsigned int
        i_output_count;

    /* -- */

    struct feed_glyph *
        a_input[128u];

    /* -- */

    struct feed_glyph *
        a_output[128u];

    /* -- */

    enum feed_syntax
        a_syntax[128u];

    /* -- */

    unsigned char
        a_raw[1024u];

}; /* struct feed_syntax_parser */

char
feed_syntax_parser_init(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_client * const
        p_client,
    enum feed_syntax const
        e_syntax);

void
feed_syntax_parser_cleanup(
    struct feed_syntax_parser * const
        p_syntax_parser);

char
feed_syntax_parser_is_ready(
    struct feed_syntax_parser * const
        p_syntax_parser);

char
feed_syntax_parser_write(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_glyph * const
        p_glyph);

char
feed_syntax_parser_read(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_glyph * * const
        pp_glyph,
    enum feed_syntax * const
        p_syntax);

/* end-of-file: feed_syntax.h */
