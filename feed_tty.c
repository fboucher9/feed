#include "feed_os.h"

#include "feed_client.h"

#include "feed_tty.h"

#include "feed_buf.h"

char
feed_tty_init(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty)
{
    char b_result;

    (void)(p_client);

    p_tty->i_output_file =
        STDOUT_FILENO;

    p_tty->i_input_file =
        STDIN_FILENO;

    p_tty->b_enabled =
        0;

    b_result = 1;

    return b_result;
}

void
feed_tty_cleanup(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty)
{
    (void)(
        p_client);
    (void)(
        p_tty);
}

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
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty)
{
    char
        b_result;

    struct termios
        o_termios;

    int
        i_term_status;

    (void)(
        p_client);

    if (
        isatty(
            p_tty->i_input_file))
    {
        i_term_status =
            tcgetattr(
                p_tty->i_input_file,
                &(
                    p_tty->o_termios));

        if (
            0 <= i_term_status)
        {
            o_termios =
                p_tty->o_termios;

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
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty)
{
    char
        b_result;

    (void)(
        p_client);

    if (
        p_tty->b_enabled)
    {
        int
            i_term_status;

        i_term_status =
            tcsetattr(
                p_tty->i_input_file,
                TCSADRAIN,
                &(
                    p_tty->o_termios));

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
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_value)
{
    char
        b_result;

    int
        i_result;

    (void)(
        p_client);

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
    struct feed_client * const
        p_client,
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

    (void)(
        p_client);

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
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    unsigned char const
        i_value)
{
    char
        b_result;

    int
        i_result;

    (void)(
        p_client);

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
    struct feed_client * const
        p_client,
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

    (void)(
        p_client);

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
    struct feed_client * const
        p_client,
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
            p_client,
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
                        p_client,
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
    struct feed_client * const
        p_client,
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
            p_client,
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
            FEED_TTY_ESC_CHAR == i_value)
        {
            /* read second character */
            b_result =
                feed_tty_read_unicode_character(
                    p_client,
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
                                p_client,
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
                                    (
                                        i_value >= '0')
                                    && (
                                        i_value <= '9'))
                                || (
                                    i_value == ';'))
                            {
                                b_more =
                                    1;
                            }
                            else
                            {
                                b_more =
                                    0;
                            }
                        }
                    }
                }
                else if (
                    (
                        'O' == i_value)
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
                            p_client,
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
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int * const
        p_rows,
    int * const
        p_cols)
{
    char
        b_result;

    int
        i_cols;

    int
        i_rows;

    int
        i_result;

    char
        a_buf[32u];

    static char const g_escape_report_cursor_location [] =
    {
        FEED_TTY_ESC_CHAR,
        '[',
        '6',
        'n'
    };

    (void)(
        p_client);

    i_result =
        (int)(write(
            p_tty->i_output_file,
            g_escape_report_cursor_location,
            sizeof(
                g_escape_report_cursor_location)));

    if (
        sizeof(
            g_escape_report_cursor_location)
        == i_result)
    {
        int
            i;

        i =
            0;

        while (
            i < (int)(sizeof(a_buf)))
        {
            i_result =
                (int)(read(
                    p_tty->i_input_file,
                    a_buf + i,
                    1));
            if (
                i_result > 0)
            {
                if (
                    'R' == a_buf[i])
                {
                    break;
                }
                else
                {
                    i++;
                }
            }
            else
            {
                break;
            }
        }

        a_buf[i] = '\000';

        if (i >= 3)
        {
            if (
                (
                    FEED_TTY_ESC_CHAR == a_buf[0u])
                && (
                    '[' == a_buf[1u]))
            {
                i_result =
                    sscanf(
                        a_buf + 2,
                        "%d;%d",
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
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_tty_move_cursor(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count,
    char const
        c_direction)
{
    char
        b_result;

    char
        a_buf[32u];

    int
        i_result;

    struct feed_buf
        o_buf;

    (void)(
        p_client);

    b_result =
        feed_buf_init(
            &(
                o_buf),
            a_buf,
            sizeof(
                a_buf));

    if (b_result)
    {
        b_result =
            feed_buf_encode_move_cursor(
                &(
                    o_buf),
                i_count,
                c_direction);

        if (
            b_result)
        {
            i_result =
                (int)(
                    write(
                        p_tty->i_output_file,
                        o_buf.p_buf,
                        o_buf.i_len));

            if (
                0 <= i_result)
            {
                b_result =
                    1;
            }
            else
            {
                b_result =
                    0;
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
feed_tty_move_cursor_left(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count)
{
    return
        feed_tty_move_cursor(
            p_client,
            p_tty,
            i_count,
            FEED_TTY_CURSOR_DIRECTION_LEFT);
}

char
feed_tty_move_cursor_right(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count)
{
    return
        feed_tty_move_cursor(
            p_client,
            p_tty,
            i_count,
            FEED_TTY_CURSOR_DIRECTION_RIGHT);
}

char
feed_tty_move_cursor_up(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count)
{
    return
        feed_tty_move_cursor(
            p_client,
            p_tty,
            i_count,
            FEED_TTY_CURSOR_DIRECTION_UP);
}

char
feed_tty_move_cursor_down(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count)
{
    return
        feed_tty_move_cursor(
            p_client,
            p_tty,
            i_count,
            FEED_TTY_CURSOR_DIRECTION_DOWN);
}

char
feed_tty_get_window_size(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int * const
        p_width,
    int * const
        p_height)
{
    char
        b_result;

    int
        i_result;

    struct winsize
        o_winsize;

    (void)(
        p_client);

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
            p_width) =
            o_winsize.ws_col;

        *(
            p_height) =
            o_winsize.ws_row;

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

