/* See LICENSE for license details */

/*

Module: feed_screen_iterator.h

Description:

*/

struct feed_screen_info;

struct feed_screen_iterator
{
    unsigned long int
        i_cursor_address;

    unsigned long int
        ul_padding[1u];

};

char
feed_screen_iterator_init(
    struct feed_screen_info const * const
        p_this,
    struct  feed_screen_iterator * const
        p_screen_iterator);

void
feed_screen_iterator_home(
    struct feed_screen_info const * const
        p_this,
    struct  feed_screen_iterator * const
        p_screen_iterator);

char
feed_screen_iterator_test_write(
    struct feed_screen_info const * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator,
    unsigned long int const
        i_width);

void
feed_screen_iterator_write(
    struct feed_screen_info const * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator,
    unsigned long int const
        i_width);

unsigned short int
feed_screen_iterator_get_cursor_x(
    struct feed_screen_info const * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator);

unsigned short int
feed_screen_iterator_get_cursor_y(
    struct feed_screen_info const * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator);

char
feed_screen_iterator_test_newline(
    struct feed_screen_info const * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator);

unsigned long int
feed_screen_iterator_clear(
    struct feed_screen_info const * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator);

unsigned long int
feed_screen_iterator_newline(
    struct feed_screen_info const * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator);

char
feed_screen_iterator_test_clear(
    struct feed_screen_info const * const
        p_this,
    struct feed_screen_iterator * const
        p_screen_iterator);

/* end-of-file: feed_screen_iterator.h */
