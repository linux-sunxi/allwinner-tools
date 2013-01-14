/* ipkg_message.h - the itsy package management system

   Copyright (C) 2003 Daniele Nicolodi <daniele@grinta.net>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/

#ifndef _IPKG_MESSAGE_H_
#define _IPKG_MESSAGE_H_

#include "ipkg.h"
#include "ipkg_conf.h"

typedef enum {
     IPKG_ERROR,	/* error conditions */
     IPKG_NOTICE,	/* normal but significant condition */
     IPKG_INFO,		/* informational message */
     IPKG_DEBUG,	/* debug level message */
     IPKG_DEBUG2,	/* more debug level message */
} message_level_t;

extern void ipkg_message(ipkg_conf_t *conf, message_level_t level, char *fmt, ...);

#endif /* _IPKG_MESSAGE_H_ */
