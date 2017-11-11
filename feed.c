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

#include "feed_list.h"

#include "feed_utf8.h"

#include "feed_glyph.h"

#include "feed_line.h"

#include "feed_text.h"

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

    void *
        pv_padding[1u];

    /* -- */

    struct feed_descriptor
        o_descriptor;

    struct feed_client
        o_client;

    /* -- */

    unsigned int
        i_screen_width;

    unsigned int
        i_screen_height;

    /* Page */
    unsigned int
        i_page_line_index;

    unsigned int
        i_page_glyph_index;

    /* -- */

    /* Cursor */
    unsigned int
        i_cursor_line_index;

    unsigned int
        i_cursor_glyph_index;

    unsigned int
        i_cursor_visible_x;

    unsigned int
        i_cursor_visible_y;

    /* -- */

    unsigned int
        i_final_line_index;

    unsigned int
        i_final_glyph_index;

    unsigned int
        i_final_cursor_x;

    unsigned int
        i_final_cursor_y;

    /* -- */

    enum feed_main_state
        e_final_state;

    enum feed_main_state
        e_page_state;

    unsigned int
        ui_padding[2u];

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

Module: feed_screen_iterator

Description:

    Iterator for visible screen glyphs.

*/
struct feed_screen_iterator
{
    unsigned long int
        i_cursor_address;

    unsigned long int
        ul_padding[1u];

};

static
char
feed_screen_iterator_init(
    struct feed_handle * const
        p_this,
    struct  feed_screen_iterator * const
        p_screen_iterator)
{
    char
        b_result;

    (void)(
        p_this);

    p_screen_iterator->i_cursor_address =
        0ul;

    b_result =
        1;

    return
        b_result;

}

#if 0
static
void
feed_screen_iterator_cleanup(
    struct feed_handle * const
        p_this,
    struct  feed_screen_iterator * const
        p_screen_iterator)
{
    (void)(
        p_this);
    (void)(
        p_screen_iterator);

}
#endif

static
void
feed_screen_iterator_home(
    struct feed_handle * const
        p_this,
    struct  feed_screen_iterator * const
        p_screen_iterator)
{
    (void)(
        p_this);

    p_screen_iterator->i_cursor_address =
        0ul;

}

#if 0
static
char
feed_screen_iterator_test_goto(
    struct feed_handle * const
        p_this,
    struct  feed_screen_iterator * const
        p_screen_iterator,
    unsigned short int const
        i_cursor_x,
    unsigned short int const
        i_cursor_y)
{
    char
        b_result;

    unsigned long int
        i_cursor_address;

    (void)(
        p_screen_iterator);

    i_cursor_address =
        (unsigned long int)(
            ((unsigned long int)(i_cursor_y) * (unsigned long int)(p_this->i_screen_width))
            + (unsigned long int)(i_cursor_x));

    if ((i_cursor_address + 1u) < (p_this->i_screen_width * p_this->i_screen_height))
    {
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
#endif

#if 1
static
void
feed_screen_iterator_goto(
    struct feed_handle * const
        p_this,
    struct  feed_screen_iterator * const
        p_screen_iterator,
    unsigned short int const
        i_cursor_x,
    unsigned short int const
        i_cursor_y)
{
    p_screen_iterator->i_cursor_address =
        (unsigned long int)(
            ((unsigned long int)(i_cursor_y) * (unsigned long int)(p_this->i_screen_width))
            + (unsigned long int)(i_cursor_x));

}
#endif

static
char
feed_screen_iterator_test_write(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator,
    unsigned long int const
        i_width)
{
    char
        b_result;

    if (
        (p_screen_iterator->i_cursor_address + i_width) < (p_this->i_screen_width * p_this->i_screen_height))
    {
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
void
feed_screen_iterator_write(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator,
    unsigned long int const
        i_width)
{
    (void)(
        p_this);

    p_screen_iterator->i_cursor_address += i_width;

}

static
unsigned short int
feed_screen_iterator_get_cursor_x(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    return
        (unsigned short int)(
            p_screen_iterator->i_cursor_address
            % p_this->i_screen_width);

}

static
unsigned short int
feed_screen_iterator_get_cursor_y(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    return
        (unsigned short int)(
            p_screen_iterator->i_cursor_address
            / p_this->i_screen_width);

}

static
char
feed_screen_iterator_test_newline(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    return
        feed_screen_iterator_test_write(
            p_this,
            p_screen_iterator,
            1u);

}

static
unsigned long int
feed_screen_iterator_clear(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    unsigned long int
        i_width;

    i_width =
        (unsigned long int)(
            (
                (unsigned long int)(p_this->i_screen_width)
                * (unsigned long int)(p_this->i_screen_height))
            - 1ul
            - p_screen_iterator->i_cursor_address);

    p_screen_iterator->i_cursor_address += i_width;

    return
        i_width;
}

static
unsigned long int
feed_screen_iterator_newline(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    unsigned long int
        i_width;

    unsigned short int
        i_cursor_x;

    i_cursor_x =
        feed_screen_iterator_get_cursor_x(
            p_this,
            p_screen_iterator);

    i_width = (unsigned long int)(p_this->i_screen_width - i_cursor_x);

    if (
        feed_screen_iterator_test_write(
            p_this,
            p_screen_iterator,
            i_width))
    {
        feed_screen_iterator_write(
            p_this,
            p_screen_iterator,
            i_width);
    }
    else
    {
        i_width =
            feed_screen_iterator_clear(
                p_this,
                p_screen_iterator);
    }

    return
        i_width;

}

static
char
feed_screen_iterator_test_clear(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    return
        feed_screen_iterator_test_write(
            p_this,
            p_screen_iterator,
            1u);

}

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
    unsigned int
        i_glyph_index;

    unsigned int
        i_line_index;

    enum feed_main_state
        e_state;

    unsigned int
        ui_padding[1u];

};

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
            if (p_iterator->p_document_line)
            {
                p_iterator->p_document_glyph =
                    feed_line_get_glyph(
                        p_iterator->p_document_line,
                        p_iterator->i_glyph_index);
            }
            else
            {
                p_iterator->p_document_glyph =
                    NULL;
            }

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
            if (p_iterator->p_document_line)
            {
                if (p_iterator->p_document_line->o_list.p_next != &(p_this->p_text->o_lines))
                {
                }
                else
                {
                    p_iterator->p_document_line = NULL;

                    p_iterator->e_state = feed_main_state_eof;
                }
            }
            else
            {
                p_iterator->e_state = feed_main_state_eof;
            }
        }

        if (feed_main_state_eof == p_iterator->e_state)
        {
        }
        else
        {
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
    unsigned int const
        i_page_line_index,
    unsigned int const
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

    p_iterator->p_document_line =
        feed_text_get_line(
            p_this->p_text,
            p_iterator->i_line_index);

    feed_main_iterator_get_glyph(
        p_this,
        p_iterator);

    feed_screen_iterator_init(
        p_this,
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

    if (p_iterator->p_prompt_glyph || p_iterator->p_document_glyph)
    {
        unsigned int i_glyph_width;

        i_glyph_width =
            feed_glyph_get_visible_width(
                p_iterator->p_prompt_glyph
                ? p_iterator->p_prompt_glyph
                : p_iterator->p_document_glyph);

        b_result =
            feed_screen_iterator_test_write(
                p_this,
                &(
                    p_iterator->o_screen_iterator),
                i_glyph_width);
    }
    else if (p_iterator->p_document_line)
    {
        b_result =
            feed_screen_iterator_test_newline(
                p_this,
                &(
                    p_iterator->o_screen_iterator));
    }
    else
    {
        b_result =
            feed_screen_iterator_test_clear(
                p_this,
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
    if (p_iterator->p_prompt_glyph || p_iterator->p_document_glyph)
    {
        i_width =
            feed_glyph_get_visible_width(
                p_iterator->p_prompt_glyph
                ? p_iterator->p_prompt_glyph
                : p_iterator->p_document_glyph);

        feed_screen_iterator_write(
            p_this,
            &(
                p_iterator->o_screen_iterator),
            i_width);
    }
    else if (p_iterator->p_document_line)
    {
        /* Empty line, so do a newline */
        i_width =
            feed_screen_iterator_newline(
                p_this,
                &(
                    p_iterator->o_screen_iterator));
    }
    else
    {
        i_width =
            feed_screen_iterator_clear(
                p_this,
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

static
char
feed_main_look_down(
    struct feed_handle * const
        p_this)
{
    struct feed_main_iterator
        o_iterator;

    unsigned long int
        i_cursor_offset;

    char
        b_found;

    i_cursor_offset =
        0ul;

    b_found =
        0;

    if (
        p_this->b_cursor_visible)
    {
        if (
            feed_main_iterator_begin(
                p_this,
                &(o_iterator),
                p_this->i_cursor_line_index,
                p_this->i_cursor_glyph_index,
                feed_main_state_text))
        {
            char
                b_more;

            b_more =
                1;

            feed_screen_iterator_goto(
                p_this,
                &(
                    o_iterator.o_screen_iterator),
                (unsigned short int)(
                    p_this->i_cursor_visible_x),
                (unsigned short int)(
                    p_this->i_cursor_visible_y));

            while (
                b_more
                && (!b_found))
            {
                if (
                    i_cursor_offset >= p_this->i_screen_width)
                {
                    if (o_iterator.e_state == feed_main_state_prompt)
                    {
                        p_this->i_cursor_line_index =
                            o_iterator.i_line_index;

                        p_this->i_cursor_glyph_index =
                            0u;
                    }
                    else
                    {
                        p_this->i_cursor_glyph_index =
                            o_iterator.i_glyph_index;

                        p_this->i_cursor_line_index =
                            o_iterator.i_line_index;
                    }

                    b_found =
                        1;
                }
                else if (
                    (o_iterator.i_line_index > p_this->i_cursor_line_index)
                    && ((o_iterator.e_state == feed_main_state_eol)
                        || (o_iterator.e_state == feed_main_state_eof)))
                {
                    p_this->i_cursor_glyph_index =
                        o_iterator.i_glyph_index;

                    p_this->i_cursor_line_index =
                        o_iterator.i_line_index;

                    b_found =
                        1;
                }
                else if (
                    o_iterator.i_line_index > (p_this->i_cursor_line_index + 1u))
                {
                    if ((p_this->i_cursor_line_index + 1u) < p_this->p_text->i_line_count)
                    {
                        p_this->i_cursor_line_index ++;

                        p_this->i_cursor_glyph_index =
                            0u;
                    }

                    b_found =
                        1;
                }
                else
                {
                    if (
                        feed_main_iterator_test(
                            p_this,
                            &(o_iterator)))
                    {
                        i_cursor_offset +=
                            feed_main_iterator_write(
                                p_this,
                                &(o_iterator));

                        b_more =
                            feed_main_iterator_next(
                                p_this,
                                &(
                                    o_iterator));
                    }
                    else
                    {
                        feed_screen_iterator_home(
                            p_this,
                            &(
                                o_iterator.o_screen_iterator));

                        i_cursor_offset ++;
                    }
                }
            }
        }
    }

    return b_found;
}

static
char
feed_main_look_up(
    struct feed_handle * const
        p_this)
{
    struct feed_main_iterator
        o_iterator;

    char
        b_found;

    b_found =
        0;

    if (
        p_this->b_cursor_visible
        && p_this->i_cursor_visible_y)
    {
        unsigned long int
            i_cursor_address;

        i_cursor_address =
            ((p_this->i_cursor_visible_y - 1u) * p_this->i_screen_width)
            + p_this->i_cursor_visible_x;

        if (
            feed_main_iterator_begin(
                p_this,
                &(o_iterator),
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
                    o_iterator.o_screen_iterator.i_cursor_address >= i_cursor_address)
                {
                    if (o_iterator.e_state == feed_main_state_prompt)
                    {
                        p_this->i_cursor_line_index =
                            o_iterator.i_line_index;

                        p_this->i_cursor_glyph_index =
                            0u;
                    }
                    else
                    {
                        p_this->i_cursor_glyph_index =
                            o_iterator.i_glyph_index;

                        p_this->i_cursor_line_index =
                            o_iterator.i_line_index;
                    }

                    b_found =
                        1;
                }
                else
                {
                    if (
                        feed_main_iterator_test(
                            p_this,
                            &(o_iterator)))
                    {
                        feed_main_iterator_write(
                            p_this,
                            &(o_iterator));

                        if (
                            (o_iterator.o_screen_iterator.i_cursor_address > i_cursor_address)
                            && ((o_iterator.e_state == feed_main_state_eol)
                                || (o_iterator.e_state == feed_main_state_eof)))
                        {
                            p_this->i_cursor_glyph_index =
                                o_iterator.i_glyph_index;

                            p_this->i_cursor_line_index =
                                o_iterator.i_line_index;

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
                    else
                    {
                        b_more =
                            0;
                    }
                }
            }
        }
    }

    return b_found;
}

static
char
feed_main_look_pageup(
    struct feed_handle * const
        p_this)
{
    struct feed_main_iterator
        o_iterator;

    unsigned int
        i_found_cursor_line_index;

    unsigned int
        i_found_cursor_glyph_index;

    unsigned int
        i_this_page_line_index;

    unsigned int
        i_this_page_glyph_index;

    enum feed_main_state
        e_this_page_state;

    unsigned int
        i_prev_page_line_index;

    unsigned int
        i_prev_page_glyph_index;

    enum feed_main_state
        e_prev_page_state;

    char
        b_found;

    b_found =
        0;

    (void)(
        i_found_cursor_line_index);
    (void)(
        i_found_cursor_glyph_index);

    if (
        p_this->b_cursor_visible)
    {
        unsigned long int
            i_cursor_address;

        /* get x and y of cursor */
        i_cursor_address =
            (p_this->i_cursor_visible_y * p_this->i_screen_width)
            + p_this->i_cursor_visible_x;

        /* how to calculate address of page? */

        if (
            feed_main_iterator_begin(
                p_this,
                &(o_iterator),
                0u,
                0u,
                feed_main_state_prompt))
        {
            char
                b_found_cursor_valid;

            char
                b_more;

            char
                b_found_in_page;

            b_more =
                1;

            b_found_in_page =
                0;

            b_found_cursor_valid =
                0;

            i_prev_page_line_index =
            i_this_page_line_index =
                o_iterator.i_line_index;

            i_prev_page_glyph_index =
            i_this_page_glyph_index =
                o_iterator.i_glyph_index;

            e_prev_page_state =
            e_this_page_state =
                o_iterator.e_state;

            while (
                b_more
                && (!b_found))
            {
                if (
                    feed_main_iterator_test(
                        p_this,
                        &(o_iterator)))
                {
                    /* Detect if cursor is visible */
                    if (
                        (
                            p_this->i_cursor_line_index == o_iterator.i_line_index)
                        && (
                            p_this->i_cursor_glyph_index == o_iterator.i_glyph_index)
                        && (
                            (o_iterator.e_state != feed_main_state_prompt)))
                    {
                        b_found =
                            1;
                    }
                    else
                    {
                        /* Locate a glyph that is at same coord as cursor */
                        if (
                            !b_found_in_page
                            && (o_iterator.o_screen_iterator.i_cursor_address >= i_cursor_address))
                        {
                            if (o_iterator.e_state == feed_main_state_prompt)
                            {
                                i_found_cursor_line_index =
                                    o_iterator.i_line_index;

                                i_found_cursor_glyph_index =
                                    0u;
                            }
                            else
                            {
                                i_found_cursor_line_index =
                                    o_iterator.i_line_index;

                                i_found_cursor_glyph_index =
                                    o_iterator.i_glyph_index;
                            }

                            b_found_cursor_valid =
                                1;

                            b_found_in_page =
                                1;
                        }

                        feed_main_iterator_write(
                            p_this,
                            &(o_iterator));

                        if (
                            !b_found_in_page
                            && (o_iterator.o_screen_iterator.i_cursor_address > i_cursor_address)
                            && ((o_iterator.e_state == feed_main_state_eol)
                                || (o_iterator.e_state == feed_main_state_eof)))
                        {
                            i_found_cursor_glyph_index =
                                o_iterator.i_glyph_index;

                            i_found_cursor_line_index =
                                o_iterator.i_line_index;

                            b_found_cursor_valid =
                                1;

                            b_found_in_page =
                                1;
                        }

                        b_more =
                            feed_main_iterator_next(
                                p_this,
                                &(
                                    o_iterator));
                    }
                }
                else
                {
                    /* Remember this is last page */
                    i_prev_page_line_index =
                        i_this_page_line_index;

                    i_prev_page_glyph_index =
                        i_this_page_glyph_index;

                    e_prev_page_state =
                        e_this_page_state;

                    i_this_page_line_index =
                        o_iterator.i_line_index;

                    i_this_page_glyph_index =
                        o_iterator.i_glyph_index;

                    e_this_page_state =
                        o_iterator.e_state;

                    b_found_in_page =
                        0;

                    feed_screen_iterator_home(
                        p_this,
                        &(
                            o_iterator.o_screen_iterator));
                }
            }

            if (
                b_found)
            {
                if (b_found_cursor_valid)
                {
                    p_this->i_page_line_index =
                        i_prev_page_line_index;

                    p_this->i_page_glyph_index =
                        i_prev_page_glyph_index;

                    p_this->e_page_state =
                        e_prev_page_state;

                    p_this->i_cursor_line_index =
                        i_found_cursor_line_index;

                    p_this->i_cursor_glyph_index =
                        i_found_cursor_glyph_index;
                }
            }

#if defined(FEED_CFG_DEBUG)
            printf("look pageup %u,%u (%d) %u,%u -> %d,%d\r\n",
                p_this->i_page_line_index,
                p_this->i_page_glyph_index,
                p_this->e_page_state,
                p_this->i_cursor_line_index,
                p_this->i_cursor_glyph_index,
                b_found,
                b_found_cursor_valid);
#endif /* #if defined(FEED_CFG_DEBUG) */

        }
    }

    return b_found;
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
        if (
            feed_tty_get_window_size(
                p_this->p_tty,
                &(
                    p_this->i_screen_width),
                &(
                    p_this->i_screen_height),
                0 /* no fallback */))
        {
            feed_screen_set_physical_size(
                p_this->p_screen,
                p_this->i_screen_width,
                p_this->i_screen_height);
        }

        feed_screen_set_cursor_pos(
            p_this->p_screen,
            0u,
            0u);
    }

    if (
        feed_main_iterator_begin(
            p_this,
            &(o_iterator),
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
            /* Detect if cursor is visible */
            if (
                !(
                    p_this->b_cursor_visible)
                && (
                    p_this->i_cursor_line_index == o_iterator.i_line_index)
                && (
                    p_this->i_cursor_glyph_index == o_iterator.i_glyph_index)
                && (
                    (o_iterator.e_state != feed_main_state_prompt)))
            {
                p_this->i_cursor_visible_x =
                    feed_screen_iterator_get_cursor_x(
                        p_this,
                        &(
                            o_iterator.o_screen_iterator));

                p_this->i_cursor_visible_y =
                    feed_screen_iterator_get_cursor_y(
                        p_this,
                        &(
                            o_iterator.o_screen_iterator));

                p_this->b_cursor_visible =
                    1;
            }

            if (
                feed_main_iterator_test(
                    p_this,
                    &(o_iterator)))
            {
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
#if defined(FEED_CFG_DEBUG)
                        printf("next_false (%u,%c%u) ",
                            o_iterator.i_line_index,
                            o_iterator.e_state == feed_main_state_prompt ? 'P' : 'T',
                            o_iterator.i_glyph_index);
#endif /* #if defined(FEED_CFG_DEBUG) */

                        b_more =
                            0;
                    }
                }
            }
            else
            {
#if defined(FEED_CFG_DEBUG)
                printf("test_false (%u,%c%u) ",
                    o_iterator.i_line_index,
                    o_iterator.e_state == feed_main_state_prompt ? 'P' : 'T',
                    o_iterator.i_glyph_index);
#endif /* #if defined(FEED_CFG_DEBUG) */

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

#if defined(FEED_CFG_DEBUG)
        printf("final:%u/%u,%u(%d) cur:%u,%u page:%u,%u(%d) last:%u/%u,%u/%u\r\n",
            p_this->i_final_line_index,
            p_this->p_text->i_line_count,
            p_this->i_final_glyph_index,
            p_this->e_final_state,
            p_this->i_cursor_line_index,
            p_this->i_cursor_glyph_index,
            p_this->i_page_line_index,
            p_this->i_page_glyph_index,
            p_this->e_page_state,
            p_this->i_final_cursor_y,
            p_this->i_screen_height,
            p_this->i_final_cursor_x,
            p_this->i_screen_width);
#endif /* #if defined(FEED_CFG_DEBUG) */

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
                feed_text_write_utf8_code(
                    p_this->p_text,
                    &(
                        o_utf8_code),
                    p_this->i_cursor_line_index,
                    p_this->i_cursor_glyph_index);

                p_this->i_cursor_glyph_index ++;
            }
        }

        feed_utf8_parser_cleanup(
            &(
                o_utf8_parser));
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

        feed_main_refresh_info(
            p_this);

        /* Detect that cursor is visible? */
        if (p_this->b_cursor_visible)
        {
        }
        else
        {
            if (p_this->e_final_state != feed_main_state_null)
            {
                p_this->i_page_line_index = p_this->i_final_line_index;
                p_this->i_page_glyph_index = p_this->i_final_glyph_index;
                p_this->e_page_state = p_this->e_final_state;
            }
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
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_this->i_cursor_line_index);

                if (
                    p_line)
                {
                    /* Find last char */
                    if (p_this->i_cursor_glyph_index)
                    {
                        struct feed_glyph *
                            p_glyph;

                        p_glyph =
                            feed_line_get_glyph(
                                p_line,
                                p_this->i_cursor_glyph_index - 1u);

                        if (
                            p_glyph)
                        {
                            /* Delete the selected char */
                            feed_glyph_destroy(
                                p_this->p_client,
                                p_glyph);

                            p_this->i_cursor_glyph_index --;

                            if (p_line->i_glyph_count)
                            {
                                p_line->i_glyph_count --;
                            }
                        }
                        else
                        {
                            b_refresh_text = 0;
                        }
                    }
                    else
                    {
                        /* Take all glyphs of current line and append at
                        end of previous line */
                        if (p_this->i_cursor_line_index)
                        {
                            struct feed_line *
                                p_line_up;

                            p_this->i_cursor_line_index --;

                            p_line_up =
                                (struct feed_line *)(
                                    p_line->o_list.p_prev);

                            p_this->i_cursor_glyph_index =
                                p_line_up->i_glyph_count;

                            while (
                                p_line->i_glyph_count)
                            {
                                struct feed_glyph *
                                    p_glyph;

                                p_glyph =
                                    (struct feed_glyph *)(
                                        p_line->o_glyphs.p_next);

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_glyph->o_list));

                                p_line->i_glyph_count --;

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_line_up->o_glyphs));

                                p_line_up->i_glyph_count ++;
                            }

                            feed_line_destroy(
                                p_line);

                            p_text->i_line_count --;
                        }
                        else
                        {
                            b_refresh_text = 0;
                        }
                    }
                }
                else
                {
                    b_refresh_text = 0;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'L') == p_event->i_code)
            {
                b_refresh_text = 1;
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_DELETE) == p_event->i_code)
            {
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_this->i_cursor_line_index);

                if (
                    p_line)
                {
                    /* Find last char */
                    if (p_this->i_cursor_glyph_index < p_line->i_glyph_count)
                    {
                        struct feed_glyph *
                            p_glyph;

                        p_glyph =
                            feed_line_get_glyph(
                                p_line,
                                p_this->i_cursor_glyph_index);

                        if (
                            p_glyph)
                        {
                            /* Delete the selected char */
                            feed_glyph_destroy(
                                p_this->p_client,
                                p_glyph);

                            if (p_line->i_glyph_count)
                            {
                                p_line->i_glyph_count --;
                            }
                        }
                        else
                        {
                            b_refresh_text = 0;
                        }
                    }
                    else
                    {
                        /* Bring next line into this line */
                        /* Delete next line */
                        if ((p_this->i_cursor_line_index + 1u) < p_text->i_line_count)
                        {
                            struct feed_line *
                                p_line_down;

                            p_line_down =
                                (struct feed_line *)(
                                    p_line->o_list.p_next);

                            while (
                                p_line_down->i_glyph_count)
                            {
                                struct feed_glyph *
                                    p_glyph;

                                p_glyph =
                                    (struct feed_glyph *)(
                                        p_line_down->o_glyphs.p_next);

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_glyph->o_list));

                                p_line_down->i_glyph_count --;

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_line->o_glyphs));

                                p_line->i_glyph_count ++;
                            }

                            feed_line_destroy(
                                p_line_down);

                            p_text->i_line_count --;
                        }
                    }
                }
                else
                {
                    b_refresh_text = 0;
                }
            }
            else if (
                ((FEED_EVENT_KEY_FLAG | FEED_KEY_HOME) == p_event->i_code)
                || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'A') == p_event->i_code))
            {
                p_this->i_cursor_glyph_index =
                    0;

                /* Adjust visible cursor position */

                b_refresh_text = 0;

                b_refresh_cursor = 1;
            }
            else if (
                ((FEED_EVENT_KEY_FLAG | FEED_KEY_END) == p_event->i_code)
                || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'E') == p_event->i_code))
            {
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_this->i_cursor_line_index);

                if (
                    p_line)
                {
                    p_this->i_cursor_glyph_index =
                        p_line->i_glyph_count;

                    /* Adjust visible cursor position */
                    feed_main_refresh_info(
                        p_this);

                    /* Adjust visible cursor position */
                    if (p_this->b_cursor_visible)
                    {
                        b_refresh_cursor = 1;

                        b_refresh_text = 0;
                    }
                    else
                    {
                        if (p_this->e_final_state != feed_main_state_null)
                        {
                            p_this->i_page_line_index = p_this->i_final_line_index;
                            p_this->i_page_glyph_index = p_this->i_final_glyph_index;
                            p_this->e_page_state = p_this->e_final_state;
                        }
                    }
                }
                else
                {
                    b_refresh_text = 0;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_LEFT) == p_event->i_code)
            {
                if (p_this->i_cursor_glyph_index)
                {
                    p_this->i_cursor_glyph_index --;

                    /* Adjust visible cursor position */
                    b_refresh_cursor = 1;

                    b_refresh_text = 0;
                }
                else
                {
                    /* Go to previous line */
                    if (p_this->i_cursor_line_index)
                    {
                        struct feed_line *
                            p_line;

                        p_line =
                            feed_text_get_line(
                                p_text,
                                p_this->i_cursor_line_index - 1);

                        if (
                            p_line)
                        {
                            p_this->i_cursor_line_index --;

                            p_this->i_cursor_glyph_index =
                                p_line->i_glyph_count;

                            b_refresh_cursor = 1;

                            b_refresh_text = 0;
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
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_RIGHT) == p_event->i_code)
            {
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_this->i_cursor_line_index);

                if (
                    p_line)
                {
                    if (p_this->i_cursor_glyph_index < p_line->i_glyph_count)
                    {
                        p_this->i_cursor_glyph_index ++;
                    }
                    else
                    {
                        if ((p_this->i_cursor_line_index + 1u) < p_text->i_line_count)
                        {
                            p_this->i_cursor_line_index ++;

                            p_this->i_cursor_glyph_index = 0u;
                        }
                    }

                    /* Adjust visible cursor position */
                    if (p_this->b_cursor_visible)
                    {
                        b_refresh_cursor = 1;

                        b_refresh_text = 0;
                    }
                    else
                    {
                        if (p_this->e_final_state != feed_main_state_null)
                        {
                            p_this->i_page_line_index = p_this->i_final_line_index;
                            p_this->i_page_glyph_index = p_this->i_final_glyph_index;
                            p_this->e_page_state = p_this->e_final_state;
                        }
                    }
                }
                else
                {
                    b_refresh_text = 0;
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
                    if (p_this->i_cursor_line_index)
                    {
                        p_this->i_cursor_line_index --;

                        p_this->i_cursor_glyph_index = 0u;

                        p_this->i_page_line_index = p_this->i_cursor_line_index;

                        p_this->i_page_glyph_index = 0u;

                        p_this->e_page_state = feed_main_state_prompt;

                        /* Todo: find origin of previous page */
                    }
                    else
                    {
                        b_refresh_text = 0;
                    }
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_DOWN) == p_event->i_code)
            {
                if (feed_main_look_down(p_this))
                {
                    /* Detect that cursor is visible? */
                    if (p_this->b_cursor_visible)
                    {
                        b_refresh_cursor = 1;

                        b_refresh_text = 0;
                    }
                    else
                    {
                        if (p_this->e_final_state != feed_main_state_null)
                        {
                            p_this->i_page_line_index = p_this->i_final_line_index;
                            p_this->i_page_glyph_index = p_this->i_final_glyph_index;
                            p_this->e_page_state = p_this->e_final_state;
                        }
                    }
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_PAGEUP) == p_event->i_code)
            {
                if (feed_main_look_pageup(p_this))
                {
                }
                else
                {
                    p_this->i_cursor_line_index = 0;
                    p_this->i_cursor_glyph_index = 0;
                    p_this->i_page_line_index = 0;
                    p_this->i_page_glyph_index = 0;
                    p_this->e_page_state = feed_main_state_prompt;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_PAGEDOWN) == p_event->i_code)
            {
                if (p_this->e_final_state != feed_main_state_null)
                {
                    p_this->i_cursor_line_index = p_this->i_final_line_index;
                    p_this->i_cursor_glyph_index = p_this->i_final_glyph_index;
                    p_this->i_page_line_index = p_this->i_final_line_index;
                    p_this->i_page_glyph_index = p_this->i_final_glyph_index;
                    p_this->e_page_state = p_this->e_final_state;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'M') == p_event->i_code)
            {
                /* Notify currently accumulated lines */

                /* Split current line */

                /* Create a new line */
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_this->i_cursor_line_index);

                if (
                    p_line)
                {
                    struct feed_glyph *
                        p_glyph;

                    struct feed_line *
                        p_line_down;

                    p_glyph =
                        feed_line_get_glyph(
                            p_line,
                            p_this->i_cursor_glyph_index);

                    p_line_down =
                        feed_line_create(
                            p_this->p_client);

                    if (
                        p_line_down)
                    {
                        feed_list_join(
                            &(
                                p_line->o_list),
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
                                    p_line->o_glyphs))
                            {
                                struct feed_glyph *
                                    p_glyph_next;

                                p_glyph_next =
                                    (struct feed_glyph *)(
                                        p_glyph->o_list.p_next);

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_glyph->o_list));

                                p_line->i_glyph_count --;

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_line_down->o_glyphs));

                                p_line_down->i_glyph_count ++;

                                p_glyph =
                                    p_glyph_next;
                            }
                        }

                        p_this->i_cursor_line_index ++;

                        p_this->i_cursor_glyph_index =
                            0u;

                        feed_main_refresh_info(
                            p_this);

                        /* Detect that cursor is visible? */
                        if (p_this->b_cursor_visible)
                        {
                        }
                        else
                        {
                            if (p_this->e_final_state != feed_main_state_null)
                            {
                                p_this->i_page_line_index = p_this->i_final_line_index;
                                p_this->i_page_glyph_index = p_this->i_final_glyph_index;
                                p_this->e_page_state = p_this->e_final_state;
                            }
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
                    if (p_this->e_final_state != feed_main_state_null)
                    {
                        p_this->i_page_line_index = p_this->i_final_line_index;
                        p_this->i_page_glyph_index = p_this->i_final_glyph_index;
                        p_this->e_page_state = p_this->e_final_state;
                    }
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
                if (
                    feed_tty_get_window_size(
                        p_this->p_tty,
                        &(
                            p_this->i_screen_width),
                        &(
                            p_this->i_screen_height),
                        1))
                {
                    feed_screen_set_physical_size(
                        p_this->p_screen,
                        p_this->i_screen_width,
                        p_this->i_screen_height);
                }

                /* Move cursor to begin of document? */
                {
                    p_this->i_cursor_glyph_index =
                        0u;

                    p_this->i_cursor_line_index =
                        0u;

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
