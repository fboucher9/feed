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

/* Header file dependencies */
#if !defined(INC_FEED_UTF8_H)
#error include feed_utf8.h before
#endif /* #if !defined(INC_FEED_UTF8_H) */

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

    struct feed_utf8_parser
        o_utf8_parser;

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

unsigned long int
feed_text_length(
    struct feed_text * const
        p_text);

void
feed_text_save(
    struct feed_text * const
        p_text,
    struct feed_buf * const
        p_buf);

void
feed_text_remove_line(
    struct feed_text * const
        p_text,
    struct feed_line * const
        p_line);

void
feed_text_insert_line_after(
    struct feed_text * const
        p_text,
    struct feed_line * const
        p_line_existing,
    struct feed_line * const
        p_line_new);

void
feed_text_insert_line_tail(
    struct feed_text * const
        p_text,
    struct feed_line * const
        p_line_new);

void
feed_text_clear(
    struct feed_text * const
        p_text);

char
feed_text_index_to_offset(
    struct feed_text * const
        p_text,
    unsigned long int const
        i_line_index,
    unsigned long int const
        i_glyph_index,
    unsigned long int * const
        p_offset);

char
feed_text_offset_to_index(
    struct feed_text * const
        p_text,
    unsigned long int const
        i_offset,
    unsigned long int * const
        p_line_index,
    unsigned long int * const
        p_glyph_index);

void
feed_text_consume(
    struct feed_text * const
        p_text,
    struct feed_buf * const
        p_buf);

/* end-of-file: feed_text.h */
