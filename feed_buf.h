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
        p_buf;

    void *
        v_padding[1u];

    unsigned int
        i_max_len;

    unsigned int
        i_len;

    unsigned int
        i_padding[2u];

}; /* struct feed_buf */

/* Interface */

char
feed_buf_init(
    struct feed_buf * const
        p_this,
    unsigned char * const
        p_buf,
    unsigned int const
        i_max_len);

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
        p_buf,
    unsigned int const
        i_buf_len);

char
feed_buf_write_number(
    struct feed_buf * const
        p_buf,
    signed long int const
        i_data);

char
feed_buf_write_unicode_character(
    struct feed_buf * const
        p_buf,
    unsigned long int const
        i_code);

/* end-of-file: feed_buf.h */
