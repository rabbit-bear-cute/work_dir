#ifndef _IC_TOOLS_H
#define _IC_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void * submem(void * dst, void * src, size_t start, size_t len);
char * substr(char * src, size_t start, size_t len);
char * trim_str(char * src, int left, int right);
char * trim_left(char * src);
char * trim_right(char * src);
char * trim(char * src);
char * strtoupper(char * src);
char * strtolower(char * src);
struct ic_mem_list * explode(char * src, char * split);
struct ic_mem_list * explode_clean(char * src, char * split);
struct ic_mhash * args_parse(char * args);

#endif
