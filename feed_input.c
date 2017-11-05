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

/* Table of key codes */
#include "feed_keys.h"

/* Buffer */
#include "feed_buf.h"

/* Object */
#include "feed_object.h"

/* Utf8 */
#include "feed_utf8.h"

/*

Enumeration: feed_input_state

Description:

    States for tty input parser.

*/
enum feed_input_state
{
    feed_input_state_idle = 1,

    feed_input_state_escape,

    feed_input_state_escape_csi,

    feed_input_state_escape_ssx

}; /* enum feed_input_state */

struct feed_input
{
    struct feed_utf8_parser
        o_utf8_parser;

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

}; /* struct feed_input */

static
char
feed_input_init(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_input * const
        p_input =
        (struct feed_input *)(
            p_object);

    if (
        p_input)
    {
        (void)(
            p_descriptor);

        if (
            feed_utf8_parser_init(
                &(
                    p_input->o_utf8_parser)))
        {
            p_input->p_client =
                p_client;

            p_input->e_state =
                feed_input_state_idle;

            b_result =
                1;
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
            0;
    }

    return
        b_result;

}

struct feed_input *
feed_input_create(
    struct feed_client * const
        p_client)
{
    struct feed_input *
        p_input;

    p_input =
        (struct feed_input *)(
            feed_object_create(
                p_client,
                (unsigned int)(
                    sizeof(
                        struct feed_input)),
                &(
                    feed_input_init),
                NULL));

    return
        p_input;

} /* feed_input_create() */

static
void
feed_input_cleanup(
    void * const
        p_object)
{
    struct feed_input * const
        p_input =
        (struct feed_input *)(
            p_object);

    feed_utf8_parser_cleanup(
        &(
            p_input->o_utf8_parser));

    p_input->p_client =
        (struct feed_client *)(
            0);

}

void
feed_input_destroy(
    struct feed_input * const
        p_input)
{
    if (
        p_input)
    {
        struct feed_client *
            p_client;

        p_client =
            p_input->p_client;

        if (
            p_client)
        {
            feed_object_destroy(
                p_client,
                (void *)(
                    p_input),
                &(
                    feed_input_cleanup));
        }
    }
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
int
feed_input_notify(
    struct feed_input * const
        p_input)
{
    feed_input_lookup(
        p_input);

#if 0
    (*(p_input->p_callback))(
        p_input->p_context,
        &(
            p_input->o_event));
#endif

    p_input->e_state =
        feed_input_state_idle;

    return
        1;

}

static
char
feed_input_store_data(
    struct feed_input * const
        p_input,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    char
        b_result;

    if (
        feed_input_state_idle
        == p_input->e_state)
    {
        memcpy(
            p_input->o_event.a_raw,
            p_utf8_code->a_raw,
            p_utf8_code->i_raw_len);

        p_input->o_event.i_raw_len =
            p_utf8_code->i_raw_len;

        p_input->o_event.i_code =
            0ul;

        b_result =
            1;
    }
    else
    {
        if (p_input->o_event.i_raw_len + p_utf8_code->i_raw_len < 30u)
        {
            memcpy(
                p_input->o_event.a_raw + p_input->o_event.i_raw_len,
                p_utf8_code->a_raw,
                p_utf8_code->i_raw_len);

            p_input->o_event.i_raw_len =
                (unsigned char)(
                    p_input->o_event.i_raw_len
                    + p_utf8_code->i_raw_len);

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
int
feed_input_process_idle_state(
    struct feed_input * const
        p_input,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    int
        i_result;

    if (27ul == p_utf8_code->i_code)
    {
        p_input->e_state =
            feed_input_state_escape;

        i_result =
            0;
    }
    else
    {
        p_input->o_event.i_code =
            p_utf8_code->i_code;

        i_result =
            feed_input_notify(
                p_input);
    }

    return
        i_result;

}

static
int
feed_input_process_escape_state(
    struct feed_input * const
        p_input,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    int
        i_result;

    if (
        '[' == p_utf8_code->i_code)
    {
        p_input->e_state =
            feed_input_state_escape_csi;

        i_result =
            0;
    }
    else if (
        (
            'O' == p_utf8_code->i_code)
        || (
            'N' == p_utf8_code->i_code))
    {
        p_input->e_state =
            feed_input_state_escape_ssx;

        i_result =
            0;
    }
    else
    {
        /* Lookup for a key */
        p_input->o_event.i_code =
            FEED_EVENT_KEY_FLAG;

        /* Notify */
        i_result =
            feed_input_notify(
                p_input);
    }

    return
        i_result;

}

static
int
feed_input_process_csi_state(
    struct feed_input * const
        p_input,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    int
        i_result;

    if (0x40u <= p_utf8_code->i_code)
    {
        p_input->o_event.i_code =
            FEED_EVENT_KEY_FLAG;

        /* Notify */
        i_result =
            feed_input_notify(
                p_input);
    }
    else
    {
        i_result =
            0;
    }

    return
        i_result;

}

static
int
feed_input_process_ssx_state(
    struct feed_input * const
        p_input,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    int
        i_result;

    (void)(
        p_utf8_code);

    p_input->o_event.i_code =
        FEED_EVENT_KEY_FLAG;

    /* Notify */
    i_result =
        feed_input_notify(
            p_input);

    return
        i_result;

}

static
int
feed_input_process_data(
    struct feed_input * const
        p_input,
    struct feed_utf8_code const * const
        p_utf8_code)
{
    int
        i_result;

    if (
        feed_input_state_idle
        == p_input->e_state)
    {
        i_result =
            feed_input_process_idle_state(
                p_input,
                p_utf8_code);
    }
    else if (
        feed_input_state_escape
        == p_input->e_state)
    {
        i_result =
            feed_input_process_escape_state(
                p_input,
                p_utf8_code);
    }
    else if (
        feed_input_state_escape_csi
        == p_input->e_state)
    {
        i_result =
            feed_input_process_csi_state(
                p_input,
                p_utf8_code);
    }
    else if (
        feed_input_state_escape_ssx
        == p_input->e_state)
    {
        i_result =
            feed_input_process_ssx_state(
                p_input,
                p_utf8_code);
    }
    else
    {
        p_input->e_state =
            feed_input_state_idle;

        i_result =
            -1;
    }

    return
        i_result;

}

int
feed_input_write(
    struct feed_input * const
        p_input,
    unsigned char const
        c_data,
    struct feed_event * const
        p_event)
{
    int
        i_result;

    struct feed_utf8_code
        o_data;

    /* Write char into utf8 parser */
    i_result =
        feed_utf8_parser_write(
            &(
                p_input->o_utf8_parser),
            c_data,
            &(
                o_data));

    if (
        0
        <= i_result)
    {
        if (
            0
            < i_result)
        {
            /* Process of resulting code */

            /* Store into raw */
            if (
                feed_input_store_data(
                    p_input,
                    &(
                        o_data)))
            {
                i_result =
                    feed_input_process_data(
                        p_input,
                        &(
                            o_data));

                if (
                    0
                    < i_result)
                {
                    *(p_event) =
                        p_input->o_event;
                }
            }
            else
            {
                i_result =
                    -1;
            }
        }
    }

    return
        i_result;

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
    else if (p_event->i_code < FEED_EVENT_KEY_FLAG)
    {
        feed_buf_write_character_array(
            p_buf,
            p_event->a_raw,
            p_event->i_raw_len);
    }
    else if (FEED_EVENT_KEY_FLAG == p_event->i_code)
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
    else if (FEED_EVENT_KEY_FLAG & p_event->i_code)
    {
        feed_keys_print(
            p_event->i_code,
            p_buf);
    }

} /* feed_input_print() */

/* end-of-file: feed_input.c */
