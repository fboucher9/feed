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

struct feed_client;

struct feed_combo_iterator;

/*

Structure: feed_syntax_parser

Description:

    Syntax state machine.

*/
struct feed_syntax_parser
{
    struct feed_client *
        p_client;

    struct feed_combo_iterator *
        p_combo_iterator;

    /* -- */

    /* Current state */
    enum feed_syntax
        e_syntax;

    unsigned int
        ui_padding[3u];

}; /* struct feed_syntax_parser */

/* Begin syntax iteration from top of page */
void
feed_syntax_parser_init(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_client * const
        p_client,
    struct feed_combo_iterator * const
        p_combo_iterator,
    enum feed_syntax const
        e_syntax);

void
feed_syntax_parser_cleanup(
    struct feed_syntax_parser * const
        p_syntax_parser);

/* Continue syntax iteration from previous glyph */
char
feed_syntax_parser_next(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_glyph * * const
        pp_glyph,
    enum feed_syntax * const
        p_syntax);

/* end-of-file: feed_syntax.h */
