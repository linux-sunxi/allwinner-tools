#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>

#if STDC_HEADERS
# include <stdlib.h>
# include <stdarg.h>
# include <stddef.h>
# include <ctype.h>
# include <errno.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif

#if HAVE_REGEX_H
# include <regex.h>
#endif

#if HAVE_STRING_H
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
/* XXX: What's the right way to pick up GNU's strndup declaration? */
# if __GNUC__
#   define __USE_GNU 1
# endif
# include <string.h>
# undef __USE_GNU
#endif

#if HAVE_STRINGS_H
# include <strings.h>
#endif

#if HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#if HAVE_UNISTD_H
# include <sys/types.h>
# include <unistd.h>
#endif

// #include "replace/replace.h"

#endif
