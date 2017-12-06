/* See LICENSE for license details */

/*

Module: feed_esc.c

Description:

    Encode of terminal escape codes.

*/

#include "feed_os.h"

#include "feed_esc.h"

#include "feed_buf.h"

#include "feed_const.h"

char
feed_esc_write_prefix(
    struct feed_buf * const
        p_buf)
{
    char
        b_result;

    b_result =
        feed_buf_write_character(
            p_buf,
            FEED_ESC_CHAR);

    if (
        b_result)
    {
        b_result =
            feed_buf_write_character(
                p_buf,
                '[');
    }

    return
        b_result;

}

static
char
feed_esc_write_n1(
    struct feed_buf * const
        p_buf,
    unsigned int const
        i_count,
    unsigned char const
        c_direction)
{
    char
        b_result;

    b_result =
        feed_esc_write_prefix(
            p_buf);

    if (
        b_result)
    {
        if (
            1 != i_count)
        {
            b_result =
                feed_buf_write_number(
                    p_buf,
                    (signed long int)(
                        i_count));
        }

        if (
            b_result)
        {
            feed_buf_write_character(
                p_buf,
                c_direction);
        }
    }

    return
        b_result;

}

char
feed_esc_write_cuu(
    struct feed_buf * const
        p_buf,
    unsigned int const
        i_count)
{
    return
        feed_esc_write_n1(
            p_buf,
            i_count,
            FEED_CSI_CUU);
}

char
feed_esc_write_cud(
    struct feed_buf * const
        p_buf,
    unsigned int const
        i_count)
{
    return
        feed_esc_write_n1(
            p_buf,
            i_count,
            FEED_CSI_CUD);
}

char
feed_esc_write_cuf(
    struct feed_buf * const
        p_buf,
    unsigned int const
        i_count)
{
    return
        feed_esc_write_n1(
            p_buf,
            i_count,
            FEED_CSI_CUF);
}

char
feed_esc_write_cub(
    struct feed_buf * const
        p_buf,
    unsigned int const
        i_count)
{
    return
        feed_esc_write_n1(
            p_buf,
            i_count,
            FEED_CSI_CUB);
}

char
feed_esc_write_ed(
    struct feed_buf * const
        p_buf,
    unsigned int const
        i_count)
{
    char
        b_result;

    b_result =
        feed_esc_write_prefix(
            p_buf);

    if (
        b_result)
    {
        if (
            0 != i_count)
        {
            b_result =
                feed_buf_write_number(
                    p_buf,
                    (signed long int)(
                        i_count));
        }

        if (
            b_result)
        {
            feed_buf_write_character(
                p_buf,
                FEED_CSI_ED);
        }
    }

    return
        b_result;

}

char
feed_esc_write_el(
    struct feed_buf * const
        p_buf,
    unsigned int const
        i_count)
{
    char
        b_result;

    b_result =
        feed_esc_write_prefix(
            p_buf);

    if (
        b_result)
    {
        if (
            0 != i_count)
        {
            b_result =
                feed_buf_write_number(
                    p_buf,
                    (signed long int)(
                        i_count));
        }

        if (
            b_result)
        {
            feed_buf_write_character(
                p_buf,
                FEED_CSI_EL);
        }
    }

    return
        b_result;

}

char
feed_esc_write_cup(
    struct feed_buf * const
        p_buf,
    unsigned int const
        i_row,
    unsigned int const
        i_column)
{
    char
        b_result;

    b_result =
        feed_esc_write_prefix(
            p_buf);

    if (
        b_result)
    {
        if (
            1 != i_row)
        {
            b_result =
                feed_buf_write_number(
                    p_buf,
                    (signed long int)(
                        i_row));
        }

        if (
            b_result)
        {
            if (
                1 != i_column)
            {
                b_result =
                    feed_buf_write_character(
                        p_buf,
                        ';');

                if (
                    b_result)
                {
                    b_result =
                        feed_buf_write_number(
                            p_buf,
                            (signed long int)(
                                i_column));
                }
            }
        }

        if (
            b_result)
        {
            b_result =
                feed_buf_write_character(
                    p_buf,
                    FEED_CSI_CUP);
        }
    }

    return
        b_result;

}

char
feed_esc_write_sgr(
    struct feed_buf * const
        p_buf,
    unsigned char const * const
        p_attr,
    unsigned int const
        i_count)
{
    char
        b_result;

    b_result =
        feed_esc_write_prefix(
            p_buf);

    if (
        b_result)
    {
        unsigned int
            i;

        i =
            0u;

        while (
            b_result
            && (i < i_count))
        {
            if (
                i)
            {
                b_result =
                    feed_buf_write_character(
                        p_buf,
                        ';');
            }

            if (
                b_result)
            {
                b_result =
                    feed_buf_write_number(
                        p_buf,
                        (signed long int)(
                            p_attr[i]));
            }

            i ++;
        }

        b_result =
            feed_buf_write_character(
                p_buf,
                FEED_CSI_SGR);
    }

    return
        b_result;

}

