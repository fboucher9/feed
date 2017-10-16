/* See LICENSE for license details */

/*

Module: feed_engine.h

Description:

    State machine for editor.

Comments:

    -   Application is responsible of reading from input device, and then
        passing each character or group of characters to this module.

    -   Process of input events.

    -   Process of timer events.

    -   Should we use a pipeline concept?
        First level will convert raw 8-bit input into keyboard events
        Second level will return lines

    -   Completion is used to all types of automatic suggestion of data
        which includes history search and word completions.

*/

struct feed_client;

struct feed_engine;

struct feed_engine_descriptor
{
    /* Request application to read from input device */
    /* This is used by tty reports */
    char (*p_input)(
        void * const
            p_application_context,
        unsigned char * const
            p_data,
        unsigned int const
            i_count,
        unsigned int * const
            p_actual);

    /* Request application to refresh the display by writing to output device */
    /* Display of prompt and refresh of lines */
    /* This is also used to send tty requests */
    char (*p_output)(
        void * const
            p_application_context,
        unsigned char const * const
            p_data,
        unsigned int const
            i_count);

    /* Once a line is completed, notify application that data is ready */
    char (*p_notify)(
        void * const
            p_application_context,
        unsigned char const * const
            p_data,
        unsigned int const
            i_count);

    /* Request application to supply a list of completions */
    char (*p_complete)(
        void * const
            p_application_context,
        unsigned char const * const
            p_data,
        unsigned int const
            i_count,
        unsigned int const
            i_position);

    void *
        p_application_context;

}; /* struct feed_engine_descriptor */

struct feed_engine *
feed_engine_create(
    struct feed_client * const
        p_client,
    struct feed_engine_descriptor const * const
        p_descriptor);

char
feed_engine_init(
    struct feed_client * const
        p_client,
    struct feed_engine * const
        p_engine,
    struct feed_engine_descriptor const * const
        p_descriptor);

void
feed_engine_destroy(
    struct feed_client * const
        p_client,
    struct feed_engine * const
        p_engine);

void
feed_engine_cleanup(
    struct feed_client * const
        p_client,
    struct feed_engine * const
        p_engine);

char
feed_engine_set_prompt(
    struct feed_client * const
        p_client,
    struct feed_engine * const
        p_engine,
    unsigned char const * const
        p_prompt,
    unsigned int const
        i_prompt_length);

void
feed_engine_process_input_loop(
    struct feed_client * const
        p_client,
    struct feed_engine * const
        p_engine);

char
feed_engine_process_input_array(
    struct feed_client * const
        p_client,
    struct feed_engine * const
        p_engine,
    unsigned char const * const
        p_data,
    unsigned int const
        i_count);

char
feed_engine_process_input_character(
    struct feed_client * const
        p_client,
    struct feed_engine * const
        p_engine,
    unsigned char const
        c_data);

char
feed_engine_add_completion(
    struct feed_client * const
        p_client,
    struct feed_engine * const
        p_engine,
    unsigned char const * const
        p_data,
    unsigned int const
        i_count);

/* end-of-file: feed_engine.h */
