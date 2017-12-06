/* See LICENSE for license details */

/*

Module: feed_screen.h

Description:

    Manager for refresh of visible characters on terminal screen.

Comments:

    Handling of limited screen width and height.

    Avoid writing on last character of screen

    Handle grow of region, which may do a vertical scroll

    Backbuffer and front buffer

    Wrap of wide characters

    Do relative movements for cursor, so that region may grow.

    When cursor y > region height, then grow region height or clip or scroll.

    When cursor x > region width, then grow or clip or wrap or scroll.

*/

/* Header file dependency */
#if !defined(INC_FEED_OS_H)
#error include feed_os.h first
#endif /* #if !defined(INC_FEED_OS_H) */

/* Reverse include guard */
#if defined(INC_FEED_SCREEN_H)
#error include feed_screen.h once
#endif /* #if defined(INC_FEED_SCREEN_H) */

#define INC_FEED_SCREEN_H

struct feed_client;

struct feed_screen;

struct feed_screen *
feed_screen_create(
    struct feed_client * const
        p_client);

void
feed_screen_destroy(
    struct feed_screen * const
        p_screen);

char
feed_screen_enter(
    struct feed_screen * const
        p_screen);

void
feed_screen_leave(
    struct feed_screen * const
        p_screen);

void
feed_screen_update_info(
    struct feed_screen * const
        p_screen,
    char const
        b_fallback);

unsigned long int
feed_screen_get_cursor_pos(
    struct feed_screen * const
        p_screen);

void
feed_screen_set_cursor_pos(
    struct feed_screen * const
        p_screen,
    unsigned long int const
        i_cursor_address);

void
feed_screen_newline(
    struct feed_screen * const
        p_screen);

void
feed_screen_write(
    struct feed_screen * const
        p_screen,
    unsigned char const * const
        p_data,
    unsigned int const
        i_count);

void
feed_screen_clear_region(
    struct feed_screen * const
        p_screen);

char
feed_screen_read_character(
    struct feed_screen * const
        p_screen,
    unsigned char * const
        p_data);

#define FEED_SCREEN_COLOR_DEFAULT 0x00
#define FEED_SCREEN_COLOR_DARK_BLACK 0x10
#define FEED_SCREEN_COLOR_DARK_RED 0x11
#define FEED_SCREEN_COLOR_DARK_GREEN 0x12
#define FEED_SCREEN_COLOR_DARK_YELLOW 0x13
#define FEED_SCREEN_COLOR_DARK_BLUE 0x14
#define FEED_SCREEN_COLOR_DARK_MAGENTA 0x15
#define FEED_SCREEN_COLOR_DARK_CYAN 0x16
#define FEED_SCREEN_COLOR_DARK_WHITE 0x17
#define FEED_SCREEN_COLOR_BRIGHT_BLACK 0x20
#define FEED_SCREEN_COLOR_BRIGHT_RED 0x21
#define FEED_SCREEN_COLOR_BRIGHT_GREEN 0x22
#define FEED_SCREEN_COLOR_BRIGHT_YELLOW 0x23
#define FEED_SCREEN_COLOR_BRIGHT_BLUE 0x24
#define FEED_SCREEN_COLOR_BRIGHT_MAGENTA 0x25
#define FEED_SCREEN_COLOR_BRIGHT_CYAN 0x26
#define FEED_SCREEN_COLOR_BRIGHT_WHITE 0x27

void
feed_screen_color(
    struct feed_screen * const
        p_screen,
    unsigned char const
        i_foreground,
    unsigned char const
        i_background);

/* end-of-file: feed_screen.h */
