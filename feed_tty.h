/* See LICENCE for license details */

/*

Module: feed_tty.h

Description:

    Basic input and output routines for tty devices.

*/

struct feed_client;

struct feed_tty
{
    union termios_aligned
    {
        struct termios
            o_termios;

        unsigned char
            a_aligned[(sizeof(struct termios) + 7u) & ~7u];

    } u;

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

char
feed_tty_move_cursor_backward(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    int const
        i_count);

char
feed_tty_move_cursor_forward(
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

/* use FEED_CSI_ED_ constants for count */
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

char
feed_tty_line_wrap(
    struct feed_client * const
        p_client,
    struct feed_tty * const
        p_tty,
    char const
        b_enable);

/* end-of-file: feed_tty.h */
