/* See LICENCE for license details */

/*

Module: feed_os.h

Description:

    Include of operating system header files and precompiled header file.

*/

/* Reverse include guard */
#if defined(INC_FEED_OS_H)
#error include feed_os.h once
#endif /* #if defined(INC_FEED_OS_H) */

/* Mark this header files as included */
#define INC_FEED_OS_H

#include <stdarg.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include <termios.h>

#include <errno.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <sys/ioctl.h>

#include <time.h>

/* Avoid unused macro warning while using precompiled header file */
#if defined(__GNUC__)
#if defined(__SSP_STRONG__)
#endif
#if defined(INC_FEED_OS_H)
#endif
#endif

/* end-of-file: feed_os.h */
