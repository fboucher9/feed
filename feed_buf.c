/* See LICENCE for license details */

#include "feed_os.h"

#include "feed_buf.h"

char
feed_buf_init(
    struct feed_buf * const
        p_this,
    char * const
        p_buf,
    unsigned int const
        i_max_len)
{
    char
        b_result;

    p_this->p_buf =
        p_buf;

    p_this->i_max_len =
        i_max_len;

    p_this->i_len =
        0u;

    b_result =
        1;

    return
        b_result;

}

static
char
g_feed_buf_empty[1u] =
{
    '@'
};

void
feed_buf_cleanup(
    struct feed_buf * const
        p_this)
{
    p_this->p_buf =
        g_feed_buf_empty;

    p_this->i_max_len =
        0;

    p_this->i_len =
        0;

}

char
feed_buf_write_character(
    struct feed_buf * const
        p_this,
    char const
        c_data)
{
    char
        b_result;

    if (p_this->i_len < p_this->i_max_len)
    {
        p_this->p_buf[p_this->i_len] =
            c_data;

        p_this->i_len ++;

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
feed_buf_write_character_array(
    struct feed_buf * const
        p_this,
    unsigned char * const
        p_buf,
    unsigned int const
        i_buf_len)
{
    char
        b_result;

    if (p_this->i_len + i_buf_len <= p_this->i_max_len)
    {
        memcpy(
            p_this->p_buf + p_this->i_len,
            p_buf,
            i_buf_len);

        p_this->i_len +=
            i_buf_len;

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
feed_buf_write_number(
    struct feed_buf * const
        p_buf,
    signed long int const
        i_data)
{
    static char const g_digit_representation[10u] =
    {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9'
    };

    char
        b_result;

    char
        a_digits[16u];

    char *
        p_digits_iterator;

    unsigned int
        i_digits;

    signed long int
        i_remainder;

    if (
        i_data < 0)
    {
        i_remainder =
            -i_data;
    }
    else
    {
        i_remainder =
            i_data;
    }

    i_digits =
        0;

    p_digits_iterator =
        a_digits;

    do
    {
        *(p_digits_iterator) =
            g_digit_representation[i_remainder % 10];

        i_remainder = (i_remainder / 10);

        p_digits_iterator ++;

        i_digits ++;
    }
    while (
        i_remainder && (i_digits < 16));

    if (
        i_data < 0)
    {
        b_result =
            feed_buf_write_character(
                p_buf,
                '-');
    }
    else
    {
        b_result =
            1;
    }

    if (
        b_result)
    {
        do
        {
            i_digits --;

            p_digits_iterator --;

            b_result =
                feed_buf_write_character(
                    p_buf,
                    *(p_digits_iterator));
        }
        while (b_result && (i_digits > 0));
    }

    return
        b_result;

}

char
feed_buf_encode_move_cursor(
    struct feed_buf * const
        p_buf,
    int const
        i_count,
    char const
        c_direction)
{
    char
        b_result;

    b_result =
        feed_buf_write_character(
            p_buf,
            '\033');

    if (
        b_result)
    {
        b_result =
            feed_buf_write_character(
                p_buf,
                '[');

        if (
            b_result)
        {
            if (
                1 != i_count)
            {
                b_result =
                    feed_buf_write_number(
                        p_buf,
                        i_count);
            }

            if (
                b_result)
            {
                feed_buf_write_character(
                    p_buf,
                    c_direction);
            }
        }
    }

    return
        b_result;

}

