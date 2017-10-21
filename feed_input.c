/* See LICENSE for license details */

/*

Module: feed_input.c

Description:

    Parser for tty input, convert input raw data to keyboard events.

*/

/* Operating system specific header files */
#include "feed_os.h"

/* Module */
#include "feed_input.h"

/* Client context */
#include "feed_client.h"

/* Heap */
#include "feed_heap.h"

/* Table of key codes */
#include "feed_keys.h"

/* Buffer */
#include "feed_buf.h"

/*

Enumeration: feed_input_state

Description:

    States for tty input parser.

*/
enum feed_input_state
{
    feed_input_state_idle = 1,

    feed_input_state_unicode,

    feed_input_state_escape,

    feed_input_state_escape_csi,

    feed_input_state_escape_ssx

}; /* enum feed_input_state */

struct feed_input
{
    struct feed_client *
        p_client;

    void *
        a_padding1[1u];

    struct feed_event
        o_event;

    enum feed_input_state
        e_state;

    unsigned int
        ai_padding[3u];

    unsigned char
        i_count;

    unsigned char
        ac_padding[7u];

}; /* struct feed_input */

struct feed_input *
feed_input_create(
    struct feed_client * const
        p_client)
{
    struct feed_input *
        p_input;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_input =
        (struct feed_input *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_input)));

    if (
        p_input)
    {
        p_input->p_client =
            p_client;

        p_input->e_state =
            feed_input_state_idle;

    }

    return
        p_input;

} /* feed_input_create() */

void
feed_input_destroy(
    struct feed_input * const
        p_input)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_input->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_input->p_client =
        (struct feed_client *)(
            0);

    feed_heap_free(
        p_heap,
        p_input);

}

static
void
feed_input_lookup(
    struct feed_input * const
        p_input)
{
    unsigned long int
        i_code;

    i_code =
        feed_keys_lookup(
            p_input->o_event.a_raw,
            p_input->o_event.i_raw_len);

    if (
        i_code)
    {
        p_input->o_event.i_code =
            i_code;
    }
}

static
void
feed_input_notify(
    struct feed_input * const
        p_input,
    void (* const p_callback)(
        void * const
            p_context,
        struct feed_event const * const
            p_event),
    void * const
        p_context)
{
    (*(p_callback))(
        p_context,
        &(
            p_input->o_event));
}

char
feed_input_write(
    struct feed_input * const
        p_input,
    unsigned char const
        c_data,
    void (* const p_callback)(
        void * const
            p_context,
        struct feed_event const * const
            p_event),
    void * const
        p_context)
{
    char
        b_result;

    /* Store into raw */
    if (
        feed_input_state_idle
        == p_input->e_state)
    {
        p_input->o_event.a_raw[0u] =
            c_data;

        p_input->o_event.i_raw_len =
            1u;

        p_input->o_event.i_code =
            0ul;

        b_result =
            1;
    }
    else
    {
        if (p_input->o_event.i_raw_len < 30u)
        {
            p_input->o_event.a_raw[p_input->o_event.i_raw_len] =
                c_data;

            p_input->o_event.i_raw_len ++;

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }

    if (
        b_result)
    {
        if (
            feed_input_state_idle
            == p_input->e_state)
        {
            if (0xC0u <= c_data)
            {
                if (0xC0u == (c_data & 0xE0u))
                {
                    p_input->e_state =
                        feed_input_state_unicode;

                    p_input->o_event.i_code =
                        (unsigned long int)(
                            c_data & 0x1Fu);

                    p_input->i_count =
                        2u;

                    b_result =
                        1;
                }
                else if (0xE0u == (c_data & 0xF0u))
                {
                    p_input->e_state =
                        feed_input_state_unicode;

                    p_input->o_event.i_code =
                        (unsigned long int)(
                            c_data & 0x0Fu);

                    p_input->i_count =
                        3u;

                    b_result =
                        1;
                }
                else if (0xF0u == (c_data & 0xF8u))
                {
                    p_input->e_state =
                        feed_input_state_unicode;

                    p_input->o_event.i_code =
                        (unsigned long int)(
                            c_data & 0x07u);

                    p_input->i_count =
                        4u;

                    b_result =
                        1;
                }
                else
                {
                    p_input->e_state =
                        feed_input_state_idle;

                    b_result =
                        0;
                }
            }
            else if (27u == c_data)
            {
                p_input->e_state =
                    feed_input_state_escape;

                b_result =
                    1;
            }
            else
            {
                p_input->o_event.i_code =
                    (unsigned long int)(
                        c_data);

                feed_input_lookup(
                    p_input);

                feed_input_notify(
                    p_input,
                    p_callback,
                    p_context);

                p_input->e_state =
                    feed_input_state_idle;

                b_result =
                    1;
            }
        }
        else if (
            feed_input_state_unicode
            == p_input->e_state)
        {
            if (0x80u == (c_data & 0xC0u))
            {
                p_input->o_event.i_code =
                    (unsigned long int)(
                        (
                            p_input->o_event.i_code << 6u)
                        | (unsigned long int)(
                            c_data & 0x3Fu));

                if (
                    p_input->o_event.i_raw_len
                    == p_input->i_count)
                {
                    /* Notify */
                    feed_input_lookup(
                        p_input);

                    feed_input_notify(
                        p_input,
                        p_callback,
                        p_context);

                    p_input->e_state =
                        feed_input_state_idle;
                }

                b_result = 1;
            }
            else
            {
                p_input->e_state =
                    feed_input_state_idle;

                b_result =
                    0;
            }
        }
        else if (
            feed_input_state_escape
            == p_input->e_state)
        {
            if (
                '[' == c_data)
            {
                p_input->e_state =
                    feed_input_state_escape_csi;

                b_result =
                    1;
            }
            else if (
                (
                    'O' == c_data)
                || (
                    'N' == c_data))
            {
                p_input->e_state =
                    feed_input_state_escape_ssx;

                b_result =
                    1;
            }
            else
            {
                /* Lookup for a key */
                p_input->o_event.i_code =
                    0x80000000ul;

                feed_input_lookup(
                    p_input);

                /* Notify */
                feed_input_notify(
                    p_input,
                    p_callback,
                    p_context);

                p_input->e_state =
                    feed_input_state_idle;

                b_result =
                    1;
            }
        }
        else if (
            feed_input_state_escape_csi
            == p_input->e_state)
        {
            if (0x40u <= c_data)
            {
                p_input->o_event.i_code =
                    0x80000000ul;

                /* Lookup for a key */
                feed_input_lookup(
                    p_input);

                /* Notify */
                feed_input_notify(
                    p_input,
                    p_callback,
                    p_context);

                p_input->e_state =
                    feed_input_state_idle;

                b_result =
                    1;
            }
            else
            {
                b_result =
                    1;
            }
        }
        else if (
            feed_input_state_escape_ssx
            == p_input->e_state)
        {
            p_input->o_event.i_code =
                0x80000000ul;

            /* Lookup for a key */
            feed_input_lookup(
                p_input);

            /* Notify */
            feed_input_notify(
                p_input,
                p_callback,
                p_context);

            p_input->e_state =
                feed_input_state_idle;

            b_result =
                1;
        }
        else
        {
            p_input->e_state =
                feed_input_state_idle;

            b_result =
                0;
        }
    }

    return
        b_result;

}

void
feed_input_print(
    struct feed_event const * const
        p_event,
    struct feed_buf * const
        p_buf)
{
    if (p_event->i_code < 32)
    {
        static unsigned char const g_feed_input_ctrl_table[32u] =
        {
            '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
            'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
            'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
            'X', 'Y', 'Z', '[', '\\', ']', '^', '_'
        };

        feed_buf_write_character(
            p_buf,
            'C');

        feed_buf_write_character(
            p_buf,
            '-');

        feed_buf_write_character(
            p_buf,
            g_feed_input_ctrl_table[
                p_event->i_code]);
    }
    else if (p_event->i_code < 0x80000000ul)
    {
        feed_buf_write_character_array(
            p_buf,
            p_event->a_raw,
            p_event->i_raw_len);
    }
    else if (0x80000000ul == p_event->i_code)
    {
        /* code that has no key equivalent */
        if ((2u == p_event->i_raw_len)
            && (27 == p_event->a_raw[0u]))
        {
            feed_buf_write_character(
                p_buf,
                'A');

            feed_buf_write_character(
                p_buf,
                '-');

            if (p_event->a_raw[1u] < 32u)
            {
                feed_buf_write_character(
                    p_buf,
                    'C');

                feed_buf_write_character(
                    p_buf,
                    '-');

                feed_buf_write_character(
                    p_buf,
                    (unsigned char)(
                        '@' + p_event->a_raw[1u]));
            }
            else if ((p_event->a_raw[1u] >= 'a') && (p_event->a_raw[1u] <= 'z'))
            {
                feed_buf_write_character(
                    p_buf,
                    'C');

                feed_buf_write_character(
                    p_buf,
                    '-');

                feed_buf_write_character(
                    p_buf,
                    (unsigned char)(
                        p_event->a_raw[1u] + 'A' - 'a'));
            }
            else
            {
                feed_buf_write_character(
                    p_buf,
                    p_event->a_raw[1u]);
            }
        }
    }
    else if (0x80000000ul & p_event->i_code)
    {
        feed_keys_print(
            p_event->i_code,
            p_buf);
    }

} /* feed_input_print() */

/* end-of-file: feed_input.c */
