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
    /* application may preview and consume a line */
    int (* p_notify)(
        void * const p_context,
        unsigned char const * const p_data,
        unsigned int const i_data_length);

    /* application must provide suggestions */
    int (* p_complete)(
        void * const p_context,
        unsigned char const * const p_data,
        unsigned int const i_data_length,
        unsigned char const * const p_word,
        unsigned int const i_word_length,
        unsigned int const i_cursor,
        enum feed_complete_type const e_type);

    void * p_context;

}; /* struct feed_descriptor */

#if defined(__cplusplus)
extern "C" {
#endif /* #if defined(__cplusplus) */

struct feed_handle * feed_create(
    struct feed_descriptor const * const p_feed_descriptor);

void feed_destroy(
    struct feed_handle * const p_feed_handle);

int feed_prompt1(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned int const i_data_length);

int feed_prompt2(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned int const i_data_length);

int feed_start(
    struct feed_handle * const p_feed_handle);

int feed_stop(
    struct feed_handle * const p_feed_handle);

int feed_suggest(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned int const i_data_length);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* #if defined(__cplusplus) */

/* end-of-file: feed.h */
