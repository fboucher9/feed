/* See LICENCE for license details */

/*

Module: feed_tty.h

Description:

    Basic input and output routines for tty devices.

*/

struct feed_client;

struct feed_tty
{
    struct termios
        o_termios;

    int
        i_output_file;

    int
        i_input_file;

    char
        b_enabled;

    char
        ac_padding[7u];

};

char
feed_tty_init(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty);

void
feed_tty_cleanup(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty);

char
feed_tty_enable(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty);

char
feed_tty_disable(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty);

char
feed_tty_read_character(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_value);

char
feed_tty_read_character_array(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_buf,
    unsigned int const
        i_buf_len);

char
feed_tty_write_character(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    unsigned char const
        i_value);

char
feed_tty_write_character_array(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    unsigned char const * const
        p_buf,
    unsigned int const
        i_buf_len);

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
        p_value);

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
        p_actual_len);

char
feed_tty_get_cursor_position(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int * const
        p_rows,
    int * const
        p_cols);

#define FEED_TTY_CURSOR_DIRECTION_UP 'A'
#define FEED_TTY_CURSOR_DIRECTION_DOWN 'B'
#define FEED_TTY_CURSOR_DIRECTION_RIGHT 'C'
#define FEED_TTY_CURSOR_DIRECTION_LEFT 'D'

char
feed_tty_move_cursor(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count,
    unsigned char const
        c_direction);

char
feed_tty_move_cursor_left(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count);

char
feed_tty_move_cursor_right(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count);

char
feed_tty_move_cursor_up(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count);

char
feed_tty_move_cursor_down(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count);

char
feed_tty_get_window_size(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int * const
        p_width,
    int * const
        p_height);

#define FEED_TTY_CLEAR_BOTTOM 0
#define FEED_TTY_CLEAR_TOP 1
#define FEED_TTY_CLEAR_SCREEN 2
#define FEED_TTY_CLEAR_HISTORY 3

char
feed_tty_clear(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count);

char
feed_tty_clear_bottom(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty);

char
feed_tty_clear_top(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty);

char
feed_tty_clear_screen(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty);

char
feed_tty_clear_history(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty);

/* end-of-file: feed_tty.h */
