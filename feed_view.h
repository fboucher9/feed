/* See LICENSE for license details */

/*

Module: feed_view.h

Description:

    Iterator for rendering of a visible page.  This object manages the rendering
    of both prompt and text onto a single visible screen.

*/

/* Reverse include guard */
#if defined(INC_FEED_VIEW_H)
#error include feed_view.h once
#endif /* #if defined(INC_FEED_VIEW_H) */

#define INC_FEED_VIEW_H

/* Predefine client handle */
struct feed_client;

/* Predefine line handle */
struct feed_line;

/* Predefine glyph handle */
struct feed_glyph;

/*

Enumeration: feed_view_state

Description:

    States used by feed_view state machine.

Comments:

    Each line is composed of the following parts:

        [prompt] [text] [eol | eof]

    The null state is after a eof.

*/
enum feed_view_state
{
    /* Display of a prompt character, if the prompt is empty then the
    state machine will automatically switch to the text state */
    feed_view_state_prompt = 1,

    /* Display of a text character, if the text is empty then the state
    machine will automatically switch to the eol state */
    feed_view_state_text

}; /* enum feed_view_state */

struct feed_view_descriptor
{
    struct feed_line *
        p_line;

    struct feed_glyph *
        p_glyph;

    unsigned long int
        i_line_index;

    unsigned long int
        i_glyph_index;

    unsigned long int
        i_line_offset;

    unsigned long int
        i_glyph_offset;

    enum feed_view_state
        e_state;

    unsigned int
        ui_padding[3u];

};

/*

Structure: feed_combo_iterator

Description:

    Iterator for combination of prompt and text

*/
struct feed_combo_iterator
{
    struct feed_client *
        p_client;

    void *
        pv_padding[1u];

    /* -- */

    /* Position in text */
    struct feed_text_iterator
        o_text_iterator;

    /* -- */

    /* Position in prompt */
    struct feed_prompt_iterator
        o_prompt_iterator;

    /* -- */

    enum feed_view_state
        e_state;

    unsigned int
        ui_padding[3u];

}; /* struct feed_combo_iterator */

char
feed_combo_iterator_init(
    struct feed_combo_iterator * const
        p_combo_iterator,
    struct feed_client * const
        p_client);

void
feed_combo_iterator_cleanup(
    struct feed_combo_iterator * const
        p_combo_iterator);

char
feed_combo_iterator_head(
    struct feed_combo_iterator * const
        p_combo_iterator,
    struct feed_view_descriptor const * const
        p_view_descriptor);

char
feed_combo_iterator_next(
    struct feed_combo_iterator * const
        p_combo_iterator);

char
feed_combo_iterator_query(
    struct feed_combo_iterator * const
        p_combo_iterator,
    struct feed_view_descriptor * const
        p_view_descriptor);

/*

refresh engine

Notes:

    -   Coordinate system for page.  Specify a starting document line index
        and an offset in number of wraps and number of columns.

    -   If number of wraps exceeds actual number of wraps used for line, then
        a new page coordinate is calculated.

        i_page_offset_x = i_offset_x
        i_page_offset_y = i_offset_y
        i_page_line = i_line_index

    -   Problem: what if nothing is visible in page?

    -   Problem: automatic scroll on changes

        Display has page_line+offset_y+offset_x -> final_line+offset_y+offset_x

    -   State of iterator:

            prompt
            text
            eol
            eof

*/
struct feed_view
{
    struct feed_client *
        p_client;

    void *
        pv_padding[1u];

    /* -- */

    /* Iterator for prompt and text combination */
    struct feed_combo_iterator
        o_combo_iterator;

    /* -- */

    /* Position in screen */
    struct feed_screen_iterator
        o_screen_iterator;

    /* -- */

    int
        b_screen_full;

    unsigned int
        ui_padding[3u];

}; /* struct feed_view */

void
feed_view_init(
    struct feed_view * const
        p_iterator,
    struct feed_client * const
        p_client);

void
feed_view_cleanup(
    struct feed_view * const
        p_iterator);

char
feed_view_head(
    struct feed_view * const
        p_iterator,
    struct feed_view_descriptor const * const
        p_view_descriptor);

char
feed_view_next(
    struct feed_view * const
        p_iterator);

char
feed_view_query(
    struct feed_view * const
        p_iterator,
    struct feed_view_descriptor * const
        p_view_descriptor);

