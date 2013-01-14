/* ipkg_conf.h - the itsy package management system

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

#ifndef IPKG_CONF_H
#define IPKG_CONF_H

typedef struct ipkg_conf ipkg_conf_t;

#include "hash_table.h"
#include "ipkg.h"
#include "args.h"
#include "pkg.h"
#include "pkg_hash.h"
#include "pkg_src_list.h"
#include "pkg_dest_list.h"
#include "nv_pair_list.h"

#define IPKG_CONF_DEFAULT_TMP_DIR_BASE "/tmp"
#define IPKG_CONF_TMP_DIR_SUFFIX "ipkg-XXXXXX"
#define IPKG_CONF_LISTS_DIR  IPKG_STATE_DIR_PREFIX "/lists"
#define IPKG_CONF_PENDING_DIR IPKG_STATE_DIR_PREFIX "/pending"

/* In case the config file defines no dest */
#define IPKG_CONF_DEFAULT_DEST_NAME "root"
#define IPKG_CONF_DEFAULT_DEST_ROOT_DIR "/"

#define IPKG_CONF_DEFAULT_HASH_LEN 1024

struct ipkg_conf
{
     pkg_src_list_t pkg_src_list;
     pkg_dest_list_t pkg_dest_list;
     nv_pair_list_t arch_list;

     int restrict_to_default_dest;
     pkg_dest_t *default_dest;

     char *tmp_dir;
     const char *lists_dir;
     const char *pending_dir;

     /* options */
     int force_depends;
     int force_defaults;
     int force_overwrite;
     int force_downgrade;
     int force_reinstall;
     int force_space;
     int force_removal_of_dependent_packages;
     int force_removal_of_essential_packages;
     int nodeps; /* do not follow dependences */
     int verbose_wget;
     int multiple_providers;
     char *offline_root;
     char *offline_root_pre_script_cmd;
     char *offline_root_post_script_cmd;
     int query_all;
     int verbosity;
     int noaction;

     /* proxy options */
     char *http_proxy;
     char *ftp_proxy;
     char *no_proxy;
     char *proxy_user;
     char *proxy_passwd;

     hash_table_t pkg_hash;
     hash_table_t file_hash;
     hash_table_t obs_file_hash;
};

enum ipkg_option_type {
     IPKG_OPT_TYPE_BOOL,
     IPKG_OPT_TYPE_INT,
     IPKG_OPT_TYPE_STRING
};
typedef enum ipkg_option_type ipkg_option_type_t;

typedef struct ipkg_option ipkg_option_t;
struct ipkg_option {
     const char *name;
     const ipkg_option_type_t type;
     const void *value;
};

int ipkg_conf_init(ipkg_conf_t *conf, const args_t *args);
void ipkg_conf_deinit(ipkg_conf_t *conf);

int ipkg_conf_write_status_files(ipkg_conf_t *conf);
char *root_filename_alloc(ipkg_conf_t *conf, char *filename);

#endif
