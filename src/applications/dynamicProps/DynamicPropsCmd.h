/* DynamicPropsCmd.h */

/* File autogenerated by gengetopt version 2.14  */

#ifndef DYNAMICPROPSCMD_H
#define DYNAMICPROPSCMD_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
#define CMDLINE_PARSER_PACKAGE "DynamicProps"
#endif

#ifndef CMDLINE_PARSER_VERSION
#define CMDLINE_PARSER_VERSION "1.00"
#endif

struct gengetopt_args_info
{
  char * input_arg;	/* input dump file.  */
  char * input_orig;	/* input dump file original value given at command line.  */
  char * output_arg;	/* output file name.  */
  char * output_orig;	/* output file name original value given at command line.  */
  char * sele1_arg;	/* select first stuntdouble set.  */
  char * sele1_orig;	/* select first stuntdouble set original value given at command line.  */
  char * sele2_arg;	/* select second stuntdouble set (if sele2 is not set, use script from sele1).  */
  char * sele2_orig;	/* select second stuntdouble set (if sele2 is not set, use script from sele1) original value given at command line.  */
  int order_arg;	/* Lengendre Polynomial Order.  */
  char * order_orig;	/* Lengendre Polynomial Order original value given at command line.  */
  
  int help_given ;	/* Whether help was given.  */
  int version_given ;	/* Whether version was given.  */
  int input_given ;	/* Whether input was given.  */
  int output_given ;	/* Whether output was given.  */
  int sele1_given ;	/* Whether sele1 was given.  */
  int sele2_given ;	/* Whether sele2 was given.  */
  int order_given ;	/* Whether order was given.  */
  int rcorr_given ;	/* Whether rcorr was given.  */
  int vcorr_given ;	/* Whether vcorr was given.  */
  int dcorr_given ;	/* Whether dcorr was given.  */
  int lcorr_given ;	/* Whether lcorr was given.  */
  int r_rcorr_given ;	/* Whether r_rcorr was given.  */
  int thetacorr_given ;	/* Whether thetacorr was given.  */
  int drcorr_given ;	/* Whether drcorr was given.  */
  int helfandEcorr_given ;	/* Whether helfandEcorr was given.  */

  int dynamicProps_group_counter; /* counter for group dynamicProps */
} ;

int cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info);
int cmdline_parser2 (int argc, char * const *argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required);
int cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info);

void cmdline_parser_print_help(void);
void cmdline_parser_print_version(void);

void cmdline_parser_init (struct gengetopt_args_info *args_info);
void cmdline_parser_free (struct gengetopt_args_info *args_info);

int cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* DYNAMICPROPSCMD_H */
