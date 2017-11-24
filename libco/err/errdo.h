//报错与日志处理工具
#ifndef _ERRDO_H
#define _ERRDO_H

////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////

//提示
#define vardo(...) vardoe(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

//报警
#define wardo(...) wardoe(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

//退出
#define cardo(...) cardoe(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

//中断
#define errdo(...) errdoe(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

////////////////////////////////////

//指定记录文件
void err(char * ek);

//退出
void erdie();

//指定调试水平
int erdic(int ic);

/*
  细节
*/

//信息提示，用于调试
int vardoe(const char * doc, const int id, const char * wk, int ic, char * ck, ...);

//报警
int wardoe(const char * doc, const int id, const char * wk, int mark, char * ck, ...);

//退出
int cardoe(const char * doc, const int id, const char * wk, int mark, char * ck, ...);

//中断
int errdoe(const char * doc, const int id, const char * wk, int mark, char * ck, ...);

/////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif
