/* See LICENSE for license details */

/*

Module: feed_input.h

Description:

    Parser for tty input, convert input raw data to keyboard events.

*/

struct feed_client;

struct feed_input;

enum feed_input_event_type
{
    feed_input_event_type_raw,
    feed_input_event_type_ascii,
    feed_input_event_type_unicode,
    feed_input_event_type_key,
    feed_input_event_type_escape
};

struct feed_input_event_raw
{
    enum feed_input_event_type
        e_type;

    unsigned char
        a_raw[31u];

    unsigned char
        i_raw_len;

};

struct feed_input_event_ascii
{
    struct feed_input_event_raw
        o_raw;

    unsigned char
        i_code;

} o_ascii;

struct feed_input_event_unicode
{
    struct feed_input_event_raw
        o_raw;

    unsigned long int
        i_code;

} o_unicode;

#define FEED_INPUT_MOD_CTRL 1u
#define FEED_INPUT_MOD_SHIFT 2u
#define FEED_INPUT_MOD_ALT 4u

struct feed_input_event_key
{
    struct feed_input_event_raw
        o_raw;

    unsigned short int
        i_keycode;

    unsigned short int
        i_modmask;

} o_key;

struct feed_input_event_escape
{
    struct feed_input_event_raw
        o_raw;

    unsigned char
        i_stop;

    unsigned char
        i_param_offset;

    unsigned char
        i_param_count;

} o_escape;

union feed_input_event
{
    enum feed_input_event_type
        e_type;

    struct feed_input_event_raw
        o_raw;

    struct feed_input_event_ascii
        o_ascii;

    struct feed_input_event_unicode
        o_unicode;

    struct feed_input_event_key
        o_key;

    struct feed_input_event_escape
        o_escape;

}; /* union feed_input_event */

struct feed_input_descriptor
{
    char
        (* p_event)(
            void * const
                p_context,
            union feed_input_event const * const
                p_input_event);

    void *
        p_context;

}; /* struct feed_input_descriptor */

struct feed_input *
feed_input_create(
    struct feed_client * const
        p_client,
    struct feed_input_descriptor const * const
        p_input_descriptor);

void
feed_input_destroy(
    struct feed_input * const
        p_input);

char
feed_input_write_character(
    struct feed_input * const
        p_input,
    unsigned char const
        c_data);

/* end-of-file: feed_input.h */
