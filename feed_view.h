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
    feed_view_state_text,

    /* After the end-of-file.  This is used to detect that the end of the file
    is within the current page. */
    feed_view_state_null

}; /* enum feed_view_state */

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

    /* Pointer to documents line */
    struct feed_line *
        p_document_line;

    /* Pointer to prompt line */
    struct feed_line *
        p_prompt_line;

    /* Pointer to glyph object */
    struct feed_glyph *
        p_glyph;

    /* Position in screen */
    struct feed_screen_iterator
        o_screen_iterator;

    /* Position in document (x is valid if not in prompt) */
    unsigned long int
        i_glyph_index;

    unsigned long int
        i_line_index;

    enum feed_view_state
        e_state;

    int
        b_screen_full;

    unsigned int
        ui_padding[2u];

};

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
    struct feed_line * const
        p_page_line,
    unsigned long int const
        i_page_line_index,
    unsigned long int const
        i_page_glyph_index,
    enum feed_view_state const
        e_page_state);

char
feed_view_next(
    struct feed_view * const
        p_iterator);

char
feed_view_tail(
    struct feed_view * const
        p_iterator,
    struct feed_line * const
        p_page_line,
    unsigned long int const
        i_page_line_index,
    unsigned long int const
        i_page_glyph_index,
    enum feed_view_state const
        e_page_state);

char
feed_view_prev(
    struct feed_view * const
        p_iterator);
