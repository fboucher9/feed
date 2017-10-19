/* See LICENSE for license details */

/*

Module: feed_input.h

Description:

    Parser for tty input, convert input raw data to keyboard events.

*/

struct feed_client;

struct feed_input;

#define FEED_EVENT_KEY_FLAG     0x80000000ul
#define FEED_EVENT_KEY_SHIFT    0x10000000ul
#define FEED_EVENT_KEY_ALT      0x20000000ul
#define FEED_EVENT_KEY_CTRL     0x40000000ul
#define FEED_EVENT_KEY_MASK     0x000000FFul

/*

Structure: feed_event

Description:

*/
struct feed_event
{
    unsigned char
        i_raw_len;

    unsigned char
        a_raw[31u];

    unsigned long int
        i_code;

}; /* struct feed_event */

struct feed_input *
feed_input_create(
    struct feed_client * const
        p_client);

void
feed_input_destroy(
    struct feed_input * const
        p_input);

char
feed_input_write(
    struct feed_input * const
        p_input,
    unsigned char const
        c_data,
    void (* const p_event)(
        void * const
            p_context,
        struct feed_event const * const
            p_event),
    void * const
        p_context);

unsigned int
feed_input_print(
    struct feed_event const * const
        p_event,
    unsigned char * const
        p_buf,
    unsigned int const
        i_buf_len);

/* end-of-file: feed_input.h */
