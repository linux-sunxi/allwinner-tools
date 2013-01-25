/* args.h - parse command-line args

  Carl D. Worth

  Copyright 2001 University of Southern California
 
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
*/

#ifndef ARGS_H
#define ARGS_H

struct args
{
    char *conf_file;
    char *dest;
    char *tmp_dir;
    int force_defaults;
    int force_depends;
    int force_overwrite;
    int force_downgrade;
    int force_reinstall;
    int force_removal_of_essential_packages;
    int force_removal_of_dependent_packages;
    int force_space;
    int noaction;
    int nodeps;
    int multiple_providers;
    int query_all;
    int verbose_wget;
    int verbosity;
    int nocheckfordirorfile;
    int noreadfeedsfile;
    char *offline_root;
    char *offline_root_pre_script_cmd;
    char *offline_root_post_script_cmd;
};
typedef struct args args_t;

#define ARGS_DEFAULT_CONF_FILE_DIR "/etc"
#define ARGS_DEFAULT_CONF_FILE_NAME "ipkg.conf"
#define ARGS_DEFAULT_DEST NULL
#define ARGS_DEFAULT_FORCE_DEFAULTS 0
#define ARGS_DEFAULT_FORCE_DEPENDS 0
#define ARGS_DEFAULT_FORCE_OVERWRITE 0 
#define ARGS_DEFAULT_FORCE_DOWNGRADE 0 
#define ARGS_DEFAULT_FORCE_REINSTALL 0
#define ARGS_DEFAULT_FORCE_REMOVAL_OF_ESSENTIAL_PACKAGES 0
#define ARGS_DEFAULT_FORCE_REMOVAL_OF_DEPENDENT_PACKAGES 0
#define ARGS_DEFAULT_FORCE_SPACE 0
#define ARGS_DEFAULT_OFFLINE_ROOT NULL
#define ARGS_DEFAULT_OFFLINE_ROOT_PRE_SCRIPT_CMD NULL
#define ARGS_DEFAULT_OFFLINE_ROOT_POST_SCRIPT_CMD NULL
#define ARGS_DEFAULT_NOACTION 0
#define ARGS_DEFAULT_NODEPS 0
#define ARGS_DEFAULT_VERBOSE_WGET 0
#define ARGS_DEFAULT_VERBOSITY 1

int args_init(args_t *args);
void args_deinit(args_t *args);
int args_parse(args_t *args, int argc, char *argv[]);
void args_usage(char *complaint);

#endif
