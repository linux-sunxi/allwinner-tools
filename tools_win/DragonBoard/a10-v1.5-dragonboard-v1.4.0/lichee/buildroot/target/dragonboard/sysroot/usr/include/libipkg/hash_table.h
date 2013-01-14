/* hash.h - hash tables for ipkg

   Steven M. Ayer, Jamey Hicks
   
   Copyright (C) 2002 Compaq Computer Corporation

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/

#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

typedef struct hash_entry hash_entry_t;
typedef struct hash_table hash_table_t;

struct hash_entry {
  const char * key;
  void * data;
  struct hash_entry * next;
};

struct hash_table {
  const char *name; 
  hash_entry_t * entries;  
  int n_entries; /* number of buckets */
  int n_elements;
  const char * (*hash_entry_key)(void * data);
};

int hash_table_init(const char *name, hash_table_t *hash, int len);
void hash_table_deinit(hash_table_t *hash);
void *hash_table_get(hash_table_t *hash, const char *key);
int hash_table_insert(hash_table_t *hash, const char *key, void *value);
void hash_table_foreach(hash_table_t *hash, void (*f)(const char *key, void *entry, void *data), void *data);

#endif /* _HASH_TABLE_H_ */
