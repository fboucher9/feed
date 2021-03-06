/* See LICENSE for license details */

/*

Module: feed_text_iterator.h

Description:

    Iterator for feed_text, feed_line and feed_glyph objects.

*/

struct feed_text;

struct feed_line;

struct feed_glyph;

struct feed_buf;

struct feed_utf8_code;

struct feed_text_iterator
{
    struct feed_client *
        p_client;

    struct feed_text *
        p_text;

    /* -- */

    struct feed_line *
        p_line;

    struct feed_glyph *
        p_glyph;

    /* -- */

    unsigned long int
        i_line_index;

    unsigned long int
        i_glyph_index;

    /* -- */

    unsigned long int
        i_line_offset;

    unsigned long int
        i_glyph_offset;

}; /* struct feed_text_iterator */

void
feed_text_iterator_init(
    struct feed_client * const
        p_client,
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator);

void
feed_text_iterator_cleanup(
    struct feed_text_iterator * const
        p_text_iterator);

void
feed_text_iterator_invalidate(
    struct feed_text_iterator * const
        p_text_iterator);

void
feed_text_iterator_validate(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_set_line(
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_line_index);

char
feed_text_iterator_next_line(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_prev_line(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_home_line(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_end_line(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_set_glyph(
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_glyph_index);

char
feed_text_iterator_next_glyph(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_prev_glyph(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_home_glyph(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_end_glyph(
    struct feed_text_iterator * const
        p_text_iterator);

struct feed_glyph *
feed_text_iterator_remove_glyph(
    struct feed_text_iterator * const
        p_text_iterator);

void
feed_text_iterator_append_glyph(
    struct feed_text_iterator * const
        p_text_iterator,
    struct feed_glyph * const
        p_glyph);

void
feed_text_iterator_write_glyph(
    struct feed_text_iterator * const
        p_text_iterator,
    struct feed_glyph * const
        p_glyph);

char
feed_text_iterator_join_lines(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_set_index(
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_line_index,
    unsigned long int const
        i_glyph_index);

char
feed_text_iterator_set_offset(
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_offset);

unsigned long int
feed_text_iterator_get_offset(
    struct feed_text_iterator * const
        p_text_iterator);

void
feed_text_iterator_save(
    struct feed_text_iterator * const
        p_text_iterator,
    struct feed_buf * const
        p_buf);

void
feed_text_iterator_delete_region(
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned long int const
        i_region_length);

void
feed_text_iterator_load(
    struct feed_text_iterator * const
        p_text_iterator,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length);

char
feed_text_iterator_insert_newline(
    struct feed_text_iterator * const
        p_text_iterator);

char
feed_text_iterator_insert_code(
    struct feed_text_iterator * const
        p_text_iterator,
    struct feed_utf8_code const * const
        p_utf8_code);

/* end-of-file: feed_text_iterator.h */
