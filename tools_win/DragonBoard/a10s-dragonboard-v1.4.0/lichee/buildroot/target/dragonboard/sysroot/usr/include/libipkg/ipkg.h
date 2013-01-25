/* ipkg.h - the itsy package management system

   Carl D. Worth

   Copyright (C) 2001 University of Southern California

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/

#ifndef IPKG_H
#define IPKG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if 0
#define IPKG_DEBUG_NO_TMP_CLEANUP
#endif

#include "includes.h"
#include "ipkg_conf.h"
#include "ipkg_message.h"

#define IPKG_PKG_EXTENSION ".ipk"
#define DPKG_PKG_EXTENSION ".deb"

#define IPKG_LEGAL_PKG_NAME_CHARS "abcdefghijklmnopqrstuvwxyz0123456789.+-"
#define IPKG_PKG_VERSION_SEP_CHAR '_'

#define IPKG_STATE_DIR_PREFIX IPKGLIBDIR"/ipkg"
#define IPKG_LISTS_DIR_SUFFIX "lists"
#define IPKG_INFO_DIR_SUFFIX "info"
#define IPKG_STATUS_FILE_SUFFIX "status"

#define IPKG_BACKUP_SUFFIX "-ipkg.backup"

#define IPKG_LIST_DESCRIPTION_LENGTH 128

enum ipkg_error {
    IPKG_SUCCESS = 0,
    IPKG_PKG_DEPS_UNSATISFIED,
    IPKG_PKG_IS_ESSENTIAL,
    IPKG_PKG_HAS_DEPENDENTS,
    IPKG_PKG_HAS_NO_CANDIDATE
};
typedef enum ipkg_error ipkg_error_t;

extern int ipkg_state_changed;


struct errlist {
    char * errmsg;
    struct errlist * next;
} ;

struct errlist* error_list;


#endif
