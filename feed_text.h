/* See LICENSE for license details */

/*

Module: feed_text.h

Description:

*/

/* Header file dependencies */
#if !defined(INC_FEED_OS_H)
#error include feed_os.h first
#endif /* #if !defined(INC_FEED_OS_H) */

/* Header file dependencies */
#if !defined(INC_FEED_LIST_H)
#error include feed_list.h before
#endif /* #if !defined(INC_FEED_LIST_H) */

/* Header file dependencies */
#if !defined(INC_FEED_INPUT_H)
#error include feed_input.h before
#endif /* #if !defined(INC_FEED_INPUT_H) */

/* Reverse include guard */
#if defined(INC_FEED_TEXT_H)
#error include feed_text.h once
#endif /* #if defined(INC_FEED_TEXT_H) */

#define INC_FEED_TEXT_H

struct feed_client;

struct feed_text
{
    struct feed_client *
        p_client;

    struct feed_list
        o_lines;

    struct feed_line *
        p_prompt;

    struct feed_event
        o_last_event;

    /* Number of lines */
    unsigned int
        i_line_count;

    /* Cursor */
    unsigned int
        i_cursor_line_index;

    unsigned int
        i_cursor_glyph_index;

    unsigned int
        a_padding[1u];

}; /* struct feed_text */

struct feed_text *
feed_text_create(
    struct feed_client * const
        p_client);

void
feed_text_destroy(
    struct feed_text * const
        p_text);

void
feed_text_write_event(
    struct feed_text * const
        p_text,
    struct feed_event const * const
        p_event);

char
feed_text_prompt(
    struct feed_text * const
        p_text,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length);

/* end-of-file: feed_text.h */
