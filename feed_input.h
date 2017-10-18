/* See LICENSE for license details */

/*

Module: feed_input.h

Description:

    Parser for tty input, convert input raw data to keyboard events.

*/

struct feed_client;

struct feed_input;

enum feed_event_type
{
    /* Raw sequence of bytes */
    feed_event_type_raw = 1,

    /* Single 7-bit ASCII character */
    feed_event_type_ascii = 2,

    /* Single 32-bit unicode character */
    feed_event_type_unicode = 3,

    /* Virtual key code and modifier mask */
    feed_event_type_key = 4

};

/*

Structure: feed_event

Description:

*/
struct feed_event
{
    unsigned char
        e_type;

    unsigned char
        i_raw_len;

    unsigned char
        a_raw[30u];

    union feed_event_data
    {
        struct feed_event_ascii
        {
            unsigned char
                i_code;

            unsigned char
                a_padding[7u];

        } o_ascii;

        struct feed_event_unicode
        {
            unsigned long int
                i_code;

        } o_unicode;

        struct feed_event_key
        {
            unsigned char
                i_keycode;

            unsigned char
                i_modmask;

            unsigned char
                a_padding[6u];

        } o_key;

    } u;

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

/* end-of-file: feed_input.h */
