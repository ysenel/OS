// Defining TRUE and FALSE is usually a Bad Idea,
// because you will probably be inconsistent with anyone
// else who had the same clever idea.
// Therefore:  DON'T USE THIS FILE.

#ifndef _bool_h
#define _bool_h 1

#ifdef MAC_OS

#include <stdbool.h>

#define TRUE true
#define FALSE false

#else

#include   <_G_config.h>

#if _G_HAVE_BOOL
#undef TRUE
#undef FALSE
#define TRUE true
#define FALSE false
#else
#undef FALSE
#undef TRUE
#undef true
#undef false
enum nachos_bool
{ FALSE = 0, TRUE = 1, };
#endif

#endif // MAC_OS_SYS

#endif
