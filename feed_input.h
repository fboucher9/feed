/* See LICENSE for license details */

/*

Module: feed_input.h

Description:

    Parser for tty input, convert input raw data to keyboard events.

*/

#if defined(INC_FEED_INPUT_H)
#error include once
#endif /* #if defined(INC_FEED_INPUT_H) */

#define INC_FEED_INPUT_H

/* Predefine client context */
struct feed_client;

/* Predefine module handle */
struct feed_input;

/* Predefine buffer object */
struct feed_buf;

/*

Structure: feed_event

Description:

*/
struct feed_event
{
    unsigned short int
        i_code;

    unsigned short int
        us_padding[3u];

    unsigned char
        i_raw_len;

    unsigned char
        a_raw[31u];

}; /* struct feed_event */

/* --- Interface --- */

struct feed_input *
feed_input_create(
    struct feed_client * const
        p_client);

void
feed_input_destroy(
    struct feed_input * const
        p_input);

int
feed_input_write(
    struct feed_input * const
        p_input,
    unsigned char const
        c_data,
    struct feed_event * const
        p_event);

void
feed_input_print(
    struct feed_event const * const
        p_event,
    struct feed_buf * const
        p_buf);

/* end-of-file: feed_input.h */
