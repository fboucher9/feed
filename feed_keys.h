/* See LICENSE for license details */

/*

Module: feed_keys.h

Description:
    Translation of escape sequences into key codes.

*/

#if defined(INC_FEED_KEYS_H)
#error include once
#endif /* #if defined(INC_FEED_KEYS_H) */

#define INC_FEED_KEYS_H

#define FEED_MOD_SHIFT (1u << 28u)

#define FEED_MOD_ALT (2u << 28u)

#define FEED_MOD_CTRL (4u << 28u)

#define FEED_KEY_UP 130

#define FEED_KEY_DOWN 131

#define FEED_KEY_RIGHT 132

#define FEED_KEY_LEFT 133

#define FEED_KEY_INSERT 134

#define FEED_KEY_DELETE 135

#define FEED_KEY_HOME 136

#define FEED_KEY_END 137

#define FEED_KEY_PAGEUP 138

#define FEED_KEY_PAGEDOWN 139

#define FEED_KEY_F1 141

#define FEED_KEY_F2 142

#define FEED_KEY_F3 143

#define FEED_KEY_F4 144

#define FEED_KEY_F5 145

#define FEED_KEY_F6 146

#define FEED_KEY_F7 147

#define FEED_KEY_F8 148

#define FEED_KEY_F9 149

#define FEED_KEY_F10 150

#define FEED_KEY_F11 151

#define FEED_KEY_F12 152

/* --- Interface --- */

unsigned long int
feed_keys_lookup(
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length);

unsigned int
feed_keys_print(
    unsigned long int const
        i_code,
    unsigned char * const
        p_buf,
    unsigned int const
        i_buf_len);

/* end-of-file: feed_keys.h */
