/* See LICENSE for license details */

/*

Module: feed.h

Description:

    Editor library for command-line utilities.

*/

/* Reverse include guard */
#if defined(INC_FEED_H)
#error include feed.h once
#endif /* #if defined(INC_FEED_H) */

/* Define include guard */
#define INC_FEED_H

/* Predefine library handle */
struct feed_handle;

/* Types of completions */
enum feed_complete_type
{
    /* Complete entire line from history */
    feed_complete_type_line = 1,

    /* Complete word */
    feed_complete_type_word,

    /* Paste last argument from history */
    feed_complete_type_last_arg

}; /* enum feed_complete_type */

/* Descriptor for initialization of library handle */
struct feed_descriptor
{
    /* application may filter events */
    int (* p_notify)(
        void * const p_context,
        unsigned char const * const p_event,
        unsigned long int const i_event_length);

    /* application must provide suggestions */
    int (* p_complete)(
        void * const p_context,
        unsigned char const * const p_data,
        unsigned long int const i_data_length,
        unsigned long int const i_cursor,
        enum feed_complete_type const e_type);

    void * p_context;

}; /* struct feed_descriptor */

#if defined(__cplusplus)
extern "C" {
#endif /* #if defined(__cplusplus) */

/* create a feed handle, register callbacks */
struct feed_handle * feed_create(
    struct feed_descriptor const * const p_feed_descriptor);

/* destroy a feed handle */
void feed_destroy(
    struct feed_handle * const p_feed_handle);

/* setup prompt of first line */
int feed_prompt1(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned long int const i_data_length);

/* setup prompt of other lines */
int feed_prompt2(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned long int const i_data_length);

/* load initial text into the buffer */
int feed_load(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned long int const i_data_length);

/* start the editor engine */
int feed_start(
    struct feed_handle * const p_feed_handle);

/* stop the editor engine.  this may be called from one of the callbacks */
int feed_stop(
    struct feed_handle * const p_feed_handle);

/* provide suggestions for completion mode */
int feed_suggest(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned long int const i_data_length);

/* query length in bytes of save buffer */
unsigned long int feed_length(
    struct feed_handle * const p_feed_handle);

/* query contents of save buffer */
int feed_save(
    struct feed_handle * const p_feed_handle,
    unsigned char * const p_data,
    unsigned long int const i_data_length);

/* consume bytes from buffer */
unsigned long int feed_consume(
    struct feed_handle * const p_feed_handle,
    unsigned char * const p_data,
    unsigned long int const i_data_length);

/* iterate contents of buffer */
int feed_iterate(
    struct feed_handle * const p_feed_handle,
    int (*p_callback)(
        void * const p_context,
        unsigned char const * const a_raw,
        unsigned char const i_raw_len),
    void * const p_context);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* #if defined(__cplusplus) */

/* end-of-file: feed.h */
