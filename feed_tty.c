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

    int
        a_padding1[2u];

    char
        b_enabled;

    char
        ac_padding[7u];

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
                        STDOUT_FILENO;

                    p_tty->i_input_file =
                        STDIN_FILENO;

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

struct feed_tty *
feed_tty_create(
    struct feed_client * const
        p_client)
{
    struct feed_tty *
        p_tty;

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
            p_tty =
                (struct feed_tty *)(
                    feed_heap_alloc(
                        p_heap,
                        (unsigned int)(
                            sizeof(
                                struct feed_tty))));

            if (
                p_tty)
            {
                if (
                    feed_tty_init(
                        p_client,
                        p_tty))
                {
                }
                else
                {
                    feed_heap_free(
                        p_heap,
                        (void *)(
                            p_tty));

                    p_tty =
                        (struct feed_tty *)(
                            0);
                }
            }
        }
        else
        {
            p_tty =
                (struct feed_tty *)(
                    0);
        }
    }
    else
    {
        p_tty =
            (struct feed_tty *)(
                0);
    }

    return
        p_tty;

} /* feed_tty_create() */

void
feed_tty_destroy(
    struct feed_tty * const
        p_tty)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    if (
        p_tty)
    {
        p_client =
            p_tty->p_client;

        if (
            p_client)
        {
            p_heap =
                feed_client_get_heap(
                    p_client);

            if (
                p_heap)
            {
                feed_tty_cleanup(
                    p_tty);

                feed_heap_free(
                    p_heap,
                    (void *)(
                        p_tty));
            }
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
    unsigned int const
        i_buf_len)
{
    char
        b_result;

    int
        i_result;

    unsigned int
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
                (unsigned int)(
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
feed_tty_write_character(
    struct feed_tty * const
        p_tty,
    unsigned char const
        i_value)
{
    char
        b_result;

    int
        i_result;

    i_result =
        (int)(
            write(
                p_tty->i_output_file,
                &(i_value),
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
feed_tty_write_character_array(
    struct feed_tty * const
        p_tty,
    unsigned char const * const
        p_buf,
    unsigned int const
        i_buf_len)
{
    char
        b_result;

    int
        i_result;

    unsigned int
        i_index;

    b_result =
        1;

    i_index =
        0u;

    while (
        b_result
        && (
            i_index
            < i_buf_len))
    {
        i_result =
            (int)(
                write(
                    p_tty->i_output_file,
                    p_buf + i_index,
                    i_buf_len - i_index));

        if (
            i_result > 0)
        {
            i_index +=
                (unsigned int)(
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
feed_tty_read_unicode_character(
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_buf,
    unsigned int const
        i_buf_len,
    unsigned int * const
        p_actual_len,
    unsigned long int * const
        p_value)
{
    char
        b_result;

    unsigned char
        a_bytes[4u];

    unsigned int
        i_count;

    unsigned int
        i_index;

    unsigned long int
        i_value;

    i_count =
        1u;

    i_index =
        0u;

    i_value =
        0ul;

    /* Read first character */
    b_result =
        feed_tty_read_character(
            p_tty,
            a_bytes + i_index);

    if (
        b_result)
    {
        i_index ++;

        if (
            0x00u == (a_bytes[0u] & 0x80u))
        {
            i_value = (a_bytes[0u] & 0x7Fu);

            i_count =
                1u;
        }
        else if (
                0xC0u == (a_bytes[0u] & 0xE0u))
        {
            i_value = (a_bytes[0u] & 0x1Fu);

            i_count =
                2u;
        }
        else if (
            0xE0u == (a_bytes[0u] & 0xF0u))
        {
            i_value = (a_bytes[0u] & 0x0Fu);

            i_count =
                3u;
        }
        else if (
            0xF0u == (a_bytes[0u] & 0xF8u))
        {
            i_value = (a_bytes[0u] & 0x07u);

            i_count =
                4u;
        }
        else
        {
            /* is this an error? */

            i_value = (a_bytes[0u]);

            i_count =
                1u;
        }

        if (
            b_result)
        {
            while (
                b_result
                && (
                    i_index < i_count))
            {
                b_result =
                    feed_tty_read_character(
                        p_tty,
                        a_bytes + i_index);

                if (
                    b_result)
                {
                    if (0x80u == (a_bytes[i_index] & 0xC0u))
                    {
                        i_value = (i_value << 6u) | (a_bytes[i_index] & 0x3Fu);

                        i_index ++;
                    }
                    else
                    {
                        b_result =
                            0;
                    }
                }
            }
        }
    }

    if (
        b_result)
    {
        if (
            i_count <= i_buf_len)
        {
            i_index =
                0u;

            while (
                i_index < i_count)
            {
                p_buf[i_index] = a_bytes[i_index];

                i_index ++;
            }

            *(
                p_actual_len) =
                i_count;

            *(
                p_value) =
                i_value;
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
feed_tty_read_escape_sequence(
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_buf,
    unsigned int const
        i_buf_len,
    unsigned int * const
        p_actual_len)
{
    /* ESC alpha */
    /* ESC [ digit ; digit alpha */

    /* read first character */
    char
        b_result;

    unsigned int
        i_char_len;

    unsigned int
        i_buf_iterator;

    unsigned long int
        i_value;

    i_buf_iterator =
        0u;

    b_result =
        feed_tty_read_unicode_character(
            p_tty,
            p_buf + i_buf_iterator,
            i_buf_len - i_buf_iterator,
            &(
                i_char_len),
            &(
                i_value));

    if (
        b_result)
    {
        i_buf_iterator +=
            i_char_len;

        if (
            FEED_ESC_CHAR == i_value)
        {
            /* read second character */
            b_result =
                feed_tty_read_unicode_character(
                    p_tty,
                    p_buf + i_buf_iterator,
                    i_buf_len - i_buf_iterator,
                    &(
                        i_char_len),
                    &(
                        i_value));

            if (
                b_result)
            {
                i_buf_iterator +=
                    i_char_len;

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
                                p_buf + i_buf_iterator,
                                i_buf_len - i_buf_iterator,
                                &(
                                    i_char_len),
                                &(
                                    i_value));

                        if (
                            b_result)
                        {
                            i_buf_iterator +=
                                i_char_len;

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
                            p_buf + i_buf_iterator,
                            i_buf_len - i_buf_iterator,
                            &(
                                i_char_len),
                            &(
                                i_value));

                    if (
                        b_result)
                    {
                        i_buf_iterator +=
                            i_char_len;
                    }
                }
            }
        }
        else
        {
        }
    }

    if (
        b_result)
    {
        *(
            p_actual_len) =
            i_buf_iterator;
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

    unsigned char
        a_buf[32u];

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
            sizeof(
                g_escape_report_cursor_location));

    if (
        b_result)
    {
        unsigned int
            i;

        i =
            0;

        b_result =
            feed_tty_read_escape_sequence(
                p_tty,
                a_buf,
                sizeof(
                    a_buf),
                &(
                    i));

        if (
            b_result)
        {
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
        o_buf;

    b_result =
        feed_buf_init(
            &(
                o_buf),
            a_buf,
            sizeof(
                a_buf));

    if (
        b_result)
    {
        b_result =
            feed_esc_write_cup(
                &(
                    o_buf),
                i_row,
                i_col);

        if (
            b_result)
        {
            b_result =
                feed_tty_write_character_array(
                    p_tty,
                    o_buf.p_buf,
                    o_buf.i_len);
        }

        feed_buf_cleanup(
            &(
                o_buf));
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
            sizeof(
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
            b_result =
                feed_tty_write_character_array(
                    p_tty,
                    o_buf.p_buf,
                    o_buf.i_len);
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
    unsigned int * const
        p_rows,
    unsigned int * const
        p_columns)
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
            o_winsize.ws_col;

        *(
            p_rows) =
            o_winsize.ws_row;

        b_result =
            1;
    }
    /* else */
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
                            i_last_col;

                        *(
                            p_rows) =
                            i_last_row;
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
        o_buf;

    (void)(
        p_tty);

    b_result =
        feed_buf_init(
            &(
                o_buf),
            a_buf,
            sizeof(
                a_buf));

    if (
        b_result)
    {
        b_result =
            feed_esc_write_ed(
                &(
                    o_buf),
                i_count);

        if (
            b_result)
        {
            b_result =
                feed_tty_write_character_array(
                    p_tty,
                    o_buf.p_buf,
                    o_buf.i_len);
        }
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
        feed_tty_write_character_array(
            p_tty,
            a_buf,
            4u);

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
        o_buf;

    b_result =
        feed_buf_init(
            &(
                o_buf),
            a_buf,
            sizeof(
                a_buf));

    if (
        b_result)
    {
        b_result =
            feed_esc_write_el(
                &(
                    o_buf),
                i_count);

        if (
            b_result)
        {
            b_result =
                feed_tty_write_character_array(
                    p_tty,
                    o_buf.p_buf,
                    o_buf.i_len);
        }

        feed_buf_cleanup(
            &(
                o_buf));
    }

    return
        b_result;

} /* feed_tty_write_el() */

/* end-of-file: feed_tty.c */
