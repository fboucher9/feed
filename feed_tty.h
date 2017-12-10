/* See LICENCE for license details */

/*

Module: feed_tty.h

Description:

    Basic input and output routines for tty devices.

*/

struct feed_client;

struct feed_tty;

struct feed_buf;

struct feed_buf_const;

struct feed_tty *
feed_tty_create(
    struct feed_client * const
        p_client);

void
feed_tty_destroy(
    struct feed_tty * const
        p_tty);

char
feed_tty_enable(
    struct feed_tty * const
        p_tty);

char
feed_tty_disable(
    struct feed_tty * const
        p_tty);

char
feed_tty_flush(
    struct feed_tty * const
        p_tty);

char
feed_tty_read_character(
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_value);

char
feed_tty_read_character_array(
    struct feed_tty * const
        p_tty,
    unsigned char * const
        p_buf,
    unsigned long int const
        i_buf_len);

char
feed_tty_write_character(
    struct feed_tty * const
        p_tty,
    unsigned char const
        i_value);

char
feed_tty_write_character_buf(
    struct feed_tty * const
        p_tty,
    struct feed_buf_const * const
        p_buf);

char
feed_tty_write_character_array(
    struct feed_tty * const
        p_tty,
    unsigned char const * const
        p_buf_min,
    unsigned char const * const
        p_buf_max);

char
feed_tty_read_unicode_character(
    struct feed_tty * const
        p_tty,
    struct feed_buf * const
        p_buf,
    unsigned long int * const
        p_value);

char
feed_tty_read_escape_sequence(
    struct feed_tty * const
        p_tty,
    struct feed_buf * const
        p_buf);

char
feed_tty_get_cursor_position(
    struct feed_tty * const
        p_tty,
    unsigned int * const
        p_rows,
    unsigned int * const
        p_cols);

char
feed_tty_move_cursor_backward(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count);

char
feed_tty_move_cursor_forward(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count);

char
feed_tty_move_cursor_up(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count);

char
feed_tty_move_cursor_down(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count);

char
feed_tty_get_window_size(
    struct feed_tty * const
        p_tty,
    unsigned short int * const
        p_columns,
    unsigned short int * const
        p_rows,
    char const
        b_use_fallback);

/* use FEED_CSI_ED_ constants for count */
char
feed_tty_clear(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count);

char
feed_tty_clear_bottom(
    struct feed_tty * const
        p_tty);

char
feed_tty_clear_top(
    struct feed_tty * const
        p_tty);

char
feed_tty_clear_screen(
    struct feed_tty * const
        p_tty);

char
feed_tty_clear_history(
    struct feed_tty * const
        p_tty);

char
feed_tty_line_wrap(
    struct feed_tty * const
        p_tty,
    char const
        b_enable);

char
feed_tty_write_el(
    struct feed_tty * const
        p_tty,
    unsigned int const
        i_count);

char
feed_tty_write_sgr(
    struct feed_tty * const
        p_tty,
    unsigned char const * const
        p_attr,
    unsigned int const
        i_count);

/* end-of-file: feed_tty.h */
