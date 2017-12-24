/* See LICENSE for license details */

/*

Module: feed_tty.c

Description:

    Basic input and output routines for tty devices.

*/

#include "feed_os.h"

#include "feed_client.h"

#include "feed_tty.h"

#include "feed_buf.h"

#include "feed_esc.h"

#include "feed_const.h"

#include "feed_heap.h"

#include "feed_object.h"

struct feed_tty
{
    struct feed_client *
        p_client;

    void *
        p_termios;

    int
        i_output_file;

    int
        i_input_file;

    unsigned int
        i_cache_len;

    int
        a_padding1[1u];

    char
        b_enabled;

    char
        ac_padding[7u];

    unsigned char
        a_cache[65536u];

};

static
char
feed_tty_init(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty)
{
    char b_result;

    struct feed_heap *
        p_heap;

    if (
        p_client)
    {
        if (
            p_tty)
        {
            p_heap =
                feed_client_get_heap(
                    p_client);

            if (
                p_heap)
            {
                p_tty->p_client =
                    p_client;

                p_tty->p_termios =
                    feed_heap_alloc(
                        p_heap,
                        sizeof(
                            struct termios));

                if (
                    p_tty->p_termios)
                {
                    p_tty->i_output_file =
                        STDERR_FILENO;

                    p_tty->i_input_file =
                        STDERR_FILENO;

                    p_tty->i_cache_len =
                        0u;

                    p_tty->b_enabled =
                        0;

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

    return b_result;
}

static
char
feed_tty_init_cb(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_tty * const
        p_tty =
        (struct feed_tty *)(
            p_object);

    (void)(
        p_descriptor);

    b_result =
        feed_tty_init(
            p_client,
            p_tty);

    return
        b_result;

}

static
void
feed_tty_cleanup(
    struct feed_tty * const
        p_tty)
{
    if (p_tty)
    {
        struct feed_client *
            p_client;

        p_client =
            p_tty->p_client;

        if (
            p_client)
        {
            struct feed_heap *
                p_heap;

            p_heap =
                feed_client_get_heap(
                    p_client);

            if (
                p_heap)
            {
                feed_tty_flush(
                    p_tty);

                if (p_tty->p_termios)
                {
                    feed_heap_free(
                        p_heap,
                        p_tty->p_termios);

                    p_tty->p_termios =
                        (void *)(
                            0);
                }
            }

            p_tty->p_client =
                (struct feed_client *)(
                    0);
        }
    }
}

static
void
feed_tty_cleanup_cb(
    void * const
        p_object)
{
    struct feed_tty * const
        p_tty =
        (struct feed_tty *)(
            p_object);

    feed_tty_cleanup(
        p_tty);

}

struct feed_tty *
feed_tty_create(
    struct feed_client * const
        p_client)
{
    struct feed_tty *
        p_tty;

    p_tty =
        (struct feed_tty *)(
            feed_object_create(
                p_client,
                (unsigned int)(
                    sizeof(
                        struct feed_tty)),
                &(
                    feed_tty_init_cb),
                (void const *)(
                    0)));

    return
        p_tty;

} /* feed_tty_create() */

void
feed_tty_destroy(
    struct feed_tty * const
        p_tty)
{
    if (
        p_tty)
    {
        struct feed_client * const
            p_client =
            p_tty->p_client;

        if (
            p_client)
        {
            feed_object_destroy(
                p_client,
                (void *)(
                    p_tty),
                &(
                    feed_tty_cleanup_cb));
        }
    }

} /* feed_tty_destroy() */

static
void
feed_tty_set_raw_options(
    struct termios * const
        p_termios)
{
    p_termios->c_iflag &=
        (unsigned int)(~(unsigned int)(
            BRKINT
            | ICRNL
            | INPCK
            | ISTRIP
            | IXON));

    p_termios->c_oflag &=
        (unsigned int)(~(unsigned int)(
            OPOST));

    p_termios->c_cflag |=
        CS8;

    p_termios->c_lflag &=
        (unsigned int)(~(unsigned int)(
            ECHO
            | ICANON
            | IEXTEN
            | ISIG));

    p_termios->c_cc[VMIN] =
        1;

    p_termios->c_cc[VTIME] =
        0;

}

char
feed_tty_enable(
    struct feed_tty * const
        p_tty)
{
    char
        b_result;

    struct termios
        o_termios;

    int
        i_term_status;

    if (
        isatty(
            p_tty->i_input_file))
    {
        i_term_status =
            tcgetattr(
                p_tty->i_input_file,
                (struct termios *)(
                    p_tty->p_termios));

        if (
            0 <= i_term_status)
        {
            memcpy(
                &(
                    o_termios),
                p_tty->p_termios,
                sizeof(
                    struct termios));

            feed_tty_set_raw_options(
                &(
                    o_termios));

            i_term_status =
                tcsetattr(
                    p_tty->i_input_file,
                    TCSADRAIN,
                    &(
                        o_termios));

            if (
                0 <= i_term_status)
            {
                p_tty->b_enabled =
                    1;

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
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_tty_disable(
    struct feed_tty * const
        p_tty)
{
    char
        b_result;

    if (
        p_tty->b_enabled)
    {
        int
            i_term_status;

        i_term_status =
            tcsetattr(
                p_tty->i_input_file,
                TCSADRAIN,
                (struct termios *)(
                    p_tty->p_termios));

        if (
            0 <= i_term_status)
        {
            p_tty->b_enabled =
                1;

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
            1;
    }

    return
        b_result;

}

char
feed_tty_read_character(
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_value)
{
    char
        b_result;

    int
        i_result;

    feed_tty_flush(
        p_tty);

    i_result =
        (int)(
            read(
                p_tty->i_input_file,
                p_value,
                1));

    if (
        i_result > 0)
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

char
feed_tty_read_character_array(
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    char
        b_result;

    int
        i_result;

    unsigned long int
        i_index;

    i_index =
        0u;

    b_result =
        1;

    while (
        b_result
        && (
            i_index
            < i_buf_len))
    {
        i_result =
            (int)(
                read(
                    p_tty->i_input_file,
                    p_buf + i_index,
                    i_buf_len - i_index));

        if (
            i_result > 0)
        {
            i_index +=
                (unsigned long int)(
                    i_result);
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

char
feed_tty_flush(
    struct feed_tty * const
        p_tty)
{
    char
        b_result;

    if (
        p_tty->i_cache_len)
    {
        unsigned long int
            i_index;

        b_result =
            1;

        i_index =
            0u;

        while (
            b_result
            && (
                i_index
                < p_tty->i_cache_len))
        {
            int
                i_result;

            i_result =
                (int)(
                    write(
                        p_tty->i_output_file,
                        p_tty->a_cache + i_index,
                        p_tty->i_cache_len - i_index));

            if (
                i_result > 0)
            {
                i_index +=
                    (unsigned long int)(
                        i_result);

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
            p_tty->i_cache_len =
                0;

        }
    }
    else
    {
        b_result =
            1;
    }

    return
        b_result;

}

char
feed_tty_write_character(
    struct feed_tty * const
        p_tty,
    unsigned char const
        i_value)
{
    char
        b_result;

    if (
        p_tty->i_cache_len + 1u >= sizeof(p_tty->a_cache))
    {
        b_result =
            feed_tty_flush(
                p_tty);
    }
    else
    {
        b_result =
            1;
    }

    if (
        b_result)
    {
        p_tty->a_cache[p_tty->i_cache_len] =
            i_value;

        p_tty->i_cache_len ++;
    }

    return
        b_result;

}

char
feed_tty_write_character_buf(
    struct feed_tty * const
        p_tty,
    struct feed_buf const * const
        p_buf)
{
    char
        b_result;

    struct feed_buf
        o_buf_read;

    unsigned char
        c_data;

    b_result =
        1;

    o_buf_read =
        *(
            p_buf);

    while (
        b_result
        && feed_buf_read_character(
            &(
                o_buf_read),
            &(
                c_data)))
    {
        b_result =
            feed_tty_write_character(
                p_tty,
                c_data);
    }

    return
        b_result;

}

char
feed_tty_write_character_array(
    struct feed_tty * const
        p_tty,
    unsigned char const * const
        p_buf_min,
    unsigned char const * const
        p_buf_max)
{
    char
        b_result;

    unsigned char const *
        p_buf_iterator;

    unsigned char
        c_data;

    b_result =
        1;

    p_buf_iterator =
        p_buf_min;

    while (
        b_result
        && (p_buf_iterator < p_buf_max))
    {
        c_data =
            *(
                p_buf_iterator);

        p_buf_iterator ++;

        b_result =
            feed_tty_write_character(
                p_tty,
                c_data);
    }

    return
        b_result;

}


char
feed_tty_read_unicode_character(
    struct feed_tty * const
        p_tty,
    struct feed_buf * const
        p_buf,
    unsigned long int * const
        p_value)
{
    char
        b_result;

    unsigned char
        c_data;

    unsigned long int
        i_value;

    i_value =
        0ul;

    /* Read first character */
    b_result =
        feed_tty_read_character(
            p_tty,
            &(
                c_data));

    if (
        b_result)
    {
        b_result =
            feed_buf_write_character(
                p_buf,
                c_data);

        if (
            b_result)
        {
            unsigned char
                i_count;

            if (
                0x00u == (c_data & 0x80u))
            {
                i_value = (c_data & 0x7Fu);

                i_count =
                    1u;
            }
            else if (
                    0xC0u == (c_data & 0xE0u))
            {
                i_value = (c_data & 0x1Fu);

                i_count =
                    2u;
            }
            else if (
                0xE0u == (c_data & 0xF0u))
            {
                i_value = (c_data & 0x0Fu);

                i_count =
                    3u;
            }
            else if (
                0xF0u == (c_data & 0xF8u))
            {
                i_value = (c_data & 0x07u);

                i_count =
                    4u;
            }
            else if (
                0xF8u == (c_data & 0xFCu))
            {
                i_value = (c_data & 0x03u);

                i_count =
                    5u;
            }
            else if (
                0xFCu == (c_data & 0xFEu))
            {
                i_value = (c_data & 0x01u);

                i_count =
                    6u;
            }
            else
            {
                /* is this an error? */

                i_value = (c_data);

                i_count =
                    1u;
            }

            if (
                b_result)
            {
                i_count --;

                while (
                    b_result
                    && i_count)
                {
                    b_result =
                        feed_tty_read_character(
                            p_tty,
                            &(
                                c_data));

                    if (
                        b_result)
                    {
                        b_result =
                            feed_buf_write_character(
                                p_buf,
                                c_data);

                        if (
                            b_result)
                        {
                            if (0x80u == (c_data & 0xC0u))
                            {
                                i_value = (i_value << 6u) | (c_data & 0x3Fu);
                            }
                            else
                            {
                                b_result =
                                    0;
                            }

                            i_count --;
                        }
                    }
                }
            }
        }
    }

    if (
        b_result)
    {
        *(
            p_value) =
            i_value;
    }

    return
        b_result;

}

char
feed_tty_read_escape_sequence(
    struct feed_tty * const
        p_tty,
    struct feed_buf * const
        p_buf)
{
    /* ESC alpha */
    /* ESC [ digit ; digit alpha */

    /* read first character */
    char
        b_result;

    unsigned long int
        i_value;

    b_result =
        feed_tty_read_unicode_character(
            p_tty,
            p_buf,
            &(
                i_value));

    if (
        b_result)
    {
        if (
            FEED_ESC_CHAR == i_value)
        {
            /* read second character */
            b_result =
                feed_tty_read_unicode_character(
                    p_tty,
                    p_buf,
                    &(
                        i_value));

            if (
                b_result)
            {
                if (
                    '[' == i_value)
                {
                    /* accept digits and semicolons */
                    char
                        b_more;

                    b_more =
                        1;

                    while (
                        b_result
                        && b_more)
                    {
                        b_result =
                            feed_tty_read_unicode_character(
                                p_tty,
                                p_buf,
                                &(
                                    i_value));

                        if (
                            b_result)
                        {
                            if (
                                (
                                    i_value >= 0x30)
                                && (
                                    i_value <= 0x3F))
                            {
                                b_more =
                                    1;
                            }
                            else if (
                                (
                                    i_value >= 0x40)
                                && (
                                    i_value <= 0x7F))
                            {
                                b_more =
                                    0;
                            }
                            else
                            {
                                b_result =
                                    0;
                            }
                        }
                    }
                }
                else if (
                    (
                        'O' == i_value)
                    || (
                        'N' == i_value)
                    || (
                        '(' == i_value)
                    || (
                        ')' == i_value)
                    || (
                        '#' == i_value))
                {
                    /* read one last character */
                    b_result =
                        feed_tty_read_unicode_character(
                            p_tty,
                            p_buf,
                            &(
                                i_value));
                }
            }
        }
        else
        {
        }
    }

    return
        b_result;

}

char
feed_tty_get_cursor_position(
    struct feed_tty * const
        p_tty,
    unsigned int * const
        p_rows,
    unsigned int * const
        p_cols)
{
    char
        b_result;

    unsigned int
        i_cols;

    unsigned int
        i_rows;

    int
        i_result;

    static unsigned char const g_escape_report_cursor_location [] =
    {
        FEED_ESC_CHAR,
        '[',
        '6',
        'n'
    };

    b_result =
        feed_tty_write_character_array(
            p_tty,
            g_escape_report_cursor_location,
            g_escape_report_cursor_location + sizeof(g_escape_report_cursor_location));

    if (
        b_result)
    {
        struct feed_buf
            o_buf_write;

        unsigned char
            a_buf[32u];

        feed_buf_init(
            &(
                o_buf_write),
            a_buf,
            a_buf + sizeof(a_buf));

        b_result =
            feed_tty_read_escape_sequence(
                p_tty,
                &(
                    o_buf_write));

        if (
            b_result)
        {
            unsigned int
                i;

            i =
                (unsigned int)(
                    o_buf_write.o_min.pc - a_buf);

            if (i >= 4)
            {
                if (
                    (
                        FEED_ESC_CHAR == a_buf[0u])
                    && (
                        '[' == a_buf[1u])
                    && (
                        'R' == a_buf[i-1u]))
                {
                    a_buf[i] = '\000';

                    i_rows =
                        1;

                    i_cols =
                        1;

                    i_result =
                        sscanf(
                            (char const *)(
                                a_buf + 2),
                            "%u;%u",
                            &(
                                i_rows),
                            &(
                                i_cols));

                    if (
                        2
                        == i_result)
                    {
                        *(
                            p_rows) =
                            i_rows;

                        *(
                            p_cols) =
                            i_cols;

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
            }
            else
            {
                b_result =
                    0;
            }
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

static
char
feed_tty_set_cursor_position(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_row,
    unsigned int const
        i_col)
{
    char
        b_result;

    unsigned char
        a_buf[32u];

    struct feed_buf
        o_buf_write;

    b_result =
        feed_buf_init(
            &(
                o_buf_write),
            a_buf,
            a_buf +
            sizeof(
                a_buf));

    if (
        b_result)
    {
        b_result =
            feed_esc_write_cup(
                &(
                    o_buf_write),
                i_row,
                i_col);

        if (
            b_result)
        {
            struct feed_buf
                o_buf_read;

            b_result =
                feed_buf_init(
                    &(
                        o_buf_read),
                    a_buf,
                    o_buf_write.o_min.pc);

            if (
                b_result)
            {
                b_result =
                    feed_tty_write_character_buf(
                        p_tty,
                        &(
                            o_buf_read));

                feed_buf_cleanup(
                    &(
                        o_buf_read));
            }
        }

        feed_buf_cleanup(
            &(
                o_buf_write));
    }

    return
        b_result;

}

enum feed_tty_move_cursor_direction
{
    feed_tty_move_cursor_direction_up,
    feed_tty_move_cursor_direction_down,
    feed_tty_move_cursor_direction_forward,
    feed_tty_move_cursor_direction_backward
};

static
char
feed_tty_move_cursor(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count,
    enum feed_tty_move_cursor_direction const
        e_direction)
{
    char
        b_result;

    unsigned char
        a_buf[32u];

    struct feed_buf
        o_buf;

    b_result =
        feed_buf_init(
            &(
                o_buf),
            a_buf,
            a_buf
            + sizeof(
                a_buf));

    if (b_result)
    {
        if (feed_tty_move_cursor_direction_up == e_direction)
        {
            b_result =
                feed_esc_write_cuu(
                    &(
                        o_buf),
                    i_count);
        }
        else if (feed_tty_move_cursor_direction_down == e_direction)
        {
            b_result =
                feed_esc_write_cud(
                    &(
                        o_buf),
                    i_count);
        }
        else if (feed_tty_move_cursor_direction_forward == e_direction)
        {
            b_result =
                feed_esc_write_cuf(
                    &(
                        o_buf),
                    i_count);
        }
        else if (feed_tty_move_cursor_direction_backward == e_direction)
        {
            b_result =
                feed_esc_write_cub(
                    &(
                        o_buf),
                    i_count);
        }
        else
        {
            b_result =
                0;
        }

        if (
            b_result)
        {
            struct feed_buf
                o_buf_read;

            b_result =
                feed_buf_init(
                    &(
                        o_buf_read),
                    a_buf,
                    o_buf.o_min.pc);

            if (
                b_result)
            {
                b_result =
                    feed_tty_write_character_buf(
                        p_tty,
                        &(
                            o_buf_read));

                feed_buf_cleanup(
                    &(
                        o_buf_read));
            }
        }

        feed_buf_cleanup(
            &(
                o_buf));
    }

    return
        b_result;

}

char
feed_tty_move_cursor_backward(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count)
{
    return
        feed_tty_move_cursor(
            p_tty,
            i_count,
            feed_tty_move_cursor_direction_backward);
}

char
feed_tty_move_cursor_forward(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count)
{
    return
        feed_tty_move_cursor(
            p_tty,
            i_count,
            feed_tty_move_cursor_direction_forward);
}

char
feed_tty_move_cursor_up(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count)
{
    return
        feed_tty_move_cursor(
            p_tty,
            i_count,
            feed_tty_move_cursor_direction_up);
}

char
feed_tty_move_cursor_down(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count)
{
    return
        feed_tty_move_cursor(
            p_tty,
            i_count,
            feed_tty_move_cursor_direction_down);
}

char
feed_tty_get_window_size(
    struct feed_tty * const
        p_tty,
    unsigned short int * const
        p_columns,
    unsigned short int * const
        p_rows,
    char const
        b_use_fallback)
{
    char
        b_result;

    int
        i_result;

    struct winsize
        o_winsize;

    i_result =
        ioctl(
            p_tty->i_output_file,
            TIOCGWINSZ,
            &(
                o_winsize));

    if (
        0 <= i_result)
    {
        *(
            p_columns) =
            (unsigned short int)(
                o_winsize.ws_col);

        *(
            p_rows) =
            (unsigned short int)(
                o_winsize.ws_row);

        b_result =
            1;
    }
    else if (
        b_use_fallback)
    {
        /* Try to determine terminal size using escape sequences */

        /* Save the current position */
        unsigned int
            i_save_row;

        unsigned int
            i_save_col;

        b_result =
            feed_tty_get_cursor_position(
                p_tty,
                &(
                    i_save_row),
                &(
                    i_save_col));

        if (
            b_result)
        {
            /* Move to bottom right corner */
            b_result =
                feed_tty_move_cursor_forward(
                    p_tty,
                    999);

            if (
                b_result)
            {
                b_result =
                    feed_tty_move_cursor_down(
                        p_tty,
                        999);

                if (
                    b_result)
                {
                    unsigned int
                        i_last_row;

                    unsigned int
                        i_last_col;

                    b_result =
                        feed_tty_get_cursor_position(
                            p_tty,
                            &(
                                i_last_row),
                            &(
                                i_last_col));

                    if (
                        b_result)
                    {
                        *(
                            p_columns) =
                            (unsigned short int)(
                                i_last_col);

                        *(
                            p_rows) =
                            (unsigned short int)(
                                i_last_row);
                    }
                }
            }

            /* Restore cursor position */
            b_result =
                feed_tty_set_cursor_position(
                    p_tty,
                    (unsigned int)(
                        i_save_row),
                    (unsigned int)(
                        i_save_col));
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

char
feed_tty_clear(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count)
{
    char
        b_result;

    unsigned char
        a_buf[64u];

    struct feed_buf
        o_buf_write;

    (void)(
        p_tty);

    b_result =
        feed_buf_init(
            &(
                o_buf_write),
            a_buf,
            a_buf
            + sizeof(
                a_buf));

    if (
        b_result)
    {
        b_result =
            feed_esc_write_ed(
                &(
                    o_buf_write),
                i_count);

        if (
            b_result)
        {
            struct feed_buf
                o_buf_read;

            b_result =
                feed_buf_init(
                    &(
                        o_buf_read),
                    a_buf,
                    o_buf_write.o_min.pc);

            if (
                b_result)
            {
                b_result =
                    feed_tty_write_character_buf(
                        p_tty,
                        &(
                            o_buf_read));

                feed_buf_cleanup(
                    &(
                        o_buf_read));
            }
        }

        feed_buf_cleanup(
            &(
                o_buf_write));
    }

    return
        b_result;

}

char
feed_tty_clear_bottom(
    struct feed_tty * const
        p_tty)
{
    return
        feed_tty_clear(
            p_tty,
            FEED_CSI_ED_DOWN);
}

char
feed_tty_clear_top(
    struct feed_tty * const
        p_tty)
{
    return
        feed_tty_clear(
            p_tty,
            FEED_CSI_ED_UP);
}

char
feed_tty_clear_screen(
    struct feed_tty * const
        p_tty)
{
    return
        feed_tty_clear(
            p_tty,
            FEED_CSI_ED_SCREEN);
}

char
feed_tty_clear_history(
    struct feed_tty * const
        p_tty)
{
    return
        feed_tty_clear(
            p_tty,
            FEED_CSI_ED_HISTORY);
}

char
feed_tty_line_wrap(
    struct feed_tty * const
        p_tty,
    char const
        b_enable)
{
    char
        b_result;

    unsigned char
        a_buf[4u];

    struct feed_buf
        o_buf_read;

    a_buf[0u] =
        FEED_ESC_CHAR;

    a_buf[1u] =
        '[';

    a_buf[2u] =
        '7';

    if (b_enable)
    {
        a_buf[3u] =
            'h';
    }
    else
    {
        a_buf[3u] =
            'l';
    }

    b_result =
        feed_buf_init(
            &(
                o_buf_read),
            a_buf,
            a_buf + 4u);

    if (
        b_result)
    {
        b_result =
            feed_tty_write_character_buf(
                p_tty,
                &(
                    o_buf_read));

        feed_buf_cleanup(
            &(
                o_buf_read));
    }

    return
        b_result;

}

char
feed_tty_write_el(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count)
{
    char
        b_result;

    unsigned char
        a_buf[16u];

    struct feed_buf
        o_buf_write;

    b_result =
        feed_buf_init(
            &(
                o_buf_write),
            a_buf,
            a_buf
            + sizeof(
                a_buf));

    if (
        b_result)
    {
        b_result =
            feed_esc_write_el(
                &(
                    o_buf_write),
                i_count);

        if (
            b_result)
        {
            struct feed_buf
                o_buf_read;

            b_result =
                feed_buf_init(
                    &(
                        o_buf_read),
                    a_buf,
                    o_buf_write.o_min.pc);

            if (
                b_result)
            {
                b_result =
                    feed_tty_write_character_buf(
                        p_tty,
                        &(
                            o_buf_read));

                feed_buf_cleanup(
                    &(
                        o_buf_read));
            }
        }

        feed_buf_cleanup(
            &(
                o_buf_write));
    }

    return
        b_result;

} /* feed_tty_write_el() */

char
feed_tty_write_sgr(
    struct feed_tty * const
        p_tty,
    unsigned char const * const
        p_attr,
    unsigned int const
        i_count)
{
    char
        b_result;

    unsigned char
        a_buf[64u];

    struct feed_buf
        o_buf_write;

    b_result =
        feed_buf_init(
            &(
                o_buf_write),
            a_buf,
            a_buf
            + sizeof(
                a_buf));

    if (
        b_result)
    {
        b_result =
            feed_esc_write_sgr(
                &(
                    o_buf_write),
                p_attr,
                i_count);

        if (
            b_result)
        {
            struct feed_buf
                o_buf_read;

            b_result =
                feed_buf_init(
                    &(
                        o_buf_read),
                    a_buf,
                    o_buf_write.o_min.pc);

            if (
                b_result)
            {
                b_result =
                    feed_tty_write_character_buf(
                        p_tty,
                        &(
                            o_buf_read));

                feed_buf_cleanup(
                    &(
                        o_buf_read));
            }
        }

        feed_buf_cleanup(
            &(
                o_buf_write));
    }

    return
        b_result;

}

/* end-of-file: feed_tty.c */
