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

/* Descriptor for initialization of library handle */
struct feed_descriptor
{
    /* notify application of input events */
    int (* p_notify)(
        void * const p_context,
        struct feed_handle * const p_feed_handle,
        unsigned short int i_key,
        unsigned char const * const p_event,
        unsigned long int const i_event_length);

    /* application context for callbacks */
    void * p_context;

    /* Option for maximum screen width */
    unsigned short int i_max_screen_width;

    /* Option for maximum screen height */
    unsigned short int i_max_screen_height;

    unsigned short int us_padding[2u];

}; /* struct feed_descriptor */

/* Define colors */
enum feed_color
{
    feed_color_default = 0,
    feed_color_dark_black,
    feed_color_dark_red,
    feed_color_dark_green,
    feed_color_dark_yellow,
    feed_color_dark_blue,
    feed_color_dark_magenta,
    feed_color_dark_cyan,
    feed_color_dark_white,
    feed_color_bright_black,
    feed_color_bright_red,
    feed_color_bright_green,
    feed_color_bright_yellow,
    feed_color_bright_blue,
    feed_color_bright_magenta,
    feed_color_bright_cyan,
    feed_color_bright_white
};

/* Define syntax groups */
enum feed_syntax
{
    feed_syntax_default = 0,
    feed_syntax_prompt1,
    feed_syntax_prompt2,
    feed_syntax_suggest,
    feed_syntax_operator,
    feed_syntax_number,
    feed_syntax_string,
    feed_syntax_keyword,
    feed_syntax_comment,
    feed_syntax_preprocessor,
    feed_syntax_escape,
    feed_syntax_special,
    feed_syntax_whitespace
};

/* Definitions for key codes */
#define FEED_KEY_SHIFT \
    ((unsigned short int)(256u))

#define FEED_KEY_ALT \
    ((unsigned short int)(512u))

#define FEED_KEY_CTRL \
    ((unsigned short int)(1024u))

#define FEED_KEY_UP \
    ((unsigned short int)(130u))

#define FEED_KEY_DOWN \
    ((unsigned short int)(131u))

#define FEED_KEY_RIGHT \
    ((unsigned short int)(132u))

#define FEED_KEY_LEFT \
    ((unsigned short int)(133u))

#define FEED_KEY_INSERT \
    ((unsigned short int)(134u))

#define FEED_KEY_DELETE \
    ((unsigned short int)(135u))

#define FEED_KEY_HOME \
    ((unsigned short int)(136u))

#define FEED_KEY_END \
    ((unsigned short int)(137u))

#define FEED_KEY_PAGEUP \
    ((unsigned short int)(138u))

#define FEED_KEY_PAGEDOWN \
    ((unsigned short int)(139u))

#define FEED_KEY_F1 \
    ((unsigned short int)(141u))

#define FEED_KEY_F2 \
    ((unsigned short int)(142u))

#define FEED_KEY_F3 \
    ((unsigned short int)(143u))

#define FEED_KEY_F4 \
    ((unsigned short int)(144u))

#define FEED_KEY_F5 \
    ((unsigned short int)(145u))

#define FEED_KEY_F6 \
    ((unsigned short int)(146u))

#define FEED_KEY_F7 \
    ((unsigned short int)(147u))

#define FEED_KEY_F8 \
    ((unsigned short int)(148u))

#define FEED_KEY_F9 \
    ((unsigned short int)(149u))

#define FEED_KEY_F10 \
    ((unsigned short int)(150u))

#define FEED_KEY_F11 \
    ((unsigned short int)(151u))

#define FEED_KEY_F12 \
    ((unsigned short int)(152u))

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

/* enter completion mode */
int feed_complete(
    struct feed_handle * const p_feed_handle,
    unsigned long int const i_word_offset,
    unsigned long int const i_word_length);

/* provide suggestions for completion mode */
int feed_suggest(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned long int const i_data_length,
    unsigned long int const i_cursor_offset);

/* query length in bytes of save buffer */
unsigned long int feed_length(
    struct feed_handle * const p_feed_handle);

/* query contents of save buffer */
unsigned long int feed_save(
    struct feed_handle * const p_feed_handle,
    unsigned char * const p_data,
    unsigned long int const i_data_length);

/* consume bytes from buffer */
unsigned long int feed_consume(
    struct feed_handle * const p_feed_handle,
    unsigned char * const p_data,
    unsigned long int const i_data_length);

/* query position of cursor */
unsigned long int feed_cursor(
    struct feed_handle * const p_feed_handle);

/* execute editor commands via keyboard codes */
int feed_exec(
    struct feed_handle * const p_feed_handle,
    unsigned char const * const p_data,
    unsigned long int const i_data_length);

/* Define color theme */
int feed_theme(
    struct feed_handle * const p_feed_handle,
    enum feed_syntax const e_syntax,
    enum feed_color const i_foreground,
    enum feed_color const i_background);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* #if defined(__cplusplus) */

/* end-of-file: feed.h */
