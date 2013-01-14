/* nv_pair_list.h - the itsy package management system

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

#ifndef NV_PAIR_LIST_H
#define NV_PAIR_LIST_H

#include "nv_pair.h"
#include "void_list.h"

typedef struct nv_pair_list_elt nv_pair_list_elt_t;
struct nv_pair_list_elt
{
    nv_pair_list_elt_t *next;
    nv_pair_t *data;
};

typedef struct nv_pair_list nv_pair_list_t;
struct nv_pair_list
{
    nv_pair_list_elt_t pre_head;
    nv_pair_list_elt_t *head;
    nv_pair_list_elt_t *tail;
};

static inline int nv_pair_list_empty(nv_pair_list_t *list)
{
     if (list->head == NULL)
	  return 1;
     else
	  return 0;
}

int nv_pair_list_elt_init(nv_pair_list_elt_t *elt, nv_pair_t *data);
void nv_pair_list_elt_deinit(nv_pair_list_elt_t *elt);

int nv_pair_list_init(nv_pair_list_t *list);
void nv_pair_list_deinit(nv_pair_list_t *list);

nv_pair_t *nv_pair_list_append(nv_pair_list_t *list,
			       const char *name, const char *value);
int nv_pair_list_push(nv_pair_list_t *list, nv_pair_t *data);
nv_pair_list_elt_t *nv_pair_list_pop(nv_pair_list_t *list);
char *nv_pair_list_find(nv_pair_list_t *list, char *name);

#endif

