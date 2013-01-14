/* pkg_hash.h - the itsy package management system

   Steven M. Ayer
   
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

#ifndef PKG_HASH_H
#define PKG_HASH_H

#include "pkg.h"
#include "pkg_vec.h"
#include "hash_table.h"


int pkg_hash_init(const char *name, hash_table_t *hash, int len);
void pkg_hash_deinit(hash_table_t *hash);
void pkg_hash_map(hash_table_t *hash, void (*f)(void *data, void *entry), void *data);

void pkg_hash_dump(hash_table_t *hash, void *data);
void pkg_hash_fetch_available(hash_table_t *hash, pkg_vec_t *available);

int pkg_hash_add_from_file(ipkg_conf_t *conf, const char *file_name,
			   pkg_src_t *src, pkg_dest_t *dest, int is_status_file);
pkg_t *hash_insert_pkg(hash_table_t *hash, pkg_t *pkg, int set_status,ipkg_conf_t *conf);

abstract_pkg_t * ensure_abstract_pkg_by_name(hash_table_t * hash, const char * pkg_name);
abstract_pkg_t * abstract_pkg_fetch_by_name(hash_table_t * hash, const char * pkg_name);
pkg_vec_t *pkg_hash_fetch_by_name(hash_table_t *hash, const char *pkg_name);
void pkg_hash_fetch_all_installed(hash_table_t *hash, pkg_vec_t *installed);
pkg_t * pkg_hash_fetch_by_name_version(hash_table_t *hash, 
				       const char *pkg_name,
				       const char * version);
abstract_pkg_vec_t *pkg_hash_fetch_all_installation_candidates(hash_table_t *hash, const char *name);
pkg_t *pkg_hash_fetch_best_installation_candidate(ipkg_conf_t *conf, abstract_pkg_t *apkg, 
						  int (*constraint_fcn)(pkg_t *pkg, void *data), void *cdata, int quiet);
pkg_t *pkg_hash_fetch_best_installation_candidate_by_name(ipkg_conf_t *conf, const char *name);
pkg_t *pkg_hash_fetch_installed_by_name(hash_table_t *hash,
					const char *pkg_name);
pkg_t *pkg_hash_fetch_installed_by_name_dest(hash_table_t *hash,
					     const char *pkg_name,
					     pkg_dest_t *dest);

pkg_t *file_hash_get_file_owner(ipkg_conf_t *conf, const char *file_name);
int file_hash_set_file_owner(ipkg_conf_t *conf, const char *file_name, pkg_t *pkg);

/* XXX: shouldn't this go in pkg_vec.[ch]? */
pkg_vec_t *pkg_vec_fetch_by_name(hash_table_t *hash, const char *pkg_name);

#endif

