/* See LICENSE for license details */

/*

Module: feed.c

Description:

    Implementation of feed library API.

*/

#include "feed_os.h"

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

    char
        b_started;

    char
        b_verbose;

    char
        b_cursor_visible;

    char
        b_page_prompt;

    char
        b_final_prompt;

    unsigned char
        uc_padding[3u];

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

static
void
feed_main_set_callback(
    void * const
        p_context,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    struct feed_handle * const
        p_this =
        (struct feed_handle *)(
            p_context);

    struct feed_text * const
        p_text =
        p_this->p_text;

    if ('\n' == p_utf8_code->a_raw[0u])
    {
        /* Notify currently accumulated lines */

        /* Split current line */

        /* Create a new line */
        if (
            feed_text_append_line(
                p_text))
        {
            p_this->i_cursor_line_index ++;

            p_this->i_cursor_glyph_index =
                0u;
        }
    }
    else
    {
        feed_text_append_utf8_code(
            p_this->p_text,
            p_utf8_code);

        p_this->i_cursor_glyph_index ++;
    }

}

static
char
feed_main_set(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    char
        b_result;

    if (p_this)
    {
        /* Go to end of buffer */

        if (
            i_data_length)
        {
            struct feed_utf8_parser
                o_utf8_parser;

            if (
                feed_utf8_parser_init(
                    &(
                        o_utf8_parser)))
            {
                unsigned long int
                    i_data_iterator;

                i_data_iterator =
                    0u;

                b_result =
                    1;

                while (
                    b_result
                    && (
                        i_data_iterator
                        < i_data_length))
                {
                    int
                        i_result;

                    struct feed_utf8_code
                        o_utf8_code;

                    i_result =
                        feed_utf8_parser_write(
                            &(
                                o_utf8_parser),
                            p_data[i_data_iterator],
                            &(
                                o_utf8_code));

                    if (
                        0
                        <= i_result)
                    {
                        if (
                            0
                            < i_result)
                        {
                            feed_main_set_callback(
                                p_this,
                                &(
                                    o_utf8_code));
                        }

                        i_data_iterator ++;
                    }
                    else
                    {
                        b_result =
                            0;
                    }
                }

                feed_utf8_parser_cleanup(
                    &(
                        o_utf8_parser));
            }
            else
            {
                b_result =
                    0;
            }
        }
        else
        {
            b_result =
                1;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

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
        feed_main_set(
            p_this,
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

#if 0
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
#endif

#if 0
static
void
feed_screen_iterator_end(
    struct feed_handle * const
        p_this,
    struct  feed_screen_iterator * const
        p_screen_iterator)
{
    p_screen_iterator->i_cursor_address =
        (
            (
                (unsigned long int)(p_this->i_screen_width)
                * (unsigned long int)(p_this->i_screen_height))
            - 1ul);

}
#endif

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

#if 0
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
    unsigned short int
        i_cursor_x;

    i_cursor_x =
        feed_screen_iterator_get_cursor_x(
            p_this,
            p_screen_iterator);

    return
        feed_screen_iterator_test_write(
            p_this,
            p_screen_iterator,
            (p_this->i_screen_width - i_cursor_x));

}

static
void
feed_screen_iterator_newline(
    struct feed_handle * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    unsigned short int
        i_cursor_x;

    i_cursor_x =
        feed_screen_iterator_get_cursor_x(
            p_this,
            p_screen_iterator);

    feed_screen_iterator_write(
        p_this,
        p_screen_iterator,
        (p_this->i_screen_width - i_cursor_x));

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
        i_page_line = i_document_y

    -   Problem: what if nothing is visible in page?

    -   Problem: automatic scroll on changes

        Display has page_line+offset_y+offset_x -> final_line+offset_y+offset_x

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
        i_document_x;

    unsigned int
        i_document_y;

    unsigned int
        ui_padding[2u];

    char
        b_prompt;

    unsigned char
        uc_padding[7u];

};

static
char
feed_main_iterator_begin(
    struct feed_handle * const
        p_this,
    struct feed_main_iterator * const
        p_iterator)
{
    p_iterator->p_document_line =
        feed_text_get_line(
            p_this->p_text,
            p_this->i_page_line_index);

    if (p_iterator->p_document_line)
    {
        if (0 == p_this->i_page_line_index)
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
    else
    {
        p_iterator->p_prompt_line =
            NULL;
    }

    if (p_iterator->p_prompt_line)
    {
        if (p_this->b_page_prompt)
        {
            p_iterator->p_prompt_glyph =
                feed_line_get_glyph(
                    p_iterator->p_prompt_line,
                    p_this->i_page_glyph_index);
        }
        else
        {
            p_iterator->p_prompt_glyph =
                NULL;
        }
    }
    else
    {
        p_iterator->p_prompt_glyph =
            NULL;
    }

    if (!(p_iterator->p_prompt_glyph))
    {
        if (
            p_iterator->p_document_line)
        {
            if (p_this->b_page_prompt)
            {
                p_iterator->p_document_glyph =
                    feed_line_get_glyph(
                        p_iterator->p_document_line,
                        p_this->i_page_glyph_index);
            }
            else
            {
                p_iterator->p_document_glyph =
                    NULL;
            }
        }
        else
        {
            p_iterator->p_document_glyph =
                NULL;
        }
    }

    feed_screen_iterator_init(
        p_this,
        &(
            p_iterator->o_screen_iterator));

    p_iterator->i_document_y =
        p_this->i_page_line_index;

    p_iterator->i_document_x =
        p_this->i_page_glyph_index;

    p_iterator->b_prompt =
        p_this->b_page_prompt;

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
            0;
    }

    return
        b_result;

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

    /* Advance screen position */
    if (p_iterator->p_prompt_glyph || p_iterator->p_document_glyph)
    {
        unsigned int i_glyph_width;

        i_glyph_width = feed_glyph_get_visible_width(p_iterator->p_prompt_glyph ? p_iterator->p_prompt_glyph : p_iterator->p_document_glyph);

        feed_screen_iterator_write(
            p_this,
            &(
                p_iterator->o_screen_iterator),
            i_glyph_width);
    }
    else if (p_iterator->p_document_line)
    {
        /* Empty line, so do a newline */
        feed_screen_iterator_newline(
            p_this,
            &(
                p_iterator->o_screen_iterator));
    }

    /* Look for next glyph */
    if (p_iterator->p_prompt_glyph)
    {
        p_iterator->i_document_x ++;

        if (p_iterator->p_prompt_glyph->o_list.p_next != &(p_iterator->p_prompt_line->o_glyphs))
        {
            p_iterator->p_prompt_glyph =
                (struct feed_glyph *)(
                    p_iterator->p_prompt_glyph->o_list.p_next);
        }
        else
        {
            /* Next line in prompt? */

            p_iterator->b_prompt = 0;

            p_iterator->i_document_x = 0u;

            p_iterator->p_prompt_glyph = NULL;

            p_iterator->p_prompt_line = NULL;

            if (
                p_iterator->p_document_line)
            {
                p_iterator->p_document_glyph =
                    feed_line_get_glyph(
                        p_iterator->p_document_line,
                        0u);
            }
            else
            {
                p_iterator->p_document_glyph =
                    NULL;
            }

            if (!(p_iterator->p_document_glyph))
            {
                if (p_iterator->p_document_line)
                {
                    if (p_iterator->p_document_line->o_list.p_next != &(p_this->p_text->o_lines))
                    {
                    }
                    else
                    {
                        p_iterator->p_document_line = NULL;
                    }
                }
            }
        }
    }
    else if (p_iterator->p_document_glyph)
    {
        p_iterator->i_document_x ++;

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

            if (p_iterator->p_document_line)
            {
                if (p_iterator->p_document_line->o_list.p_next != &(p_this->p_text->o_lines))
                {
                }
                else
                {
                    p_iterator->p_document_line = NULL;
                }
            }
        }
    }
    else if (p_iterator->p_document_line)
    {
        /* Go to next line */
        if (p_iterator->p_document_line->o_list.p_next != &(p_this->p_text->o_lines))
        {
            p_iterator->i_document_x = 0u;

            p_iterator->i_document_y ++;

            p_iterator->p_document_line =
                (struct feed_line *)(
                    p_iterator->p_document_line->o_list.p_next);

            p_iterator->p_document_glyph =
                feed_line_get_glyph(
                    p_iterator->p_document_line,
                    0u);

            /* Switch to prompt mode */
            p_iterator->b_prompt = 1;

            p_iterator->p_prompt_line =
                feed_prompt_get2(
                    p_this->p_prompt);

            if (p_iterator->p_prompt_line)
            {
                p_iterator->p_prompt_glyph =
                    feed_line_get_glyph(
                        p_iterator->p_prompt_line,
                        0u);
            }
        }
        else
        {
            /* End of file */
            p_iterator->p_document_line = NULL;

            p_iterator->p_document_glyph = NULL;
        }
    }
    else
    {
        /* no more lines... */
    }

    b_result =
        1;

    return
        b_result;

}

static
void
feed_main_refresh_job(
    struct feed_handle * const
        p_this,
    char const
        b_cursor_only)
{
    struct feed_main_iterator
        o_iterator;

    unsigned int
        i_cursor_x;

    unsigned int
        i_cursor_y;

    char
        b_cursor;

    i_cursor_x =
        0;

    i_cursor_y =
        0;

    b_cursor =
        0;

    if (!b_cursor_only)
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

    if (feed_main_iterator_begin(p_this, &(o_iterator)))
    {
        char
            b_more;

        b_more =
            1;

        while (
            b_more)
        {
            /* Remember this line and glyph and offset */
            p_this->i_final_line_index =
                o_iterator.i_document_y;

            p_this->i_final_glyph_index =
                o_iterator.i_document_x;

            p_this->b_final_prompt =
                o_iterator.b_prompt;

            /* Detect if cursor is visible */
            if (
                !(
                    b_cursor)
                && (
                    p_this->i_cursor_line_index == o_iterator.i_document_y)
                && (
                    p_this->i_cursor_glyph_index == o_iterator.i_document_x)
                && (
                    !(o_iterator.b_prompt)))
            {
                i_cursor_x =
                    feed_screen_iterator_get_cursor_x(
                        p_this,
                        &(
                            o_iterator.o_screen_iterator));

                i_cursor_y =
                    feed_screen_iterator_get_cursor_y(
                        p_this,
                        &(
                            o_iterator.o_screen_iterator));

                b_cursor =
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
                    if (!b_cursor_only)
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
                    if (!b_cursor_only)
                    {
                        feed_screen_newline(
                            p_this->p_screen);
                    }
                }
                else
                {
                    b_more =
                        0;
                }

                if (b_more)
                {
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

        if (!b_cursor_only)
        {
            feed_screen_get_cursor_pos(
                p_this->p_screen,
                &(
                    p_this->i_final_cursor_x),
                &(
                    p_this->i_final_cursor_y));

            feed_screen_clear_region(
                p_this->p_screen);
        }

        if (b_cursor)
        {
            feed_screen_set_cursor_pos(
                p_this->p_screen,
                i_cursor_x,
                i_cursor_y);
        }

        feed_tty_flush(
            p_this->p_tty);
    }
}


#if 0
static
void
feed_main_refresh_job(
    struct feed_handle * const
        p_this,
    char const
        b_cursor_only)
{
    struct feed_screen *
        p_screen;

    struct feed_text *
        p_text;

    unsigned int
        i_cursor_line_iterator;

    unsigned int
        i_cursor_glyph_iterator;

    unsigned int
        i_emulated_x;

    unsigned int
        i_emulated_y;

    p_screen =
        p_this->p_screen;

    p_text =
        p_this->p_text;

    i_cursor_line_iterator =
        0u;

    p_this->b_cursor_visible =
        0;

    p_this->i_cursor_visible_y =
        0u;

    p_this->i_cursor_visible_x =
        0u;

    /* get window size has problem when keyboard is typed at same time */
    if (!b_cursor_only)
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
    }

    /* Grow size of drawing region */

    /* Move cursor to beginning of drawing region */
    i_emulated_x =
        0;

    i_emulated_y =
        0;

    if (!b_cursor_only)
    {
        feed_screen_set_cursor_pos(
            p_this->p_screen,
            0u,
            0u);
    }

    /* Print state of body text */
    {
        struct feed_list *
            p_line_iterator;

        struct feed_list *
            p_glyph_iterator;

        /* For all lines in body text */
        p_line_iterator =
            p_text->o_lines.p_next;

        while (
            (
                p_line_iterator
                != &(
                    p_text->o_lines))
            && (
                i_emulated_y
                < p_this->i_screen_height))
        {
            struct feed_line *
                p_line;

            p_line =
                (struct feed_line *)(
                    p_line_iterator);

            /* If line is within refresh window */
            if (i_cursor_line_iterator >= p_this->i_page_line_index)
            {
                /* Return to home position */

                /* Note: do not draw prompt if horizontal scroll is enabled */

                if (((0 == p_this->i_page_glyph_index) && (i_cursor_line_iterator == p_this->i_page_line_index))
                    || (i_cursor_line_iterator != p_this->i_page_line_index))
                {
                    struct feed_line *
                        p_prompt_line;

                    p_this->i_final_line_index =
                        i_cursor_line_iterator;

                    p_this->i_final_glyph_index =
                        0u;

                    if (0u == i_cursor_line_iterator)
                    {
                        /* Draw a prompt on first line */
                        p_prompt_line =
                            feed_prompt_get1(
                                p_this->p_prompt);
                    }
                    else
                    {
                        p_prompt_line =
                            feed_prompt_get2(
                                p_this->p_prompt);
                    }

                    if (i_cursor_line_iterator > p_this->i_page_line_index)
                    {
                        if (!b_cursor_only)
                        {
                            feed_screen_newline(
                                p_this->p_screen);
                        }

                        i_emulated_x =
                            0u;

                        i_emulated_y ++;
                    }

                    if (p_prompt_line)
                    {
                        p_glyph_iterator =
                            p_prompt_line->o_glyphs.p_next;

                        while (
                            (
                                p_glyph_iterator
                                != &(
                                    p_prompt_line->o_glyphs))
                            && (
                                i_emulated_y
                                < p_this->i_screen_height))
                        {
                            struct feed_glyph const *
                                p_glyph;

                            p_glyph =
                                (struct feed_glyph const *)(
                                    p_glyph_iterator);

                            /* If char is within refresh window */
                            if (!b_cursor_only)
                            {
                                unsigned char
                                    a_visible[15u];

                                unsigned char
                                    i_visible_length;

                                i_visible_length =
                                    feed_glyph_render_visible(
                                        p_glyph,
                                        a_visible);

                                feed_screen_write(
                                    p_this->p_screen,
                                    a_visible,
                                    i_visible_length);

                                p_this->i_final_line_index =
                                    i_cursor_line_iterator;

                                p_this->i_final_glyph_index =
                                    0u;

                            }

                            i_emulated_x +=
                                feed_glyph_get_visible_width(p_glyph);

                            if (i_emulated_x >= p_this->i_screen_width)
                            {
                                i_emulated_y ++;

                                i_emulated_x -= p_this->i_screen_width;
                            }

                            p_glyph_iterator =
                                p_glyph_iterator->p_next;
                        }
                    }
                }

                if (i_cursor_line_iterator == p_this->i_cursor_line_index)
                {
                    p_this->i_cursor_visible_x =
                        i_emulated_x;

                    p_this->i_cursor_visible_y =
                        i_emulated_y;

                    p_this->b_cursor_visible =
                        1;
                }

                /* For all chars in line */
                p_glyph_iterator =
                    p_line->o_glyphs.p_next;

                i_cursor_glyph_iterator =
                    0u;

                while (
                    (
                        p_glyph_iterator
                        != &(
                            p_line->o_glyphs))
                    && (
                        i_emulated_y
                        < p_this->i_screen_height))
                {
                    struct feed_glyph const *
                        p_glyph;

                    p_glyph =
                        (struct feed_glyph const *)(
                            p_glyph_iterator);

                    if (((i_cursor_line_iterator == p_this->i_page_line_index) && (i_cursor_glyph_iterator >= p_this->i_page_glyph_index))
                        || (i_cursor_line_iterator != p_this->i_page_line_index))
                    {
                        if (!b_cursor_only)
                        {
                            unsigned char
                                a_visible[15u];

                            unsigned char
                                i_visible_length;

                            /* If char is within refresh window */
                            i_visible_length =
                                feed_glyph_render_visible(
                                    p_glyph,
                                    a_visible);

                            feed_screen_write(
                                p_screen,
                                a_visible,
                                i_visible_length);

                            p_this->i_final_line_index =
                                i_cursor_line_iterator;

                            p_this->i_final_glyph_index =
                                i_cursor_glyph_iterator;
                        }

                        i_emulated_x += feed_glyph_get_visible_width(p_glyph);
                        if (i_emulated_x >= p_this->i_screen_width)
                        {
                            i_emulated_x -= p_this->i_screen_width;
                            i_emulated_y ++;
                        }

                        if ((i_cursor_line_iterator == p_this->i_cursor_line_index)
                            && (i_cursor_glyph_iterator < p_this->i_cursor_glyph_index))
                        {
                            p_this->i_cursor_visible_x =
                                i_emulated_x;

                            p_this->i_cursor_visible_y =
                                i_emulated_y;

                            p_this->b_cursor_visible =
                                1;
                        }
                    }

                    i_cursor_glyph_iterator ++;

                    p_glyph_iterator =
                        p_glyph_iterator->p_next;
                }

                /* Was this line broken? */
            }

            i_cursor_line_iterator ++;

            p_line_iterator =
                p_line_iterator->p_next;
        }

        /* Was the line broken? */
    }

    /* Draw status line */
    if (!b_cursor_only)
    {
#if 0
        if (1)
        {
            if (
                i_cursor_line_iterator > p_this->i_page_line_index)
            {
                feed_screen_newline(p_screen);
            }

            feed_main_print_status(
                p_this,
                &(
                    p_text->o_last_event));
        }
#endif

        feed_screen_get_cursor_pos(
            p_screen,
            &(
                p_this->i_final_cursor_x),
            &(
                p_this->i_final_cursor_y));

        /* Erase old lines */
        feed_screen_clear_region(
            p_screen);
    }

    /* Position the cursor */
    if (p_this->b_cursor_visible)
    {
        feed_screen_set_cursor_pos(
            p_screen,
            p_this->i_cursor_visible_x,
            p_this->i_cursor_visible_y);
    }

    feed_tty_flush(
        p_this->p_tty);

}
#endif

static
void
feed_main_refresh_text(
    struct feed_handle * const
        p_this)
{
    feed_main_refresh_job(
        p_this,
        0);
}

static
void
feed_main_refresh_cursor(
    struct feed_handle * const
        p_this)
{
    feed_main_refresh_job(
        p_this,
        1);
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
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_HOME) == p_event->i_code)
            {
                p_this->i_cursor_glyph_index =
                    0;

                b_refresh_text = 0;

                b_refresh_cursor = 1;
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_END) == p_event->i_code)
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

                    b_refresh_cursor = 1;
                }

                b_refresh_text = 0;
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_LEFT) == p_event->i_code)
            {
                if (p_this->i_cursor_glyph_index)
                {
                    p_this->i_cursor_glyph_index --;

                    /* Adjust visible cursor position */
                    b_refresh_cursor = 1;
                }
                else
                {
                    /* Go to previous line */
                }

                b_refresh_text = 0;
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

                        /* Adjust visible cursor position */
                        b_refresh_cursor = 1;
                    }
                }

                b_refresh_text = 0;
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_UP) == p_event->i_code)
            {
                if (p_this->i_cursor_line_index)
                {
                    p_this->i_cursor_line_index --;

                    p_this->i_cursor_glyph_index = 0u;

                    if (p_this->i_cursor_line_index < p_this->i_page_line_index)
                    {
                        /* Todo: find origin of previous page */
                        p_this->i_page_line_index = p_this->i_cursor_line_index;
                    }
                    else
                    {
                        b_refresh_cursor = 1;

                        b_refresh_text = 0;
                    }
                }
                else
                {
                    b_refresh_text = 0;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_DOWN) == p_event->i_code)
            {
                if ((p_this->i_cursor_line_index + 1u) < p_text->i_line_count)
                {
                    p_this->i_cursor_line_index ++;

                    p_this->i_cursor_glyph_index = 0u;

                    /* Adjust visible cursor position */
                    if (p_this->i_cursor_line_index > p_this->i_final_line_index)
                    {
                        p_this->i_page_line_index =
                            p_this->i_cursor_line_index;

                        b_refresh_text = 1;
                    }
                    else
                    {
                        b_refresh_cursor = 1;

                        b_refresh_text = 0;
                    }
                }
                else
                {
                    b_refresh_text = 0;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_PAGEUP) == p_event->i_code)
            {
                if (p_this->i_page_line_index > p_this->i_screen_height)
                {
                    p_this->i_page_line_index -= p_this->i_screen_height;

                    if (p_this->i_cursor_line_index > p_this->i_screen_height)
                    {
                        p_this->i_cursor_line_index -= p_this->i_screen_height;
                    }
                }
                else
                {
                    if (p_this->i_cursor_line_index >= p_this->i_page_line_index)
                    {
                        p_this->i_cursor_line_index -= p_this->i_page_line_index;
                    }

                    p_this->i_page_line_index = 0u;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_PAGEDOWN) == p_event->i_code)
            {
                if ((p_this->i_page_line_index + p_this->i_screen_height) < p_this->p_text->i_line_count)
                {
                    p_this->i_page_line_index += p_this->i_screen_height;

                    p_this->i_cursor_line_index += p_this->i_screen_height;

                    if (p_this->i_cursor_line_index >= p_this->p_text->i_line_count)
                    {
                        p_this->i_cursor_line_index = (p_this->p_text->i_line_count - 1u);
                    }
                }
                else
                {
                    b_refresh_text = 0;
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

                        /* Adjust view */
                        if (p_this->i_cursor_line_index > p_this->i_final_line_index)
                        {
                            if ((p_this->i_page_line_index + p_this->i_screen_height) < p_this->p_text->i_line_count)
                            {
                                p_this->i_page_line_index += p_this->i_screen_height;

                                p_this->i_cursor_line_index += p_this->i_screen_height;

                                if (p_this->i_cursor_line_index >= p_this->p_text->i_line_count)
                                {
                                    p_this->i_cursor_line_index = (p_this->p_text->i_line_count - 1u);
                                }
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
            }

            if (b_refresh_text)
            {
                feed_main_refresh_text(
                    p_this);
            }
            else if (b_refresh_cursor)
            {
                feed_main_refresh_cursor(
                    p_this);
            }
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

                    p_this->b_page_prompt =
                        1;
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
