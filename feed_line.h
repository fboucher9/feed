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

struct feed_event;

struct feed_buf;

struct feed_line
{
    struct feed_list
        o_list;

    struct feed_client *
        p_client;

    struct feed_list
        o_glyphs;

    /* Number of characters */
    unsigned int
        i_glyph_count;

    unsigned int
        a_padding[3u];

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
feed_line_write_event(
    struct feed_line * const
        p_line,
    struct feed_event const * const
        p_event,
    unsigned int const
        i_glyph_index);

struct feed_glyph *
feed_line_get_glyph(
    struct feed_line * const
        p_line,
    unsigned int const
        i_glyph_index);

unsigned int
feed_line_get_raw_length(
    struct feed_line * const
        p_line);

void
feed_line_get_raw_buffer(
    struct feed_line * const
        p_line,
    struct feed_buf * const
        p_buf);

/* end-of-file: feed_line.h */
