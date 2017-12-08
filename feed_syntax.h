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

*/

struct feed_syntax_parser
{
    struct feed_client *
        p_client;


};

void
feed_syntax_parser_init(
    struct feed_syntax_parser * const
        p_syntax_parser,
    struct feed_client * const
        p_client);

/* end-of-file: feed_syntax.h */
