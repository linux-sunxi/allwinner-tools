/* pkg_src_list.h - the itsy package management system

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

#ifndef PKG_SRC_LIST_H
#define PKG_SRC_LIST_H

#include "pkg_src.h"

typedef struct pkg_src_list_elt pkg_src_list_elt_t;
struct pkg_src_list_elt
{
    pkg_src_list_elt_t *next;
    pkg_src_t *data;
};

typedef struct pkg_src_list pkg_src_list_t;
struct pkg_src_list
{
    pkg_src_list_elt_t pre_head;
    pkg_src_list_elt_t *head;
    pkg_src_list_elt_t *tail;
};

static inline int pkg_src_list_empty(pkg_src_list_t *list)
{
     if (list->head == NULL)
	  return 1;
     else
	  return 0;
}

int pkg_src_list_elt_init(pkg_src_list_elt_t *elt, nv_pair_t *data);
void pkg_src_list_elt_deinit(pkg_src_list_elt_t *elt);

int pkg_src_list_init(pkg_src_list_t *list);
void pkg_src_list_deinit(pkg_src_list_t *list);

pkg_src_t *pkg_src_list_append(pkg_src_list_t *list, const char *name, const char *root_dir, const char *extra_data, int gzip);
int pkg_src_list_push(pkg_src_list_t *list, pkg_src_t *data);
pkg_src_list_elt_t *pkg_src_list_pop(pkg_src_list_t *list);

#endif

