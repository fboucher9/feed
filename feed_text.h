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

struct feed_utf8_code;

struct feed_text
{
    struct feed_client *
        p_client;

    struct feed_list
        o_lines;

    struct feed_event
        o_last_event;

    /* Number of lines */
    unsigned long int
        i_line_count;

    unsigned long int
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

struct feed_line *
feed_text_get_line(
    struct feed_text const * const
        p_text,
    unsigned long int const
        i_line_index);

void
feed_text_write_utf8_code(
    struct feed_text * const
        p_text,
    struct feed_utf8_code const * const
        p_utf8_code,
    unsigned long int const
        i_line_index,
    unsigned long int const
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

char
feed_text_load(
    struct feed_text * const
        p_text,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length);

/* end-of-file: feed_text.h */
