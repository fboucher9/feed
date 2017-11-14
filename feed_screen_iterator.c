/* See LICENSE for license details */

/*

*/

#include "feed_os.h"

#include "feed_screen_iterator.h"

#include "feed_screen_info.h"

char
feed_screen_iterator_init(
    struct feed_screen_info const * const
        p_screen_info,
    struct  feed_screen_iterator * const
        p_screen_iterator)
{
    char
        b_result;

    (void)(
        p_screen_info);

    p_screen_iterator->i_cursor_address =
        0ul;

    b_result =
        1;

    return
        b_result;

}

#if 0
void
feed_screen_iterator_cleanup(
    struct feed_screen_info const * const
        p_screen_info,
    struct  feed_screen_iterator * const
        p_screen_iterator)
{
    (void)(
        p_screen_info);
    (void)(
        p_screen_iterator);

}
#endif

void
feed_screen_iterator_home(
    struct feed_screen_info const * const
        p_screen_info,
    struct  feed_screen_iterator * const
        p_screen_iterator)
{
    (void)(
        p_screen_info);

    p_screen_iterator->i_cursor_address =
        0ul;

}

#if 0
char
feed_screen_iterator_test_goto(
    struct feed_screen_info const * const
        p_screen_info,
    struct  feed_screen_iterator * const
        p_screen_iterator,
    unsigned short int const
        i_cursor_x,
    unsigned short int const
        i_cursor_y)
{
    char
        b_result;

    unsigned long int
        i_cursor_address;

    (void)(
        p_screen_iterator);

    i_cursor_address =
        (unsigned long int)(
            ((unsigned long int)(i_cursor_y) * (unsigned long int)(p_screen_info->i_screen_width))
            + (unsigned long int)(i_cursor_x));

    if ((i_cursor_address + 1u) < (p_screen_info->i_screen_size))
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
#endif

#if 0
void
feed_screen_iterator_goto(
    struct feed_screen_info const * const
        p_screen_info,
    struct  feed_screen_iterator * const
        p_screen_iterator,
    unsigned short int const
        i_cursor_x,
    unsigned short int const
        i_cursor_y)
{
    p_screen_iterator->i_cursor_address =
        (unsigned long int)(
            ((unsigned long int)(i_cursor_y) * (unsigned long int)(p_screen_info->i_screen_width))
            + (unsigned long int)(i_cursor_x));

}
#endif

char
feed_screen_iterator_test_write(
    struct feed_screen_info const * const
        p_screen_info,
    struct feed_screen_iterator * const
        p_screen_iterator,
    unsigned long int const
        i_width)
{
    char
        b_result;

    if (
        (p_screen_iterator->i_cursor_address + i_width) < p_screen_info->i_screen_size)
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

void
feed_screen_iterator_write(
    struct feed_screen_info const * const
        p_screen_info,
    struct feed_screen_iterator * const
        p_screen_iterator,
    unsigned long int const
        i_width)
{
    (void)(
        p_screen_info);

    p_screen_iterator->i_cursor_address += i_width;

}

unsigned short int
feed_screen_iterator_get_cursor_x(
    struct feed_screen_info const * const
        p_screen_info,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    return
        (unsigned short int)(
            p_screen_iterator->i_cursor_address
            % p_screen_info->i_screen_width);

}

unsigned short int
feed_screen_iterator_get_cursor_y(
    struct feed_screen_info const * const
        p_screen_info,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    return
        (unsigned short int)(
            p_screen_iterator->i_cursor_address
            / p_screen_info->i_screen_width);

}

char
feed_screen_iterator_test_newline(
    struct feed_screen_info const * const
        p_screen_info,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    return
        feed_screen_iterator_test_write(
            p_screen_info,
            p_screen_iterator,
            1u);

}

unsigned long int
feed_screen_iterator_clear(
    struct feed_screen_info const * const
        p_screen_info,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    unsigned long int
        i_width;

    i_width =
        (unsigned long int)(
            p_screen_info->i_screen_size
            - 1ul
            - p_screen_iterator->i_cursor_address);

    p_screen_iterator->i_cursor_address += i_width;

    return
        i_width;
}

unsigned long int
feed_screen_iterator_newline(
    struct feed_screen_info const * const
        p_screen_info,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    unsigned long int
        i_width;

    unsigned short int
        i_cursor_x;

    i_cursor_x =
        feed_screen_iterator_get_cursor_x(
            p_screen_info,
            p_screen_iterator);

    i_width = (unsigned long int)(p_screen_info->i_screen_width - i_cursor_x);

    if (
        feed_screen_iterator_test_write(
            p_screen_info,
            p_screen_iterator,
            i_width))
    {
        feed_screen_iterator_write(
            p_screen_info,
            p_screen_iterator,
            i_width);
    }
    else
    {
        i_width =
            feed_screen_iterator_clear(
                p_screen_info,
                p_screen_iterator);
    }

    return
        i_width;

}

char
feed_screen_iterator_test_clear(
    struct feed_screen_info const * const
        p_screen_info,
    struct feed_screen_iterator * const
        p_screen_iterator)
{
    return
        feed_screen_iterator_test_write(
            p_screen_info,
            p_screen_iterator,
            1u);

}

