/* See LICENSE for license details */

/*

Module: feed_glyph.h

Description:

    Object to store a single glyph, which represents an array of bytes
    in raw file or an array of bytes to display onto terminal.

*/

/* Header file dependencies */
#if !defined(INC_FEED_OS_H)
#error include feed_os.h first
#endif /* #if !defined(INC_FEED_OS_H) */

/* Header file dependencies */
#if !defined(INC_FEED_LIST_H)
#error include feed_list.h first
#endif /* #if !defined(INC_FEED_LIST_H) */

/* Header file dependencies */
#if !defined(INC_FEED_UTF8_H)
#error include feed_utf8.h first
#endif /* #if !defined(INC_FEED_UTF8_H) */

/* Reverse include guard */
#if defined(INC_FEED_GLYPH_H)
#error include feed_glyph.h once
#endif /* #if defined(INC_FEED_GLYPH_H */

#define INC_FEED_GLYPH_H

struct feed_client;

/*

*/
struct feed_glyph
{
    /* This object is part of a list */
    struct feed_list
        o_list;

    /* Raw representation of character */
    struct feed_utf8_code
        o_utf8_code;

}; /* struct feed_glyph */


/* -- Interface -- */

struct feed_glyph *
feed_glyph_create(
    struct feed_client * const
        p_client,
    struct feed_utf8_code const * const
        p_utf8_code);

void
feed_glyph_destroy(
    struct feed_client * const
        p_client,
    struct feed_glyph * const
        p_glyph);

unsigned char
feed_glyph_render_visible(
    struct feed_glyph const * const
        p_glyph,
    unsigned char * const
        a_visible);

unsigned char
feed_glyph_get_visible_width(
    struct feed_glyph const * const
        p_glyph);

/* end-of-file: feed_glyph.h */
