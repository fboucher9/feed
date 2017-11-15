/* See LICENSE for license details */

/*

Module: feed.c

Description:

    Implementation of feed library API.

*/

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed.h"

#include "feed_client.h"

#include "feed_heap.h"

#include "feed_tty.h"

#include "feed_input.h"

#include "feed_keys.h"

#include "feed_prompt.h"

#include "feed_screen.h"

#include "feed_screen_info.h"

#include "feed_screen_iterator.h"

#include "feed_list.h"

#include "feed_utf8.h"

#include "feed_glyph.h"

#include "feed_line.h"

#include "feed_text.h"

#include "feed_text_iterator.h"

#include "feed_buf.h"

/*

Enumeration: feed_main_state

Description:

    States used by feed_main_iterator state machine.

Comments:

    Each line is composed of the following parts:

        [prompt] [text] [eol | eof]

    The null state is after a eof.

*/
enum feed_main_state
{
    /* Display of a prompt character, if the prompt is empty then the
    state machine will automatically switch to the text state */
    feed_main_state_prompt = 1,

    /* Display of a text character, if the text is empty then the state
    machine will automatically switch to the eol state */
    feed_main_state_text,

    /* Display an end-of-line.  If this is the last line of the text then
    the state machine will automatically switch to the eof state */
    feed_main_state_eol,

    /* Display an end-of-file.  The state machine should remain in this state
    until forced to go to next state, in that case it will switch to null
    state. */
    feed_main_state_eof,

    /* After the end-of-file.  This is used to detect that the end of the file
    is within the current page. */
    feed_main_state_null

}; /* enum feed_main_state */

struct feed_handle
{
    /* -- */

    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    /* -- */

    struct feed_prompt *
        p_prompt;

    struct feed_input *
        p_input;

    /* -- */

    struct feed_tty *
        p_tty;

    struct feed_screen *
        p_screen;

    /* -- */

    struct feed_text *
        p_text;

    struct feed_line *
        p_page_line;

    /* -- */

    struct feed_text_iterator
        o_cursor;

    /* -- */

    struct feed_descriptor
        o_descriptor;

    struct feed_client
        o_client;

    struct feed_screen_info
        o_screen_info;

    /* Page */
    unsigned long int
        i_page_line_index;

    unsigned long int
        i_page_glyph_index;

    /* -- */

    unsigned long int
        i_final_line_index;

    unsigned long int
        i_final_glyph_index;

    /* -- */

    enum feed_main_state
        e_final_state;

    enum feed_main_state
        e_page_state;

    unsigned int
        ui_padding[2u];

    /* -- */

    unsigned short int
        i_cursor_visible_x;

    unsigned short int
        i_cursor_visible_y;

    /* -- */

    unsigned short int
        i_final_cursor_x;

    unsigned short int
        i_final_cursor_y;

    /* -- */

    char
        b_started;

    char
        b_verbose;

    char
        b_cursor_visible;

    unsigned char
        uc_padding[5u];

}; /* struct feed_handle */

static
char
feed_init(
    struct feed_handle * const
        p_this,
    struct feed_heap * const
        p_heap,
    struct feed_descriptor const * const
        p_feed_descriptor)
{
    char
        b_result;

    memset(
        p_this,
        0x00u,
        sizeof(
            *(
                p_this)));

    p_this->p_client =
        &(
            p_this->o_client);

    feed_client_init(
        p_this->p_client);

    p_this->p_heap =
        p_heap;

    p_this->o_client.p_heap =
        p_heap;

    p_this->o_descriptor =
        *(
            p_feed_descriptor);

    p_this->p_prompt =
        feed_prompt_create(
            p_this->p_client);

    p_this->p_input =
        feed_input_create(
            p_this->p_client);

    p_this->p_tty =
    p_this->p_client->p_tty =
        feed_tty_create(
            p_this->p_client);

    p_this->p_screen =
        feed_screen_create(
            p_this->p_client);

    p_this->p_text =
        feed_text_create(
            p_this->p_client);

    feed_screen_info_init(
        &(
            p_this->o_screen_info));

    b_result =
        1;

    return
        b_result;

} /* feed_init() */

static
void
feed_cleanup(
    struct feed_handle * const
        p_this)
{
    feed_screen_info_cleanup(
        &(
            p_this->o_screen_info));

    if (
        p_this->p_text)
    {
        feed_text_destroy(
            p_this->p_text);

        p_this->p_text =
            (struct feed_text *)(
                0);
    }

    if (
        p_this->p_screen)
    {
        feed_screen_destroy(
            p_this->p_screen);

        p_this->p_screen =
            (struct feed_screen *)(
                0);
    }

    if (
        p_this->p_tty)
    {
        feed_tty_destroy(
            p_this->p_tty);

        p_this->p_tty =
            (struct feed_tty *)(
                0);
    }

    if (
        p_this->p_input)
    {
        feed_input_destroy(
            p_this->p_input);

        p_this->p_input =
            (struct feed_input *)(
                0);
    }

    if (
        p_this->p_prompt)
    {
        feed_prompt_destroy(
            p_this->p_prompt);

        p_this->p_prompt =
            (struct feed_prompt *)(
                0);
    }

    if (
        p_this->p_client)
    {
        feed_client_cleanup(
            p_this->p_client);

        p_this->p_client =
            (struct feed_client *)(
                0);
    }

} /* feed_cleanup() */

/*

Function: feed_create

Description:

    Create a feed handle and register application callbacks.  Initialize
    all objects used by feed library.

*/
struct feed_handle *
feed_create(
    struct feed_descriptor const * const
        p_feed_descriptor)
{
    struct feed_handle *
        p_this;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_heap_create();

    if (
        p_heap)
    {
        p_this =
            (struct feed_handle *)(
                feed_heap_alloc(
                    p_heap,
                    sizeof(
                        struct feed_handle)));

        if (
            p_this)
        {
            if (
                feed_init(
                    p_this,
                    p_heap,
                    p_feed_descriptor))
            {
            }
            else
            {
                feed_heap_free(
                    p_heap,
                    (void *)(
                        p_this));

                p_this =
                    (struct feed_handle *)(
                        0);
            }
        }

        if (
            !(p_this))
        {
            feed_heap_destroy(
                p_heap);
        }
    }

    return
        p_this;

} /* feed_create() */

void
feed_destroy(
    struct feed_handle * const
        p_this)
{
    if (
        p_this)
    {
        struct feed_heap *
            p_heap;

        p_heap =
            p_this->p_heap;

        if (
            p_heap)
        {
            feed_cleanup(
                p_this);

            p_this->p_heap =
                (struct feed_heap *)(
                    0);

            feed_heap_free(
                p_heap,
                p_this);

            feed_heap_destroy(
                p_heap);
        }
    }

} /* feed_destroy() */

int
feed_prompt1(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    if (
        p_this)
    {
        if (
            feed_prompt_set1(
                p_this->p_prompt,
                p_data,
                i_data_length))
        {
            i_result =
                0;
        }
        else
        {
            i_result =
                -1;
        }
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_prompt1() */

int
feed_prompt2(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    if (
        p_this)
    {
        if (
            feed_prompt_set2(
                p_this->p_prompt,
                p_data,
                i_data_length))
        {
            i_result =
                0;
        }
        else
        {
            i_result =
                -1;
        }
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_prompt2() */

int
feed_load(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    if (
        feed_text_load(
            p_this->p_text,
            p_data,
            i_data_length))
    {
        i_result =
            0;
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_load() */

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
struct feed_main_iterator
{
    /* Pointer to documents line */
    struct feed_line *
        p_document_line;

    /* Pointer to glyph object */
    struct feed_glyph *
        p_document_glyph;

    /* Pointer to prompt line */
    struct feed_line *
        p_prompt_line;

    /* Pointer to prompt glyph */
    struct feed_glyph *
        p_prompt_glyph;

    /* Position in screen */
    struct feed_screen_iterator
        o_screen_iterator;

    /* Position in document (x is valid if not in prompt) */
    unsigned long int
        i_glyph_index;

    unsigned long int
        i_line_index;

    enum feed_main_state
        e_state;

    unsigned int
        ui_padding[3u];

};

static
void
feed_main_iterator_get_prompt_line(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator)
{
    if (0 == p_iterator->i_line_index)
    {
        p_iterator->p_prompt_line =
            feed_prompt_get1(
                p_this->p_prompt);
    }
    else
    {
        p_iterator->p_prompt_line =
            feed_prompt_get2(
                p_this->p_prompt);
    }
}

static
void
feed_main_iterator_get_prompt_glyph(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator)
{
    (void)(
        p_this);

    if (p_iterator->p_prompt_line)
    {
        p_iterator->p_prompt_glyph =
            feed_line_get_glyph(
                p_iterator->p_prompt_line,
                p_iterator->i_glyph_index);
    }
    else
    {
        p_iterator->p_prompt_glyph =
            NULL;
    }
}

static
void
feed_main_iterator_get_glyph(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator)
{
    if (p_iterator->p_document_line)
    {
        if (feed_main_state_prompt == p_iterator->e_state)
        {
            feed_main_iterator_get_prompt_line(
                p_this,
                p_iterator);

            feed_main_iterator_get_prompt_glyph(
                p_this,
                p_iterator);

            if (p_iterator->p_prompt_glyph)
            {
            }
            else
            {
                p_iterator->i_glyph_index =
                    0u;

                p_iterator->e_state =
                    feed_main_state_text;

            }
        }

        if (feed_main_state_text == p_iterator->e_state)
        {
            p_iterator->p_document_glyph =
                feed_line_get_glyph(
                    p_iterator->p_document_line,
                    p_iterator->i_glyph_index);

            if (p_iterator->p_document_glyph)
            {
            }
            else
            {
                p_iterator->e_state = feed_main_state_eol;
            }
        }

        if (feed_main_state_eol == p_iterator->e_state)
        {
            if (p_iterator->p_document_line->o_list.p_next != &(p_this->p_text->o_lines))
            {
            }
            else
            {
                p_iterator->e_state = feed_main_state_eof;
            }
        }

        if (feed_main_state_eof == p_iterator->e_state)
        {
            p_iterator->p_document_line = NULL;
        }
    }
    else
    {
        p_iterator->i_line_index =
            p_this->p_text->i_line_count;

        p_iterator->i_glyph_index =
            0u;

        p_iterator->p_prompt_line =
            NULL;

        p_iterator->p_prompt_glyph =
            NULL;

        p_iterator->p_document_glyph =
            NULL;

        p_iterator->e_state =
            feed_main_state_null;
    }
}

static
char
feed_main_iterator_begin(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator,
    struct feed_line * const
        p_page_line,
    unsigned long int const
        i_page_line_index,
    unsigned long int const
        i_page_glyph_index,
    enum feed_main_state const
        e_page_state)
{
    p_iterator->i_line_index =
        i_page_line_index;

    p_iterator->i_glyph_index =
        i_page_glyph_index;

    if (feed_main_state_prompt == e_page_state)
    {
        p_iterator->e_state =
            feed_main_state_prompt;
    }
    else
    {
        p_iterator->e_state =
            feed_main_state_text;
    }

    p_iterator->p_prompt_glyph =
        NULL;

    p_iterator->p_document_glyph =
        NULL;

    if (p_page_line)
    {
        p_iterator->p_document_line =
            p_page_line;
    }
    else
    {
        p_iterator->p_document_line =
            feed_text_get_line(
                p_this->p_text,
                p_iterator->i_line_index);
    }

    feed_main_iterator_get_glyph(
        p_this,
        p_iterator);

    feed_screen_iterator_init(
        &(
            p_this->o_screen_info),
        &(
            p_iterator->o_screen_iterator));

    return
        1;

}

/*

    TEXT PROMPT EOL
    TEXT PROMPT EOF NULL

*/
static
char
feed_main_iterator_begin_reverse(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator,
    struct feed_line * const
        p_page_line,
    unsigned long int const
        i_page_line_index,
    unsigned long int const
        i_page_glyph_index,
    enum feed_main_state const
        e_page_state)
{
    p_iterator->i_line_index =
        i_page_line_index;

    p_iterator->i_glyph_index =
        i_page_glyph_index;

    if (feed_main_state_prompt == e_page_state)
    {
        p_iterator->e_state =
            feed_main_state_prompt;
    }
    else
    {
        p_iterator->e_state =
            feed_main_state_text;
    }

    p_iterator->p_prompt_glyph =
        NULL;

    p_iterator->p_prompt_line =
        NULL;

    p_iterator->p_document_glyph =
        NULL;

    p_iterator->p_document_line =
        NULL;

    if (p_page_line)
    {
        p_iterator->p_document_line =
            p_page_line;
    }
    else
    {
        p_iterator->p_document_line =
            feed_text_get_line(
                p_this->p_text,
                p_iterator->i_line_index);
    }

    if (!(p_iterator->p_document_line))
    {
        /* we are at end of file */
        if (p_this->p_text->i_line_count)
        {
            p_iterator->i_line_index =
                (p_this->p_text->i_line_count - 1ul);

            p_iterator->p_document_line =
                feed_text_get_line(
                    p_this->p_text,
                    p_iterator->i_line_index);

            if (p_iterator->p_document_line)
            {
                if (p_iterator->p_document_line->i_glyph_count)
                {
                    p_iterator->i_glyph_index =
                        (p_iterator->p_document_line->i_glyph_count - 1ul);

                    p_iterator->e_state =
                        feed_main_state_text;
                }
                else
                {
                    p_iterator->e_state =
                        feed_main_state_eol;
                }
            }
            else
            {
                p_iterator->e_state =
                    feed_main_state_eol;
            }
        }
        else
        {
            p_iterator->e_state =
                feed_main_state_eof;
        }
    }

    if (p_iterator->p_document_line)
    {
        if (feed_main_state_text == p_iterator->e_state)
        {
            p_iterator->p_document_glyph =
                feed_line_get_glyph(
                    p_iterator->p_document_line,
                    p_iterator->i_glyph_index);

            if (p_iterator->p_document_glyph)
            {
            }
            else
            {
                p_iterator->e_state =
                    feed_main_state_prompt;
            }
        }

        if (feed_main_state_prompt == p_iterator->e_state)
        {
            feed_main_iterator_get_prompt_line(
                p_this,
                p_iterator);

            feed_main_iterator_get_prompt_glyph(
                p_this,
                p_iterator);

            if (p_iterator->p_prompt_glyph)
            {
            }
            else
            {
                p_iterator->e_state =
                    feed_main_state_eol;
            }
        }

        if (feed_main_state_eol == p_iterator->e_state)
        {
            if (p_iterator->p_document_line->o_list.p_prev != &(p_this->p_text->o_lines))
            {
            }
            else
            {
                p_iterator->e_state =
                    feed_main_state_eof;
            }
        }

        if (feed_main_state_eof == p_iterator->e_state)
        {
            p_iterator->p_document_line =
                NULL;
        }
    }
    else
    {
        p_iterator->i_line_index =
            p_this->p_text->i_line_count;

        p_iterator->i_glyph_index =
            0u;

        p_iterator->p_prompt_line =
            NULL;

        p_iterator->p_prompt_glyph =
            NULL;

        p_iterator->p_document_glyph =
            NULL;

        p_iterator->e_state =
            feed_main_state_null;
    }

    feed_screen_iterator_init(
        &(
            p_this->o_screen_info),
        &(
            p_iterator->o_screen_iterator));

    return
        1;

}

static
char
feed_main_iterator_test(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator)
{
    char
        b_result;

    if ((p_iterator->e_state == feed_main_state_prompt)
        || (p_iterator->e_state == feed_main_state_text))
    {
        unsigned short int i_glyph_width;

        i_glyph_width =
            feed_glyph_get_visible_width(
                p_iterator->p_prompt_glyph
                ? p_iterator->p_prompt_glyph
                : p_iterator->p_document_glyph);

        b_result =
            feed_screen_iterator_test_write(
                &(
                    p_this->o_screen_info),
                &(
                    p_iterator->o_screen_iterator),
                i_glyph_width);
    }
    else if (p_iterator->e_state == feed_main_state_eol)
    {
        b_result =
            feed_screen_iterator_test_newline(
                &(
                    p_this->o_screen_info),
                &(
                    p_iterator->o_screen_iterator));
    }
    else
    {
        b_result =
            feed_screen_iterator_test_clear(
                &(
                    p_this->o_screen_info),
                &(
                    p_iterator->o_screen_iterator));
    }

    return
        b_result;

}

static
unsigned long int
feed_main_iterator_write(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator)
{
    unsigned long int
        i_width;

    /* Advance screen position */
    if ((p_iterator->e_state == feed_main_state_prompt)
        || (p_iterator->e_state == feed_main_state_text))
    {
        i_width =
            feed_glyph_get_visible_width(
                p_iterator->p_prompt_glyph
                ? p_iterator->p_prompt_glyph
                : p_iterator->p_document_glyph);

        feed_screen_iterator_write(
            &(
                p_this->o_screen_info),
            &(
                p_iterator->o_screen_iterator),
            i_width);
    }
    else if (p_iterator->e_state == feed_main_state_eol)
    {
        /* Empty line, so do a newline */
        i_width =
            feed_screen_iterator_newline(
                &(
                    p_this->o_screen_info),
                &(
                    p_iterator->o_screen_iterator));
    }
    else
    {
        i_width =
            feed_screen_iterator_clear(
                &(
                    p_this->o_screen_info),
                &(
                    p_iterator->o_screen_iterator));
    }

    return
        i_width;

}

static
char
feed_main_iterator_next(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator)
{
    char
        b_result;

    if (p_iterator->p_document_line)
    {
        /* Look for next glyph */
        if (p_iterator->p_prompt_glyph)
        {
            p_iterator->i_glyph_index ++;

            if (p_iterator->p_prompt_glyph->o_list.p_next != &(p_iterator->p_prompt_line->o_glyphs))
            {
                p_iterator->p_prompt_glyph =
                    (struct feed_glyph *)(
                        p_iterator->p_prompt_glyph->o_list.p_next);
            }
            else
            {
                /* Next line in prompt? */
                p_iterator->p_prompt_glyph = NULL;

                p_iterator->i_glyph_index = 0u;

                p_iterator->e_state = feed_main_state_text;

                feed_main_iterator_get_glyph(
                    p_this,
                    p_iterator);
            }
        }
        else if (p_iterator->p_document_glyph)
        {
            p_iterator->i_glyph_index ++;

            if (p_iterator->p_document_glyph->o_list.p_next != &(p_iterator->p_document_line->o_glyphs))
            {
                p_iterator->p_document_glyph =
                    (struct feed_glyph *)(
                        p_iterator->p_document_glyph->o_list.p_next);
            }
            else
            {
                /* Next line in document */
                p_iterator->p_document_glyph = NULL;

                p_iterator->e_state = feed_main_state_eol;

                feed_main_iterator_get_glyph(
                    p_this,
                    p_iterator);
            }
        }
        else if (p_iterator->p_document_line)
        {
            /* Go to next line */
            if (p_iterator->p_document_line->o_list.p_next != &(p_this->p_text->o_lines))
            {
                p_iterator->i_glyph_index = 0u;

                p_iterator->i_line_index ++;

                p_iterator->p_document_line =
                    (struct feed_line *)(
                        p_iterator->p_document_line->o_list.p_next);

                /* Switch to prompt mode */
                p_iterator->e_state = feed_main_state_prompt;

                feed_main_iterator_get_glyph(
                    p_this,
                    p_iterator);
            }
            else
            {
                /* End of file */
                p_iterator->p_document_line = NULL;

                p_iterator->e_state = feed_main_state_eof;

                feed_main_iterator_get_glyph(
                    p_this,
                    p_iterator);
            }
        }
        else
        {
            /* no more lines... */
        }

        b_result =
            1;
    }
    else
    {
        feed_main_iterator_get_glyph(
            p_this,
            p_iterator);

        b_result =
            0;
    }

    return
        b_result;

}

/*

Function: feed_main_iterator_prev

Description:

    Move to previous character in document and in screen.

Comments:

    TEXT PROMPT EOL
    TEXT PROMPT EOF NULL

*/
static
char
feed_main_iterator_prev(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator)
{
    char
        b_result;

    if (p_iterator->p_document_line)
    {
        /* Look for next glyph */
        if (p_iterator->p_prompt_glyph)
        {
            if (p_iterator->p_prompt_glyph->o_list.p_prev != &(p_iterator->p_prompt_line->o_glyphs))
            {
                p_iterator->i_glyph_index --;

                p_iterator->p_prompt_glyph =
                    (struct feed_glyph *)(
                        p_iterator->p_prompt_glyph->o_list.p_prev);

                p_iterator->e_state = feed_main_state_prompt;
            }
            else
            {
                /* Next line in prompt? */
                p_iterator->p_prompt_glyph = NULL;

                /* Go to previous line in document */
                if (p_iterator->p_document_line->o_list.p_prev != &(p_this->p_text->o_lines))
                {
                    p_iterator->p_document_line =
                        (struct feed_line *)(
                            p_iterator->p_document_line->o_list.p_prev);

                    p_iterator->i_line_index --;

                    p_iterator->p_document_glyph = NULL;

                    p_iterator->e_state = feed_main_state_eol;
                }
                else
                {
                    p_iterator->p_document_line = NULL;

                    p_iterator->p_document_glyph = NULL;

                    p_iterator->e_state = feed_main_state_eof;
                }
            }
        }
        else if (p_iterator->p_document_glyph)
        {
            if (p_iterator->p_document_glyph->o_list.p_prev != &(p_iterator->p_document_line->o_glyphs))
            {
                p_iterator->i_glyph_index --;

                p_iterator->p_document_glyph =
                    (struct feed_glyph *)(
                        p_iterator->p_document_glyph->o_list.p_prev);

                p_iterator->e_state = feed_main_state_text;
            }
            else
            {
                p_iterator->p_document_glyph = NULL;

                feed_main_iterator_get_prompt_line(
                    p_this,
                    p_iterator);

                if (p_iterator->p_prompt_line
                    && p_iterator->p_prompt_line->i_glyph_count)
                {
                    p_iterator->i_glyph_index =
                        p_iterator->p_prompt_line->i_glyph_count - 1u;

                    p_iterator->p_prompt_glyph =
                        feed_line_get_glyph(
                            p_iterator->p_prompt_line,
                            p_iterator->i_glyph_index);

                    p_iterator->e_state = feed_main_state_prompt;
                }
                else
                {
                    p_iterator->p_prompt_glyph =
                        NULL;

                    /* Go to previous line in document */
                    if (p_iterator->p_document_line->o_list.p_prev != &(p_this->p_text->o_lines))
                    {
                        p_iterator->p_document_line =
                            (struct feed_line *)(
                                p_iterator->p_document_line->o_list.p_prev);

                        p_iterator->i_line_index --;

                        p_iterator->p_document_glyph = NULL;

                        p_iterator->e_state = feed_main_state_eol;
                    }
                    else
                    {
                        p_iterator->p_document_line = NULL;

                        p_iterator->p_document_glyph = NULL;

                        p_iterator->e_state = feed_main_state_eof;
                    }
                }
            }
        }
        else if (p_iterator->p_document_line)
        {
            if (p_iterator->p_document_line->i_glyph_count)
            {
                p_iterator->i_glyph_index =
                    p_iterator->p_document_line->i_glyph_count - 1u;

                p_iterator->p_document_glyph =
                    feed_line_get_glyph(
                        p_iterator->p_document_line,
                        p_iterator->i_glyph_index);

                p_iterator->e_state =
                    feed_main_state_text;
            }
            else
            {
                p_iterator->p_document_glyph = NULL;

                if (0 == p_iterator->i_line_index)
                {
                    p_iterator->p_prompt_line =
                        feed_prompt_get1(
                            p_this->p_prompt);
                }
                else
                {
                    p_iterator->p_prompt_line =
                        feed_prompt_get2(
                            p_this->p_prompt);
                }

                if (p_iterator->p_prompt_line
                    && p_iterator->p_prompt_line->i_glyph_count)
                {
                    p_iterator->i_glyph_index =
                        p_iterator->p_prompt_line->i_glyph_count - 1u;

                    p_iterator->p_prompt_glyph =
                        feed_line_get_glyph(
                            p_iterator->p_prompt_line,
                            p_iterator->i_glyph_index);

                    p_iterator->e_state = feed_main_state_prompt;
                }
                else
                {
                    p_iterator->p_prompt_glyph =
                        NULL;

                    /* Go to previous line in document */
                    if (p_iterator->p_document_line->o_list.p_prev != &(p_this->p_text->o_lines))
                    {
                        p_iterator->p_document_line =
                            (struct feed_line *)(
                                p_iterator->p_document_line->o_list.p_prev);

                        p_iterator->i_line_index --;

                        p_iterator->p_document_glyph = NULL;

                        p_iterator->e_state = feed_main_state_eol;
                    }
                    else
                    {
                        p_iterator->p_document_line = NULL;

                        p_iterator->p_document_glyph = NULL;

                        p_iterator->e_state = feed_main_state_eof;
                    }
                }
            }
        }
        else
        {
            /* no more lines... */
        }

        b_result =
            1;
    }
    else
    {
        p_iterator->e_state = feed_main_state_null;

        b_result =
            0;
    }

    return
        b_result;

}

static
char
feed_main_look_xy(
    struct feed_handle * const
        p_this,
    unsigned long int const
        i_cursor_address)
{
    char
        b_found;

    struct feed_main_iterator
        o_iterator;

    b_found =
        0;

    if (
        feed_main_iterator_begin(
            p_this,
            &(o_iterator),
            p_this->p_page_line,
            p_this->i_page_line_index,
            p_this->i_page_glyph_index,
            p_this->e_page_state))
    {
        char
            b_more;

        b_more =
            1;

        while (
            b_more
            && (!b_found))
        {
            if (
                feed_main_iterator_test(
                    p_this,
                    &(o_iterator)))
            {
                /* Locate a glyph that is at same coord as cursor */
                if (
                    (o_iterator.o_screen_iterator.i_cursor_address >= i_cursor_address)
                    && (o_iterator.e_state != feed_main_state_prompt))
                {
                    p_this->o_cursor.i_line_index =
                        o_iterator.i_line_index;

                    p_this->o_cursor.p_line =
                        o_iterator.p_document_line;

                    p_this->o_cursor.i_glyph_index =
                        o_iterator.i_glyph_index;

                    p_this->o_cursor.p_glyph =
                        o_iterator.p_document_glyph;

                    b_found =
                        1;
                }
                else
                {
                    feed_main_iterator_write(
                        p_this,
                        &(o_iterator));

                    if (
                        (o_iterator.o_screen_iterator.i_cursor_address > i_cursor_address)
                        && ((o_iterator.e_state == feed_main_state_eol)
                            || (o_iterator.e_state == feed_main_state_eof)))
                    {
                        p_this->o_cursor.i_line_index =
                            o_iterator.i_line_index;

                        p_this->o_cursor.p_line =
                            o_iterator.p_document_line;

                        p_this->o_cursor.i_glyph_index =
                            o_iterator.i_glyph_index;

                        p_this->o_cursor.p_glyph =
                            o_iterator.p_document_glyph;

                        b_found =
                            1;
                    }
                    else
                    {
                        b_more =
                            feed_main_iterator_next(
                                p_this,
                                &(
                                    o_iterator));
                    }
                }
            }
            else
            {
                b_more =
                    0;
            }
        }
    }

    return b_found;
}

static
char
feed_main_latch_next_page(
    struct feed_handle * const
        p_this)
{
    char
        b_result;

    if (p_this->e_final_state != feed_main_state_null)
    {
        p_this->p_page_line = NULL;

        p_this->i_page_line_index = p_this->i_final_line_index;

        p_this->i_page_glyph_index = p_this->i_final_glyph_index;

        if (p_this->e_final_state == feed_main_state_prompt)
        {
            p_this->e_page_state = feed_main_state_prompt;
        }
        else
        {
            p_this->e_page_state = feed_main_state_text;
        }

        b_result =
            1;

    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

static
char
feed_main_look_down(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    if (
        p_this->b_cursor_visible)
    {
        if (p_this->i_cursor_visible_y < (p_this->o_screen_info.i_screen_height - 1u))
        {
            b_found =
                feed_main_look_xy(
                    p_this,
                    ((unsigned long int)(p_this->i_cursor_visible_y) + 1ul) * (unsigned long int)(p_this->o_screen_info.i_screen_width)
                    + (unsigned long int)(p_this->i_cursor_visible_x));
        }
        else
        {
            if (feed_main_latch_next_page(p_this))
            {
                feed_main_look_xy(
                    p_this,
                    p_this->i_cursor_visible_x);

                b_found =
                    0;
            }
            else
            {
                b_found =
                    1;
            }

        }
    }

    return b_found;
}

static
char
feed_main_scroll_pageup(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    struct feed_main_iterator
        o_iterator;

    if (
        feed_main_iterator_begin_reverse(
            p_this,
            &(o_iterator),
            p_this->p_page_line,
            p_this->i_page_line_index,
            p_this->i_page_glyph_index,
            p_this->e_page_state))
    {
        char
            b_more;

        char
            b_reach_top;

        struct feed_line *
            p_final_line;

        unsigned long int
            i_final_line_index;

        unsigned long int
            i_final_glyph_index;

        enum feed_main_state
            e_final_state;

        b_more =
            1;

        b_reach_top =
            0;

        if (0u == o_iterator.i_glyph_index)
        {
            feed_main_iterator_prev(
                p_this,
                &(
                    o_iterator));
        }

        while (
            b_more
            && (!b_reach_top))
        {
            if (
                feed_main_iterator_test(
                    p_this,
                    &(o_iterator)))
            {
                /* Remember this as a valid glyph */
                p_final_line =
                    o_iterator.p_document_line;

                i_final_line_index =
                    o_iterator.i_line_index;

                i_final_glyph_index =
                    o_iterator.i_glyph_index;

                e_final_state =
                    o_iterator.e_state;

                b_found =
                    1;

                feed_main_iterator_write(
                    p_this,
                    &(o_iterator));

                if (
                    feed_main_iterator_prev(
                        p_this,
                        &(
                            o_iterator)))
                {
                }
                else
                {
                    b_reach_top =
                        1;

                    b_more =
                        0;
                }
            }
            else
            {
                b_more =
                    0;
            }
        }

        if (b_found)
        {
            p_this->p_page_line =
                p_final_line;

            p_this->i_page_line_index =
                i_final_line_index;

            if ((e_final_state == feed_main_state_eol)
                || (e_final_state == feed_main_state_eof))
            {
                p_this->i_page_glyph_index =
                    0u;

                p_this->e_page_state =
                    feed_main_state_prompt;
            }
            else
            {
                p_this->i_page_glyph_index =
                    i_final_glyph_index;

                p_this->e_page_state =
                    e_final_state;
            }

            if (b_reach_top)
            {
                b_found =
                    0;
            }
        }
    }
    else
    {
        b_found =
            0;
    }

    return
        b_found;

}

static
char
feed_main_look_up(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    if (
        p_this->b_cursor_visible)
    {
        /* try a misaligned up in same visible page */
        if (p_this->i_cursor_visible_y)
        {
            b_found =
                feed_main_look_xy(
                    p_this,
                    (p_this->i_cursor_visible_y - 1u) * p_this->o_screen_info.i_screen_width
                    + p_this->i_cursor_visible_x);
        }
        else
        {
            if (p_this->i_page_line_index
                || p_this->i_page_glyph_index)
            {
                if (
                    feed_main_scroll_pageup(
                        p_this))
                {
                    /* up with aligned page */
                    feed_main_look_xy(
                        p_this,
                        (p_this->o_screen_info.i_screen_height - 1u) * p_this->o_screen_info.i_screen_width
                        + p_this->i_cursor_visible_x);

                }

                b_found =
                    0;
            }
        }
    }
    else
    {
        b_found =
            1;
    }

    return b_found;
}

/*

Function: feed_main_look_pageup

Description:

    Search for page that is previous to page which holds cursor.

Comments:

    -   Problem may occur when current page is not aligned on a page
        boundary.  Perhaps first step is to align the current page,
        then to search for previous page.

    -   Concept of x,y relative to page is not valid when page is not
        aligned.

    -   We need to remember last N lines so that we may scroll by one
        line at a time.

            [C-N] line_index, glyph_index, state
            [C-1] line_index, glyph_index, state
            [C-0] etc.

    -   At each iteration, if screen y changes, then add a new entry into
        the cached line table.

*/
static
char
feed_main_look_pageup(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    if (
        p_this->b_cursor_visible)
    {
        if (
            feed_main_scroll_pageup(
               p_this))
        {
            /* Find cursor at same position */
            b_found =
                feed_main_look_xy(
                    p_this,
                    p_this->i_cursor_visible_y * (unsigned long int)(p_this->o_screen_info.i_screen_width)
                    + p_this->i_cursor_visible_x);
        }
        else
        {
            b_found =
                0;
        }
    }
    else
    {
        b_found =
            0;
    }

    return b_found;
}

static
char
feed_main_look_pagedown(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    if (
        p_this->b_cursor_visible)
    {
        if (feed_main_latch_next_page(p_this))
        {
            feed_main_look_xy(
                p_this,
                (unsigned long int)(p_this->i_cursor_visible_y) * (unsigned long int)(p_this->o_screen_info.i_screen_width)
                + (unsigned long int)(p_this->i_cursor_visible_x));

            b_found =
                0;
        }
        else
        {
            b_found =
                1;
        }
    }
    else
    {
        b_found =
            1;
    }

    return b_found;
}

static
void
feed_main_update_screen_info(
    struct feed_handle * const
        p_this,
    char const
        b_fallback)
{
    unsigned short int
        i_screen_width;

    unsigned short int
        i_screen_height;

    if (
        feed_tty_get_window_size(
            p_this->p_tty,
            &(
                i_screen_width),
            &(
                i_screen_height),
            b_fallback))
    {
        /* Todo: detect screen size change and invalidate current
        page information */

#if 0
        if (i_screen_height > 13u)
        {
            i_screen_height = 13u;
        }

        if (i_screen_width > 50u)
        {
            i_screen_width = 50u;
        }
#endif

        feed_screen_info_update(
            &(
                p_this->o_screen_info),
            i_screen_width,
            i_screen_height);

        feed_screen_set_physical_size(
            p_this->p_screen,
            i_screen_width,
            i_screen_height);
    }

}

static
void
feed_main_refresh_job(
    struct feed_handle * const
        p_this,
    char const
        b_refresh_text,
    char const
        b_refresh_cursor)
{
    struct feed_main_iterator
        o_iterator;

    p_this->b_cursor_visible =
        0;

    p_this->i_cursor_visible_x =
        0u;

    p_this->i_cursor_visible_y =
        0u;

    if (b_refresh_text)
    {
        feed_main_update_screen_info(
            p_this,
            0);

        feed_screen_set_cursor_pos(
            p_this->p_screen,
            0u,
            0u);
    }

    if (!(p_this->p_page_line))
    {
        p_this->p_page_line =
            feed_text_get_line(
                p_this->p_text,
                p_this->i_page_line_index);
    }

    if (
        feed_main_iterator_begin(
            p_this,
            &(o_iterator),
            p_this->p_page_line,
            p_this->i_page_line_index,
            p_this->i_page_glyph_index,
            p_this->e_page_state))
    {
        char
            b_more;

        b_more =
            1;

        while (
            b_more)
        {
            if (
                feed_main_iterator_test(
                    p_this,
                    &(o_iterator)))
            {
                /* Detect if cursor is visible */
                if (
                    !(
                        p_this->b_cursor_visible)
                    && (
                        p_this->o_cursor.i_line_index == o_iterator.i_line_index)
                    && (
                        p_this->o_cursor.i_glyph_index == o_iterator.i_glyph_index)
                    && (
                        (o_iterator.e_state != feed_main_state_prompt)))
                {
                    p_this->i_cursor_visible_x =
                        feed_screen_iterator_get_cursor_x(
                            &(
                                p_this->o_screen_info),
                            &(
                                o_iterator.o_screen_iterator));

                    p_this->i_cursor_visible_y =
                        feed_screen_iterator_get_cursor_y(
                            &(
                                p_this->o_screen_info),
                            &(
                                o_iterator.o_screen_iterator));

                    p_this->b_cursor_visible =
                        1;
                }

                if (
                    o_iterator.p_prompt_glyph || o_iterator.p_document_glyph)
                {
                    if (b_refresh_text)
                    {
                        unsigned char
                            a_visible[15u];

                        unsigned char
                            i_visible_length;

                        i_visible_length =
                            feed_glyph_render_visible(
                                o_iterator.p_prompt_glyph
                                ? o_iterator.p_prompt_glyph
                                : o_iterator.p_document_glyph,
                                a_visible);

                        feed_screen_write(
                            p_this->p_screen,
                            a_visible,
                            i_visible_length);
                    }
                }
                else if (
                    o_iterator.p_document_line)
                {
                    if (b_refresh_text)
                    {
                        feed_screen_newline(
                            p_this->p_screen);
                    }
                }
                else
                {
                    /* todo: call clear region */
                    if (b_refresh_text)
                    {
                        feed_screen_clear_region(
                            p_this->p_screen);
                    }
                }

                if (b_more)
                {
                    feed_main_iterator_write(
                        p_this,
                        &(o_iterator));

                    if (
                        feed_main_iterator_next(
                            p_this,
                            &(
                                o_iterator)))
                    {
                    }
                    else
                    {
                        b_more =
                            0;
                    }
                }
            }
            else
            {
                b_more =
                    0;
            }
        }

        p_this->i_final_line_index =
            o_iterator.i_line_index;

        p_this->i_final_glyph_index =
            o_iterator.i_glyph_index;

        p_this->e_final_state =
            o_iterator.e_state;

        if (b_refresh_text)
        {
            feed_screen_get_cursor_pos(
                p_this->p_screen,
                &(
                    p_this->i_final_cursor_x),
                &(
                    p_this->i_final_cursor_y));
        }

        if (b_refresh_text || b_refresh_cursor)
        {
            if (p_this->b_cursor_visible)
            {
                feed_screen_set_cursor_pos(
                    p_this->p_screen,
                    p_this->i_cursor_visible_x,
                    p_this->i_cursor_visible_y);
            }

            feed_tty_flush(
                p_this->p_tty);
        }
    }
}

static
void
feed_main_refresh_text(
    struct feed_handle * const
        p_this)
{
    feed_main_refresh_job(
        p_this,
        1,
        1);
}

static
void
feed_main_refresh_info(
    struct feed_handle * const
        p_this)
{
    feed_main_refresh_job(
        p_this,
        0,
        0);
}

static
void
feed_main_insert_event(
    struct feed_handle * const
        p_this,
    struct feed_event const * const
        p_event)
{
    struct feed_utf8_parser
        o_utf8_parser;

    if (
        feed_utf8_parser_init(
            &(
                o_utf8_parser)))
    {
        unsigned char
            i_raw_iterator;

        for (
            i_raw_iterator = 0u;
            i_raw_iterator < p_event->i_raw_len;
            i_raw_iterator ++)
        {
            struct feed_utf8_code
                o_utf8_code;

            if (
                0 < feed_utf8_parser_write(
                    &(
                        o_utf8_parser),
                    p_event->a_raw[i_raw_iterator],
                    &(
                        o_utf8_code)))
            {
                struct feed_glyph *
                    p_glyph;

                p_glyph =
                    feed_glyph_create(
                        p_this->p_client,
                        &(
                            o_utf8_code));

                if (
                    p_glyph)
                {
                    feed_text_iterator_write_glyph(
                        p_this->p_text,
                        &(
                            p_this->o_cursor),
                        p_glyph);
                }
            }
        }

        feed_utf8_parser_cleanup(
            &(
                o_utf8_parser));
    }
}

static
void
feed_main_detect_visible_cursor(
    struct feed_handle * const
        p_this)
{
    p_this->b_cursor_visible = 0;

    if (p_this->o_cursor.i_line_index < p_this->i_page_line_index)
    {
    }
    else if (p_this->o_cursor.i_line_index == p_this->i_page_line_index)
    {
        if ((p_this->o_cursor.i_glyph_index < p_this->i_page_glyph_index)
            && (feed_main_state_text == p_this->e_page_state))
        {
        }
        else
        {
            p_this->b_cursor_visible = 1;
        }
    }
    else
    {
        p_this->b_cursor_visible = 1;
    }

    if (p_this->b_cursor_visible)
    {
        if (p_this->o_cursor.i_line_index < p_this->i_final_line_index)
        {
        }
        else if (p_this->o_cursor.i_line_index == p_this->i_final_line_index)
        {
            if ((p_this->o_cursor.i_glyph_index < p_this->i_final_glyph_index)
                && (p_this->e_final_state == feed_main_state_text))
            {
            }
            else
            {
                p_this->b_cursor_visible = 0;
            }
        }
        else
        {
            p_this->b_cursor_visible = 0;
        }
    }
}

static
void
feed_main_event_callback(
    void * const
        p_context,
    struct feed_event const * const
        p_event)
{
    struct feed_handle *
        p_this;

    struct feed_text *
        p_text;

    p_this =
        (struct feed_handle *)(
            p_context);

    p_text =
        p_this->p_text;

    p_text->o_last_event =
        *(
            p_event);

    if (p_this->b_verbose)
    {
        p_this->b_verbose =
            0;

        /* Split event into unicode characters */
        feed_main_insert_event(
            p_this,
            p_event);

        /* Detect that cursor is still within page */

        feed_main_refresh_info(
            p_this);

        /* Detect that cursor is visible? */
        if (p_this->b_cursor_visible)
        {
        }
        else
        {
            feed_main_latch_next_page(p_this);
        }

        feed_main_refresh_text(
            p_this);
    }
    else
    {
        /* ((unsigned long int)(unsigned char)('q') == p_event->i_code) */
        if ((FEED_EVENT_KEY_FLAG | FEED_EVENT_KEY_CTRL | 'D') == p_event->i_code)
        {
            p_this->b_started =
                0;
        }
        else
        {
            char
                b_refresh_text;

            char
                b_refresh_cursor;

            b_refresh_text =
                1;

            b_refresh_cursor =
                0;

            p_text->o_last_event =
                *(
                    p_event);

            if ((FEED_EVENT_KEY_FLAG | FEED_EVENT_KEY_CTRL | 'H') == p_event->i_code)
            {
                if (p_this->o_cursor.i_line_index
                    || p_this->o_cursor.i_glyph_index)
                {
                    if (
                        feed_text_iterator_prev_glyph(
                            p_this->p_text,
                            &(
                                p_this->o_cursor)))
                    {
                    }
                    else
                    {
                        if (
                            feed_text_iterator_prev_line(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor)))
                        {
                            feed_text_iterator_end_glyph(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor));
                        }
                    }

                    /* TODO: detect that cursor is still visible */
                    feed_main_detect_visible_cursor(
                        p_this);

                    /* Adjust visible cursor position */
                    if (p_this->b_cursor_visible)
                    {
                    }
                    else
                    {
                        feed_main_scroll_pageup(
                            p_this);
                    }

                    {
                        struct feed_glyph *
                            p_glyph;

                        p_glyph =
                            feed_text_iterator_remove_glyph(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor));

                        if (
                            p_glyph)
                        {
                            feed_glyph_destroy(
                                p_this->p_client,
                                p_glyph);
                        }
                        else
                        {
                            /* Bring next line into this line */
                            /* Delete next line */
                            feed_text_iterator_join_lines(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor));
                        }
                    }
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'L') == p_event->i_code)
            {
                b_refresh_text = 1;
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_DELETE) == p_event->i_code)
            {
                struct feed_glyph *
                    p_glyph;

                p_glyph =
                    feed_text_iterator_remove_glyph(
                        p_this->p_text,
                        &(
                            p_this->o_cursor));

                if (
                    p_glyph)
                {
                    feed_glyph_destroy(
                        p_this->p_client,
                        p_glyph);
                }
                else
                {
                    /* Bring next line into this line */
                    /* Delete next line */
                    feed_text_iterator_join_lines(
                        p_this->p_text,
                        &(
                            p_this->o_cursor));
                }
            }
            else if (
                ((FEED_EVENT_KEY_FLAG | FEED_KEY_HOME) == p_event->i_code)
                || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'A') == p_event->i_code))
            {
                feed_text_iterator_home_glyph(
                    p_this->p_text,
                    &(
                        p_this->o_cursor));

                /* detect that cursor is still visible? */
                feed_main_detect_visible_cursor(
                    p_this);

                if (p_this->b_cursor_visible)
                {
                    b_refresh_text = 0;

                    b_refresh_cursor = 1;
                }
                else
                {
                    feed_main_scroll_pageup(
                        p_this);
                }
            }
            else if (
                ((FEED_EVENT_KEY_FLAG | FEED_KEY_END) == p_event->i_code)
                || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'E') == p_event->i_code))
            {
                feed_text_iterator_end_glyph(
                    p_this->p_text,
                    &(
                        p_this->o_cursor));

                /* detect that cursor is still visible */
                feed_main_detect_visible_cursor(
                    p_this);

                if (p_this->b_cursor_visible)
                {
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
                else
                {
                    feed_main_latch_next_page(p_this);
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_PAGEUP) == p_event->i_code)
            {
                p_this->p_page_line = NULL;

                p_this->i_page_line_index = 0ul;

                p_this->i_page_glyph_index = 0ul;

                p_this->e_page_state = feed_main_state_prompt;

                feed_text_iterator_set_line(
                    p_this->p_text,
                    &(
                        p_this->o_cursor),
                    p_this->i_page_line_index);

                feed_text_iterator_set_glyph(
                    p_this->p_text,
                    &(
                        p_this->o_cursor),
                    p_this->i_page_glyph_index);
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_PAGEDOWN) == p_event->i_code)
            {
                p_this->p_page_line = NULL;

                p_this->i_page_line_index = p_this->p_text->i_line_count - 1u;

                p_this->i_page_glyph_index = 0ul;

                p_this->e_page_state = feed_main_state_prompt;

                feed_text_iterator_set_line(
                    p_this->p_text,
                    &(
                        p_this->o_cursor),
                    p_this->i_page_line_index);

                feed_text_iterator_set_glyph(
                    p_this->p_text,
                    &(
                        p_this->o_cursor),
                    p_this->i_page_glyph_index);
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_HOME) == p_event->i_code)
            {
                feed_main_look_xy(
                    p_this,
                    (unsigned long int)(p_this->i_cursor_visible_x));
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_END) == p_event->i_code)
            {
                feed_main_look_xy(
                    p_this,
                    (unsigned long int)(p_this->o_screen_info.i_screen_height - 1u) * (unsigned long int)(p_this->o_screen_info.i_screen_width)
                    + (unsigned long int)(p_this->i_cursor_visible_x));

            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_LEFT) == p_event->i_code)
            {
                if (
                    feed_text_iterator_prev_glyph(
                        p_this->p_text,
                        &(
                            p_this->o_cursor)))
                {
                }
                else
                {
                    if (
                        feed_text_iterator_prev_line(
                            p_this->p_text,
                            &(
                                p_this->o_cursor)))
                    {
                        feed_text_iterator_end_glyph(
                            p_this->p_text,
                            &(
                                p_this->o_cursor));
                    }
                }

                /* detect that cursor is still visible */
                feed_main_detect_visible_cursor(
                    p_this);

                if (p_this->b_cursor_visible)
                {
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
                else
                {
                    feed_main_scroll_pageup(
                        p_this);
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_RIGHT) == p_event->i_code)
            {
                if (feed_text_iterator_next_glyph(
                        p_this->p_text,
                        &(
                            p_this->o_cursor)))
                {
                }
                else
                {
                    if (feed_text_iterator_next_line(
                        p_this->p_text,
                        &(
                            p_this->o_cursor)))
                    {
                        feed_text_iterator_home_glyph(
                            p_this->p_text,
                            &(
                                p_this->o_cursor));
                    }
                }

                feed_main_detect_visible_cursor(
                    p_this);

                /* Adjust visible cursor position */
                if (p_this->b_cursor_visible)
                {
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
                else
                {
                    feed_main_latch_next_page(p_this);
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_UP) == p_event->i_code)
            {
                if (feed_main_look_up(p_this))
                {
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
                else
                {
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_DOWN) == p_event->i_code)
            {
                if (feed_main_look_down(p_this))
                {
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_PAGEUP) == p_event->i_code)
            {
                if (p_this->i_page_line_index
                    || p_this->i_page_glyph_index)
                {
                    if (feed_main_look_pageup(p_this))
                    {
                    }
                    else
                    {
                        /* reset to top of document? */
                    }
                }
                else
                {
                    b_refresh_text = 0;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_PAGEDOWN) == p_event->i_code)
            {
                if (feed_main_look_pagedown(p_this))
                {
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
                else
                {
                    /* reset to bottom of document? */
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_LEFT) == p_event->i_code)
            {
                /* Go left until reach begin of word */
                char
                    b_found;

                b_found =
                    0;

                while (
                    !b_found)
                {
                    if (
                        feed_text_iterator_prev_glyph(
                            p_this->p_text,
                            &(
                                p_this->o_cursor)))
                    {
                    }
                    else
                    {
                        if (
                            feed_text_iterator_prev_line(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor)))
                        {
                            feed_text_iterator_end_glyph(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor));
                        }
                        else
                        {
                            break;
                        }
                    }

                    feed_text_iterator_validate(
                        p_this->p_text,
                        &(
                            p_this->o_cursor));

                    if (p_this->o_cursor.p_glyph)
                    {
                        if (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                        {
                        }
                        else
                        {
                            b_found = 1;
                        }
                    }
                    else
                    {
                    }
                }

                if (b_found)
                {
                    b_found =
                        0;

                    while (
                        !b_found)
                    {
                        if (
                            feed_text_iterator_prev_glyph(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor)))
                        {
                        }
                        else
                        {
                            if (
                                feed_text_iterator_prev_line(
                                    p_this->p_text,
                                    &(
                                        p_this->o_cursor)))
                            {
                                feed_text_iterator_end_glyph(
                                    p_this->p_text,
                                    &(
                                        p_this->o_cursor));
                            }
                            else
                            {
                                break;
                            }
                        }

                        feed_text_iterator_validate(
                            p_this->p_text,
                            &(
                                p_this->o_cursor));

                        if (p_this->o_cursor.p_glyph)
                        {
                            if (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                            {
                                b_found =
                                    1;
                            }
                        }
                        else
                        {
                            b_found =
                                1;
                        }
                    }

                    if (b_found)
                    {
                        if (
                            feed_text_iterator_next_glyph(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor)))
                        {
                        }
                        else
                        {
                            if (
                                feed_text_iterator_next_line(
                                    p_this->p_text,
                                    &(
                                        p_this->o_cursor)))
                            {
                                feed_text_iterator_home_glyph(
                                    p_this->p_text,
                                    &(
                                        p_this->o_cursor));
                            }
                        }
                    }

                }

                feed_main_detect_visible_cursor(
                    p_this);

                if (p_this->b_cursor_visible)
                {
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
                else
                {
                    feed_main_scroll_pageup(
                        p_this);
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_RIGHT) == p_event->i_code)
            {
                /* Go right until reach begin of next word */
                char
                    b_found;

                b_found =
                    0;

                while (
                    !b_found)
                {
                    feed_text_iterator_validate(
                        p_this->p_text,
                        &(
                            p_this->o_cursor));

                    if (p_this->o_cursor.p_glyph)
                    {
                        if (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                        {
                            b_found = 1;
                        }
                    }
                    else
                    {
                        b_found = 1;
                    }

                    if (!b_found)
                    {
                        if (
                            feed_text_iterator_next_glyph(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor)))
                        {
                        }
                        else
                        {
                            if (
                                feed_text_iterator_next_line(
                                    p_this->p_text,
                                    &(
                                        p_this->o_cursor)))
                            {
                                feed_text_iterator_home_glyph(
                                    p_this->p_text,
                                    &(
                                        p_this->o_cursor));
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                if (b_found)
                {
                    b_found =
                        0;

                    while (
                        !b_found)
                    {
                        if (
                            feed_text_iterator_next_glyph(
                                p_this->p_text,
                                &(
                                    p_this->o_cursor)))
                        {
                        }
                        else
                        {
                            if (
                                feed_text_iterator_next_line(
                                    p_this->p_text,
                                    &(
                                        p_this->o_cursor)))
                            {
                                feed_text_iterator_home_glyph(
                                    p_this->p_text,
                                    &(
                                        p_this->o_cursor));
                            }
                            else
                            {
                                break;
                            }
                        }

                        feed_text_iterator_validate(
                            p_this->p_text,
                            &(
                                p_this->o_cursor));

                        if (p_this->o_cursor.p_glyph)
                        {
                            if (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                            {
                            }
                            else
                            {
                                b_found =
                                    1;
                            }
                        }
                        else
                        {
                        }
                    }
                }

                feed_main_detect_visible_cursor(
                    p_this);

                if (p_this->b_cursor_visible)
                {
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
                else
                {
                    feed_main_latch_next_page(
                        p_this);
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'M') == p_event->i_code)
            {
                /* Notify currently accumulated lines */

                /* Split current line */

                /* Create a new line */

                feed_text_iterator_validate(
                    p_this->p_text,
                    &(
                        p_this->o_cursor));

                if (
                    p_this->o_cursor.p_line)
                {
                    struct feed_glyph *
                        p_glyph;

                    struct feed_line *
                        p_line_down;

                    if (p_this->o_cursor.p_glyph)
                    {
                        p_glyph =
                            p_this->o_cursor.p_glyph;
                    }
                    else
                    {
                        p_glyph =
                            feed_line_get_glyph(
                                p_this->o_cursor.p_line,
                                p_this->o_cursor.i_glyph_index);
                    }

                    p_line_down =
                        feed_line_create(
                            p_this->p_client);

                    if (
                        p_line_down)
                    {
                        feed_list_join(
                            &(
                                p_this->o_cursor.p_line->o_list),
                            &(
                                p_line_down->o_list));

                        p_text->i_line_count ++;

                        if (p_glyph)
                        {
                            /* Transfer characters from this line to next */
                            while (
                                &(
                                    p_glyph->o_list)
                                != &(
                                    p_this->o_cursor.p_line->o_glyphs))
                            {
                                struct feed_glyph *
                                    p_glyph_next;

                                p_glyph_next =
                                    (struct feed_glyph *)(
                                        p_glyph->o_list.p_next);

                                feed_line_remove_glyph(
                                    p_this->o_cursor.p_line,
                                    p_glyph);

                                feed_line_append_glyph(
                                    p_line_down,
                                    p_glyph);

                                p_glyph =
                                    p_glyph_next;
                            }
                        }

                        p_this->o_cursor.i_line_index ++;

                        p_this->o_cursor.p_line = NULL;

                        p_this->o_cursor.i_glyph_index =
                            0u;

                        p_this->o_cursor.p_glyph = NULL;

                        feed_main_refresh_info(
                            p_this);

                        /* Detect that cursor is visible? */
                        if (p_this->b_cursor_visible)
                        {
                        }
                        else
                        {
                            feed_main_latch_next_page(p_this);
                        }
                    }
                    else
                    {
                        b_refresh_text = 0;
                    }
                }
                else
                {
                    b_refresh_text = 0;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'V') == p_event->i_code)
            {
                /* Verbose state */
                p_this->b_verbose = 1;

                b_refresh_text = 0;
            }
            else
            {
                feed_main_insert_event(
                    p_this,
                    p_event);

                feed_main_refresh_info(
                    p_this);

                /* Detect that cursor is visible? */
                if (p_this->b_cursor_visible)
                {
                }
                else
                {
                    feed_main_latch_next_page(p_this);
                }
            }

            feed_main_refresh_job(
                p_this,
                b_refresh_text,
                b_refresh_cursor);
        }
    }

}

static
void
feed_main_step(
    struct feed_handle * const
        p_this)
{
    unsigned char
        c;

    if (
        feed_tty_read_character(
            p_this->p_tty,
            &(
                c)))
    {
        int
            i_result;

        struct feed_event
            o_event;

        i_result =
            feed_input_write(
                p_this->p_input,
                c,
                &(
                    o_event));

        if (
            0
            <= i_result)
        {
            if (
                0
                < i_result)
            {
                feed_main_event_callback(
                    p_this,
                    &(
                        o_event));
            }
        }
        else
        {
            p_this->b_started =
                0;
        }
    }
    else
    {
        p_this->b_started =
            0;
    }
}

static
void
feed_main_loop(
    struct feed_handle * const
        p_this)
{
    while (
        p_this->b_started)
    {
        feed_main_step(
            p_this);

    }
}

int
feed_start(
    struct feed_handle * const
        p_this)
{
    int
        i_result;

    if (
        p_this)
    {
        if (
            !(p_this->b_started))
        {
            p_this->b_started =
                1;

            if (
                feed_tty_enable(
                    p_this->p_tty))
            {
                feed_main_update_screen_info(
                    p_this,
                    1);

                /* Move cursor to begin of document? */
                {
                    p_this->o_cursor.i_glyph_index =
                        0u;

                    p_this->o_cursor.p_glyph =
                        NULL;

                    p_this->o_cursor.i_line_index =
                        0u;

                    p_this->o_cursor.p_line =
                        NULL;

                    p_this->p_page_line =
                        NULL;

                    p_this->i_page_line_index =
                        0u;

                    p_this->i_page_glyph_index =
                        0u;

                    p_this->e_page_state =
                        feed_main_state_prompt;
                }

                feed_main_refresh_text(
                    p_this);

                feed_main_loop(
                    p_this);

                {
                    feed_screen_set_cursor_pos(
                        p_this->p_screen,
                        p_this->i_final_cursor_x,
                        p_this->i_final_cursor_y);

                    feed_screen_newline_raw(
                        p_this->p_screen);

                    feed_tty_flush(
                        p_this->p_tty);
                }

                feed_tty_disable(
                    p_this->p_tty);

                i_result =
                    0;
            }
            else
            {
                i_result =
                    -1;
            }
        }
        else
        {
            i_result =
                -1;
        }
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_start() */

int
feed_stop(
    struct feed_handle * const
        p_this)
{
    (void)(
        p_this);

    return
        -1;

} /* feed_stop() */

int
feed_suggest(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    (void)(
        p_this);
    (void)(
        p_data);
    (void)(
        i_data_length);

    return
        -1;

} /* feed_suggest() */

unsigned long int
feed_length(
    struct feed_handle * const
        p_this)
{
    return
        feed_text_get_raw_length(
            p_this->p_text);

} /* feed_length() */

int
feed_save(
    struct feed_handle * const
        p_this,
    unsigned char * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    struct feed_buf
        o_raw_content;

    if (
        feed_buf_init(
            &(
                o_raw_content),
            p_data,
            i_data_length))
    {
        feed_text_get_raw_buffer(
            p_this->p_text,
            &(
                o_raw_content));

        i_result =
            0;

        feed_buf_cleanup(
            &(
                o_raw_content));
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_save() */

/* end-of-file: feed.c */
