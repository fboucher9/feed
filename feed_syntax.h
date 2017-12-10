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

/*

Structure: feed_syntax_parser

Description:

    Syntax state machine.

*/
struct feed_syntax_parser
{
    struct feed_client *
        p_client;

    /* Queue */
    struct feed_glyph *
        a_glyphs[256u];

    /* Raw word */
    unsigned char
        a_word[2048u];

    /* Number of glyphs in queue */
    unsigned int
        i_glyph_count;

    /* Number of bytes in word */
    unsigned int
        i_word_length;

    /* Current state */
    enum feed_syntax
        e_syntax;

    /* Word detection state */
    char
        b_word;

    /* Display of prompt */
    char
        b_prompt;

}; /* struct feed_syntax_parser */

void
feed_syntax_parser_init(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_client * const
        p_client);

void
feed_syntax_parser_cleanup(
    struct feed_syntax_parser * const
        p_syntax_parser);

void
feed_syntax_parser_head(
    struct feed_syntax_parser * const
        p_syntax_parser,
    enum feed_syntax const
        e_syntax);

char
feed_syntax_parser_write(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_glyph * const
        p_glyph,
    char const
        b_prompt);

char
feed_syntax_parser_read(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_glyph * * const
        pp_glyph,
    enum feed_syntax * const
        p_syntax);

/* end-of-file: feed_syntax.h */
