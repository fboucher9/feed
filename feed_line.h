/* See LICENSE for license details */

/*

Module: feed_line.h

Description:

    List of glyphs to form a single line of glyphs.

*/

/* Header file dependency */
#if !defined(INC_FEED_OS_H)
#error include feed_os.h first
#endif /* #if !defined(INC_FEED_OS_H) */

/* Header file dependency */
#if !defined(INC_FEED_LIST_H)
#error include feed_list.h before
#endif /* #if !defined(INC_FEED_LIST_H) */

/* Reverse include guard */
#if defined(INC_FEED_LINE_H)
#error include feed_line.h once
#endif /* #if defined(INC_FEED_LINE_H) */

#define INC_FEED_LINE_H

struct feed_client;

struct feed_utf8_code;

struct feed_buf;

struct feed_glyph;

struct feed_line
{
    struct feed_list
        o_list;

    struct feed_client *
        p_client;

    struct feed_list
        o_glyphs;

    /* Number of characters */
    unsigned long int
        i_glyph_count;

    /* Number of visible characters */
    unsigned long int
        i_width;

    /* Number of raw bytes */
    unsigned long int
        i_raw_len;

    unsigned long int
        a_padding[1u];

}; /* struct feed_line */

struct feed_line *
feed_line_create(
    struct feed_client * const
        p_client);

void
feed_line_destroy(
    struct feed_line * const
        p_line);

void
feed_line_write_utf8_code(
    struct feed_line * const
        p_line,
    struct feed_utf8_code const * const
        p_utf8_code,
    unsigned long int const
        i_glyph_index);

void
feed_line_append_glyph(
    struct feed_line * const
        p_line,
    struct feed_glyph * const
        p_glyph);

void
feed_line_append_utf8_code(
    struct feed_line * const
        p_line,
    struct feed_utf8_code const * const
        p_utf8_code);

struct feed_glyph *
feed_line_get_glyph(
    struct feed_line * const
        p_line,
    unsigned long int const
        i_glyph_index);

unsigned long int
feed_line_get_raw_length(
    struct feed_line * const
        p_line);

void
feed_line_get_raw_buffer(
    struct feed_line * const
        p_line,
    struct feed_buf * const
        p_buf);

char
feed_line_set(
    struct feed_line * const
        p_line,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length);

void
feed_line_remove_glyph(
    struct feed_line * const
        p_line,
    struct feed_glyph * const
        p_glyph);

void
feed_line_insert_glyph_before(
    struct feed_line * const
        p_line,
    struct feed_glyph * const
        p_glyph_new,
    struct feed_glyph * const
        p_glyph_existing);

/* end-of-file: feed_line.h */
