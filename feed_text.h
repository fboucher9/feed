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

struct feed_buf;

struct feed_text
{
    struct feed_client *
        p_client;

    struct feed_list
        o_lines;

    struct feed_event
        o_last_event;

    /* Number of lines */
    unsigned int
        i_line_count;

    unsigned int
        a_padding[3u];

}; /* struct feed_text */

struct feed_text *
feed_text_create(
    struct feed_client * const
        p_client);

void
feed_text_destroy(
    struct feed_text * const
        p_text);

struct feed_line *
feed_text_get_line(
    struct feed_text * const
        p_text,
    unsigned int const
        i_line_index);

void
feed_text_write_event(
    struct feed_text * const
        p_text,
    struct feed_event const * const
        p_event,
    unsigned int const
        i_line_index,
    unsigned int const
        i_glyph_index);

unsigned int
feed_text_get_raw_length(
    struct feed_text * const
        p_text);

void
feed_text_get_raw_buffer(
    struct feed_text * const
        p_text,
    struct feed_buf * const
        p_buf);

/* end-of-file: feed_text.h */
