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

/* Flag to indicate that the code is for a special key */
#define FEED_EVENT_KEY_FLAG     0x80000000ul

/* Mask of shift modifier */
#define FEED_EVENT_KEY_SHIFT    0x10000000ul

/* Mask of alt modifier */
#define FEED_EVENT_KEY_ALT      0x20000000ul

/* Mask of control modifier */
#define FEED_EVENT_KEY_CTRL     0x40000000ul

/* Mask of base key code */
#define FEED_EVENT_KEY_MASK     0x7FFFFFFFul

/*

Structure: feed_event

Description:

*/
struct feed_event
{
    unsigned long int
        i_code;

    unsigned char
        i_raw_len;

    unsigned char
        a_raw[31u];

}; /* struct feed_event */

/* --- Interface --- */

struct feed_input *
feed_input_create(
    struct feed_client * const
        p_client,
    void (* const p_callback)(
        void * const
            p_context,
        struct feed_event const * const
            p_event),
    void * const
        p_context);

void
feed_input_destroy(
    struct feed_input * const
        p_input);

char
feed_input_write(
    struct feed_input * const
        p_input,
    unsigned char const
        c_data);

void
feed_input_print(
    struct feed_event const * const
        p_event,
    struct feed_buf * const
        p_buf);

/* end-of-file: feed_input.h */
