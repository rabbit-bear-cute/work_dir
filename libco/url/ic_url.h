//
#ifndef _IC_URL_H
#define _IC_URL_H
////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
///////////////////////////////////////////////////////
#define IC_URL_BUF_SIZE 1024
//
struct MemoryStruct {
	char * mem;
	size_t size;
};

/////////////////////////////////////
struct ic_url_box {
	//内容
	void * cont;
	size_t cont_len;
	/////////////////////////////
	char * buf;
	size_t buf_size;
	//
	CURL * curl_handler;
};

/////////////////////////
//
char * get_url_contents(char * url);

//初始化
struct ic_url_box * ic_url_init(char * ua);
//释放
void ic_url_free(struct ic_url_box * url_box);
//抓取
void * ic_url_get(struct ic_url_box * url_box, char * url);

#endif
