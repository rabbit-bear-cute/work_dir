//
#ifndef _UTF8_UTILS_H
#define _UTF8_UTILS_H
/////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "ic_mem.h"
//
#define BIT_LEN 8
#define UTF8_BIT_MAX 6
//
/////////////////////////////////////////////////////////////////////////////
int utf8_char_len(char chr);
struct ic_mem_list * utf8_char_list(char * src);
//
size_t utf8_strlen(char * src, size_t * bit_len);
char * utf8_truncate(char * src, size_t utf8_len);
char * utf8_trunc_bit(char * src, size_t utf8_bit_len);
//
#endif
