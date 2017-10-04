/* See LICENCE for license details */

#include "feed_os.h"

#include "feed_buf.h"

#include "feed_const.h"

char
feed_buf_init(
    struct feed_buf * const
        p_this,
    unsigned char * const
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
unsigned char
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
    unsigned char const
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
    static unsigned char const g_digit_representation[10u] =
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

    unsigned char
        a_digits[16u];

    unsigned char *
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
feed_buf_write_unicode_character(
    struct feed_buf * const
        p_buf,
    unsigned long int const
        i_code)
{
    char
        b_result;

    unsigned char
        a_byte[4u];

    if (
        i_code < 128ul)
    {
        b_result =
            feed_buf_write_character(
                p_buf,
                (unsigned char)(
                    i_code & 0x7Ful));
    }
    else if (
        i_code < (1ul << 11ul))
    {
        a_byte[0u] =
            (unsigned char)(
                0xC0u
                | ((i_code >> 6u) & 0x1Fu));

        a_byte[1u] =
            (unsigned char)(
                0x80u
                | ((i_code >> 0u) & 0x3Fu));

        b_result =
            feed_buf_write_character_array(
                p_buf,
                a_byte,
                2u);
    }
    else if (
        i_code < (1ul << 16ul))
    {
        a_byte[0u] =
            (unsigned char)(
                0xC0u
                | ((i_code >> 12u) & 0x0Fu));

        a_byte[1u] =
            (unsigned char)(
                0x80u
                | ((i_code >> 6u) & 0x3Fu));

        a_byte[2u] =
            (unsigned char)(
                0x80u
                | ((i_code >> 0u) & 0x3Fu));

        b_result =
            feed_buf_write_character_array(
                p_buf,
                a_byte,
                3u);
    }
    else if (
        i_code < (1ul << 21ul))
    {
        a_byte[0u] =
            (unsigned char)(
                0xC0u
                | ((i_code >> 18u) & 0x07u));

        a_byte[1u] =
            (unsigned char)(
                0x80u
                | ((i_code >> 12u) & 0x3Fu));

        a_byte[2u] =
            (unsigned char)(
                0x80u
                | ((i_code >> 6u) & 0x3Fu));

        a_byte[3u] =
            (unsigned char)(
                0x80u
                | ((i_code >> 0u) & 0x3Fu));

        b_result =
            feed_buf_write_character_array(
                p_buf,
                a_byte,
                4u);
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

