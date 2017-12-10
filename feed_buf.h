/* See LICENCE for license details */

/*

Module: feed_buf.h

Description:

    Buffer to contain arrays of characters.  Method to read from the buffer
    or write to the buffer.

*/

/* Header include protection */
#if defined(INC_FEED_BUF_H)
#error include once
#endif /* #if defined(INC_FEED_BUF_H) */

#define INC_FEED_BUF_H

struct feed_buf
{
    unsigned char *
        p_buf_min;

    unsigned char *
        p_buf_max;

}; /* struct feed_buf */

/* Interface */

char
feed_buf_init(
    struct feed_buf * const
        p_this,
    unsigned char * const
        p_buf_min,
    unsigned char * const
        p_buf_max);

void
feed_buf_cleanup(
    struct feed_buf * const
        p_this);

char
feed_buf_write_character(
    struct feed_buf * const
        p_this,
    unsigned char const
        c_data);

char
feed_buf_write_character_array(
    struct feed_buf * const
        p_this,
    unsigned char const * const
        p_data_min,
    unsigned char const * const
        p_data_max);

char
feed_buf_write_number(
    struct feed_buf * const
        p_this,
    signed long int const
        i_data);

char
feed_buf_write_unicode_character(
    struct feed_buf * const
        p_this,
    unsigned long int const
        i_code);

struct feed_buf_const
{
    unsigned char const *
        p_buf_min;

    unsigned char const *
        p_buf_max;

}; /* struct feed_buf_const */

char
feed_buf_const_init(
    struct feed_buf_const * const
        p_this,
    unsigned char const * const
        p_buf_min,
    unsigned char const * const
        p_buf_max);

void
feed_buf_const_cleanup(
    struct feed_buf_const * const
        p_this);

char
feed_buf_const_read_character(
    struct feed_buf_const * const
        p_this,
    unsigned char * const
        p_data);

char
feed_buf_const_read_character_array(
    struct feed_buf_const * const
        p_this,
    unsigned char * const
        p_data_min,
    unsigned char * const
        p_data_max);

/* end-of-file: feed_buf.h */
