/* pkg_dest.h - the itsy package management system

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

#ifndef PKG_DEST_H
#define PKG_DEST_H

typedef struct pkg_dest pkg_dest_t;
struct pkg_dest
{
    char *name;
    char *root_dir;
    char *ipkg_dir;
    char *lists_dir;
    char *info_dir;
    char *status_file_name;
    char *status_file_tmp_name;
    FILE *status_file;
};

int pkg_dest_init(pkg_dest_t *dest, const char *name, const char *root_dir,const char *lists_dir);
void pkg_dest_deinit(pkg_dest_t *dest);

#endif

