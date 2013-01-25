/* str_list.h - the itsy package management system

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

#ifndef STR_LIST_H
#define STR_LIST_H

#include "void_list.h"

typedef struct str_list_elt str_list_elt_t;
struct str_list_elt
{
    str_list_elt_t *next;
    char *data;
};

typedef struct xstr_list str_list_t;
struct xstr_list
{
    str_list_elt_t pre_head;
    str_list_elt_t *head;
    str_list_elt_t *tail;
};

int str_list_elt_init(str_list_elt_t *elt, char *data);
void str_list_elt_deinit(str_list_elt_t *elt);

str_list_t *str_list_alloc(void);
int str_list_init(str_list_t *list);
void str_list_deinit(str_list_t *list);

int str_list_append(str_list_t *list, char *data);
int str_list_push(str_list_t *list, char *data);
str_list_elt_t *str_list_pop(str_list_t *list);
str_list_elt_t *str_list_remove(str_list_t *list, str_list_elt_t **iter);
char *str_list_remove_elt(str_list_t *list, const char *target_str);

#endif
