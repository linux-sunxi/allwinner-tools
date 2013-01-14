/* pkg_depends.h - the itsy package management system

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

#ifndef PKG_DEPENDS_H
#define PKG_DEPENDS_H

#include "pkg.h"
#include "pkg_hash.h"

enum depend_type {
    PREDEPEND,
    DEPEND,
    CONFLICTS,
    GREEDY_DEPEND,
    RECOMMEND,
    SUGGEST
};
typedef enum depend_type depend_type_t;

enum version_constraint {
    NONE,
    EARLIER,
    EARLIER_EQUAL,
    EQUAL,
    LATER_EQUAL,
    LATER
};
typedef enum version_constraint version_constraint_t;

struct depend{
    version_constraint_t constraint;
    char * version;
    abstract_pkg_t * pkg;
};
typedef struct depend depend_t;
    
struct compound_depend{
    depend_type_t type;
    int possibility_count;
    struct depend ** possibilities;
};
typedef struct compound_depend compound_depend_t;

#include "hash_table.h"

int buildProvides(hash_table_t * hash, abstract_pkg_t * ab_pkg, pkg_t * pkg);
int buildConflicts(hash_table_t * hash, abstract_pkg_t * ab_pkg, pkg_t * pkg);
int buildReplaces(hash_table_t * hash, abstract_pkg_t * ab_pkg, pkg_t * pkg);
int buildDepends(hash_table_t * hash, pkg_t * pkg);

/**
 * pkg_has_common_provides returns 1 if pkg and replacee both provide
 * the same abstract package and 0 otherwise.
 */
int pkg_has_common_provides(pkg_t *pkg, pkg_t *replacee);

/**
 * pkg_provides returns 1 if pkg->provides contains providee and 0
 * otherwise.
 */
int pkg_provides_abstract(pkg_t *pkg, abstract_pkg_t *providee);

/**
 * pkg_replaces returns 1 if pkg->replaces contains one of replacee's provides and 0
 * otherwise.
 */
int pkg_replaces(pkg_t *pkg, pkg_t *replacee);

/**
 * pkg_conflicts_abstract returns 1 if pkg->conflicts contains conflictee provides and 0
 * otherwise.
 */
int pkg_conflicts_abstract(pkg_t *pkg, abstract_pkg_t *conflicts);

/**
 * pkg_conflicts returns 1 if pkg->conflicts contains one of conflictee's provides and 0
 * otherwise.
 */
int pkg_conflicts(pkg_t *pkg, pkg_t *conflicts);

char *pkg_depend_str(pkg_t *pkg, int index);
void buildDependedUponBy(pkg_t * pkg, abstract_pkg_t * ab_pkg);
void freeDepends(pkg_t *pkg);
void printDepends(pkg_t * pkg);
int version_constraints_satisfied(depend_t * depends, pkg_t * pkg);
int pkg_hash_fetch_unsatisfied_dependencies(ipkg_conf_t *conf, pkg_t * pkg, pkg_vec_t *depends, char *** unresolved);
pkg_vec_t * pkg_hash_fetch_conflicts(hash_table_t * hash, pkg_t * pkg);
int pkg_dependence_satisfiable(ipkg_conf_t *conf, depend_t *depend);
int pkg_dependence_satisfied(ipkg_conf_t *conf, depend_t *depend);

#endif
