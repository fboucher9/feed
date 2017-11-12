/* See LICENSE for license details */

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed_text_iterator.h"

#include "feed_list.h"

#include "feed_input.h"

#include "feed_utf8.h"

#include "feed_text.h"

#include "feed_line.h"

#include "feed_glyph.h"

void
feed_text_iterator_init(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    (void)(
        p_text);

    p_text_iterator->p_line =
        (struct feed_line *)(
            0);

    p_text_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    p_text_iterator->i_line_index =
        0ul;

    p_text_iterator->i_glyph_index =
        0ul;

}

void
feed_text_iterator_cleanup(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
    (void)(
        p_text);

    p_text_iterator->p_line =
        (struct feed_line *)(
            0);

    p_text_iterator->p_glyph =
        (struct feed_glyph *)(
            0);

    p_text_iterator->i_line_index =
        0ul;

    p_text_iterator->i_glyph_index =
        0ul;

}

#if 0
void
feed_text_iterator_invalidate(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

void
feed_text_iterator_validate(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

char
feed_text_iterator_next_line(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

char
feed_text_iterator_prev_line(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

char
feed_text_iterator_home_line(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

char
feed_text_iterator_end_line(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

char
feed_text_iterator_next_glyph(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

char
feed_text_iterator_prev_glyph(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

char
feed_text_iterator_home_glyph(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}

char
feed_text_iterator_end_glyph(
    struct feed_text * const
        p_text,
    struct feed_text_iterator * const
        p_text_iterator)
{
}
#endif

/* end-of-file: feed_text_iterator.c */
