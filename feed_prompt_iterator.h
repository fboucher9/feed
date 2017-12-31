/* See LICENSE for license details */

/*

Module: feed_prompt_iterator.h

Description:

*/

/* Reverse include guard */
#if defined(INC_FEED_PROMPT_ITERATOR_H)
#error include feed_prompt_iterator.h once
#endif /* #if defined(INC_FEED_PROMPT_ITERATOR_H) */

#define INC_FEED_PROMPT_ITERATOR_H

/* Predefine prompt handle */
struct feed_prompt;

/* Predefine line handle */
struct feed_line;

/* Predefine glyph handle */
struct feed_glyph;

struct feed_prompt_iterator
{
    struct feed_client *
        p_client;

    struct feed_prompt *
        p_prompt;

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

}; /* struct feed_prompt_iterator */

char
feed_prompt_iterator_init(
    struct feed_client * const
        p_client,
    struct feed_prompt * const
        p_prompt,
    struct feed_prompt_iterator * const
        p_iterator);

void
feed_prompt_iterator_cleanup(
    struct feed_prompt_iterator * const
        p_iterator);

char
feed_prompt_iterator_set_line(
    struct feed_prompt_iterator * const
        p_iterator,
    unsigned long int const
        i_line_index);

char
feed_prompt_iterator_set_glyph(
    struct feed_prompt_iterator * const
        p_iterator,
    unsigned long int const
        i_glyph_index);

char
feed_prompt_iterator_set_index(
    struct feed_prompt_iterator * const
        p_iterator,
    unsigned long int const
        i_line_index,
    unsigned long int const
        i_glyph_index);

char
feed_prompt_iterator_first_glyph(
    struct feed_prompt_iterator * const
        p_iterator);

char
feed_prompt_iterator_last_glyph(
    struct feed_prompt_iterator * const
        p_iterator);

char
feed_prompt_iterator_next_glyph(
    struct feed_prompt_iterator * const
        p_iterator);

char
feed_prompt_iterator_prev_glyph(
    struct feed_prompt_iterator * const
        p_iterator);

/* end-of-file: feed_prompt_iterator.h */
