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

union feed_ptr
{
    unsigned char *
        p;

    unsigned char const *
        pc;

};

struct feed_buf
{
    union feed_ptr
        o_min;

    union feed_ptr
        o_max;

}; /* struct feed_buf */

/* Interface */

char
feed_buf_init(
    struct feed_buf * const
        p_this,
    unsigned char const * const
        p_buf_min,
    unsigned char const * const
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

char
feed_buf_read_character(
    struct feed_buf * const
        p_this,
    unsigned char * const
        p_data);

char
feed_buf_read_character_array(
    struct feed_buf * const
        p_this,
    unsigned char * const
        p_data_min,
    unsigned char * const
        p_data_max);

char
feed_buf_read_number(
    struct feed_buf * const
        p_this,
    signed long int * const
        p_data);

int
feed_buf_compare(
    struct feed_buf const * const
        p_buf1,
    struct feed_buf const * const
        p_buf2);

/* end-of-file: feed_buf.h */
