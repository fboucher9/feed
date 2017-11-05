/* See LICENSE for license details */

/*

Module: feed_text.c

Description:

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_input.h"

#include "feed_text.h"

#include "feed_line.h"

#include "feed_object.h"

#include "feed_buf.h"

static
char
feed_text_init(
    struct feed_text * const
        p_text,
    struct feed_client * const
        p_client)
{
    char
        b_result;

    memset(
        p_text,
        0x00u,
        sizeof(
            struct feed_text));

    p_text->p_client =
        p_client;

    feed_list_init(
        &(
            p_text->o_lines));

    p_text->i_line_count =
        0;

    {
        struct feed_line *
            p_line;

        /* Create a line */
        p_line =
            feed_line_create(
                p_client);

        /* Store the line into the list */
        if (p_line)
        {
            feed_list_join(
                &(
                    p_line->o_list),
                &(
                    p_text->o_lines));

            p_text->i_line_count ++;

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }

    return
        b_result;

}

static
char
feed_text_init_cb(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_text * const
        p_text =
        (struct feed_text *)(
            p_object);

    (void)(
        p_descriptor);

    b_result =
        feed_text_init(
            p_text,
            p_client);

    return
        b_result;

}


static
void
feed_text_cleanup(
    struct feed_text * const
        p_text)
{
    /* Delete all the lines */
    struct feed_list *
        p_iterator;

    p_iterator =
        p_text->o_lines.p_next;

    while (
        p_iterator
        != &(
            p_text->o_lines))
    {
        struct feed_list *
            p_next;

        struct feed_line *
            p_line;

        p_next =
            p_iterator->p_next;

        p_line =
            (struct feed_line *)(
                p_iterator);

        feed_line_destroy(
            p_line);

        p_iterator =
            p_next;

    }

} /* feed_text_cleanup() */

static
void
feed_text_cleanup_cb(
    void * const
        p_object)
{
    struct feed_text * const
        p_text =
        (struct feed_text *)(
            p_object);

    feed_text_cleanup(
        p_text);

}

struct feed_text *
feed_text_create(
    struct feed_client * const
        p_client)
{
    struct feed_text *
        p_text;

    p_text =
        (struct feed_text *)(
            feed_object_create(
                p_client,
                sizeof(
                    struct feed_text),
                &(
                    feed_text_init_cb),
                (void const *)(
                    0)));

    return
        p_text;

}

void
feed_text_destroy(
    struct feed_text * const
        p_text)
{
    if (
        p_text)
    {
        struct feed_client * const
            p_client =
            p_text->p_client;

        if (
            p_client)
        {
            feed_object_destroy(
                p_client,
                (void *)(
                    p_text),
                &(
                    feed_text_cleanup_cb));
        }
    }
}

struct feed_line *
feed_text_get_line(
    struct feed_text * const
        p_text,
    unsigned int const
        i_line_index)
{
    struct feed_line *
        p_line;

    struct feed_list *
        p_iterator;

    unsigned int
        i_line_iterator;

    i_line_iterator =
        0u;

    p_iterator =
        p_text->o_lines.p_next;

    while (
        (
            i_line_iterator < i_line_index)
        && (
            p_iterator != &(p_text->o_lines)))
    {
        i_line_iterator ++;

        p_iterator =
            p_iterator->p_next;
    }

    if (
        p_iterator != &(p_text->o_lines))
    {
        p_line =
            (struct feed_line *)(
                p_iterator);
    }
    else
    {
        p_line =
            (struct feed_line *)(
                0);
    }

    return
        p_line;

}

void
feed_text_append_event(
    struct feed_text * const
        p_text,
    struct feed_event const * const
        p_event)
{
    struct feed_line *
        p_line;

    p_line =
        feed_text_get_line(
            p_text,
            p_text->i_line_count - 1u);

    if (
        p_line)
    {
        feed_line_append_event(
            p_line,
            p_event);
    }

}

void
feed_text_write_event(
    struct feed_text * const
        p_text,
    struct feed_event const * const
        p_event,
    unsigned int const
        i_line_index,
    unsigned int const
        i_glyph_index)
{
    struct feed_line *
        p_line;

    p_line =
        feed_text_get_line(
            p_text,
            i_line_index);

    if (
        p_line)
    {
        feed_line_write_event(
            p_line,
            p_event,
            i_glyph_index);
    }

}

unsigned int
feed_text_get_raw_length(
    struct feed_text * const
        p_text)
{
    unsigned int
        i_buf_len;

    struct feed_list *
        p_iterator;

    i_buf_len =
        0u;

    p_iterator =
        p_text->o_lines.p_next;

    while (
        p_iterator
        != &(p_text->o_lines))
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_iterator);

        i_buf_len +=
            feed_line_get_raw_length(
                p_line);

        p_iterator =
            p_iterator->p_next;
    }

    return
        i_buf_len;

}

void
feed_text_get_raw_buffer(
    struct feed_text * const
        p_text,
    struct feed_buf * const
        p_buf)
{
    struct feed_list *
        p_iterator;

    p_iterator =
        p_text->o_lines.p_next;

    while (
        p_iterator
        != &(p_text->o_lines))
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_iterator);

        feed_line_get_raw_buffer(
            p_line,
            p_buf);

        p_iterator =
            p_iterator->p_next;
    }

}

static
void
feed_text_set_callback(
    void * const
        p_context,
    struct feed_event const * const
        p_event)
{
    if (
        p_context)
    {
        struct feed_text *
            p_text;

        p_text =
            (struct feed_text *)(
                p_context);

        feed_text_append_event(
            p_text,
            p_event);
    }

} /* feed_prompt_set_callback() */

char
feed_text_set(
    struct feed_text * const
        p_text,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length)
{
    char
        b_result;

    if (p_text)
    {
        /* Go to end of buffer */

        if (
            i_data_length)
        {
            struct feed_input *
                p_input;

            p_input =
                feed_input_create(
                    p_text->p_client);

            if (
                p_input)
            {
                unsigned int
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
                    if ('\n' == p_data[i_data_iterator])
                    {
                        struct feed_line *
                            p_line;

                        p_line =
                            feed_line_create(
                                p_text->p_client);

                        if (
                            p_line)
                        {
                            feed_list_join(
                                &(
                                    p_line->o_list),
                                &(
                                    p_text->o_lines));

                            p_text->i_line_count ++;

                            i_data_iterator ++;
                        }
                        else
                        {
                            b_result =
                                0;
                        }
                    }
                    else
                    {
                        int
                            i_result;

                        struct feed_event
                            o_event;

                        i_result =
                            feed_input_write(
                                p_input,
                                p_data[i_data_iterator],
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
                                feed_text_set_callback(
                                    p_text,
                                    &(
                                        o_event));
                            }

                            i_data_iterator ++;
                        }
                        else
                        {
                            b_result =
                                0;
                        }
                    }
                }

                feed_input_destroy(
                    p_input);
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

struct feed_line *
feed_text_append_line(
    struct feed_text * const
        p_text)
{
    struct feed_line *
        p_line;

    p_line =
        feed_line_create(
            p_text->p_client);

    if (
        p_line)
    {
        feed_list_join(
            &(
                p_line->o_list),
            &(
                p_text->o_lines));

        p_text->i_line_count ++;
    }

    return
        p_line;

}

/* end-of-file: feed_text.c */
