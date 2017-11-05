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

/* Reverse include guard */
#if defined(INC_FEED_GLYPH_H)
#error include feed_glyph.h once
#endif /* #if defined(INC_FEED_GLYPH_H */

#define INC_FEED_GLYPH_H

struct feed_client;

struct feed_utf8_code;

/*

*/
struct feed_glyph
{
    /* This object is part of a list */
    struct feed_list
        o_list;

    /* Pointer to parent execution context */
    struct feed_client *
        p_client;

    /* Raw representation of character */
    unsigned char
        a_raw[7u];

    /* Length in bytes of raw character */
    unsigned char
        i_raw_length;

    /* Visible representation of character */
    unsigned char
        a_visible[14u];

    /* Length in bytes of visible character */
    unsigned char
        i_visible_length;

    /* Width of visible representation */
    unsigned char
        i_visible_width;

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
    struct feed_glyph * const
        p_glyph);

/* end-of-file: feed_glyph.h */
