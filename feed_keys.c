/* See LICENSE for license details */

/*

Module: feed_keys.c

Description:
    Translation of escape sequences into key codes.

*/

#include "feed_os.h"

#include "feed_keys.h"

#include "feed_buf.h"

/* Up ^[[A */
static unsigned char const g_feed_key_up[] =
{ FEED_KEY_UP, 0, 27, '[', 'A' };

static unsigned char const g_feed_key_up_s[] =
{ FEED_KEY_UP, 1, 27, '[', '1', ';', '2', 'A' };

static unsigned char const g_feed_key_up_a[] =
{ FEED_KEY_UP, 2, 27, '[', '1', ';', '3', 'A' };

static unsigned char const g_feed_key_up_s_a[] =
{ FEED_KEY_UP, 3, 27, '[', '1', ';', '4', 'A' };

static unsigned char const g_feed_key_up_c[] =
{ FEED_KEY_UP, 4, 27, '[', '1', ';', '5', 'A' };

static unsigned char const g_feed_key_up_s_c[] =
{ FEED_KEY_UP, 5, 27, '[', '1', ';', '6', 'A' };

static unsigned char const g_feed_key_up_a_c[] =
{ FEED_KEY_UP, 6, 27, '[', '1', ';', '7', 'A' };

static unsigned char const g_feed_key_up_s_a_c[] =
{ FEED_KEY_UP, 7, 27, '[', '1', ';', '8', 'A' };

/* Down ^[[B */
static unsigned char const g_feed_key_down[] =
{ FEED_KEY_DOWN, 0, 27, '[', 'B' };

static unsigned char const g_feed_key_down_s[] =
{ FEED_KEY_DOWN, 1, 27, '[', '1', ';', '2', 'B' };

static unsigned char const g_feed_key_down_a[] =
{ FEED_KEY_DOWN, 2, 27, '[', '1', ';', '3', 'B' };

static unsigned char const g_feed_key_down_s_a[] =
{ FEED_KEY_DOWN, 3, 27, '[', '1', ';', '4', 'B' };

static unsigned char const g_feed_key_down_c[] =
{ FEED_KEY_DOWN, 4, 27, '[', '1', ';', '5', 'B' };

static unsigned char const g_feed_key_down_s_c[] =
{ FEED_KEY_DOWN, 5, 27, '[', '1', ';', '6', 'B' };

static unsigned char const g_feed_key_down_a_c[] =
{ FEED_KEY_DOWN, 6, 27, '[', '1', ';', '7', 'B' };

static unsigned char const g_feed_key_down_s_a_c[] =
{ FEED_KEY_DOWN, 7, 27, '[', '1', ';', '8', 'B' };

/* Right ^[[C */
static unsigned char const g_feed_key_right[] =
{ FEED_KEY_RIGHT, 0, 27, '[', 'C' };

static unsigned char const g_feed_key_right_s[] =
{ FEED_KEY_RIGHT, 1, 27, '[', '1', ';', '2', 'C' };

static unsigned char const g_feed_key_right_a[] =
{ FEED_KEY_RIGHT, 2, 27, '[', '1', ';', '3', 'C' };

static unsigned char const g_feed_key_right_s_a[] =
{ FEED_KEY_RIGHT, 3, 27, '[', '1', ';', '4', 'C' };

static unsigned char const g_feed_key_right_c[] =
{ FEED_KEY_RIGHT, 4, 27, '[', '1', ';', '5', 'C' };

static unsigned char const g_feed_key_right_s_c[] =
{ FEED_KEY_RIGHT, 5, 27, '[', '1', ';', '6', 'C' };

static unsigned char const g_feed_key_right_a_c[] =
{ FEED_KEY_RIGHT, 6, 27, '[', '1', ';', '7', 'C' };

static unsigned char const g_feed_key_right_s_a_c[] =
{ FEED_KEY_RIGHT, 7, 27, '[', '1', ';', '8', 'C' };

/* Left ^[[D */
static unsigned char const g_feed_key_left[] =
{ FEED_KEY_LEFT, 0, 27, '[', 'D' };

static unsigned char const g_feed_key_left_s[] =
{ FEED_KEY_LEFT, 1, 27, '[', '1', ';', '2', 'D' };

static unsigned char const g_feed_key_left_a[] =
{ FEED_KEY_LEFT, 2, 27, '[', '1', ';', '3', 'D' };

static unsigned char const g_feed_key_left_s_a[] =
{ FEED_KEY_LEFT, 3, 27, '[', '1', ';', '4', 'D' };

static unsigned char const g_feed_key_left_c[] =
{ FEED_KEY_LEFT, 4, 27, '[', '1', ';', '5', 'D' };

static unsigned char const g_feed_key_left_s_c[] =
{ FEED_KEY_LEFT, 5, 27, '[', '1', ';', '6', 'D' };

static unsigned char const g_feed_key_left_a_c[] =
{ FEED_KEY_LEFT, 6, 27, '[', '1', ';', '7', 'D' };

static unsigned char const g_feed_key_left_s_a_c[] =
{ FEED_KEY_LEFT, 7, 27, '[', '1', ';', '8', 'D' };

/* Home ^[[H */
static unsigned char const g_feed_key_home[] =
{ FEED_KEY_HOME, 0, 27, '[', 'H' };

static unsigned char const g_feed_key_home_s[] =
{ FEED_KEY_HOME, 1, 27, '[', '1', ';', '2', 'H' };

static unsigned char const g_feed_key_home_a[] =
{ FEED_KEY_HOME, 2, 27, '[', '1', ';', '3', 'H' };

static unsigned char const g_feed_key_home_s_a[] =
{ FEED_KEY_HOME, 3, 27, '[', '1', ';', '4', 'H' };

static unsigned char const g_feed_key_home_c[] =
{ FEED_KEY_HOME, 4, 27, '[', '1', ';', '5', 'H' };

static unsigned char const g_feed_key_home_s_c[] =
{ FEED_KEY_HOME, 5, 27, '[', '1', ';', '6', 'H' };

static unsigned char const g_feed_key_home_a_c[] =
{ FEED_KEY_HOME, 6, 27, '[', '1', ';', '7', 'H' };

static unsigned char const g_feed_key_home_s_a_c[] =
{ FEED_KEY_HOME, 7, 27, '[', '1', ';', '8', 'H' };

/* End ^[[F */
static unsigned char const g_feed_key_end[] =
{ FEED_KEY_END, 0, 27, '[', 'F' };

static unsigned char const g_feed_key_end_s[] =
{ FEED_KEY_END, 1, 27, '[', '1', ';', '2', 'F' };

static unsigned char const g_feed_key_end_a[] =
{ FEED_KEY_END, 2, 27, '[', '1', ';', '3', 'F' };

static unsigned char const g_feed_key_end_s_a[] =
{ FEED_KEY_END, 3, 27, '[', '1', ';', '4', 'F' };

static unsigned char const g_feed_key_end_c[] =
{ FEED_KEY_END, 4, 27, '[', '1', ';', '5', 'F' };

static unsigned char const g_feed_key_end_s_c[] =
{ FEED_KEY_END, 5, 27, '[', '1', ';', '6', 'F' };

static unsigned char const g_feed_key_end_a_c[] =
{ FEED_KEY_END, 6, 27, '[', '1', ';', '7', 'F' };

static unsigned char const g_feed_key_end_s_a_c[] =
{ FEED_KEY_END, 7, 27, '[', '1', ';', '8', 'F' };

/* Insert ^[[2;1~ */
static unsigned char const g_feed_key_insert[] =
{ FEED_KEY_INSERT, 0, 27, '[', '2', '~' };

static unsigned char const g_feed_key_insert_s[] =
{ FEED_KEY_INSERT, 1, 27, '[', '2', ';', '2', '~' };

static unsigned char const g_feed_key_insert_a[] =
{ FEED_KEY_INSERT, 2, 27, '[', '2', ';', '3', '~' };

static unsigned char const g_feed_key_insert_s_a[] =
{ FEED_KEY_INSERT, 3, 27, '[', '2', ';', '4', '~' };

static unsigned char const g_feed_key_insert_c[] =
{ FEED_KEY_INSERT, 4, 27, '[', '2', ';', '5', '~' };

static unsigned char const g_feed_key_insert_s_c[] =
{ FEED_KEY_INSERT, 5, 27, '[', '2', ';', '6', '~' };

static unsigned char const g_feed_key_insert_a_c[] =
{ FEED_KEY_INSERT, 6, 27, '[', '2', ';', '7', '~' };

static unsigned char const g_feed_key_insert_s_a_c[] =
{ FEED_KEY_INSERT, 7, 27, '[', '2', ';', '8', '~' };

/* Delete ^[[3;1~ */
static unsigned char const g_feed_key_delete[] =
{ FEED_KEY_DELETE, 0, 27, '[', '3', '~' };

static unsigned char const g_feed_key_delete_s[] =
{ FEED_KEY_DELETE, 1, 27, '[', '3', ';', '2', '~' };

static unsigned char const g_feed_key_delete_a[] =
{ FEED_KEY_DELETE, 2, 27, '[', '3', ';', '3', '~' };

static unsigned char const g_feed_key_delete_s_a[] =
{ FEED_KEY_DELETE, 3, 27, '[', '3', ';', '4', '~' };

static unsigned char const g_feed_key_delete_c[] =
{ FEED_KEY_DELETE, 4, 27, '[', '3', ';', '5', '~' };

static unsigned char const g_feed_key_delete_s_c[] =
{ FEED_KEY_DELETE, 5, 27, '[', '3', ';', '6', '~' };

static unsigned char const g_feed_key_delete_a_c[] =
{ FEED_KEY_DELETE, 6, 27, '[', '3', ';', '7', '~' };

static unsigned char const g_feed_key_delete_s_a_c[] =
{ FEED_KEY_DELETE, 7, 27, '[', '3', ';', '8', '~' };

/* PageUp ^[[5;1~ */
static unsigned char const g_feed_key_pageup[] =
{ FEED_KEY_PAGEUP, 0, 27, '[', '5', '~' };

static unsigned char const g_feed_key_pageup_s[] =
{ FEED_KEY_PAGEUP, 1, 27, '[', '5', ';', '2', '~' };

static unsigned char const g_feed_key_pageup_a[] =
{ FEED_KEY_PAGEUP, 2, 27, '[', '5', ';', '3', '~' };

static unsigned char const g_feed_key_pageup_s_a[] =
{ FEED_KEY_PAGEUP, 3, 27, '[', '5', ';', '4', '~' };

static unsigned char const g_feed_key_pageup_c[] =
{ FEED_KEY_PAGEUP, 4, 27, '[', '5', ';', '5', '~' };

static unsigned char const g_feed_key_pageup_s_c[] =
{ FEED_KEY_PAGEUP, 5, 27, '[', '5', ';', '6', '~' };

static unsigned char const g_feed_key_pageup_a_c[] =
{ FEED_KEY_PAGEUP, 6, 27, '[', '5', ';', '7', '~' };

static unsigned char const g_feed_key_pageup_s_a_c[] =
{ FEED_KEY_PAGEUP, 7, 27, '[', '5', ';', '8', '~' };

/* PageDown ^[[6;1~ */
static unsigned char const g_feed_key_pagedown[] =
{ FEED_KEY_PAGEDOWN, 0, 27, '[', '6', '~' };

static unsigned char const g_feed_key_pagedown_s[] =
{ FEED_KEY_PAGEDOWN, 1, 27, '[', '6', ';', '2', '~' };

static unsigned char const g_feed_key_pagedown_a[] =
{ FEED_KEY_PAGEDOWN, 2, 27, '[', '6', ';', '3', '~' };

static unsigned char const g_feed_key_pagedown_s_a[] =
{ FEED_KEY_PAGEDOWN, 3, 27, '[', '6', ';', '4', '~' };

static unsigned char const g_feed_key_pagedown_c[] =
{ FEED_KEY_PAGEDOWN, 4, 27, '[', '6', ';', '5', '~' };

static unsigned char const g_feed_key_pagedown_s_c[] =
{ FEED_KEY_PAGEDOWN, 5, 27, '[', '6', ';', '6', '~' };

static unsigned char const g_feed_key_pagedown_a_c[] =
{ FEED_KEY_PAGEDOWN, 6, 27, '[', '6', ';', '7', '~' };

static unsigned char const g_feed_key_pagedown_s_a_c[] =
{ FEED_KEY_PAGEDOWN, 7, 27, '[', '6', ';', '8', '~' };

/* Note: Fn keys do not support ctrl+alt combination */

/* F1 ^[OP */
static unsigned char const g_feed_key_f1[] =
{ FEED_KEY_F1, 0, 27, 'O', 'P' };

static unsigned char const g_feed_key_f1_s[] =
{ FEED_KEY_F1, 1, 27, '[', '1', ';', '2', 'P' };

static unsigned char const g_feed_key_f1_a[] =
{ FEED_KEY_F1, 2, 27, '[', '1', ';', '3', 'P' };

static unsigned char const g_feed_key_f1_s_a[] =
{ FEED_KEY_F1, 3, 27, '[', '1', ';', '4', 'P' };

static unsigned char const g_feed_key_f1_c[] =
{ FEED_KEY_F1, 4, 27, '[', '1', ';', '5', 'P' };

static unsigned char const g_feed_key_f1_s_c[] =
{ FEED_KEY_F1, 5, 27, '[', '1', ';', '6', 'P' };

static unsigned char const g_feed_key_f1_a_c[] =
{ FEED_KEY_F1, 6, 27, '[', '1', ';', '7', 'P' };

static unsigned char const g_feed_key_f1_s_a_c[] =
{ FEED_KEY_F1, 7, 27, '[', '1', ';', '8', 'P' };

/* F2 ^[OQ */
static unsigned char const g_feed_key_f2[] =
{ FEED_KEY_F2, 0, 27, 'O', 'Q' };

static unsigned char const g_feed_key_f2_s[] =
{ FEED_KEY_F2, 1, 27, '[', '1', ';', '2', 'Q' };

static unsigned char const g_feed_key_f2_a[] =
{ FEED_KEY_F2, 2, 27, '[', '1', ';', '3', 'Q' };

static unsigned char const g_feed_key_f2_s_a[] =
{ FEED_KEY_F2, 3, 27, '[', '1', ';', '4', 'Q' };

static unsigned char const g_feed_key_f2_c[] =
{ FEED_KEY_F2, 4, 27, '[', '1', ';', '5', 'Q' };

static unsigned char const g_feed_key_f2_s_c[] =
{ FEED_KEY_F2, 5, 27, '[', '1', ';', '6', 'Q' };

static unsigned char const g_feed_key_f2_a_c[] =
{ FEED_KEY_F2, 6, 27, '[', '1', ';', '7', 'Q' };

static unsigned char const g_feed_key_f2_s_a_c[] =
{ FEED_KEY_F2, 7, 27, '[', '1', ';', '8', 'Q' };

/* F3 ^[OR */
static unsigned char const g_feed_key_f3[] =
{ FEED_KEY_F3, 0, 27, 'O', 'R' };

static unsigned char const g_feed_key_f3_s[] =
{ FEED_KEY_F3, 1, 27, '[', '1', ';', '2', 'R' };

static unsigned char const g_feed_key_f3_a[] =
{ FEED_KEY_F3, 2, 27, '[', '1', ';', '3', 'R' };

static unsigned char const g_feed_key_f3_s_a[] =
{ FEED_KEY_F3, 3, 27, '[', '1', ';', '4', 'R' };

static unsigned char const g_feed_key_f3_c[] =
{ FEED_KEY_F3, 4, 27, '[', '1', ';', '5', 'R' };

static unsigned char const g_feed_key_f3_s_c[] =
{ FEED_KEY_F3, 5, 27, '[', '1', ';', '6', 'R' };

static unsigned char const g_feed_key_f3_a_c[] =
{ FEED_KEY_F3, 6, 27, '[', '1', ';', '7', 'R' };

static unsigned char const g_feed_key_f3_s_a_c[] =
{ FEED_KEY_F3, 7, 27, '[', '1', ';', '8', 'R' };

/* F4 ^[OS */
static unsigned char const g_feed_key_f4[] =
{ FEED_KEY_F4, 0, 27, 'O', 'S' };

static unsigned char const g_feed_key_f4_s[] =
{ FEED_KEY_F4, 1, 27, '[', '1', ';', '2', 'S' };

static unsigned char const g_feed_key_f4_a[] =
{ FEED_KEY_F4, 2, 27, '[', '1', ';', '3', 'S' };

static unsigned char const g_feed_key_f4_s_a[] =
{ FEED_KEY_F4, 3, 27, '[', '1', ';', '4', 'S' };

static unsigned char const g_feed_key_f4_c[] =
{ FEED_KEY_F4, 4, 27, '[', '1', ';', '5', 'S' };

static unsigned char const g_feed_key_f4_s_c[] =
{ FEED_KEY_F4, 5, 27, '[', '1', ';', '6', 'S' };

static unsigned char const g_feed_key_f4_a_c[] =
{ FEED_KEY_F4, 6, 27, '[', '1', ';', '7', 'S' };

static unsigned char const g_feed_key_f4_s_a_c[] =
{ FEED_KEY_F4, 7, 27, '[', '1', ';', '8', 'S' };

/* F5 ^[[15;1~ */
static unsigned char const g_feed_key_f5[] =
{ FEED_KEY_F5, 0, 27, '[', '1', '5', '~' };

static unsigned char const g_feed_key_f5_s[] =
{ FEED_KEY_F5, 1, 27, '[', '1', '5', ';', '2', '~' };

static unsigned char const g_feed_key_f5_a[] =
{ FEED_KEY_F5, 2, 27, '[', '1', '5', ';', '3', '~' };

static unsigned char const g_feed_key_f5_s_a[] =
{ FEED_KEY_F5, 3, 27, '[', '1', '5', ';', '4', '~' };

static unsigned char const g_feed_key_f5_c[] =
{ FEED_KEY_F5, 4, 27, '[', '1', '5', ';', '5', '~' };

static unsigned char const g_feed_key_f5_s_c[] =
{ FEED_KEY_F5, 5, 27, '[', '1', '5', ';', '6', '~' };

static unsigned char const g_feed_key_f5_a_c[] =
{ FEED_KEY_F5, 6, 27, '[', '1', '5', ';', '7', '~' };

static unsigned char const g_feed_key_f5_s_a_c[] =
{ FEED_KEY_F5, 7, 27, '[', '1', '5', ';', '8', '~' };

/* F6 ^[[17;1~ */
static unsigned char const g_feed_key_f6[] =
{ FEED_KEY_F6, 0, 27, '[', '1', '7', '~' };

static unsigned char const g_feed_key_f6_s[] =
{ FEED_KEY_F6, 1, 27, '[', '1', '7', ';', '2', '~' };

static unsigned char const g_feed_key_f6_a[] =
{ FEED_KEY_F6, 2, 27, '[', '1', '7', ';', '3', '~' };

static unsigned char const g_feed_key_f6_s_a[] =
{ FEED_KEY_F6, 3, 27, '[', '1', '7', ';', '4', '~' };

static unsigned char const g_feed_key_f6_c[] =
{ FEED_KEY_F6, 4, 27, '[', '1', '7', ';', '5', '~' };

static unsigned char const g_feed_key_f6_s_c[] =
{ FEED_KEY_F6, 5, 27, '[', '1', '7', ';', '6', '~' };

static unsigned char const g_feed_key_f6_a_c[] =
{ FEED_KEY_F6, 6, 27, '[', '1', '7', ';', '7', '~' };

static unsigned char const g_feed_key_f6_s_a_c[] =
{ FEED_KEY_F6, 7, 27, '[', '1', '7', ';', '8', '~' };

/* F7 ^[[18;1~ */
static unsigned char const g_feed_key_f7[] =
{ FEED_KEY_F7, 0, 27, '[', '1', '8', '~' };

static unsigned char const g_feed_key_f7_s[] =
{ FEED_KEY_F7, 1, 27, '[', '1', '8', ';', '2', '~' };

static unsigned char const g_feed_key_f7_a[] =
{ FEED_KEY_F7, 2, 27, '[', '1', '8', ';', '3', '~' };

static unsigned char const g_feed_key_f7_s_a[] =
{ FEED_KEY_F7, 3, 27, '[', '1', '8', ';', '4', '~' };

static unsigned char const g_feed_key_f7_c[] =
{ FEED_KEY_F7, 4, 27, '[', '1', '8', ';', '5', '~' };

static unsigned char const g_feed_key_f7_s_c[] =
{ FEED_KEY_F7, 5, 27, '[', '1', '8', ';', '6', '~' };

static unsigned char const g_feed_key_f7_a_c[] =
{ FEED_KEY_F7, 6, 27, '[', '1', '8', ';', '7', '~' };

static unsigned char const g_feed_key_f7_s_a_c[] =
{ FEED_KEY_F7, 7, 27, '[', '1', '8', ';', '8', '~' };

/* F8 ^[[19;1~ */
static unsigned char const g_feed_key_f8[] =
{ FEED_KEY_F8, 0, 27, '[', '1', '9', '~' };

static unsigned char const g_feed_key_f8_s[] =
{ FEED_KEY_F8, 1, 27, '[', '1', '9', ';', '2', '~' };

static unsigned char const g_feed_key_f8_a[] =
{ FEED_KEY_F8, 2, 27, '[', '1', '9', ';', '3', '~' };

static unsigned char const g_feed_key_f8_s_a[] =
{ FEED_KEY_F8, 3, 27, '[', '1', '9', ';', '4', '~' };

static unsigned char const g_feed_key_f8_c[] =
{ FEED_KEY_F8, 4, 27, '[', '1', '9', ';', '5', '~' };

static unsigned char const g_feed_key_f8_s_c[] =
{ FEED_KEY_F8, 5, 27, '[', '1', '9', ';', '6', '~' };

static unsigned char const g_feed_key_f8_a_c[] =
{ FEED_KEY_F8, 6, 27, '[', '1', '9', ';', '7', '~' };

static unsigned char const g_feed_key_f8_s_a_c[] =
{ FEED_KEY_F8, 7, 27, '[', '1', '9', ';', '8', '~' };

/* F9 ^[[20;1~ */
static unsigned char const g_feed_key_f9[] =
{ FEED_KEY_F9, 0, 27, '[', '2', '0', '~' };

static unsigned char const g_feed_key_f9_s[] =
{ FEED_KEY_F9, 1, 27, '[', '2', '0', ';', '2', '~' };

static unsigned char const g_feed_key_f9_a[] =
{ FEED_KEY_F9, 2, 27, '[', '2', '0', ';', '3', '~' };

static unsigned char const g_feed_key_f9_s_a[] =
{ FEED_KEY_F9, 3, 27, '[', '2', '0', ';', '4', '~' };

static unsigned char const g_feed_key_f9_c[] =
{ FEED_KEY_F9, 4, 27, '[', '2', '0', ';', '5', '~' };

static unsigned char const g_feed_key_f9_s_c[] =
{ FEED_KEY_F9, 5, 27, '[', '2', '0', ';', '6', '~' };

static unsigned char const g_feed_key_f9_a_c[] =
{ FEED_KEY_F9, 6, 27, '[', '2', '0', ';', '7', '~' };

static unsigned char const g_feed_key_f9_s_a_c[] =
{ FEED_KEY_F9, 7, 27, '[', '2', '0', ';', '8', '~' };

/* F10 ^[[21;1~ */
static unsigned char const g_feed_key_f10[] =
{ FEED_KEY_F10, 0, 27, '[', '2', '1', '~' };

static unsigned char const g_feed_key_f10_s[] =
{ FEED_KEY_F10, 1, 27, '[', '2', '1', ';', '2', '~' };

static unsigned char const g_feed_key_f10_a[] =
{ FEED_KEY_F10, 2, 27, '[', '2', '1', ';', '3', '~' };

static unsigned char const g_feed_key_f10_s_a[] =
{ FEED_KEY_F10, 3, 27, '[', '2', '1', ';', '4', '~' };

static unsigned char const g_feed_key_f10_c[] =
{ FEED_KEY_F10, 4, 27, '[', '2', '1', ';', '5', '~' };

static unsigned char const g_feed_key_f10_s_c[] =
{ FEED_KEY_F10, 5, 27, '[', '2', '1', ';', '6', '~' };

static unsigned char const g_feed_key_f10_a_c[] =
{ FEED_KEY_F10, 6, 27, '[', '2', '1', ';', '7', '~' };

static unsigned char const g_feed_key_f10_s_a_c[] =
{ FEED_KEY_F10, 7, 27, '[', '2', '1', ';', '8', '~' };

/* F11 ^[[23;1~ */
static unsigned char const g_feed_key_f11[] =
{ FEED_KEY_F11, 0, 27, '[', '2', '3', '~' };

static unsigned char const g_feed_key_f11_s[] =
{ FEED_KEY_F11, 1, 27, '[', '2', '3', ';', '2', '~' };

static unsigned char const g_feed_key_f11_a[] =
{ FEED_KEY_F11, 2, 27, '[', '2', '3', ';', '3', '~' };

static unsigned char const g_feed_key_f11_s_a[] =
{ FEED_KEY_F11, 3, 27, '[', '2', '3', ';', '4', '~' };

static unsigned char const g_feed_key_f11_c[] =
{ FEED_KEY_F11, 4, 27, '[', '2', '3', ';', '5', '~' };

static unsigned char const g_feed_key_f11_s_c[] =
{ FEED_KEY_F11, 5, 27, '[', '2', '3', ';', '6', '~' };

static unsigned char const g_feed_key_f11_a_c[] =
{ FEED_KEY_F11, 6, 27, '[', '2', '3', ';', '7', '~' };

static unsigned char const g_feed_key_f11_s_a_c[] =
{ FEED_KEY_F11, 7, 27, '[', '2', '3', ';', '8', '~' };

/* F12 ^[[24;1~ */
static unsigned char const g_feed_key_f12[] =
{ FEED_KEY_F12, 0, 27, '[', '2', '4', '~' };

static unsigned char const g_feed_key_f12_s[] =
{ FEED_KEY_F12, 1, 27, '[', '2', '4', ';', '2', '~' };

static unsigned char const g_feed_key_f12_a[] =
{ FEED_KEY_F12, 2, 27, '[', '2', '4', ';', '3', '~' };

static unsigned char const g_feed_key_f12_s_a[] =
{ FEED_KEY_F12, 3, 27, '[', '2', '4', ';', '4', '~' };

static unsigned char const g_feed_key_f12_c[] =
{ FEED_KEY_F12, 4, 27, '[', '2', '4', ';', '5', '~' };

static unsigned char const g_feed_key_f12_s_c[] =
{ FEED_KEY_F12, 5, 27, '[', '2', '4', ';', '6', '~' };

static unsigned char const g_feed_key_f12_a_c[] =
{ FEED_KEY_F12, 6, 27, '[', '2', '4', ';', '7', '~' };

static unsigned char const g_feed_key_f12_s_a_c[] =
{ FEED_KEY_F12, 7, 27, '[', '2', '4', ';', '8', '~' };

/* Backspace */
static unsigned char const g_feed_key_backspace[] =
{ 'H', 4, 127 };

/* Shift + Tab */
static unsigned char const g_feed_key_I_s_c[] =
{ 'I', 5, 27, '[', 'Z' };

/* etc. */

struct feed_keys_node
{
    unsigned char const *
        p_name;

    size_t
        i_name_length;

}; /* struct feed_keys_node */

static struct feed_keys_node const g_feed_keys_lookup_table[] =
{
    { g_feed_key_up, sizeof(g_feed_key_up) },

    { g_feed_key_up_s, sizeof(g_feed_key_up_s) },

    { g_feed_key_up_a, sizeof(g_feed_key_up_a) },

    { g_feed_key_up_s_a, sizeof(g_feed_key_up_s_a) },

    { g_feed_key_up_c, sizeof(g_feed_key_up_c) },

    { g_feed_key_up_s_c, sizeof(g_feed_key_up_s_c) },

    { g_feed_key_up_a_c, sizeof(g_feed_key_up_a_c) },

    { g_feed_key_up_s_a_c, sizeof(g_feed_key_up_s_a_c) },

    { g_feed_key_down, sizeof(g_feed_key_down) },

    { g_feed_key_down_s, sizeof(g_feed_key_down_s) },

    { g_feed_key_down_a, sizeof(g_feed_key_down_a) },

    { g_feed_key_down_s_a, sizeof(g_feed_key_down_s_a) },

    { g_feed_key_down_c, sizeof(g_feed_key_down_c) },

    { g_feed_key_down_s_c, sizeof(g_feed_key_down_s_c) },

    { g_feed_key_down_a_c, sizeof(g_feed_key_down_a_c) },

    { g_feed_key_down_s_a_c, sizeof(g_feed_key_down_s_a_c) },

    { g_feed_key_right, sizeof(g_feed_key_right) },

    { g_feed_key_right_s, sizeof(g_feed_key_right_s) },

    { g_feed_key_right_a, sizeof(g_feed_key_right_a) },

    { g_feed_key_right_s_a, sizeof(g_feed_key_right_s_a) },

    { g_feed_key_right_c, sizeof(g_feed_key_right_c) },

    { g_feed_key_right_s_c, sizeof(g_feed_key_right_s_c) },

    { g_feed_key_right_a_c, sizeof(g_feed_key_right_a_c) },

    { g_feed_key_right_s_a_c, sizeof(g_feed_key_right_s_a_c) },

    { g_feed_key_left, sizeof(g_feed_key_left) },

    { g_feed_key_left_s, sizeof(g_feed_key_left_s) },

    { g_feed_key_left_a, sizeof(g_feed_key_left_a) },

    { g_feed_key_left_s_a, sizeof(g_feed_key_left_s_a) },

    { g_feed_key_left_c, sizeof(g_feed_key_left_c) },

    { g_feed_key_left_s_c, sizeof(g_feed_key_left_s_c) },

    { g_feed_key_left_a_c, sizeof(g_feed_key_left_a_c) },

    { g_feed_key_left_s_a_c, sizeof(g_feed_key_left_s_a_c) },

    { g_feed_key_home, sizeof(g_feed_key_home) },

    { g_feed_key_home_s, sizeof(g_feed_key_home_s) },

    { g_feed_key_home_a, sizeof(g_feed_key_home_a) },

    { g_feed_key_home_s_a, sizeof(g_feed_key_home_s_a) },

    { g_feed_key_home_c, sizeof(g_feed_key_home_c) },

    { g_feed_key_home_s_c, sizeof(g_feed_key_home_s_c) },

    { g_feed_key_home_a_c, sizeof(g_feed_key_home_a_c) },

    { g_feed_key_home_s_a_c, sizeof(g_feed_key_home_s_a_c) },

    { g_feed_key_end, sizeof(g_feed_key_end) },

    { g_feed_key_end_s, sizeof(g_feed_key_end_s) },

    { g_feed_key_end_a, sizeof(g_feed_key_end_a) },

    { g_feed_key_end_s_a, sizeof(g_feed_key_end_s_a) },

    { g_feed_key_end_c, sizeof(g_feed_key_end_c) },

    { g_feed_key_end_s_c, sizeof(g_feed_key_end_s_c) },

    { g_feed_key_end_a_c, sizeof(g_feed_key_end_a_c) },

    { g_feed_key_end_s_a_c, sizeof(g_feed_key_end_s_a_c) },

    { g_feed_key_insert, sizeof(g_feed_key_insert) },

    { g_feed_key_insert_s, sizeof(g_feed_key_insert_s) },

    { g_feed_key_insert_a, sizeof(g_feed_key_insert_a) },

    { g_feed_key_insert_s_a, sizeof(g_feed_key_insert_s_a) },

    { g_feed_key_insert_c, sizeof(g_feed_key_insert_c) },

    { g_feed_key_insert_s_c, sizeof(g_feed_key_insert_s_c) },

    { g_feed_key_insert_a_c, sizeof(g_feed_key_insert_a_c) },

    { g_feed_key_insert_s_a_c, sizeof(g_feed_key_insert_s_a_c) },

    { g_feed_key_delete, sizeof(g_feed_key_delete) },

    { g_feed_key_delete_s, sizeof(g_feed_key_delete_s) },

    { g_feed_key_delete_a, sizeof(g_feed_key_delete_a) },

    { g_feed_key_delete_s_a, sizeof(g_feed_key_delete_s_a) },

    { g_feed_key_delete_c, sizeof(g_feed_key_delete_c) },

    { g_feed_key_delete_s_c, sizeof(g_feed_key_delete_s_c) },

    { g_feed_key_delete_a_c, sizeof(g_feed_key_delete_a_c) },

    { g_feed_key_delete_s_a_c, sizeof(g_feed_key_delete_s_a_c) },

    { g_feed_key_pageup, sizeof(g_feed_key_pageup) },

    { g_feed_key_pageup_s, sizeof(g_feed_key_pageup_s) },

    { g_feed_key_pageup_a, sizeof(g_feed_key_pageup_a) },

    { g_feed_key_pageup_s_a, sizeof(g_feed_key_pageup_s_a) },

    { g_feed_key_pageup_c, sizeof(g_feed_key_pageup_c) },

    { g_feed_key_pageup_s_c, sizeof(g_feed_key_pageup_s_c) },

    { g_feed_key_pageup_a_c, sizeof(g_feed_key_pageup_a_c) },

    { g_feed_key_pageup_s_a_c, sizeof(g_feed_key_pageup_s_a_c) },

    { g_feed_key_pagedown, sizeof(g_feed_key_pagedown) },

    { g_feed_key_pagedown_s, sizeof(g_feed_key_pagedown_s) },

    { g_feed_key_pagedown_a, sizeof(g_feed_key_pagedown_a) },

    { g_feed_key_pagedown_s_a, sizeof(g_feed_key_pagedown_s_a) },

    { g_feed_key_pagedown_c, sizeof(g_feed_key_pagedown_c) },

    { g_feed_key_pagedown_s_c, sizeof(g_feed_key_pagedown_s_c) },

    { g_feed_key_pagedown_a_c, sizeof(g_feed_key_pagedown_a_c) },

    { g_feed_key_pagedown_s_a_c, sizeof(g_feed_key_pagedown_s_a_c) },

    { g_feed_key_f1, sizeof(g_feed_key_f1) },

    { g_feed_key_f1_s, sizeof(g_feed_key_f1_s) },

    { g_feed_key_f1_a, sizeof(g_feed_key_f1_a) },

    { g_feed_key_f1_s_a, sizeof(g_feed_key_f1_s_a) },

    { g_feed_key_f1_c, sizeof(g_feed_key_f1_c) },

    { g_feed_key_f1_s_c, sizeof(g_feed_key_f1_s_c) },

    { g_feed_key_f1_a_c, sizeof(g_feed_key_f1_a_c) },

    { g_feed_key_f1_s_a_c, sizeof(g_feed_key_f1_s_a_c) },

    { g_feed_key_f2, sizeof(g_feed_key_f2) },

    { g_feed_key_f2_s, sizeof(g_feed_key_f2_s) },

    { g_feed_key_f2_a, sizeof(g_feed_key_f2_a) },

    { g_feed_key_f2_s_a, sizeof(g_feed_key_f2_s_a) },

    { g_feed_key_f2_c, sizeof(g_feed_key_f2_c) },

    { g_feed_key_f2_s_c, sizeof(g_feed_key_f2_s_c) },

    { g_feed_key_f2_a_c, sizeof(g_feed_key_f2_a_c) },

    { g_feed_key_f2_s_a_c, sizeof(g_feed_key_f2_s_a_c) },

    { g_feed_key_f3, sizeof(g_feed_key_f3) },

    { g_feed_key_f3_s, sizeof(g_feed_key_f3_s) },

    { g_feed_key_f3_a, sizeof(g_feed_key_f3_a) },

    { g_feed_key_f3_s_a, sizeof(g_feed_key_f3_s_a) },

    { g_feed_key_f3_c, sizeof(g_feed_key_f3_c) },

    { g_feed_key_f3_s_c, sizeof(g_feed_key_f3_s_c) },

    { g_feed_key_f3_a_c, sizeof(g_feed_key_f3_a_c) },

    { g_feed_key_f3_s_a_c, sizeof(g_feed_key_f3_s_a_c) },

    { g_feed_key_f4, sizeof(g_feed_key_f4) },

    { g_feed_key_f4_s, sizeof(g_feed_key_f4_s) },

    { g_feed_key_f4_a, sizeof(g_feed_key_f4_a) },

    { g_feed_key_f4_s_a, sizeof(g_feed_key_f4_s_a) },

    { g_feed_key_f4_c, sizeof(g_feed_key_f4_c) },

    { g_feed_key_f4_s_c, sizeof(g_feed_key_f4_s_c) },

    { g_feed_key_f4_a_c, sizeof(g_feed_key_f4_a_c) },

    { g_feed_key_f4_s_a_c, sizeof(g_feed_key_f4_s_a_c) },

    { g_feed_key_f5, sizeof(g_feed_key_f5) },

    { g_feed_key_f5_s, sizeof(g_feed_key_f5_s) },

    { g_feed_key_f5_a, sizeof(g_feed_key_f5_a) },

    { g_feed_key_f5_s_a, sizeof(g_feed_key_f5_s_a) },

    { g_feed_key_f5_c, sizeof(g_feed_key_f5_c) },

    { g_feed_key_f5_s_c, sizeof(g_feed_key_f5_s_c) },

    { g_feed_key_f5_a_c, sizeof(g_feed_key_f5_a_c) },

    { g_feed_key_f5_s_a_c, sizeof(g_feed_key_f5_s_a_c) },

    { g_feed_key_f6, sizeof(g_feed_key_f6) },

    { g_feed_key_f6_s, sizeof(g_feed_key_f6_s) },

    { g_feed_key_f6_a, sizeof(g_feed_key_f6_a) },

    { g_feed_key_f6_s_a, sizeof(g_feed_key_f6_s_a) },

    { g_feed_key_f6_c, sizeof(g_feed_key_f6_c) },

    { g_feed_key_f6_s_c, sizeof(g_feed_key_f6_s_c) },

    { g_feed_key_f6_a_c, sizeof(g_feed_key_f6_a_c) },

    { g_feed_key_f6_s_a_c, sizeof(g_feed_key_f6_s_a_c) },

    { g_feed_key_f7, sizeof(g_feed_key_f7) },

    { g_feed_key_f7_s, sizeof(g_feed_key_f7_s) },

    { g_feed_key_f7_a, sizeof(g_feed_key_f7_a) },

    { g_feed_key_f7_s_a, sizeof(g_feed_key_f7_s_a) },

    { g_feed_key_f7_c, sizeof(g_feed_key_f7_c) },

    { g_feed_key_f7_s_c, sizeof(g_feed_key_f7_s_c) },

    { g_feed_key_f7_a_c, sizeof(g_feed_key_f7_a_c) },

    { g_feed_key_f7_s_a_c, sizeof(g_feed_key_f7_s_a_c) },

    { g_feed_key_f8, sizeof(g_feed_key_f8) },

    { g_feed_key_f8_s, sizeof(g_feed_key_f8_s) },

    { g_feed_key_f8_a, sizeof(g_feed_key_f8_a) },

    { g_feed_key_f8_s_a, sizeof(g_feed_key_f8_s_a) },

    { g_feed_key_f8_c, sizeof(g_feed_key_f8_c) },

    { g_feed_key_f8_s_c, sizeof(g_feed_key_f8_s_c) },

    { g_feed_key_f8_a_c, sizeof(g_feed_key_f8_a_c) },

    { g_feed_key_f8_s_a_c, sizeof(g_feed_key_f8_s_a_c) },

    { g_feed_key_f9, sizeof(g_feed_key_f9) },

    { g_feed_key_f9_s, sizeof(g_feed_key_f9_s) },

    { g_feed_key_f9_a, sizeof(g_feed_key_f9_a) },

    { g_feed_key_f9_s_a, sizeof(g_feed_key_f9_s_a) },

    { g_feed_key_f9_c, sizeof(g_feed_key_f9_c) },

    { g_feed_key_f9_s_c, sizeof(g_feed_key_f9_s_c) },

    { g_feed_key_f9_a_c, sizeof(g_feed_key_f9_a_c) },

    { g_feed_key_f9_s_a_c, sizeof(g_feed_key_f9_s_a_c) },

    { g_feed_key_f10, sizeof(g_feed_key_f10) },

    { g_feed_key_f10_s, sizeof(g_feed_key_f10_s) },

    { g_feed_key_f10_a, sizeof(g_feed_key_f10_a) },

    { g_feed_key_f10_s_a, sizeof(g_feed_key_f10_s_a) },

    { g_feed_key_f10_c, sizeof(g_feed_key_f10_c) },

    { g_feed_key_f10_s_c, sizeof(g_feed_key_f10_s_c) },

    { g_feed_key_f10_a_c, sizeof(g_feed_key_f10_a_c) },

    { g_feed_key_f10_s_a_c, sizeof(g_feed_key_f10_s_a_c) },

    { g_feed_key_f11, sizeof(g_feed_key_f11) },

    { g_feed_key_f11_s, sizeof(g_feed_key_f11_s) },

    { g_feed_key_f11_a, sizeof(g_feed_key_f11_a) },

    { g_feed_key_f11_s_a, sizeof(g_feed_key_f11_s_a) },

    { g_feed_key_f11_c, sizeof(g_feed_key_f11_c) },

    { g_feed_key_f11_s_c, sizeof(g_feed_key_f11_s_c) },

    { g_feed_key_f11_a_c, sizeof(g_feed_key_f11_a_c) },

    { g_feed_key_f11_s_a_c, sizeof(g_feed_key_f11_s_a_c) },

    { g_feed_key_f12, sizeof(g_feed_key_f12) },

    { g_feed_key_f12_s, sizeof(g_feed_key_f12_s) },

    { g_feed_key_f12_a, sizeof(g_feed_key_f12_a) },

    { g_feed_key_f12_s_a, sizeof(g_feed_key_f12_s_a) },

    { g_feed_key_f12_c, sizeof(g_feed_key_f12_c) },

    { g_feed_key_f12_s_c, sizeof(g_feed_key_f12_s_c) },

    { g_feed_key_f12_a_c, sizeof(g_feed_key_f12_a_c) },

    { g_feed_key_f12_s_a_c, sizeof(g_feed_key_f12_s_a_c) },

    { g_feed_key_I_s_c, sizeof(g_feed_key_I_s_c) },

    { g_feed_key_backspace, sizeof(g_feed_key_backspace) },

};

static unsigned int const g_feed_keys_lookup_table_length =
(unsigned int)(
    sizeof(g_feed_keys_lookup_table)
    / sizeof(g_feed_keys_lookup_table[0u]));

static
char
feed_keys_compare_node(
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length,
    struct feed_keys_node const * const
        p_node)
{
    char
        b_result;

    if (
        (i_data_length + 2u)
        == p_node->i_name_length)
    {
        if (
            0
            == memcmp(
                p_data,
                p_node->p_name + 2u,
                i_data_length))
        {
            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

unsigned long int
feed_keys_lookup(
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length)
{
    unsigned long int
        i_code;

    unsigned int
        i;

    struct feed_keys_node const *
        p_node;

    i_code =
        0ul;

    i =
        0;

    while (
        !(i_code)
        && (
            i < g_feed_keys_lookup_table_length))
    {
        p_node =
            g_feed_keys_lookup_table + i;

        if (
            feed_keys_compare_node(
                p_data,
                i_data_length,
                p_node))
        {
            i_code =
                (unsigned long int)(
                    0x80000000ul
                    | (unsigned long int)(unsigned char)(p_node->p_name[0u])
                    | ((unsigned long int)(unsigned char)(p_node->p_name[1u]) << 28u));
        }
        else
        {
            i ++;
        }
    }

    if (!i_code)
    {
        static unsigned char const g_feed_keys_control_table[32u] =
        {
            '@', 'A', 'B', 'C',
            'D', 'E', 'F', 'G',
            'H', 'I', 'J', 'K',
            'L', 'M', 'N', 'O',
            'P', 'Q', 'R', 'S',
            'T', 'U', 'V', 'W',
            'X', 'Y', 'Z', '[',
            '\\', ']', '^', '_'
        };

        /* TODO: Detect simple ctrl and alt sequences */

        if ((i_data_length == 1u) && (p_data[0u] < 32))
        {
            /* ctrl+@ */
            i_code =
                (unsigned long int)(
                    0x80000000ul
                    | (unsigned long int)(unsigned char)(g_feed_keys_control_table[p_data[0u]])
                    | FEED_KEY_CTRL);
        }
        else if ((i_data_length == 2u) && (p_data[0u] == 27) && (p_data[1u] < 32))
        {
            /* alt+ctrl+@ */
            i_code =
                (unsigned long int)(
                    0x80000000ul
                    | (unsigned long int)(unsigned char)(g_feed_keys_control_table[p_data[1u]])
                    | FEED_KEY_CTRL
                    | FEED_KEY_ALT);
        }
        else if ((i_data_length == 2u) && (p_data[0u] == 27) && (p_data[1u] >= 'a') && (p_data[1u] <= 'z'))
        {
            /* alt+A */
            i_code =
                (unsigned long int)(
                    0x80000000ul
                    | ((unsigned long int)(unsigned char)(p_data[1u]) + 'A' - 'a')
                    | FEED_KEY_ALT);

        }
        else if ((i_data_length == 2u) && (p_data[0u] == 27) && (p_data[1u] >= 'A') && (p_data[1u] <= 'Z'))
        {
            /* alt+shift+A */
            i_code =
                (unsigned long int)(
                    0x80000000ul
                    | (unsigned long int)(unsigned char)(p_data[1u])
                    | FEED_KEY_SHIFT
                    | FEED_KEY_ALT);
        }
        else if ((i_data_length == 2u) && (p_data[0u] == 27) && (p_data[1u] < 127))
        {
            /* alt+key */
            i_code =
                (unsigned long int)(
                    0x80000000ul
                    | (unsigned long int)(unsigned char)(p_data[1u])
                    | FEED_KEY_ALT);
        }
        else if ((i_data_length == 2u) && (p_data[0u] == 27) && (p_data[1u] == 127))
        {
            /* alt+backspace */
            i_code =
                (unsigned long int)(
                    0x80000000ul
                    | (unsigned long int)(unsigned char)('H')
                    | FEED_KEY_ALT
                    | FEED_KEY_CTRL);
        }
        else
        {
            /* ? */
        }
    }

    return
        i_code;

} /* feed_keys_lookup() */

static unsigned char const g_feed_key_print_up[] =
{ FEED_KEY_UP, 'U', 'p' };

static unsigned char const g_feed_key_print_down[] =
{ FEED_KEY_DOWN, 'D', 'o', 'w', 'n' };

static unsigned char const g_feed_key_print_right[] =
{ FEED_KEY_RIGHT, 'R', 'i', 'g', 'h', 't' };

static unsigned char const g_feed_key_print_left[] =
{ FEED_KEY_LEFT, 'L', 'e', 'f', 't' };

static unsigned char const g_feed_key_print_home[] =
{ FEED_KEY_HOME, 'H', 'o', 'm', 'e' };

static unsigned char const g_feed_key_print_end[] =
{ FEED_KEY_END, 'E', 'n', 'd' };

static unsigned char const g_feed_key_print_insert[] =
{ FEED_KEY_INSERT, 'I', 'n', 's', 'e', 'r', 't' };

static unsigned char const g_feed_key_print_delete[] =
{ FEED_KEY_DELETE, 'D', 'e', 'l', 'e', 't', 'e' };

static unsigned char const g_feed_key_print_pageup[] =
{ FEED_KEY_PAGEUP, 'P', 'a', 'g', 'e', 'U', 'p' };

static unsigned char const g_feed_key_print_pagedown[] =
{ FEED_KEY_PAGEDOWN, 'P', 'a', 'g', 'e', 'D', 'o', 'w', 'n' };

static unsigned char const g_feed_key_print_f1[] =
{ FEED_KEY_F1, 'F', '1' };

static unsigned char const g_feed_key_print_f2[] =
{ FEED_KEY_F2, 'F', '2' };

static unsigned char const g_feed_key_print_f3[] =
{ FEED_KEY_F3, 'F', '3' };

static unsigned char const g_feed_key_print_f4[] =
{ FEED_KEY_F4, 'F', '4' };

static unsigned char const g_feed_key_print_f5[] =
{ FEED_KEY_F5, 'F', '5' };

static unsigned char const g_feed_key_print_f6[] =
{ FEED_KEY_F6, 'F', '6' };

static unsigned char const g_feed_key_print_f7[] =
{ FEED_KEY_F7, 'F', '7' };

static unsigned char const g_feed_key_print_f8[] =
{ FEED_KEY_F8, 'F', '8' };

static unsigned char const g_feed_key_print_f9[] =
{ FEED_KEY_F9, 'F', '9' };

static unsigned char const g_feed_key_print_f10[] =
{ FEED_KEY_F10, 'F', '1', '0' };

static unsigned char const g_feed_key_print_f11[] =
{ FEED_KEY_F11, 'F', '1', '1' };

static unsigned char const g_feed_key_print_f12[] =
{ FEED_KEY_F12, 'F', '1', '2' };

static unsigned char const g_feed_key_print_I[] =
{ 'I', 'I' };

static struct feed_keys_node const g_feed_keys_print_table[] =
{
    { g_feed_key_print_up, sizeof(g_feed_key_print_up) },

    { g_feed_key_print_down, sizeof(g_feed_key_print_down) },

    { g_feed_key_print_right, sizeof(g_feed_key_print_right) },

    { g_feed_key_print_left, sizeof(g_feed_key_print_left) },

    { g_feed_key_print_home, sizeof(g_feed_key_print_home) },

    { g_feed_key_print_end, sizeof(g_feed_key_print_end) },

    { g_feed_key_print_insert, sizeof(g_feed_key_print_insert) },

    { g_feed_key_print_delete, sizeof(g_feed_key_print_delete) },

    { g_feed_key_print_pageup, sizeof(g_feed_key_print_pageup) },

    { g_feed_key_print_pagedown, sizeof(g_feed_key_print_pagedown) },

    { g_feed_key_print_f1, sizeof(g_feed_key_print_f1) },

    { g_feed_key_print_f2, sizeof(g_feed_key_print_f2) },

    { g_feed_key_print_f3, sizeof(g_feed_key_print_f3) },

    { g_feed_key_print_f4, sizeof(g_feed_key_print_f4) },

    { g_feed_key_print_f5, sizeof(g_feed_key_print_f5) },

    { g_feed_key_print_f6, sizeof(g_feed_key_print_f6) },

    { g_feed_key_print_f7, sizeof(g_feed_key_print_f7) },

    { g_feed_key_print_f8, sizeof(g_feed_key_print_f8) },

    { g_feed_key_print_f9, sizeof(g_feed_key_print_f9) },

    { g_feed_key_print_f10, sizeof(g_feed_key_print_f10) },

    { g_feed_key_print_f11, sizeof(g_feed_key_print_f11) },

    { g_feed_key_print_f12, sizeof(g_feed_key_print_f12) },

    { g_feed_key_print_I, sizeof(g_feed_key_print_I) },

};

static unsigned int const g_feed_keys_print_table_length =
(unsigned int)(
    sizeof(g_feed_keys_print_table)
    / sizeof(g_feed_keys_print_table[0u]));

void
feed_keys_print(
    unsigned long int const
        i_code,
    struct feed_buf * const
        p_buf)
{
    if (
        0x80000000ul & i_code)
    {
        char
            b_result;

        unsigned int
            i;

        unsigned int
            i_name_length;

        struct feed_keys_node const *
            p_node;

        b_result =
            0;

        i =
            0u;

        while (
            (!b_result)
            && (
                i < g_feed_keys_print_table_length))
        {
            p_node = g_feed_keys_print_table + i;

            if (
                (i_code & 0xFFul)
                == p_node->p_name[0u])
            {
                if (
                    FEED_KEY_SHIFT & i_code)
                {
                    feed_buf_write_character(
                        p_buf,
                        'S');

                    feed_buf_write_character(
                        p_buf,
                        '-');
                }

                if (
                    FEED_KEY_ALT & i_code)
                {
                    feed_buf_write_character(
                        p_buf,
                        'A');

                    feed_buf_write_character(
                        p_buf,
                        '-');
                }

                if (
                    FEED_KEY_CTRL & i_code)
                {
                    feed_buf_write_character(
                        p_buf,
                        'C');

                    feed_buf_write_character(
                        p_buf,
                        '-');
                }

                i_name_length =
                    (unsigned int)(
                        p_node->i_name_length - 1u);

                feed_buf_write_character_array(
                    p_buf,
                    p_node->p_name + 1u,
                    i_name_length);

                b_result =
                    1;
            }
            else
            {
                i++;
            }
        }

        if (!(b_result))
        {
            if (
                FEED_KEY_SHIFT & i_code)
            {
                feed_buf_write_character(
                    p_buf,
                    'S');

                feed_buf_write_character(
                    p_buf,
                    '-');
            }

            if (
                FEED_KEY_ALT & i_code)
            {
                feed_buf_write_character(
                    p_buf,
                    'A');

                feed_buf_write_character(
                    p_buf,
                    '-');
            }

            if (
                FEED_KEY_CTRL & i_code)
            {
                feed_buf_write_character(
                    p_buf,
                    'C');

                feed_buf_write_character(
                    p_buf,
                    '-');
            }

            feed_buf_write_character(
                p_buf,
                (unsigned char)(i_code & 0x7Ful));
        }
    }

} /* feed_keys_print() */

/* end-of-file: feed_keys.c */
