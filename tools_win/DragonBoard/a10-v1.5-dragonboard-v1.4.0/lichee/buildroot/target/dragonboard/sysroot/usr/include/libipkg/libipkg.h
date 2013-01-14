/* ipkglib.h - the itsy package management system

   Florian Boor <florian.boor@kernelconcepts.de>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/

#ifndef IPKGLIB_H
#define IPKGLIB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#ifdef IPKG_LIB

#include "ipkg_conf.h"
#include "ipkg_message.h"

#include "args.h"
#include "pkg.h"

typedef int (*ipkg_message_callback)(ipkg_conf_t *conf, message_level_t level, 
	char *msg);
typedef int (*ipkg_list_callback)(char *name, char *desc, char *version, 
	pkg_state_status_t status, void *userdata);
typedef int (*ipkg_status_callback)(char *name, int istatus, char *desc,
	void *userdata);
typedef char* (*ipkg_response_callback)(char *question);

extern int ipkg_op(int argc, char *argv[]); /* ipkglib.c */
extern int ipkg_init (ipkg_message_callback mcall, 
                      ipkg_response_callback rcall,
					  args_t * args);

extern int ipkg_deinit (args_t *args);
extern int ipkg_packages_list(args_t *args, 
                              const char *packages, 
                              ipkg_list_callback cblist,
                              void *userdata);
extern int ipkg_packages_status(args_t *args, 
                                const char *packages, 
                                ipkg_status_callback cbstatus,
								void *userdata);
extern int ipkg_packages_info(args_t *args,
                              const char *packages,
                              ipkg_status_callback cbstatus,
                              void *userdata);
extern int ipkg_packages_install(args_t *args, const char *name);
extern int ipkg_packages_remove(args_t *args, const char *name, int purge);
extern int ipkg_lists_update(args_t *args);
extern int ipkg_packages_upgrade(args_t *args);
extern int ipkg_packages_download(args_t *args, const char *name);
extern int ipkg_package_files(args_t *args,
                              const char *name,
							  ipkg_list_callback cblist,
							  void *userdata);
extern int ipkg_file_search(args_t *args,
                            const char *file,
							ipkg_list_callback cblist,
							void *userdata);
extern int ipkg_package_whatdepends(args_t *args, const char *file);
extern int ipkg_package_whatrecommends(args_t *args, const char *file);
extern int ipkg_package_whatprovides(args_t *args, const char *file);
extern int ipkg_package_whatconflicts(args_t *args, const char *file);
extern int ipkg_package_whatreplaces(args_t *args, const char *file);

extern ipkg_message_callback ipkg_cb_message; /* ipkglib.c */
extern ipkg_response_callback ipkg_cb_response;
extern ipkg_status_callback ipkg_cb_status;
extern ipkg_list_callback ipkg_cb_list;
extern void push_error_list(struct errlist **errors,char * msg);
extern void reverse_error_list(struct errlist **errors);
extern void free_error_list();

#endif


#endif
